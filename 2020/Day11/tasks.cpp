
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>

using Position                  = Utilities::Position<int>;
using Chairs                    = Matrix::Matrix<char>;
using VisibleNeighbors          = Matrix::Matrix<int>;
using getVisibleNeighborsLambda = std::function<VisibleNeighbors(const Chairs&)>;

auto parseInput = [](const auto& input){
    const auto n = input.size();
    const auto m = input[0].size();
    return Chairs(n, m, input | std::views::join);
};

class GameOfLife{
    bool seatsChanged{true};
    int neighborTolerance{};
    size_t N{};
    getVisibleNeighborsLambda getVisibleNeighbors{};
    Chairs chairs{};
    Chairs tempChairs{};

    auto playOneRound(){
        auto getSeatState = [this](const char currentSeat, const int visibleNeighbors){

            if(currentSeat == '.') return '.';

            if(currentSeat == 'L'){
                if (visibleNeighbors==0){
                    this->seatsChanged = true;
                    return '#';
                }
                return 'L';
            }
            if(visibleNeighbors>=this->neighborTolerance){
                this->seatsChanged = true;
                return 'L';
            }
            return '#';
        };

        const auto visibleNeighbors = getVisibleNeighbors(chairs);
        std::ranges::transform(chairs, visibleNeighbors, std::begin(tempChairs), getSeatState);
    }

public:
    GameOfLife(getVisibleNeighborsLambda getVisibleNeighbors_, const Chairs& chairs_, const int param_)
      : neighborTolerance{param_},
        N{chairs_.rows()*chairs_.cols()},
        getVisibleNeighbors{getVisibleNeighbors_},
        chairs{chairs_},
        tempChairs{chairs.rows(), chairs.cols()} {
    }

    void play(){
        while(seatsChanged){
            seatsChanged = false;
            playOneRound();
            std::swap(tempChairs, chairs);
        }
    }

    auto getOccupiedSeats(){
        return std::ranges::count(chairs, '#');
    }
};



auto getVisibleNeighbors1 = [](const Chairs& chairs){
    Matrix::Matrix<int> visibleNeighbors{chairs.rows(), chairs.cols(), 0};
    for(auto index = 0u; index<chairs.rows()*chairs.cols(); ++index){
        if(chairs[index]=='#'){
            std::ranges::for_each( getNeighborsIncludingDiagonals(visibleNeighbors, index),
                [&visibleNeighbors](const auto& neighbor){
                    ++visibleNeighbors(neighbor);
                }
            );
        }
    }
    return visibleNeighbors;
};

auto getVisibleNeighbors2 = [](const Chairs& chairs){
    const auto [n, m] = chairs.size();
    auto isInBounds = [n, m](const auto& position){
        const auto& [i,j] = position;
        return 0<=i && std::less{}(i,n) && 0<=j && std::less{}(j,m);
    };

    VisibleNeighbors visibleNeighbors{n, m, 0};
    constexpr std::array<Position, 8> directions{{ {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1} }};
    for(auto index = 0u; index<n*m; ++index){
        if(chairs[index] == '#'){
            const Position currentPos{index/m, index%m};
            auto incrementVisibleSeat = [isInBounds, &visibleNeighbors, &chairs, &currentPos](const auto& direction){
                auto pos = currentPos;
                do{
                    pos = pos+direction;
                    if(!isInBounds(pos)) return;
                }while(chairs(pos) == '.');
                ++visibleNeighbors(pos);
            };
            std::ranges::for_each(directions, incrementVisibleSeat);
        }
    }
    return visibleNeighbors;
};

auto getNumberOfOccupiedSeats = [](const auto& chairs, const auto neighborTolerance, auto& getVisibleNeighbors){
    GameOfLife gameOfLife{ getVisibleNeighbors, chairs, neighborTolerance };
    gameOfLife.play();
    return gameOfLife.getOccupiedSeats();
};


int main(){
    const auto chairs = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto numberOfOccupiedSeats1 = getNumberOfOccupiedSeats(chairs, 4, getVisibleNeighbors1);
    std::cout << "With the first ruleset, there are " << numberOfOccupiedSeats1 << " seats occupied.\n";

    //Task 2
    const auto numberOfOccupiedSeats2 = getNumberOfOccupiedSeats(chairs, 5, getVisibleNeighbors2);
    std::cout << "With the second ruleset, there are " << numberOfOccupiedSeats2 << " seats occupied.\n";

    VerifySolution::verifySolution(numberOfOccupiedSeats1, numberOfOccupiedSeats2);
}