
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
    Computer* const computerPtr{nullptr};

    Instruction(Computer* const _computerPtr) : computerPtr{_computerPtr}{}
    Instruction(const Instruction&) = default;
    Instruction& operator=(const Instruction&) = default;
    Instruction(Instruction&&) = default;
    Instruction& operator=(Instruction&&) = default;
    virtual ~Instruction() = default;

    virtual void execute() = 0;
};

struct set : public Instruction {
    const int source{};
    const int destinationOffset{};
    set(auto* const _computerPtr, int const _source, int const _destinationOffset)
        : Instruction{_computerPtr}, source{_source}, destinationOffset{_destinationOffset}{}
    void execute() override {
        computerPtr->reg[destinationOffset] = source;
        computerPtr->programPosition++;
    }
};

struct cpy : public Instruction {
    const int sourceOffset{};
    const int destinationOffset{};
    cpy(auto* const _computerPtr, int  const _sourceOffset, int const _destinationOffset)
        : Instruction{_computerPtr}, sourceOffset{_sourceOffset}, destinationOffset{_destinationOffset}{}
    void execute() override {
        computerPtr->reg[destinationOffset] = computerPtr->reg[sourceOffset];
        computerPtr->programPosition++;
    }
};

struct inc : public Instruction {
    const int regOffset{};
    inc(auto* const _computerPtr, int const _regOffset) : Instruction{_computerPtr}, regOffset{_regOffset}{}
    void execute() override {
        computerPtr->reg[regOffset]++;
        computerPtr->programPosition++;
    }
};

struct dec : public Instruction {
    const int regOffset{};
    dec(auto* const _computerPtr, int const _regOffset) : Instruction{_computerPtr}, regOffset{_regOffset}{}
    void execute() override {
        computerPtr->reg[regOffset]--;
        computerPtr->programPosition++;
    }
};

struct jnz : public Instruction {
    const int regOffset{0};
    const int offset{0};
    jnz(Computer* const _computerPtr, const int _regOffset, const int _offset) : Instruction{_computerPtr}, regOffset{_regOffset}, offset{_offset}{}
    void execute() override {
        if(computerPtr->reg[regOffset]!=0) computerPtr->programPosition+=offset;
        else computerPtr->programPosition++;
    }
};

struct jmp : public Instruction {
    const int offset{0};
    jmp(Computer* const _computerPtr, const int _offset) : Instruction{_computerPtr}, offset{_offset}{}
    void execute() override {
        computerPtr->programPosition+=offset;
    }
};

Computer::Computer(const std::vector<std::string>& input){
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        if(split[0] == "inc"){
            program.emplace_back(std::make_unique<inc>(this, split[1][0]-'a'));
        }
        else if(split[0] == "dec"){
            program.emplace_back(std::make_unique<dec>(this, split[1][0]-'a'));
        }
        else if(split[0] == "cpy"){
            if(std::isdigit(split[1][0])){
                program.emplace_back(std::make_unique<set>(this, std::stoi(split[1]),  split[2][0]-'a' ));
            }
            else{
                program.emplace_back(std::make_unique<cpy>(this, split[1][0]-'a',  split[2][0]-'a' ));
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

    VerifySolution::verifySolution(registerAfterExecution, registerAfterExecution2);
}