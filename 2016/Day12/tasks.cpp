
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

struct Instruction;

struct Computer{
    int programPosition{0};
    std::vector<std::unique_ptr<Instruction>> program{};
    std::array<int, 4> reg{0, 0, 0, 0};

    Computer(const std::vector<std::string>& input);

    void execute();

    void reset(){
        programPosition = 0;
        reg.fill(0);
    }
};

struct Instruction{
    Computer* programPtr{nullptr};

    Instruction(Computer* _programPtr) : programPtr{_programPtr}{}
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
    cpy(auto* _programPositionPtr, int* _source, int* _destination) : Instruction{_programPositionPtr}, source{_source}, destination{_destination}{}
    cpy(auto* _programPositionPtr, int  _source, int* _destination) : Instruction{_programPositionPtr}, source{_source}, destination{_destination}{}
    void execute() override{
        if(source.index()==0){
            *destination = std::get<int>(source);
        }
        else{
            *destination = *std::get<int*>(source);
        }
        programPtr->programPosition++;
    }
};

struct inc : public Instruction{
    int* registerPtr{nullptr};
    inc(auto* _programPositionPtr, int* _registerPtr) : Instruction{_programPositionPtr}, registerPtr{_registerPtr}{}
    void execute() override{
        (*registerPtr)++;
        programPtr->programPosition++;
    }
};

struct dec : public Instruction{
    int* registerPtr{nullptr};
    dec(auto* _programPositionPtr, int* _registerPtr) : Instruction{_programPositionPtr}, registerPtr{_registerPtr}{}
    void execute() override{
        (*registerPtr)--;
        programPtr->programPosition++;
    }
};

struct jnz : public Instruction{
    int regOffset{0};
    int offset{0};
    jnz(Computer* _programPtr, int _register, int _offset) : Instruction{_programPtr}, regOffset{_register}, offset{_offset}{}
    void execute() override{
        if(programPtr->reg[regOffset]!=0) programPtr->programPosition+=offset;
        else programPtr->programPosition++;
    }
};

struct jmp : public Instruction{
    int offset{0};
    jmp(Computer* _programPtr, int _offset) : Instruction{_programPtr}, offset{_offset}{}
    void execute() override {
        programPtr->programPosition+=offset;
    }
};

Computer::Computer(const std::vector<std::string>& input){
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        if(split[0] == "inc"){
            program.emplace_back(std::make_unique<inc>(this, &reg[0]+split[1][0]-'a'));
        }
        else if(split[0] == "dec"){
            program.emplace_back(std::make_unique<dec>(this, &reg[0]+split[1][0]-'a'));
        }
        else if(split[0] == "cpy"){
            if(std::isdigit(split[1][0])){
                program.emplace_back(std::make_unique<cpy>(this, std::stoi(split[1]),  &reg[0]+split[2][0]-'a' ));
            }
            else{
                program.emplace_back(std::make_unique<cpy>(this, &reg[0]+split[1][0]-'a',  &reg[0]+split[2][0]-'a' ));
            }
        }
        else if(std::isdigit(split[1][0]) ){
            program.emplace_back(std::make_unique<jmp>(this, std::stoi(split[2])));
        }
        else{ //jnz
            program.emplace_back(std::make_unique<jnz>(this, split[1][0]-'a', std::stoi(split[2])));
        }
    }
}

void Computer::execute(){
    do{
        program[programPosition]->execute();
    }while( 0 <= programPosition && std::cmp_less(programPosition,program.size()));
}

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

}