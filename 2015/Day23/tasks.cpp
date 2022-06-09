
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

    auto add_hlf(unsigned int reg){
        auto hlf = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            r/=2;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(hlf));
    }

    auto add_tpl(unsigned int reg){
        auto tpl = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            r*=3;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(tpl));
    }

    auto add_inc(unsigned int reg){
        auto inc = [reg](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            ++r;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(inc));
    }

    auto add_jmp(int offset){
        auto jmp = [offset](DataComputer* compPtr){
            compPtr->advanceCurrentPosition(offset);
        };
        Factory::addCommand(std::move(jmp));
    }

    auto add_jie(unsigned int reg, int offset){
        auto jie = [reg, offset](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            if(r%2==0){
                compPtr->advanceCurrentPosition(offset);
            }
            else{
                compPtr->advanceCurrentPosition();
            }
        };
        Factory::addCommand(std::move(jie));
    }

    auto add_jio(unsigned int reg, int offset){
        auto jio = [reg, offset](DataComputer* compPtr){
            auto& r = compPtr->getDataPtr()->at(reg);
            if(r==1){
                compPtr->advanceCurrentPosition(offset);
            }
            else{
                compPtr->advanceCurrentPosition();
            }
        };
        Factory::addCommand(std::move(jio));
    }

    void makeCommand(const std::string& instr) override {
        const auto split = Utilities::split(instr);
        if(split[0] == "hlf"){
            add_hlf(split[1][0]-'a');
        }
        else if(split[0] == "tpl"){
            add_tpl(split[1][0]-'a');
        }
        else if(split[0] == "inc"){
            add_inc(split[1][0]-'a');
        }
        else if(split[0] == "jmp"){
            add_jmp(std::stoi(split[1]));
        }
        else if(split[0] == "jie"){
            add_jie(split[1][0]-'a', std::stoi(split[2]));
        }
        else{ //jio
            add_jio(split[1][0]-'a', std::stoi(split[2]));
        }
    }
};

struct Instruction{
    int* pos{nullptr};
    Instruction(int* _pos) : pos{_pos}{}
    Instruction(const Instruction&) = default;
    Instruction& operator=(const Instruction&) = default;
    Instruction(Instruction&&) = default;
    Instruction& operator=(Instruction&&) = default;
    virtual ~Instruction() = default;
    virtual void execute() = 0;
};

struct hlf : public Instruction{
    unsigned int* r{nullptr};
    hlf(int* _pos, unsigned int* _r) : Instruction{_pos}, r{_r}{}
    void execute() override{
        (*r)/=2;
        (*pos)++;
    }
};

struct tpl : public Instruction{
    unsigned int* r{nullptr};
    tpl(int* _pos, unsigned int* _r) : Instruction{_pos}, r{_r}{}
    void execute() override{
        (*r)*=3;
        (*pos)++;
    }
};

struct inc : public Instruction{
    unsigned int* r{nullptr};
    inc(int* _pos, unsigned int* _r) : Instruction{_pos}, r{_r}{}
    void execute() override{
        (*r)++;
        (*pos)++;
    }
};

struct jmp : public Instruction{
    int offset{0};
    jmp(int* _pos, int _offset) : Instruction{_pos}, offset{_offset}{}
    void execute() override{
        (*pos)+=offset;
    }
};

struct jie : public Instruction{
    unsigned int* r{nullptr};
    int offset{0};
    jie(int* _pos, unsigned int* _r, int _offset) : Instruction{_pos}, r{_r}, offset{_offset}{}
    void execute() override{
        if(*r%2==0) (*pos)+=offset;
        else (*pos)++;
    }
};

struct jio : public Instruction{
    unsigned int* r{nullptr};
    int offset{0};
    jio(int* _pos, unsigned int* _r, int _offset) : Instruction{_pos}, r{_r}, offset{_offset}{}
    void execute() override{
        if(*r==1) (*pos)+=offset;
        else (*pos)++;
    }
};

struct MyComputer{
    int pos{0};
    std::vector<std::unique_ptr<Instruction>> program{};
    std::array<unsigned int, 2> reg = {0,0};

    MyComputer(std::vector<std::string> input){
        for(const auto& row : input){
            const auto split = Utilities::split(row);
            if(split[0] == "hlf"){
                program.emplace_back(std::make_unique<hlf>(&pos, &reg[0]+split[1][0]-'a'));
            }
            else if(split[0] == "tpl"){
                program.emplace_back(std::make_unique<tpl>(&pos, &reg[0]+split[1][0]-'a'));
            }
            else if(split[0] == "inc"){
                program.emplace_back(std::make_unique<inc>(&pos, &reg[0]+split[1][0]-'a'));
            }
            else if(split[0] == "jmp"){
                program.emplace_back(std::make_unique<jmp>(&pos, std::stoi(split[1])));
            }
            else if(split[0] == "jie"){
                program.emplace_back(std::make_unique<jie>(&pos, &reg[0]+split[1][0]-'a', std::stoi(split[2])));
            }
            else{ //jio
                program.emplace_back(std::make_unique<jio>(&pos, &reg[0]+split[1][0]-'a', std::stoi(split[2])));
            }
        }
    }

    void execute(){
        do{
            program[pos]->execute();
        }while( 0 <= pos && std::cmp_less(pos,program.size()));
    }

    void reset(){
        pos = 0;
        reg = {0,0};
    }
};


int main(){
    MyComputer computer{readFile::vectorOfStrings("input.txt")};

    //Task 1
    computer.execute();
    const auto registerAfterExecution1 = computer.reg[1];
    std::cout << "The value of register b at the end of the program is " << registerAfterExecution1 << ".\n";

    //Task 2
    computer.reset();
    computer.reg[0]=1;
    computer.execute();
    const auto registerAfterExecution2 = computer.reg[1];
    std::cout << "With register a starting at 1, the value of the b reg is " << registerAfterExecution2 << ".\n";

    VerifySolution::verifySolution(registerAfterExecution1, registerAfterExecution2);
}