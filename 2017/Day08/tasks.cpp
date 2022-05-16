
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <vector>
#include <memory>

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
        Wrapper(Lambda &&lambda): Lambda{std::forward<Lambda>(lambda)} {}
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

    auto addIf(const auto condition, const auto regAddress, const auto val, const auto registerModificationInstr){
        return instructions.size();
    }

    auto addRegisterModification(const auto regAddress, const auto mode, const auto amount){
        if(mode == "inc") return addRegisterIncrease(regAddress, amount);
        if(mode == "dec") return addRegisterDecrease(regAddress, amount);
        return instructions.size();
    }

    auto addRegisterIncrease(const auto _regAddress, const auto _amount){
        auto increase = [regAddress = _regAddress, amount = _amount, this] () mutable {
            this->reg[regAddress]+=amount;
            ++this->currentInstructionPosition;
        };
        return add(std::move(increase));
        // instructions.emplace_back(std::make_unique<Wrapper<decltype(increase)>>(std::move(increase)));
        // return instructions.size();
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
};

auto split(auto& instr){
    const auto i = instr.find(" if ");
    return std::make_pair(instr.substr(0, i), Utilities::split(instr.substr(i+4)));
}

auto parseInput(const auto& input){
    auto comp = Computer{};
    for(const auto& instr : input){
        auto [ifInstructions, condition] = split(instr);
        comp.addIf(condition[1], condition[0], condition[2], ifInstructions);
    }
    return comp;
}

auto getResult1 = [](const auto& input){
    return 0;
};

auto getResult2 = [](const auto& input){
    return 0;
};

int main(){
    const auto input = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result1 = getResult1(input);
    std::cout << "Task 1: " << result1 << ".\n";

    // //Task 2
    // const auto result2 = getResult2(input);
    // std::cout << "Task 2: " << result2 << ".\n";
    //
    // VerifySolution::verifySolution(result1, result2);
}