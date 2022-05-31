
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
        // maxRegisterValueAfterEachExecution.push_back(
        //     std::ranges::max( computer->getRegisterView() | std::views::values )
        // );
    }
};

// class MyComp : public Computer::Computer<Register>{
// public:
    // struct Instruction{
    //     Instruction() = default;
    //     Instruction(const Instruction&) = default;
    //     Instruction& operator=(const Instruction&) = default;
    //     Instruction(Instruction&&) = default;
    //     Instruction& operator=(Instruction&&) = default;
    //     virtual ~Instruction() = default;
    //
    //     virtual void execute() = 0;
    // };
    //
    // template<typename Lambda>
    // struct Wrapper : public Instruction, public Lambda{
    //     Wrapper(Lambda &&lambda): Lambda{std::forward<Lambda>(lambda)} {}
    //     void execute() override { Lambda::operator()(); }
    // };
    //
    // std::vector<std::unique_ptr<Instruction>> instructions{};
    // int currentInstructionPosition{0};
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

    // void addOddJump(const auto offset){
    //     auto jump = [_offset = offset, comp] () mutable {
    //         comp->currentInstructionPosition+=_offset;
    //         if(_offset>=3) --_offset;
    //         else           ++_offset;
    //     };
    //     comp->add(std::move(jump));
    //     // instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    // }
};

struct ComputerFactory2{
    // static void addJump(const auto offset, Computer_* comp){
    //     auto jump = [_offset = offset, comp] () mutable {
    //         comp->currentInstructionPosition+=_offset;
    //         ++_offset;
    //     };
    //     comp->add(std::move(jump));
    // }

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
        // instructions.emplace_back(std::make_unique<Wrapper<decltype(jump)>>(std::move(jump)));
    }
};
    // bool currentInstructionPositionIsValid(){
    //     return 0<=currentInstructionPosition && std::less{}(currentInstructionPosition, instructions.size());
    // }

    // auto countExecutions(){
    //     auto count = 0;
    //     for(currentInstructionPosition = 0; currentInstructionPositionIsValid();){
    //         instructions[currentInstructionPosition]->execute();
    //         ++count;
    //     }
    //     return count;
    // }
// };

auto getStepCount = [](const auto& input){
    auto computer = ComputerFactory::make(input);

    // for(const auto& offset : input){
    //     computer.addJump(offset);
    // }
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