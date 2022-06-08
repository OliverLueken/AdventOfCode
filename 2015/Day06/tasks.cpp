
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
        int x_start{};
        int x_end{};
        int y_start{};
        int y_end{};
        DataComputer* computer{nullptr};
        Command& command{};

    public:
        CommandWrapper(int a, int b, int c, int d, DataComputer* ptr, Command& _command)
            : x_start{a}, x_end{b}, y_start{c}, y_end{d}, computer{ptr}, command{_command}{}

        auto operator()(){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    command(lightsPtr->operator[](x+y*1000));
                }
            }
            computer->advanceCurrentPosition(1);
        }
    };

    template<typename Command>
    void addCommand(int x_start, int x_end, int y_start, int y_end, DataComputer* computer, Command& command) const {
        return computer->add(CommandWrapper<Command>(x_start, x_end, y_start, y_end, computer, command));
    }

public:
    auto make(const auto& input){
        auto computer = DataComputer{};

        auto parse = [computer = &computer, this](const auto& string){
            const auto split = Utilities::splitOnEach(string, " ,l");

            const auto x_start = std::stoi(split[2]);
            const auto y_start = std::stoi(split[3]);
            const auto x_end   = std::stoi(split[5]);
            const auto y_end   = std::stoi(split[6]);

            if(split[1] == "on"){
                this->addCommand(x_start, x_end, y_start, y_end, computer, Commands::turnOnCommand);
            }
            else if(split[1] == "off"){
                this->addCommand(x_start, x_end, y_start, y_end, computer, Commands::turnOffCommand);
            }
            else{ //toggle
                this->addCommand(x_start, x_end, y_start, y_end, computer, Commands::toggleCommand);
            }
        };

        std::ranges::for_each(input, parse);
        return computer;
    }
};

struct Command1{
    static constexpr auto turnOnCommand = [](auto& light){
        light=1;
    };
    static constexpr auto turnOffCommand = [](auto& light){
        light=0;
    };
    static constexpr auto toggleCommand = [](auto& light){
        light=1^light;
    };
};

struct Command2{
    static constexpr auto turnOnCommand = [](auto& light){
        ++light;
    };
    static constexpr auto turnOffCommand = [](auto& light){
        if(light>0) --light;
    };
    static constexpr auto toggleCommand = [](auto& light){
        light+=2;
    };
};

auto getBrightness = [](const auto& instructions, auto&& factory){
    auto computer = factory.make(instructions);
    computer.execute();
    return Utilities::sum(computer.getDataView());
};

int main(){
    const auto instructions = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto brightnessTaskOne = getBrightness(instructions, ComputerFactory<Command1>{});
    std::cout << "There are a total of " << brightnessTaskOne << " lights lit now.\n";

    //Task 2
    const auto brightnessTaskTwo = getBrightness(instructions, ComputerFactory<Command2>{});
    std::cout << "The total brightness of the lights combine to " << brightnessTaskTwo << ".\n";

    VerifySolution::verifySolution(brightnessTaskOne, brightnessTaskTwo);
}
