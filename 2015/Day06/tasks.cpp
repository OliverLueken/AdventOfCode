
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using Register = std::array<int, 1000*1000>;
using DataComputer = Computer::Computer<Register>;
using Factory = Computer::ComputerFactory<Register>;


template<typename Commands>
class MyFactory : public Factory{

    template<typename Command>
    void addCommand(const Command& command, const int x_start, const int x_end, const int y_start, const int y_end){
        auto outerCommand = [&command, x_start, x_end, y_start, y_end](DataComputer* _computerPtr){
            auto& lights = _computerPtr->getData();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    command(lights[x+1000*y]);
                }
            }
            _computerPtr->advanceCurrentPosition();
        };
        Factory::addCommand(
            std::move(outerCommand)
        );
    }

    void makeCommand(const std::string& string) override {
        const auto split = Utilities::splitOnEach(string, " ,l");

        const auto x_start = std::stoi(split[2]);
        const auto y_start = std::stoi(split[3]);
        const auto x_end   = std::stoi(split[5]);
        const auto y_end   = std::stoi(split[6]);

        if(split[1] == "on"){
            addCommand(Commands::turnOnCommand, x_start, x_end, y_start, y_end);
        }
        else if(split[1] == "off"){
            addCommand(Commands::turnOffCommand, x_start, x_end, y_start, y_end);
        }
        else{ //toggle
            addCommand(Commands::toggleCommand, x_start, x_end, y_start, y_end);
        }
    }

};

struct Commands1{
    static constexpr auto turnOnCommand  = [](auto& light){ light=1;       };
    static constexpr auto turnOffCommand = [](auto& light){ light=0;       };
    static constexpr auto toggleCommand  = [](auto& light){ light=1^light; };
};

struct Commands2{
    static constexpr auto turnOnCommand  = [](auto& light){ ++light;             };
    static constexpr auto turnOffCommand = [](auto& light){ if(light>0) --light; };
    static constexpr auto toggleCommand  = [](auto& light){ light+=2;            };
};

template<typename Commands>
auto getBrightness = [](const auto& instructions){
    auto computer = MyFactory<Commands>{}.make(instructions);
    computer.execute();
    return Utilities::sum(computer.getDataView());
};

int main(){
    const auto instructions = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto brightnessTaskOne = getBrightness<Commands1>(instructions);
    std::cout << "There are a total of " << brightnessTaskOne << " lights lit now.\n";

    //Task 2
    const auto brightnessTaskTwo = getBrightness<Commands2>(instructions);
    std::cout << "The total brightness of the lights combine to " << brightnessTaskTwo << ".\n";

    VerifySolution::verifySolution(brightnessTaskOne, brightnessTaskTwo);
}
