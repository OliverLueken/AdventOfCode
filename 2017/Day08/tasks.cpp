
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ranges>
#include <functional>
#include <algorithm>

using Register = std::unordered_map<std::string, int>;
using DataComputer = Computer::Computer<Register>;
using Factory = Computer::ComputerFactory<Register>;


struct RegisterValueLogger : public Computer::Logger {
    DataComputer* computer{nullptr};
    std::vector<int> maxRegisterValueAfterEachExecution{};

    RegisterValueLogger(DataComputer* comp) : computer{comp}{
        computer->addLogger(this);
    }

    void update() override {
        maxRegisterValueAfterEachExecution.push_back(
            std::ranges::max( computer->getDataView() | std::views::values )
        );
    }
};



struct MyFactory : public Factory{
    auto getComparison(const auto comparator) -> std::function<bool(int, int)> const {
        if(comparator == "<")  return std::less<>{};
        if(comparator == "<=") return std::less_equal<>{};
        if(comparator == ">")  return std::greater<>{};
        if(comparator == ">=") return std::greater_equal<>{};
        if(comparator == "==") return std::equal_to<>{};
        return std::not_equal_to<>{};
    }

    auto addIf(const auto comparator, const auto _regAddress, const auto _val){
        auto _conditional = getComparison(comparator);
        auto ifLambda = [regAddress = _regAddress, conditional = _conditional, val = _val](DataComputer* comp){
            if(conditional( comp->getData()[regAddress], val )){
                comp->advanceCurrentPosition(1);
            }
            else{
                comp->advanceCurrentPosition(2);
            }
        };
        Factory::addCommand(std::move(ifLambda));
    }

    auto addRegisterModification(const auto regAddress, const auto mode, const auto amount){
        if(mode == "inc") return addRegisterIncrease(regAddress, amount);
        if(mode == "dec") return addRegisterDecrease(regAddress, amount);
    }

    auto addRegisterIncrease(const auto _regAddress, const auto _amount){
        auto increase = [regAddress = _regAddress, amount = _amount] (DataComputer* comp) mutable {
            comp->getData()[regAddress]+=amount;
            comp->advanceCurrentPosition(1);
        };
        Factory::addCommand(std::move(increase));
    }

    auto addRegisterDecrease(const auto _regAddress, const auto _amount){
        return addRegisterIncrease(_regAddress, -1*_amount);
    }


    void makeCommand(const std::string& instr) override {
        auto split = Utilities::split(instr);
        addIf(split[5], split[4], std::stoi(split[6]));
        addRegisterModification(split[0], split[1], std::stoi(split[2]));
    }
};

auto getHighestRegisterValues = [](auto& computer){
    auto logger = RegisterValueLogger{&computer};

    computer.execute();

    const auto maxRegisterValueAfterExecution  = logger.maxRegisterValueAfterEachExecution.back();
    const auto maxRegisterValueDuringExecution = std::ranges::max( logger.maxRegisterValueAfterEachExecution );
    return std::make_pair(maxRegisterValueAfterExecution, maxRegisterValueDuringExecution);
};

int main(){
    auto computer = MyFactory{}.make(readFile::vectorOfStrings());

    //Task 1
    const auto [maxRegisterValueAfterExecution, maxRegisterValueDuringExecution] = getHighestRegisterValues(computer);
    std::cout << "The largest register value after execution is " << maxRegisterValueAfterExecution << ".\n";

    //Task 2
    std::cout << "While executing the largest register value was " << maxRegisterValueDuringExecution << ".\n";

    VerifySolution::verifySolution(maxRegisterValueAfterExecution, maxRegisterValueDuringExecution);
}