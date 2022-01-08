
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <utility>
#include <variant>

struct Instruction{
    int* programPositionPtr{nullptr};
    Instruction(int* _programPositionPtr) : programPositionPtr{_programPositionPtr}{}
    Instruction(const Instruction&) = default;
    Instruction& operator=(const Instruction&) = default;
    Instruction(Instruction&&) = default;
    Instruction& operator=(Instruction&&) = default;
    virtual ~Instruction() = default;

    virtual void execute() = 0;
};

struct cpy : public Instruction{
    std::variant<int, int*> source;
    int* destination{nullptr};
    cpy(int* _programPositionPtr, int* _source, int* _destination) : Instruction{_programPositionPtr}, source{_source}, destination{_destination}{}
    cpy(int* _programPositionPtr, int  _source, int* _destination) : Instruction{_programPositionPtr}, source{_source}, destination{_destination}{}
    void execute() override{
        if(source.index()==0){
            *destination = std::get<int>(source);
        }
        else{
            *destination = *std::get<int*>(source);
        }
        (*programPositionPtr)++;
    }
};

struct inc : public Instruction{
    int* registerPtr{nullptr};
    inc(int* _programPositionPtr, int* _registerPtr) : Instruction{_programPositionPtr}, registerPtr{_registerPtr}{}
    void execute() override{
        (*registerPtr)++;
        (*programPositionPtr)++;
    }
};

struct dec : public Instruction{
    int* registerPtr{nullptr};
    dec(int* _programPositionPtr, int* _registerPtr) : Instruction{_programPositionPtr}, registerPtr{_registerPtr}{}
    void execute() override{
        (*registerPtr)--;
        (*programPositionPtr)++;
    }
};

struct jnz : public Instruction{
    int* registerPtr{nullptr};
    int offset{0};
    jnz(int* _programPositionPtr, int* _registerPtr, int _offset) : Instruction{_programPositionPtr}, registerPtr{_registerPtr}, offset{_offset}{}
    void execute() override{
        if(*registerPtr!=0) (*programPositionPtr)+=offset;
        else (*programPositionPtr)++;
    }
};

struct Computer{
    int programPosition{0};
    std::vector<std::unique_ptr<Instruction>> program{};
    std::array<int, 4> reg = {0, 0, 0, 0};

    Computer(std::vector<std::string> input){
        for(const auto& row : input){
            const auto split = Utilities::split(row);
            if(split[0] == "inc"){
                program.emplace_back(std::make_unique<inc>(&programPosition, &reg[0]+split[1][0]-'a'));
            }
            else if(split[0] == "dec"){
                program.emplace_back(std::make_unique<dec>(&programPosition, &reg[0]+split[1][0]-'a'));
            }
            else if(split[0] == "cpy"){
                if(std::isdigit(split[1][0])){
                    program.emplace_back(std::make_unique<cpy>(&programPosition, std::stoi(split[1]),  &reg[0]+split[2][0]-'a' ));
                }
                else{
                    program.emplace_back(std::make_unique<cpy>(&programPosition, &reg[0]+split[1][0]-'a',  &reg[0]+split[2][0]-'a' ));
                }
            }
            else{ //jnz
                program.emplace_back(std::make_unique<jnz>(&programPosition, &reg[0]+split[1][0]-'a', std::stoi(split[2])));
            }
        }
    }

    void execute(){
        do{
            program[programPosition]->execute();
        }while( 0 <= programPosition && std::cmp_less(programPosition,program.size()));
    }

    void reset(){
        programPosition = 0;
        reg = {0,0,0,0};
    }
};


int main(){
    Computer computer{readFile::vectorOfStrings("input.txt")};

    //Task 1
    computer.execute();
    const auto registerAfterExecution = computer.reg[0];
    std::cout << "The value of register a at the end of the program is " << registerAfterExecution << ".\n";

    //Task 2
    computer.reset();
    computer.reg[2]=1;
    computer.execute();
    const auto registerAfterExecution2 = computer.reg[0];
    std::cout << "With register c starting at 1, the value of the a reg is " << registerAfterExecution2 << ".\n";

    VerifySolution::verifySolution(registerAfterExecution, registerAfterExecution2);
}