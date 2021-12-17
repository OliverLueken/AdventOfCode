
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <ranges>
#include <cmath>

using Position     = Utilities::Position<int>;
// using positionHash = Utilities::positionHash;

auto parseInput = [](const auto& input){
    const auto split = Utilities::splitOnEach(input, "=.");
    const auto xBounds = Position(std::stoi(split[1]), std::stoi(split[2]));
    const auto yBounds = Position(std::stoi(split[3]), std::stoi(split[4]));
    return std::make_pair(xBounds, yBounds);
};

auto getHighestYPosition(const auto& yBounds){
    const auto maxYVelocity     = std::ranges::max(std::abs(yBounds.first), std::abs(yBounds.second))-1;
    const auto highestYPosition = maxYVelocity*(maxYVelocity+1)/2;
    return highestYPosition;
}

auto getVelocityToHitPosInNSteps(const auto posToHit, const auto steps){
    const auto div = std::div(2*posToHit+steps*(steps-1), 2*steps);
    return std::make_pair( div.rem==0, div.quot );
}

auto addValidVelocityValuesToHit(const auto xn, const auto yn, auto& validVelocityValues){
    //Adding angles that hit the target before velocity of probe's x directions reaches 0
    //That happens after nMax steps
    const auto nMax = static_cast<int>(0.5*(std::sqrt(1+8*xn)-1));
    for(auto steps=1; steps<=nMax; steps++){
        const auto [xVelocityIsInteger, xVelocity] = getVelocityToHitPosInNSteps(xn, steps);
        const auto [yVelocityIsInteger, yVelocity] = getVelocityToHitPosInNSteps(yn, steps);
        if(xVelocityIsInteger && yVelocityIsInteger) {
            validVelocityValues.emplace(xVelocity, yVelocity);
        }
    }

    //Adding angles that cause probe's x velocity to reach 0
    const auto xVelocity = nMax;
    if( 2*xn == xVelocity*(xVelocity+1) ){ //checks if xVelocity is an integer
        const auto mMax = 2*std::abs(yn)-nMax; //after nMax+mMax steps probes will never hit yn again
        for(auto m=1; m<=mMax; m++){
            const auto steps = nMax+m;
            const auto [yVelocityIsInteger, yVelocity] = getVelocityToHitPosInNSteps(yn, steps);
            if(yVelocityIsInteger) {
                validVelocityValues.emplace(xVelocity, yVelocity);
            }
        }
    }
}

auto getNumberOfValidVelocityValues = [](auto xBounds, auto yBounds){
    std::unordered_set<Position> validVelocityValues{};
    for(auto xn = xBounds.first; xn<=xBounds.second; xn++){
        for(auto yn = yBounds.first; yn<=yBounds.second; yn++){
            addValidVelocityValuesToHit(xn, yn, validVelocityValues);
        }
    }

    const auto numberOfValidVelocityValues = validVelocityValues.size();
    return numberOfValidVelocityValues;
};

int main(){
    const auto [xBounds, yBounds] = parseInput(readFile::string("input.txt"));

    //Task 1
    const auto highestYPosition = getHighestYPosition(yBounds);
    std::cout << "The highest y position the probe reaches is " << highestYPosition  << ".\n";

    //Task 2
    const auto numberOfValidVelocityValues = getNumberOfValidVelocityValues(xBounds, yBounds);
    std::cout << "There are " << numberOfValidVelocityValues << " possible initial velocity vectors.\n";


}