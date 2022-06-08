
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

class ComputerFactory{
    virtual void addTurnOn(int, int, int, int, DataComputer*) const = 0;
    virtual void addTurnOff(int, int, int, int, DataComputer*) const = 0;
    virtual void addToggle(int, int, int, int, DataComputer*) const = 0;

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
                this->addTurnOn(x_start, x_end, y_start, y_end, computer);
            }
            else if(split[1] == "off"){
                this->addTurnOff(x_start, x_end, y_start, y_end, computer);
            }
            else{ //toggle
                this->addToggle(x_start, x_end, y_start, y_end, computer);
            }
        };

        std::ranges::for_each(input, parse);
        return computer;
    }
};

class Factory1 : public ComputerFactory{
    void addTurnOn(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    lightsPtr->operator[](x+y*1000)=1;
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
    void addTurnOff(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    lightsPtr->operator[](x+y*1000)=0;
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
    void addToggle(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    lightsPtr->operator[](x+y*1000)=1^lightsPtr->operator[](x+y*1000);
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
};

class Factory2 : public ComputerFactory{
    void addTurnOn(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    ++lightsPtr->operator[](x+y*1000);
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
    void addTurnOff(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    if(lightsPtr->operator[](x+y*1000)>0) --lightsPtr->operator[](x+y*1000);
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
    void addToggle(int x_start, int x_end, int y_start, int y_end, DataComputer* computer) const override {
        auto turnOn  = [x_start, x_end, y_start, y_end, computer](){
            auto lightsPtr = computer->getDataPtr();
            for(auto y=y_start; y<=y_end; y++){
                for(auto x=x_start; x<=x_end; x++){
                    lightsPtr->operator[](x+y*1000)+=2;
                }
            }
            computer->advanceCurrentPosition(1);
        };
        return computer->add(std::move(turnOn));
    }
};


auto getBrightness = [](const auto& instructions, auto&& factory){
    auto computer = factory.make(instructions);
    computer.execute();
    return 0;
};

int main(){
    const auto instructions = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto brightnessTaskOne = getBrightness(instructions, Factory1{});
    std::cout << "There are a total of " << brightnessTaskOne << " lights lit now.\n";

    //Task 2
    const auto brightnessTaskTwo = getBrightness(instructions, Factory2{});
    std::cout << "The total brightness of the lights combine to " << brightnessTaskTwo << ".\n";

    VerifySolution::verifySolution(brightnessTaskOne, brightnessTaskTwo);
}
