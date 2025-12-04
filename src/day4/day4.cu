#include <iostream>
#include <math.h>
#include "input.h"

struct Grid
{
  std::unique_ptr<char[]> data;
  size_t width;
  size_t height;
};

__global__ void rowConvolve(char *input, char *result, int width, int height, int convolutionSize)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  int row = i / width;
  int col = i % width;

  int convolutionRadius = convolutionSize / 2;

  for (int convCol = col - convolutionRadius; convCol <= col + convolutionRadius; convCol++)
  {
    if (convCol >= 0 && convCol < width)
    {
      result[i] += input[row * width + convCol];
    }
  }
}

__global__ void colConvolve(char *input, char *result, int width, int height, int convolutionSize)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  int row = i / width;
  int col = i % width;

  int convolutionRadius = convolutionSize / 2;

  for (int convRow = row - convolutionRadius; convRow <= row + convolutionRadius; convRow++)
  {
    if (convRow >= 0 && convRow < height)
    {
      result[i] += input[convRow * width + col];
    }
  }
}

__global__ void removeRolls(char *original, char *adjacency, int width, int height, int *totalRollsRemoved)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  // We have to do < 5 (instead of < 4 described in the puzzle),
  // since the convolution also sums the roll itself, not just the neighbours
  if (original[i] == 1 && adjacency[i] < 5)
  {
    original[i] = 0;
    atomicAdd(totalRollsRemoved, 1);
  }
}

int removeRollsIterative(Grid &grid, int maxIterations = 1, int convolutionSize = 3)
{
  const int width = grid.width;
  const int height = grid.height;

  int totalRollsRemoved = 0;
  int prevTotalRollsRemoved = 0;

  // Device CUDA matrixes
  char *d_OriginalGrid;
  cudaMalloc(&d_OriginalGrid, width * height * sizeof(char));
  cudaMemcpy(d_OriginalGrid, grid.data.get(), width * height * sizeof(char), cudaMemcpyHostToDevice);

  char *d_AdjacencyRow;
  cudaMalloc(&d_AdjacencyRow, width * height * sizeof(char));

  char *d_AdjacencyCol;
  cudaMalloc(&d_AdjacencyCol, width * height * sizeof(char));

  int *d_totalRollsRemoved;
  cudaMalloc(&d_totalRollsRemoved, sizeof(int));

  // Make sure we never infinitely loop (but also allow with maxIterations = 1 to just run it once)
  for (int i = 0; i < maxIterations; i++)
  {
    // First convolve the rows
    cudaMemset(d_AdjacencyRow, 0, width * height * sizeof(char));
    rowConvolve<<<width, height>>>(d_OriginalGrid, d_AdjacencyRow, width, height, convolutionSize);

    // Then the columns
    cudaMemset(d_AdjacencyCol, 0, width * height * sizeof(char));
    colConvolve<<<width, height>>>(d_AdjacencyRow, d_AdjacencyCol, width, height, convolutionSize);

    // And remove the rolls that have less than 4 neighbours
    removeRolls<<<width, height>>>(d_OriginalGrid, d_AdjacencyCol, width, height, d_totalRollsRemoved);

    prevTotalRollsRemoved = totalRollsRemoved;
    cudaMemcpy(&totalRollsRemoved, d_totalRollsRemoved, sizeof(int), cudaMemcpyDeviceToHost);

    if (totalRollsRemoved == prevTotalRollsRemoved)
      break;
  }

  // Clean up
  cudaFree(d_OriginalGrid);
  cudaFree(d_AdjacencyRow);
  cudaFree(d_AdjacencyCol);
  cudaFree(d_totalRollsRemoved);

  return totalRollsRemoved;
}

Grid getNewGrid(const std::string &fileName)
{
  const std::vector<std::string> lines = util::readLines(fileName);

  // Create a 2D matrix of chars
  const int height = lines.size();
  const int width = lines[0].length();

  auto originalGrid = std::make_unique<char[]>(width * height);
  for (int row = 0; row < height; row++)
  {
    for (int col = 0; col < width; col++)
    {
      originalGrid[row * width + col] = lines[row][col] == '@' ? 1 : 0;
    }
  }

  Grid grid;
  grid.data = std::move(originalGrid);
  grid.width = width;
  grid.height = height;

  return grid;
}

void partOne()
{
  Grid originalGrid = getNewGrid("src/day4/input.txt");

  // Part one
  int removedRolls = removeRollsIterative(originalGrid);
  std::cout
      << "Answer: "
      << removedRolls << std::endl;
}

void partTwo()
{
  Grid originalGrid = getNewGrid("src/day4/input.txt");

  // Part one
  int removedRolls = removeRollsIterative(originalGrid, 10000);
  std::cout
      << "Answer: "
      << removedRolls << std::endl;
}

int main(void)
{
  partTwo();
  return 0;
}