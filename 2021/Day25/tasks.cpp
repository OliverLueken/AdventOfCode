
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <ranges>

enum class seaFloorOccupant {Nothing, EastFacingSeaCucumber, SouthFacingSeaCucumber};

auto parseInput = [](const auto& input){
    Matrix::Matrix<seaFloorOccupant> seaCucumberPositions{input.size(), input[0].size(),
        input | std::views::join | std::views::transform( [](const auto c){
            switch(c){
                break; case '>': return seaFloorOccupant::EastFacingSeaCucumber;
                break; case 'v': return seaFloorOccupant::SouthFacingSeaCucumber;
                break; case '.': return seaFloorOccupant::Nothing;
            }
            return seaFloorOccupant::Nothing;
        }
    )};
    return seaCucumberPositions;
};

auto moveSeaCucumbers = [](auto& seaCucumberPositions){
    Matrix::Matrix<seaFloorOccupant> newSeaCucumberPositions{seaCucumberPositions.rows(), seaCucumberPositions.cols(), seaFloorOccupant::Nothing};
    auto cucumbersMoved = false;
    // moving the east facinc sea cucumbers first
    for(auto i=0u; i<seaCucumberPositions.rows(); i++){
        for(auto j=0u; j<seaCucumberPositions.cols(); j++){
            if(seaCucumberPositions(i,j) == seaFloorOccupant::EastFacingSeaCucumber){
                if(seaCucumberPositions(i, (j+1)%seaCucumberPositions.cols() ) == seaFloorOccupant::Nothing ){
                    newSeaCucumberPositions(i, (j+1)%seaCucumberPositions.cols()) = seaFloorOccupant::EastFacingSeaCucumber;
                    cucumbersMoved=true;
                }
                else{
                    newSeaCucumberPositions(i,j) = seaFloorOccupant::EastFacingSeaCucumber;
                }
            }
        }
    }
    //moving the south facing sea cucumbers
    for(auto i=0u; i<seaCucumberPositions.rows(); i++){
        for(auto j=0u; j<seaCucumberPositions.cols(); j++){
            if(seaCucumberPositions(i,j) == seaFloorOccupant::SouthFacingSeaCucumber){
                if(newSeaCucumberPositions((i+1)%seaCucumberPositions.rows(), j ) ==seaFloorOccupant::Nothing
                &&    seaCucumberPositions((i+1)%seaCucumberPositions.rows(), j ) !=seaFloorOccupant::SouthFacingSeaCucumber ){
                    newSeaCucumberPositions((i+1)%seaCucumberPositions.rows(), j) = seaFloorOccupant::SouthFacingSeaCucumber;
                    cucumbersMoved=true;
                }
                else{
                    newSeaCucumberPositions(i,j) = seaFloorOccupant::SouthFacingSeaCucumber;
                }
            }
        }
    }
    std::swap(newSeaCucumberPositions, seaCucumberPositions);
    return cucumbersMoved;
};

auto countStepsUntilEquilibrium = [](auto& seaCucumberPositions){
    auto steps=1u;
    while(moveSeaCucumbers(seaCucumberPositions)){
        steps++;
    }
    return steps;
};


int main(){
    auto seaCucumberPositions = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto stepsUntilEquilibrium = countStepsUntilEquilibrium(seaCucumberPositions);
    std::cout << "It takes " << stepsUntilEquilibrium << " steps until the sea cucumbers stop moving.\n";

    VerifySolution::verifySolution(stepsUntilEquilibrium);
}