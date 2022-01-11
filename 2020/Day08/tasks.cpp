
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <variant>

struct acc{
    int value{};
};

struct jmp{
    int value{};
};

struct nop{
    int value{};
};

template<typename ... Base>
struct Executor : Base ... {
	using Base::operator()...;
};
template<typename ... T> Executor(T...) -> Executor<T...>;

struct Computer{
    int _reg{0};
    int _currentLine{0};
    std::vector<std::variant<acc, jmp, nop>> commands{};

    auto getRegister(){ return _reg; }
    auto getCurrentLine(){ return _currentLine; }

    void execute(){
        auto& reg         = _reg;
        auto& currentLine = _currentLine;
        Executor executor{
            [&reg, &currentLine](const acc& cmd) -> void { reg+=cmd.value; currentLine++;          },
            [&reg, &currentLine](const jmp& cmd) -> void {                 currentLine+=cmd.value; },
            [&reg, &currentLine](const nop&    ) -> void {                 currentLine++;          }
        };

        std::visit(executor, commands[currentLine]);
    }

    bool isInValidState(){
        return Utilities::isBetween(_currentLine, 0, (int)commands.size());
    }

    void reset(){
        _reg         = 0;
        _currentLine = 0;
    }
};

auto parseInput = [](const auto& input){
    std::vector<std::variant<acc, jmp, nop>> commands;
    for(const auto& row : input){
        const auto split = Utilities::split(row, ' ');
        const auto value = std::stoi(split[1]);
        if(split[0] == "acc"){
            commands.emplace_back(acc{value});
        }
        else if(split[0] == "jmp"){
            commands.emplace_back(jmp{value});
        }
        else{
            commands.emplace_back(nop{value});
        }
    }
    return commands;
};

auto runComputer(auto& computer){
    std::unordered_set<int> visitedLines{};
    while(!visitedLines.contains( computer.getCurrentLine() ) && computer.isInValidState() ){
        visitedLines.insert(computer.getCurrentLine());
        computer.execute();
    }
    return !computer.isInValidState();
}

auto getRegisterAfterFirstLoop = [](auto& computer){
    runComputer(computer);
    const auto reg = computer.getRegister();
    computer.reset();
    return reg;
};

auto getRegisterAfterSuccessfullyCorrectingProgram = [](auto& computer){
    for(auto& operation : computer.commands){
        auto tmp = operation;
        if(std::holds_alternative<jmp>(operation)){
            operation = nop{std::get<jmp>(operation).value};
        }
        else if(std::holds_alternative<nop>(operation)){
            operation = jmp{std::get<nop>(operation).value};
        }
        else{
            continue;
        }
        if(runComputer(computer)) break;
        operation = std::move(tmp);
        computer.reset();
    }
    return computer.getRegister();
};

int main(){
    Computer computer{0, 0, parseInput(readFile::vectorOfStrings("input.txt"))};

    //Task 1
    const auto registerAfterFirstLoop = getRegisterAfterFirstLoop(computer);
    std::cout << "The register after the first detected loop is " << registerAfterFirstLoop << ".\n";

    //Task 2
    const auto registerAferSuccessfulExecution = getRegisterAfterSuccessfullyCorrectingProgram(computer);
    std::cout << "The register after successfully correcting the program is " << registerAferSuccessfulExecution << ".\n";

    VerifySolution::verifySolution(registerAfterFirstLoop, registerAferSuccessfulExecution);
}