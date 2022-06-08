
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <vector>
#include <memory>

using JumpingComputer = Computer::Computer<int>;
using Factory = Computer::ComputerFactory<int>;


struct ExecutionLogger : public Computer::Logger {
    unsigned int executionCount{};

    ExecutionLogger(JumpingComputer* comp){
        comp->addLogger(this);
    }

    void update() override {
        ++executionCount;
    }
};

enum class JumpVariant{IncreasingJump, ResettingJump};


template<JumpVariant mode>
struct MyFactory : public Factory{

    auto addIncreasingJumpInstruction(const int offset){
        auto jump = [_offset = offset] (JumpingComputer* comp) mutable {
            comp->advanceCurrentPosition(_offset);
            ++_offset;
        };
        Factory::addCommand(std::move(jump));
    }

    auto addResettingJumpInstruction(const int offset){
        auto jump = [_offset = offset] (JumpingComputer* comp) mutable {
            comp->advanceCurrentPosition(_offset);
            if(_offset>=3) --_offset;
            else           ++_offset;
        };
        Factory::addCommand(std::move(jump));
    }

    void makeCommand(const std::string& offset) override {
        if constexpr(mode == JumpVariant::IncreasingJump){
            addIncreasingJumpInstruction(std::stoi(offset));
        }
        else{
            addResettingJumpInstruction(std::stoi(offset));
        }
    }
};

template<JumpVariant mode>
auto getStepCount = [](const auto& jumpOffsets){
    auto computer = MyFactory<mode>{}.make(jumpOffsets);

    auto logger = ExecutionLogger{&computer};
    computer.execute();
    return logger.executionCount;
};


int main(){
    const auto jumpOffsets = readFile::vectorOfStrings();

    //Task 1
    const auto firstStepCount = getStepCount<JumpVariant::IncreasingJump>(jumpOffsets);
    std::cout << "It takes " << firstStepCount << " steps to reach the exit.\n";

    //Task 2
    const auto secondStepCount = getStepCount<JumpVariant::ResettingJump>(jumpOffsets);
    std::cout << "With the odd jumpts it takes " << secondStepCount << " steps to reach the exit.\n";

    VerifySolution::verifySolution(firstStepCount, secondStepCount);
}