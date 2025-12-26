#include <vector>
#include <string>
#include <input.h>
#include <regex>
#include <cstdint>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <z3++.h>

struct Machine
{
    uint16_t lights;
    std::vector<std::vector<size_t>> buttons;
    std::vector<int> joltages;

    Machine(const std::string &line)
    {
        lights = 0;

        // Parse lights [#...#] or [.##.] etc.
        size_t start = line.find('[') + 1;
        size_t end = line.find(']');
        std::string lightsStr = line.substr(start, end - start);
        for (size_t i = 0; i < lightsStr.length(); i++)
        {
            if (lightsStr[i] == '#')
            {
                lights |= (1 << i);
            }
        }

        // Parse buttons - groups like (1,3) (2,3,4) etc.
        std::regex buttonRegex(R"(\(([0-9,]+)\))");
        std::sregex_iterator iter(line.begin(), line.end(), buttonRegex);
        std::sregex_iterator end_iter;

        while (iter != end_iter)
        {
            std::string buttonStr = (*iter)[1].str();
            std::vector<size_t> button;

            size_t pos = 0;
            while (pos < buttonStr.length())
            {
                size_t commaPos = buttonStr.find(',', pos);
                if (commaPos == std::string::npos)
                {
                    button.push_back(std::stoul(buttonStr.substr(pos)));
                    break;
                }
                button.push_back(std::stoul(buttonStr.substr(pos, commaPos - pos)));
                pos = commaPos + 1;
            }

            buttons.push_back(button);
            ++iter;
        }

        // Parse joltages {37,24,60,50,16}
        start = line.find('{') + 1;
        end = line.find('}');
        std::string joltagesStr = line.substr(start, end - start);

        size_t pos = 0;
        while (pos < joltagesStr.length())
        {
            size_t commaPos = joltagesStr.find(',', pos);
            if (commaPos == std::string::npos)
            {
                joltages.push_back(std::stoi(joltagesStr.substr(pos)));
                break;
            }
            joltages.push_back(std::stoi(joltagesStr.substr(pos, commaPos - pos)));
            pos = commaPos + 1;
        }
    }
};

std::vector<size_t> findButtonSequenceLights(Machine &m)
{
    struct State
    {
        uint16_t lights;
        std::vector<size_t> buttonPresses;
    };

    std::queue<State> queue;
    std::unordered_set<uint16_t> visited;

    // Start with all lights off
    queue.push({0, {}});
    visited.insert(0);

    while (!queue.empty())
    {
        State current = queue.front();
        queue.pop();

        // Check if we match the required pattern of lights
        if (current.lights == m.lights)
        {
            return current.buttonPresses;
        }

        // Try pressing each button
        for (size_t buttonIndex = 0; buttonIndex < m.buttons.size(); buttonIndex++)
        {
            uint16_t newLights = current.lights;
            for (size_t lightIndex : m.buttons[buttonIndex])
            {
                newLights ^= (1 << lightIndex);
            }

            // Only explore if we haven't seen this state before
            if (visited.find(newLights) == visited.end())
            {
                visited.insert(newLights);
                std::vector<size_t> newPresses = current.buttonPresses;
                newPresses.push_back(buttonIndex);
                queue.push({newLights, newPresses});
            }
        }
    }

    return {}; // No solution found
}

void partOne()
{
    const std::vector<std::string> &lines = util::readLines("src/day10/input.txt");

    int totalPresses = 0;
    for (const std::string &line : lines)
    {
        Machine m(line);

        std::vector<size_t> presses = findButtonSequenceLights(m);
        totalPresses += presses.size();
    }

    std::cout << totalPresses << std::endl;
}

int findMinNrPressesJoltage(Machine &m)
{
    z3::context ctx;
    z3::optimize opt(ctx);

    // Create variables for the number of times each button is pressed
    std::vector<z3::expr> buttonPresses;
    for (size_t i = 0; i < m.buttons.size(); i++)
    {
        std::string varName = "button_" + std::to_string(i);
        buttonPresses.push_back(ctx.int_const(varName.c_str()));
        // Each button press count must be non-negative
        opt.add(buttonPresses[i] >= 0);
    }

    // Add constraints for each joltage value
    for (size_t joltageIndex = 0; joltageIndex < m.joltages.size(); joltageIndex++)
    {
        z3::expr_vector contributions(ctx);

        for (size_t buttonIndex = 0; buttonIndex < m.buttons.size(); buttonIndex++)
        {
            // Check if this button affects this joltage index
            bool affectsJoltage = false;
            for (size_t lightIndex : m.buttons[buttonIndex])
            {
                if (lightIndex == joltageIndex)
                {
                    affectsJoltage = true;
                    break;
                }
            }

            if (affectsJoltage)
            {
                contributions.push_back(buttonPresses[buttonIndex]);
            }
        }

        // The total joltage for this index must equal the target
        if (contributions.size() > 0)
        {
            opt.add(z3::sum(contributions) == m.joltages[joltageIndex]);
        }
    }

    // Minimize the total number of button presses
    z3::expr_vector allPresses(ctx);
    for (const auto &press : buttonPresses)
    {
        allPresses.push_back(press);
    }
    z3::expr totalPresses = z3::sum(allPresses);
    opt.minimize(totalPresses);

    // Solve
    if (opt.check() == z3::sat)
    {
        z3::model model = opt.get_model();
        int result = model.eval(totalPresses).get_numeral_int();
        return result;
    }

    return -1; // No solution found
}

void partTwo()
{
    const std::vector<std::string> &lines = util::readLines("src/day10/input.txt");

    int totalPresses = 0;
    for (const std::string &line : lines)
    {
        Machine m(line);

        totalPresses += findMinNrPressesJoltage(m);
    }

    std::cout << totalPresses << std::endl;
}

int main()
{
    partTwo();
    return 0;
}