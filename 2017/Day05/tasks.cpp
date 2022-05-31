
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


struct ComputerFactory{
    static void addJump(const auto offset, Computer_* comp){
        auto jump = [_offset = offset, comp] () mutable {
            comp->advanceCurrentPosition(_offset);
            ++_offset;
        };
        comp->add(std::move(jump));
    }

    static Computer_ make(const auto& input){
        Computer_ comp{};

        for(const auto& offset : input){
            addJump(offset, &comp);
        }
        return comp;
    }
};

struct ComputerFactory2{

    static Computer_ make(const auto& input){
        Computer_ comp{};

        for(const auto& offset : input){
            addOddJump(offset, &comp);
        }
        return comp;
    }

    static void addOddJump(const auto offset, Computer_* comp){
        auto jump = [_offset = offset, comp] () mutable {
            comp->advanceCurrentPosition(_offset);
            if(_offset>=3) --_offset;
            else           ++_offset;
        };
        comp->add(std::move(jump));
    }
};

auto getStepCount = [](const auto& input){
    auto computer = ComputerFactory::make(input);
    
    auto logger = ExecutionLogger{&computer};
    computer.execute();
    return logger.executionCount;
};

auto getStepCountWithOddJumps = [](const auto& input){
    auto computer = ComputerFactory2::make(input);

    auto logger = ExecutionLogger{&computer};
    computer.execute();
    return logger.executionCount;
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