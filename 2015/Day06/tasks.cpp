
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

// struct RegisterValueLogger : public Computer::Logger {
//     DataComputer* computer{nullptr};
//     std::vector<int> maxRegisterValueAfterEachExecution{};
//
//     RegisterValueLogger(DataComputer* comp) : computer{comp}{
//         computer->addLogger(this);
//     }
//
//     void update() override {
//         maxRegisterValueAfterEachExecution.push_back(
//             std::ranges::max( computer->getDataView() | std::views::values )
//         );
//     }
// };

class ComputerFactory{
    virtual void addTurnOn(int, int, int, int, DataComputer*) const = 0;
    virtual void addTurnOff(int, int, int, int, DataComputer*) const = 0;
    virtual void addToggle(int, int, int, int, DataComputer*) const = 0;

public:
    auto make(const auto& input){
        auto computer = DataComputer{};

        auto parse = [computer = &computer, this](const auto& string){
            // Instruction instruction{};
            const auto split = Utilities::splitOnEach(string, " ,l");

            const auto x_start = std::stoi(split[2]);
            const auto y_start = std::stoi(split[3]);
            const auto x_end   = std::stoi(split[5]);
            const auto y_end   = std::stoi(split[6]);

            if(split[1] == "on"){
                this->addTurnOn(x_start, x_end, y_start, y_end, computer);
                // instruction.commandPtr = &commands.turnOn;
            }
            else if(split[1] == "off"){
                this->addTurnOff(x_start, x_end, y_start, y_end, computer);
                // instruction.commandPtr = &commands.turnOff;
            }
            else{ //toggle
                this->addToggle(x_start, x_end, y_start, y_end, computer);
                // instruction.commandPtr = &commands.toggle;
            }


            // return instruction;
        };

        // std::vector<Instruction> instructions;
        std::ranges::for_each(input, parse);
        return computer;
    }
};
//
// using Command = std::function<void(int&)>;
//
// class Commands{
// public:
//     Command turnOn{};
//     Command turnOff{};
//     Command toggle{};
// };
//
// struct Instruction{
//     Command* commandPtr;
//     unsigned int x_start;
//     unsigned int y_start;
//     unsigned int x_end;
//     unsigned int y_end;
// };
//
//
// auto parseInput = [](const auto& input, Commands& commands){
//
//     auto parse = [&commands](const auto& string){
//         Instruction instruction{};
//         const auto split = Utilities::splitOnEach(string, " ,l");
//
//         if(split[1] == "on"){
//             instruction.commandPtr = &commands.turnOn;
//         }
//         else if(split[1] == "off"){
//             instruction.commandPtr = &commands.turnOff;
//         }
//         else{ //toggle
//             instruction.commandPtr = &commands.toggle;
//         }
//
//         instruction.x_start = std::stoi(split[2]);
//         instruction.y_start = std::stoi(split[3]);
//         instruction.x_end   = std::stoi(split[5]);
//         instruction.y_end   = std::stoi(split[6]);
//
//         return instruction;
//     };
//
//     std::vector<Instruction> instructions;
//     std::ranges::transform(input, std::back_inserter(instructions), parse);
//     return instructions;
// };
//
//
//
//
class Factory1 : public ComputerFactory{
    void addTurnOn(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    lightsPtr->operator[](x+y*1000)=1;
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    void addTurnOff(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    lightsPtr->operator[](x+y*1000)=0;
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    void addToggle(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    lightsPtr->operator[](x+y*1000)=1^lightsPtr->operator[](x+y*1000);
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    //     auto turnOff = [](){light=0;};
    //     auto toggle  = [](){light=1^light;};
    //     return Commands(turnOn, turnOff, toggle);
    // }
};
class Factory2 : public ComputerFactory{
    void addTurnOn(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    ++lightsPtr->operator[](x+y*1000);
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    void addTurnOff(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    if(lightsPtr->operator[](x+y*1000)>0) --lightsPtr->operator[](x+y*1000);
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    void addToggle(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    // command(lightsPtr->operator[](x+y*1000));
                    lightsPtr->operator[](x+y*1000)+=2;
                }
            }
        };
        return computer->add(std::move(turnOn));
    }
    //     auto turnOff = [](){light=0;};
    //     auto toggle  = [](auto& light){light=1^light;};
    //     return Commands(turnOn, turnOff, toggle);
    // }
};
// auto getTaskTwoCommands(){
//     auto turnOn  = [](auto& light){light++;};
//     auto turnOff = [](auto& light){if(light>0) light--;};
//     auto toggle  = [](auto& light){light+=2;};
//     return Commands(turnOn, turnOff, toggle);
// }


auto getBrightness = [](const auto& instructions){
    auto factory = Factory1{};
    auto computer = factory.make(instructions);
    // std::array<int, 1000*1000> lights{};

    // auto followInstruction = [](const auto& {
    //     // auto& command = *instruction.commandPtr;
    //     // for(auto y=instruction.y_start; y<=instruction.y_end; y++){
    //     //     for(auto x=instruction.x_start; x<=instruction.x_end; x++){
    //             // command(lightsPtr->operator[](x+y*1000));
    //     //     }
    //     // }
    // };

    // std::ranges::for_each(instructions, followInstruction);
    // return Utilities::sum(lights);
    computer.execute();
    return 0;
};
int main(){
    // Commands commands{};
    const auto instructions = readFile::vectorOfStrings("input.txt");

    //Task 1
    // commands = getTaskOneCommands();
    const auto brightnessTaskOne = getBrightness(instructions);
    std::cout << "There are a total of " << brightnessTaskOne << " lights lit now.\n";

    //Task 2
    // commands = getTaskTwoCommands();
    const auto brightnessTaskTwo = getBrightness(instructions);
    std::cout << "The total brightness of the lights combine to " << brightnessTaskTwo << ".\n";

    VerifySolution::verifySolution(brightnessTaskOne, brightnessTaskTwo);
}
