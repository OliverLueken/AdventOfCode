
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <vector>
#include <memory>

using Computer_ = Computer::Computer<int>;

struct ExecutionLogger : public Computer::Logger {
    Computer_* computer{nullptr};
    unsigned int executionCount{};

    ExecutionLogger(Computer_* comp) : computer{comp}{
        computer->addLogger(this);
    }

    void update() override {
        ++executionCount;
    }
};

template<size_t mode>
auto makeJumpInstruction(const int offset, Computer_* comp);


template<>
auto makeJumpInstruction<0>(const int offset, Computer_* comp){
    auto jump = [_offset = offset, comp] () mutable {
        comp->advanceCurrentPosition(_offset);
        ++_offset;
    };
    return jump;
}

template<>
auto makeJumpInstruction<1>(const int offset, Computer_* comp){
    auto jump = [_offset = offset, comp] () mutable {
        comp->advanceCurrentPosition(_offset);
        if(_offset>=3) --_offset;
        else           ++_offset;
    };
    return jump;
}

template<size_t mode>
struct ComputerFactory{
    static Computer_ make(const auto& input){
        Computer_ comp{};

        for(const auto& offset : input){
            comp.add(makeJumpInstruction<mode>(offset, &comp));
        }
        return comp;
    }
};

template<size_t mode>
auto getStepCount = [](const auto& input){
    auto computer = ComputerFactory<mode>::make(input);

    auto logger = ExecutionLogger{&computer};
    computer.execute();
    return logger.executionCount;
};


int main(){
    const auto input = readFile::vectorOfInts();

    //Task 1
    const auto firstStepCount = getStepCount<0>(input);
    std::cout << "It takes " << firstStepCount << " steps to reach the exit.\n";

    //Task 2
    const auto secondStepCount = getStepCount<1>(input);
    std::cout << "With the odd jumpts it takes " << secondStepCount << " steps to reach the exit.\n";

    VerifySolution::verifySolution(firstStepCount, secondStepCount);
}