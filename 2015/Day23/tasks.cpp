
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <utility>


using Register = std::array<unsigned int, 2>;
using DataComputer = Computer::Computer<Register>;
using Factory = Computer::ComputerFactory<Register>;

struct MyFactory : public Factory{

    auto add_half(unsigned int reg){
        auto half = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            r/=2;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(half));
    }

    auto add_triple(unsigned int reg){
        auto triple = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            r*=3;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(triple));
    }

    auto add_increment(unsigned int reg){
        auto increment = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            ++r;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(increment));
    }

    auto add_jump(int offset){
        auto jump = [offset](DataComputer* compPtr){
            compPtr->advanceCurrentPosition(offset);
        };
        Factory::addCommand(std::move(jump));
    }

    auto add_jumpIfEven(unsigned int reg, int offset){
        auto jumpIfEven = [reg, offset](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            if(r%2==0){
                compPtr->advanceCurrentPosition(offset);
            }
            else{
                compPtr->advanceCurrentPosition();
            }
        };
        Factory::addCommand(std::move(jumpIfEven));
    }

    auto add_jumpIfOne(unsigned int reg, int offset){
        auto jumpIfOne = [reg, offset](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            if(r==1){
                compPtr->advanceCurrentPosition(offset);
            }
            else{
                compPtr->advanceCurrentPosition();
            }
        };
        Factory::addCommand(std::move(jumpIfOne));
    }

    void makeCommand(const std::string& instr) override {
        const auto split = Utilities::split(instr);
        if(split[0] == "hlf"){
            add_half(split[1][0]-'a');
        }
        else if(split[0] == "tpl"){
            add_triple(split[1][0]-'a');
        }
        else if(split[0] == "inc"){
            add_increment(split[1][0]-'a');
        }
        else if(split[0] == "jmp"){
            add_jump(std::stoi(split[1]));
        }
        else if(split[0] == "jie"){
            add_jumpIfEven(split[1][0]-'a', std::stoi(split[2]));
        }
        else{ //jumpIfOne
            add_jumpIfOne(split[1][0]-'a', std::stoi(split[2]));
        }
    }
};


int main(){
    auto computer = MyFactory{}.make(readFile::vectorOfStrings("input.txt"));

    //Task 1
    computer.execute();
    const auto registerAfterExecution1 = computer.getDataPtr()->at(1);
    std::cout << "The value of register b at the end of the program is " << registerAfterExecution1 << ".\n";

    //Task 2
    computer.reset();
    computer.getDataPtr()->at(0)=1;
    computer.execute();
    const auto registerAfterExecution2 = computer.getDataPtr()->at(1);
    std::cout << "With register a starting at 1, the value of the b reg is " << registerAfterExecution2 << ".\n";

    VerifySolution::verifySolution(registerAfterExecution1, registerAfterExecution2);
}