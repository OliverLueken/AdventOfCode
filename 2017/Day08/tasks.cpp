
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <vector>
#include <memory>

struct Computer{

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

    std::vector<std::unique_ptr<Instruction>> instructions{};
    int currentInstructionPosition{0};

    // void addJump(const auto offset){
    //     auto jump = [_offset = offset, this] () mutable {
    //         this->currentInstructionPosition+=_offset;
    //         ++_offset;
    //     };
    //     instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    // }
    //
    // void addOddJump(const auto offset){
    //     auto jump = [_offset = offset, this] () mutable {
    //         this->currentInstructionPosition+=_offset;
    //         if(_offset>=3) --_offset;
    //         else           ++_offset;
    //     };
    //     instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    // }

    bool currentInstructionPositionIsValid(){
        return 0<=currentInstructionPosition && std::less{}(currentInstructionPosition, instructions.size());
    }

    auto execute(){
        for(currentInstructionPosition = 0; currentInstructionPositionIsValid();){
            instructions[currentInstructionPosition]->execute();
        }
    }
};

auto parseInput(const auto& input){
    return 0;
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