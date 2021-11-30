
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <utility>
#include <variant>

struct Computer;

struct Instruction{
    Computer* computerPtr{nullptr};
    Instruction(Computer* computerPtr) : computerPtr{computerPtr}{}
    virtual void execute() = 0;
    virtual std::unique_ptr<Instruction> toggle() const = 0;
    virtual std::string name() const = 0;
};

struct Computer{
    int programPosition{0};
    std::vector<std::unique_ptr<Instruction>> program{};
    std::array<int, 4> reg = {0, 0, 0, 0};

    Computer(std::vector<std::string> input);

    bool isValidProgramPosition(const auto position) const {
        return 0 <= position && std::cmp_less(position, program.size());
    }

    void execute(){
        do{
            program[programPosition]->execute();
        }while( isValidProgramPosition(programPosition));
    }

    void reset(){
        programPosition = 0;
        reg = {0,0,0,0};
    }
};

auto toInt(const std::string& s, const Computer* computerPtr){
    try{
        return std::stoi(s);
    }
    catch(const std::exception&){
        return computerPtr->reg[s[0]-'a'];
    }
}

struct cpy : public Instruction{
    std::string source{};
    std::string destination{};
    cpy(Computer* computerPtr, std::string source, std::string destination)
        : Instruction{computerPtr}, source{source}, destination{destination}{}

    void execute() override {
        if(std::isdigit(destination[0]) || destination[0] == '-') return;
        const int x = toInt(source, computerPtr);
        computerPtr->reg[destination[0]-'a'] = x;
        computerPtr->programPosition++;
    }

    std::string name() const override {return "cpy";}

    std::unique_ptr<Instruction> toggle() const override;
};

struct dec : public Instruction{
    std::string destination{};
    dec(Computer* computerPtr, std::string destination) : Instruction{computerPtr}, destination{destination}{}
    void execute() override{
        if(std::isdigit(destination[0]) || destination[0]=='-') return;
        computerPtr->reg[destination[0]-'a']--;
        computerPtr->programPosition++;
    }

    std::unique_ptr<Instruction> toggle() const override;
    std::string name() const override{return "dec";}
};


struct jnz : public Instruction{
    std::string destination{};
    std::string offset{};

    jnz(Computer* computer, std::string destination, std::string offset)
        : Instruction{computer}, destination{destination}, offset{offset}{}

    void execute() override{
        const int val = toInt(destination, computerPtr);
        const int d   = toInt(offset, computerPtr);;
        if(val!=0) computerPtr->programPosition+=d;
        else computerPtr->programPosition++;
    }

    std::unique_ptr<Instruction> toggle() const override {
        return std::make_unique<cpy>(computerPtr, destination, offset);
    }
    std::string name() const override {return "jnz";}
};

struct tgl : public Instruction{
    std::string destination{};

    tgl(Computer* computerPtr, std::string destination)
        : Instruction{computerPtr}, destination{destination}{}

    void execute() override{
        const auto x = toInt(destination, computerPtr);;
        const auto currentPos = computerPtr->programPosition;
        if(computerPtr->isValidProgramPosition(currentPos + x)){
            computerPtr->program[currentPos+x] = computerPtr->program[currentPos+x]->toggle();
        }
        computerPtr->programPosition++;
    }

    std::unique_ptr<Instruction> toggle() const override ;
    std::string name() const override {return "tgl";}
};

template<typename T>
std::string getDestination(const int pos, const auto& program){
    return ((T*) program[pos].get())->destination;
}

struct inc : public Instruction{
    std::string destination{};

    inc(Computer* computerPtr, std::string destination) : Instruction{computerPtr}, destination{destination}{}

    bool multiplyPattern() const {
        const auto pos = computerPtr->programPosition;
        const auto& program = computerPtr->program;
        return program[pos-1]->name() == "cpy"
            && program[pos+1]->name() == "dec"
            && program[pos+2]->name() == "jnz"
            && program[pos+3]->name() == "dec"
            && program[pos+4]->name() == "jnz"
            && getDestination<cpy>(pos-1, program) == getDestination<dec>(pos+1, program)
            && getDestination<cpy>(pos-1, program) == getDestination<jnz>(pos+2, program)
            && getDestination<dec>(pos+3, program) == getDestination<jnz>(pos+4, program)
            && getDestination<cpy>(pos-1, program) != getDestination<inc>(pos  , program)
            && getDestination<dec>(pos+3, program) != getDestination<inc>(pos  , program)
            && getDestination<dec>(pos+1, program) != getDestination<dec>(pos+3, program)
            && getDestination<cpy>(pos-1, program) != getDestination<inc>(pos  , program)
            && ((cpy*) program[pos-1].get())->source != getDestination<dec>(pos+1, program)
            && ((cpy*) program[pos-1].get())->source != getDestination<dec>(pos+3, program)
            ;
    }

    void multiply(){
        std::string cReg = getDestination<dec>(computerPtr->programPosition+1, computerPtr->program);
        std::string dReg = getDestination<dec>(computerPtr->programPosition+3, computerPtr->program);
        computerPtr->reg[destination[0]-'a']+=toInt(cReg, computerPtr)*toInt(dReg, computerPtr);
        computerPtr->reg[cReg[0]-'a']=0;
        computerPtr->reg[dReg[0]-'a']=0;
        computerPtr->programPosition+=5;
    }

    void execute() override{
        if(std::isdigit(destination[0]) || destination[0] == '-') return;
        if(multiplyPattern()){
            multiply();
        }
        else{
            computerPtr->reg[destination[0]-'a']++;
            computerPtr->programPosition++;
        }
    }

    std::string name() const override {return "inc";}
    std::unique_ptr<Instruction> toggle() const override {
        return std::make_unique<dec>(computerPtr, destination);
    }
};


std::unique_ptr<Instruction> dec::toggle() const {
    return std::make_unique<inc>(computerPtr, destination);
}

std::unique_ptr<Instruction> tgl::toggle() const {
    return std::make_unique<inc>(computerPtr, destination);
}

std::unique_ptr<Instruction> cpy::toggle() const {
    return std::make_unique<jnz>(computerPtr, source, destination);
}


Computer::Computer(std::vector<std::string> input){
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        if(split[0] == "inc"){
            program.emplace_back(std::make_unique<inc>( this, split[1] ));
        }
        else if(split[0] == "dec"){
            program.emplace_back(std::make_unique<dec>( this, split[1] ));
        }
        else if(split[0] == "cpy"){
            program.emplace_back(std::make_unique<cpy>( this, split[1], split[2] ));
        }
        else if(split[0] == "tgl"){
            program.emplace_back(std::make_unique<tgl>( this, split[1] ));
        }
        else{ //jnz
            program.emplace_back(std::make_unique<jnz>( this, split[1], split[2] ));
        }
    }
}


//output is basically n!+70*87
auto calc = [](const auto& input, auto n){
    Computer computer{input};
    computer.reg[0]=n;
    computer.execute();
    return computer.reg[0];
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto aValue1 = calc(input, 7);
    std::cout << "The value of register a at the end of the program is " << aValue1 << ".\n";

    //Task 2
    const auto aValue2 = calc(input, 12);
    std::cout << "With register a starting at 12, the value of the a reg is " << aValue2 << ".\n";

}