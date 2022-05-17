
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ranges>

class Computer{

    struct Instruction{
        Instruction() = default;
        Instruction(const Instruction&) = default;
        Instruction& operator=(const Instruction&) = default;
        Instruction(Instruction&&) = default;
        Instruction& operator=(Instruction&&) = default;
        virtual ~Instruction() = default;

        virtual void execute() = 0;
    };

    template<typename Lambda>
    struct Wrapper : public Instruction, public Lambda{
        Wrapper(Lambda&& lambda): Lambda{std::forward<Lambda>(lambda)} {}
        void execute() override { Lambda::operator()(); }
    };

    using Register = std::unordered_map<std::string, int>;
    using Instructions = std::vector<std::unique_ptr<Instruction>>;

    int currentInstructionPosition{0};
    Instructions instructions{};
    Register reg{};

    template<typename Lambda>
    auto add(Lambda&& l){
        instructions.emplace_back(std::make_unique<Wrapper<Lambda>>(std::forward<Lambda>(l)));
    }

    auto getComparison(const auto comparator) -> std::function<bool(int, int)>{
        if(comparator == "<")  return std::less<>{};
        if(comparator == "<=") return std::less_equal<>{};
        if(comparator == ">")  return std::greater<>{};
        if(comparator == ">=") return std::greater_equal<>{};
        if(comparator == "==") return std::equal_to<>{};
        return std::not_equal_to<>{};
    }

public:
    auto addJump(const auto offset){
        auto jump = [_offset = offset, this] () mutable {
            this->currentInstructionPosition+=_offset;
            // ++_offset;
        };
        return add(std::move(jump));
    }
    //
    // void addOddJump(const auto offset){
    //     auto jump = [_offset = offset, this] () mutable {
    //         this->currentInstructionPosition+=_offset;
    //         if(_offset>=3) --_offset;
    //         else           ++_offset;
    //     };
    //     instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    // }

    auto addIf(const auto comparator, const auto _regAddress, const auto _val){
        auto _conditional = getComparison(comparator);
        auto ifLambda = [regAddress = _regAddress, conditional = _conditional, val = _val, this](){
            if(conditional( this->reg[regAddress], val )){
                ++this->currentInstructionPosition;
            }
            else{
                this->currentInstructionPosition+=2;
            }
        };
        return add(std::move(ifLambda));
    }

    auto addRegisterModification(const auto regAddress, const auto mode, const auto amount){
        if(mode == "inc") return addRegisterIncrease(regAddress, amount);
        if(mode == "dec") return addRegisterDecrease(regAddress, amount);
    }

    auto addRegisterIncrease(const auto _regAddress, const auto _amount){
        auto increase = [regAddress = _regAddress, amount = _amount, this] () mutable {
            this->reg[regAddress]+=amount;
            ++this->currentInstructionPosition;
        };
        return add(std::move(increase));
    }

    auto addRegisterDecrease(const auto _regAddress, const auto _amount){
        return addRegisterIncrease(_regAddress, -1*_amount);
    }

    bool currentInstructionPositionIsValid(){
        return 0<=currentInstructionPosition && std::less{}(currentInstructionPosition, instructions.size());
    }

    auto execute(){
        for(currentInstructionPosition = 0; currentInstructionPositionIsValid();){
            instructions[currentInstructionPosition]->execute();
        }
    }

    auto getRegisterView() const {
        return std::views::all(reg);
    }
};

auto parseInput(const auto& input){
    auto comp = Computer{};
    for(const auto& instr : input){
        auto split = Utilities::split(instr);
        comp.addIf(split[5], split[4], std::stoi(split[6]));
        comp.addRegisterModification(split[0], split[1], std::stoi(split[2]));
    }
    return comp;
}

auto getHighestRegisterValue = [](auto& computer){
    computer.execute();
    return std::ranges::max( computer.getRegisterView() | std::views::values );
};

auto getResult2 = [](const auto& computer){
    return 0;
};

int main(){
    auto computer = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result1 = getHighestRegisterValue(computer);
    std::cout << "Task 1: " << result1 << ".\n";

    // //Task 2
    // const auto result2 = getResult2(computer);
    // std::cout << "Task 2: " << result2 << ".\n";
    //
    // VerifySolution::verifySolution(result1, result2);
}