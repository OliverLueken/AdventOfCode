
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

template<typename Commands>
class ComputerFactory{

    template<typename Command>
    class CommandWrapper{
        const int x_start{};
        const int x_end{};
        const int y_start{};
        const int y_end{};
        DataComputer* const computer{nullptr};
        const Command& command{};

    public:
        CommandWrapper(const int a, const int b, const int c, const int d, DataComputer* const ptr, const Command& _command)
            : x_start{a}, x_end{b}, y_start{c}, y_end{d}, computer{ptr}, command{_command}{}

        auto operator()() const {
            const auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    command(lightsPtr->at(x+1000*y));
                }
            }
            computer->advanceCurrentPosition(1);
        }
    };

    template<typename Command>
    static void addCommand(const int x_start, const int x_end, const int y_start, const int y_end, DataComputer* const computer, const Command& command){
        return computer->add(CommandWrapper<Command>(x_start, x_end, y_start, y_end, computer, command));
    }

public:
    static auto make(const auto& input){
        auto computer = DataComputer{};

        auto parse = [computer = &computer](const auto& string){
            const auto split = Utilities::splitOnEach(string, " ,l");

            const auto x_start = std::stoi(split[2]);
            const auto y_start = std::stoi(split[3]);
            const auto x_end   = std::stoi(split[5]);
            const auto y_end   = std::stoi(split[6]);

            if(split[1] == "on"){
                addCommand(x_start, x_end, y_start, y_end, computer, Commands::turnOnCommand);
            }
            else if(split[1] == "off"){
                addCommand(x_start, x_end, y_start, y_end, computer, Commands::turnOffCommand);
            }
            else{ //toggle
                addCommand(x_start, x_end, y_start, y_end, computer, Commands::toggleCommand);
            }
        };

        std::ranges::for_each(input, parse);
        return computer;
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
    auto computer = ComputerFactory<Commands>::make(instructions);
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
