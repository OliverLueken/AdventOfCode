
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

    void addJump(const auto offset){
        auto jump = [_offset = offset, this] () mutable {
            this->currentInstructionPosition+=_offset;
            ++_offset;
        };
        instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    }

    void addOddJump(const auto offset){
        auto jump = [_offset = offset, this] () mutable {
            this->currentInstructionPosition+=_offset;
            if(_offset>=3) --_offset;
            else           ++_offset;
        };
        instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    }

    bool currentInstructionPositionIsValid(){
        return 0<=currentInstructionPosition && std::less{}(currentInstructionPosition, instructions.size());
    }

    auto countExecutions(){
        auto count = 0;
        for(currentInstructionPosition = 0; currentInstructionPositionIsValid();){
            instructions[currentInstructionPosition]->execute();
            ++count;
        }
        return count;
    }
};

auto getStepCount = [](const auto& input){
    auto computer = Computer{};
    for(const auto& offset : input){
        computer.addJump(offset);
    }
    return computer.countExecutions();
};

auto getStepCountWithOddJumps = [](const auto& input){
    auto computer = Computer{};
    for(const auto& offset : input){
        computer.addOddJump(offset);
    }
    return computer.countExecutions();
};

int main(){
    const auto input = readFile::vectorOfInts();

    //Task 1
    const auto firstStepCount = getStepCount(input);
    std::cout << "It takes " << firstStepCount << " steps to reach the exit.\n";

    //Task 2
    const auto secondStepCount = getStepCountWithOddJumps(input);
    std::cout << "With the odd jumpts it takes " << secondStepCount << " steps to reach the exit.\n";

    VerifySolution::verifySolution(firstStepCount, secondStepCount);
}