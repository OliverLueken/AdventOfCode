
#include "../../readFile.h"
#include "../../utilities.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <utility>

struct Instruction{
    int* pos{nullptr};
    Instruction(int* pos) : pos{pos}{}
    virtual void execute() = 0;
};

struct hlf : public Instruction{
    unsigned int* r{nullptr};
    hlf(int* pos, unsigned int* r) : Instruction{pos}, r{r}{}
    void execute() override{
        (*r)/=2;
        (*pos)++;
    }
};

struct tpl : public Instruction{
    unsigned int* r{nullptr};
    tpl(int* pos, unsigned int* r) : Instruction{pos}, r{r}{}
    void execute() override{
        (*r)*=3;
        (*pos)++;
    }
};

struct inc : public Instruction{
    unsigned int* r{nullptr};
    inc(int* pos, unsigned int* r) : Instruction{pos}, r{r}{}
    void execute() override{
        (*r)++;
        (*pos)++;
    }
};

struct jmp : public Instruction{
    int offset{0};
    jmp(int* pos, int offset) : Instruction{pos}, offset{offset}{}
    void execute() override{
        (*pos)+=offset;
    }
};

struct jie : public Instruction{
    unsigned int* r{nullptr};
    int offset{0};
    jie(int* pos, unsigned int* r, int offset) : Instruction{pos}, r{r}, offset{offset}{}
    void execute() override{
        if(*r%2==0) (*pos)+=offset;
        else (*pos)++;
    }
};

struct jio : public Instruction{
    unsigned int* r{nullptr};
    int offset{0};
    jio(int* pos, unsigned int* r, int offset) : Instruction{pos}, r{r}, offset{offset}{}
    void execute() override{
        if(*r==1) (*pos)+=offset;
        else (*pos)++;
    }
};

struct Computer{
    int pos{0};
    std::vector<std::unique_ptr<Instruction>> program{};
    std::array<unsigned int, 2> reg = {0,0};

    Computer(std::vector<std::string> input){
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
    Computer computer{readFile::vectorOfStrings("input.txt")};

    //Task 1
    computer.execute();
    std::cout << "The value of register b at the end of the program is " << computer.reg[1] << ".\n";

    //Task 2
    computer.reset();
    computer.reg[0]=1;
    computer.execute();
    std::cout << "With register a starting at 1, the value of the b reg is " << computer.reg[1] << ".\n";

}