
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <vector>
#include <memory>

using JumpingComputer = Computer::Computer<int>;

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

template<JumpVariant>
auto makeJumpInstruction(const int offset);

template<>
auto makeJumpInstruction<JumpVariant::IncreasingJump>(const int offset){
    auto jump = [_offset = offset] (JumpingComputer* comp) mutable {
        comp->advanceCurrentPosition(_offset);
        ++_offset;
    };
    return jump;
}

template<>
auto makeJumpInstruction<JumpVariant::ResettingJump>(const int offset){
    auto jump = [_offset = offset] (JumpingComputer* comp) mutable {
        comp->advanceCurrentPosition(_offset);
        if(_offset>=3) --_offset;
        else           ++_offset;
    };
    return jump;
}


template<JumpVariant mode>
struct ComputerFactory{
    static JumpingComputer make_computer(const auto& jumpOffsets){
        auto comp = JumpingComputer{};

        for(const auto& offset : jumpOffsets){
            comp.add(makeJumpInstruction<mode>(offset));
        }
        return comp;
    }
};

template<JumpVariant mode>
auto getStepCount = [](const auto& jumpOffsets){
    auto computer = ComputerFactory<mode>::make_computer(jumpOffsets);

    auto logger = ExecutionLogger{&computer};
    computer.execute();
    return logger.executionCount;
};


int main(){
    const auto jumpOffsets = readFile::vectorOfInts();

    //Task 1
    const auto firstStepCount = getStepCount<JumpVariant::IncreasingJump>(jumpOffsets);
    std::cout << "It takes " << firstStepCount << " steps to reach the exit.\n";

    //Task 2
    const auto secondStepCount = getStepCount<JumpVariant::ResettingJump>(jumpOffsets);
    std::cout << "With the odd jumpts it takes " << secondStepCount << " steps to reach the exit.\n";

    VerifySolution::verifySolution(firstStepCount, secondStepCount);
}