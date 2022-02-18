
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <iostream>
#include <complex>
#include <cmath>


using Position = Utilities::Position<int>;

auto parseInput(const auto& input) {
    std::vector<std::pair<char, int>> navigationInstructions;
    std::ranges::transform(
        input, std::back_inserter(navigationInstructions), [](const auto& row) {
            return std::make_pair(row[0], std::stoi(row.substr(1)));
        });
    return navigationInstructions;
}

auto moveShip(const auto& navigationInstructions){

    using namespace std::complex_literals;
    std::complex<int> facingDirection{1};
    std::complex<int> shipPos{0};

    for(const auto& [c, val] : navigationInstructions){
        switch(c){
            break; case 'N':
                shipPos+=std::complex<int>{0,val};
            break; case 'S':
                shipPos-=std::complex<int>{0,val};
            break; case 'E':
                shipPos+=val;
            break; case 'W':
                shipPos-=val;
            break; case 'L':
                facingDirection*=std::exp(std::complex{0.,  val*std::numbers::pi/180});
            break; case 'R':
                facingDirection*=std::exp(std::complex{0., -val*std::numbers::pi/180});
            break; case 'F':
                shipPos+=facingDirection*val;
        }
    }
    return std::abs(shipPos.real()) + std::abs(shipPos.imag());
}

auto moveShip2(const auto& navigationInstructions){

    using namespace std::complex_literals;
    std::complex<double> shipPos{0};
    std::complex<double> waypointPos{10,1};

    for(const auto& [c, val_] : navigationInstructions){
        const auto val = static_cast<double>(val_);
        switch(c){
            break; case 'N':
                waypointPos+=std::complex<double>{0,val};
            break; case 'S':
                waypointPos-=std::complex<double>{0,val};
            break; case 'E':
                waypointPos+=val;
            break; case 'W':
                waypointPos-=val;
            break; case 'L':
                waypointPos*=std::exp(std::complex{0.,  val*std::numbers::pi/180});
            break; case 'R':
                waypointPos*=std::exp(std::complex{0., -val*std::numbers::pi/180});
            break; case 'F':
                shipPos+=waypointPos*std::complex<double>{0,val};
        }
    }
    return static_cast<int>(std::nearbyint(std::abs(shipPos.real()) + std::abs(shipPos.imag())));
}

int main() {
    const auto navigationInstructions = parseInput(readFile::vectorOfStrings());

    // Task 1
    const auto distanceTraveled1 = moveShip(navigationInstructions);
    std::cout << "With the first interpretation, the ship traveled " << distanceTraveled1
              << " units.\n";

    // Task 2
    const auto distanceTraveled2 = moveShip2(navigationInstructions);
    std::cout << "With the second ruleset, the ship traveled " << distanceTraveled2
              << " units.\n";

    VerifySolution::verifySolution(distanceTraveled1, distanceTraveled2);
}