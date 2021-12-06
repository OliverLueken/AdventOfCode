
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using Command = std::function<void(int&)>;

class Commands{
public:
    Command turnOn{};
    Command turnOff{};
    Command toggle{};
};

struct Instruction{
    Command* commandPtr;
    unsigned int x_start;
    unsigned int y_start;
    unsigned int x_end;
    unsigned int y_end;
};


auto parseInput = [](const auto& input, Commands& commands){

    auto parse = [&commands](const auto& string){
        Instruction instruction{};
        const auto split = Utilities::splitOnEach(string, " ,l");

        if(split[1] == "on"){
            instruction.commandPtr = &commands.turnOn;
        }
        else if(split[1] == "off"){
            instruction.commandPtr = &commands.turnOff;
        }
        else{ //toggle
            instruction.commandPtr = &commands.toggle;
        }

        instruction.x_start = std::stoi(split[2]);
        instruction.y_start = std::stoi(split[3]);
        instruction.x_end   = std::stoi(split[5]);
        instruction.y_end   = std::stoi(split[6]);

        return instruction;
    };

    std::vector<Instruction> instructions;
    std::ranges::transform(input, std::back_inserter(instructions), parse);
    return instructions;
};


auto getBrightness = [](const auto& instructions){

    std::array<int, 1000*1000> lights{};

    auto followInstruction = [&lights](const auto& instruction){
        auto& command = *instruction.commandPtr;
        for(auto y=instruction.y_start; y<=instruction.y_end; y++){
            for(auto x=instruction.x_start; x<=instruction.x_end; x++){
                command(lights[x+y*1000]);
            }
        }
    };

    std::ranges::for_each(instructions, followInstruction);
    return Utilities::sum(lights);
};


auto getTaskOneCommands(){
    auto turnOn  = [](auto& light){light=1;};
    auto turnOff = [](auto& light){light=0;};
    auto toggle  = [](auto& light){light=1^light;};
    return Commands(turnOn, turnOff, toggle);
}

auto getTaskTwoCommands(){
    auto turnOn  = [](auto& light){light++;};
    auto turnOff = [](auto& light){if(light>0) light--;};
    auto toggle  = [](auto& light){light+=2;};
    return Commands(turnOn, turnOff, toggle);
}


int main(){
    Commands commands{};
    const auto instructions = parseInput(readFile::vectorOfStrings("input.txt"), commands);

    //Task 1
    commands = getTaskOneCommands();
    const auto brightnessTaskOne = getBrightness(instructions);
    std::cout << "There are a total of " << brightnessTaskOne << " lights lit now.\n";

    //Task 2
    commands = getTaskTwoCommands();
    const auto brightnessTaskTwo = getBrightness(instructions);
    std::cout << "The total brightness of the lights combine to " << brightnessTaskTwo << ".\n";
}