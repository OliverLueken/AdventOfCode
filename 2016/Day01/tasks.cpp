
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/computer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <unordered_set>



using namespace std::complex_literals;
struct Data{
    std::complex<int> facing{1i};
    std::complex<int> currentPosition{0};
};
using DataComputer = Computer::Computer<Data>;
using Factory = Computer::ComputerFactory<Data>;

struct PositionLogger : Computer::Logger{
    std::unordered_set<std::complex<int>> visitedPlaces{0};

    void update() override{
        
    }
};

auto infNorm(const auto& z){
    return std::abs(z.real()) + std::abs(z.imag());
}

struct MyFactory : public Factory{
    auto addTurnAndMove(const auto turnDirection, const auto numberOfBlocksToWalk){
        auto turnAndMove = [turnDirection, numberOfBlocksToWalk](DataComputer* compPtr){
            auto& facing = compPtr->getData().facing;
            facing*= turnDirection;
            compPtr->getData().currentPosition += facing*numberOfBlocksToWalk;
            compPtr->advanceCurrentPosition();
        };
        Factory::addCommand(std::move(turnAndMove));
    }
    void makeCommand(const std::string& instr) override {
        const auto turnDirection = instr[0]=='L' ? 1i : -1i;
        const auto numberOfBlocksToWalk = std::stoi(instr.substr(1));
        addTurnAndMove(turnDirection, numberOfBlocksToWalk);
    }
};

auto followInstruction = [](const auto& instruction, auto& facing, auto& currentPosition){
    using namespace std::complex_literals;

    const auto turnDirection = instruction[0];
    facing*= turnDirection=='L' ? 1i : -1i;

    const auto numberOfBlocksToWalk = std::stoi(instruction.substr(1));
    currentPosition += facing*numberOfBlocksToWalk;
};

auto distanceToDestination = [](const auto& instructions){
    auto computer = MyFactory{}.make(instructions);
    computer.execute();
    return infNorm(computer.getData().currentPosition);
};


auto distanceToFirstPlaceVisitedTwice = [](const auto& instructions){
    using namespace std::complex_literals;
    std::unordered_set<std::complex<int>> visitedPlaces{0};

    std::complex<int> facing{1i};
    std::complex<int> currentPosition{0}, nextPosition{0};
    for(const auto& instruction : instructions){
        nextPosition = currentPosition;
        followInstruction(instruction, facing, nextPosition);
        while(currentPosition != nextPosition){
            currentPosition+=facing;
            const auto inserted = visitedPlaces.insert(currentPosition).second;
            if(!inserted){
                return infNorm(currentPosition);
            }
        }
    }
    return 0;
};

int main(){
    const auto instructions = Utilities::split(readFile::string("input.txt"));

    //Task 1
    const auto distance = distanceToDestination(instructions);
    std::cout << "The Easter Bunny HQ is " << distance << " blocks away.\n";

    //Task 2
    const auto trueDistance = distanceToFirstPlaceVisitedTwice(instructions);
    std::cout << "The Easter Bunny HQ is actually " << trueDistance << " blocks away.\n";

    VerifySolution::verifySolution(distance, trueDistance);
}