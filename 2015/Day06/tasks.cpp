
#include "../../readFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <numeric>

struct instruction{
    std::string instr;
    unsigned int x_start;
    unsigned int y_start;
    unsigned int x_end;
    unsigned int y_end;
};

auto parseInput = [](const auto& input){

    auto parse = [](const auto& string){
        instruction instruction{};
        auto pos = string.find_first_of("0123456789");
        instruction.instr = string.substr(0,pos-1); //-1 gets rid of the trailing white space
        instruction.x_start = std::stoi(string.substr(pos));
        pos = string.find(',', pos);
        instruction.y_start = std::stoi(string.substr(pos+1));
        pos = string.find(' ', pos+5);
        instruction.x_end = std::stoi(string.substr(pos+1));
        pos = string.find(',', pos+1);
        instruction.y_end = std::stoi(string.substr(pos+1));

        return instruction;
    };

    std::vector<instruction> instructions;
    std::ranges::transform(input, std::back_inserter(instructions), parse);
    return instructions;
};


auto followInstructions = [](auto& lights, const auto& instructions, const auto& TurnOn, const auto& TurnOff, const auto& Toggle){
    auto followInstruction = [&lights, &TurnOn, &TurnOff, &Toggle](const auto& instruction){

        auto makeLights = [&lights, &instruction](const auto& instr){
            for(auto y=instruction.y_start; y<=instruction.y_end; y++){
                for(auto x=instruction.x_start; x<=instruction.x_end; x++){
                    instr(lights[x+y*1000]);
                }
            }
        };

        if(instruction.instr == "turn on"){
            makeLights(TurnOn);
        }
        else if(instruction.instr == "turn off"){
            makeLights(TurnOff);
        }
        else{
            makeLights(Toggle);
        }
    };

    std::ranges::for_each(instructions, followInstruction);
};


int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    const auto instructions = parseInput(input);

    {  //Task 1
        std::array<bool, 1000*1000> lights{};

        auto TurnOn  = [](auto& light){light=true;};
        auto TurnOff = [](auto& light){light=false;};
        auto Toggle  = [](auto& light){light=!light;};

        followInstructions(lights, instructions, TurnOn, TurnOff, Toggle);

        auto lightsOn = std::accumulate(std::begin(lights), std::end(lights), 0u);
        std::cout << "There are a total of " << lightsOn << " lights lit now.\n";
    }

    //Task 2
    std::array<unsigned int, 1000*1000> lights{};

    auto TurnOn  = [](auto& light){light++;};
    auto TurnOff = [](auto& light){if(light>0) light--;};
    auto Toggle  = [](auto& light){light+=2;};

    followInstructions(lights, instructions, TurnOn, TurnOff, Toggle);

    auto brightness = std::accumulate(std::begin(lights), std::end(lights), 0u);
    std::cout << "The total brightness of the lights combine to " << brightness << ".\n";
}