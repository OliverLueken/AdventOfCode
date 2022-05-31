
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
using Computer_ = Computer::Computer<Register>;

struct RegisterValueLogger : public Computer::Logger {
    Computer_* computer{nullptr};
    std::vector<int> maxRegisterValueAfterEachExecution{};

    RegisterValueLogger(Computer_* comp) : computer{comp}{
        computer->addLogger(this);
    }

    void update() override {
        maxRegisterValueAfterEachExecution.push_back(
            std::ranges::max( computer->getRegisterView() | std::views::values )
        );
    }
};



struct ComputerFactory{
    static auto getComparison(const auto comparator) -> std::function<bool(int, int)> const {
        if(comparator == "<")  return std::less<>{};
        if(comparator == "<=") return std::less_equal<>{};
        if(comparator == ">")  return std::greater<>{};
        if(comparator == ">=") return std::greater_equal<>{};
        if(comparator == "==") return std::equal_to<>{};
        return std::not_equal_to<>{};
    }

    static auto addIf(const auto comparator, const auto _regAddress, const auto _val, Computer_* comp){
        auto _conditional = getComparison(comparator);
        auto ifLambda = [regAddress = _regAddress, conditional = _conditional, val = _val, comp](){
            if(conditional( comp->getRegisterPtr()->operator[](regAddress), val )){
                comp->advanceCurrentPosition(1);
            }
            else{
                comp->advanceCurrentPosition(2);
            }
        };
        return comp->add(std::move(ifLambda));
    }

    static auto addRegisterModification(const auto regAddress, const auto mode, const auto amount, Computer_* comp){
        if(mode == "inc") return addRegisterIncrease(regAddress, amount, comp);
        if(mode == "dec") return addRegisterDecrease(regAddress, amount, comp);
    }

    static auto addRegisterIncrease(const auto _regAddress, const auto _amount, Computer_* comp){
        auto increase = [regAddress = _regAddress, amount = _amount, comp] () mutable {
            comp->getRegisterPtr()->operator[](regAddress)+=amount;
            comp->advanceCurrentPosition(1);
        };
        return comp->add(std::move(increase));
    }

    static auto addRegisterDecrease(const auto _regAddress, const auto _amount, Computer_* comp){
        return addRegisterIncrease(_regAddress, -1*_amount, comp);
    }


    static auto make(const auto& instructions){
        auto comp = Computer_{};
        for(const auto& instr : instructions){
            auto split = Utilities::split(instr);
            addIf(split[5], split[4], std::stoi(split[6]), &comp);
            addRegisterModification(split[0], split[1], std::stoi(split[2]), &comp);
        }
        return comp;
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
    auto computer = ComputerFactory::make(readFile::vectorOfStrings());

    //Task 1
    const auto [maxRegisterValueAfterExecution, maxRegisterValueDuringExecution] = getHighestRegisterValues(computer);
    std::cout << "The largest register value after execution is " << maxRegisterValueAfterExecution << ".\n";

    //Task 2
    std::cout << "While executing the largest register value was " << maxRegisterValueDuringExecution << ".\n";

    VerifySolution::verifySolution(maxRegisterValueAfterExecution, maxRegisterValueDuringExecution);
}