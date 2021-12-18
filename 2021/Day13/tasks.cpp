
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <unordered_set>

using Position = Utilities::Position<size_t>;

auto parseInput = [](const auto& input){
    std::vector<std::pair<char, size_t>> instructions;
    std::vector<Position> points{};

    for(const auto& row : input){
        const auto split = Utilities::splitOnEach(row, " ,=");
        if(split[0]!= "fold"){
            const auto j = std::stoul(split[0]);
            const auto i = std::stoul(split[1]);
            points.emplace_back(i, j);
        }
        else{
            instructions.emplace_back( split[2][0], std::stoul(split[3]) );
        }
    }

    return std::make_pair(points, instructions);
};

auto printPaper(const auto& points){

    auto max_i=std::ranges::max(points | std::views::elements<0>);
    auto max_j=std::ranges::max(points | std::views::elements<1>);

    Matrix::Matrix<int> paper{max_i+1,max_j+1};
    for(const auto& [i,j] : points){
        paper(i,j) = 1;
    }

    for(auto i=0u; i<paper.rows(); i++){
        const auto row = paper.row(i);
        for(const auto i : row){
            std::cout << ( i>0 ? '#' : ' ' );
        }
        std::cout << '\n';
    }
}

auto foldNumber(auto i, auto b){
    return i<b ? i : 2*b-i;
}

auto fold(auto& points, const auto& instruction){
    std::vector<Position> foldedPoints{};
    if(instruction.first=='y'){ //horizontal folding (first coordinate needs folding)
        std::ranges::transform(points, std::back_inserter(foldedPoints),
            [&instruction](const auto& point){
                const auto& [i,j] = point;
                return Position{ foldNumber(i, instruction.second), j };
            }
        );
    }
    else{ //vertical folding
        std::ranges::transform(points, std::back_inserter(foldedPoints),
            [&instruction](const auto& point){
                const auto& [i,j] = point;
                return Position{ i, foldNumber(j, instruction.second) };
            }
        );
    }
    std::swap(points, foldedPoints);
}

auto getVisibleDotsAfterOneFold = [](auto& points, const auto& instruction){
    fold(points, instruction);
    std::unordered_set<Position> pointSet{};
    for(const auto& point : points){
        pointSet.insert(point);
    }
    return pointSet.size();
};


auto finishFolding = [](auto& points, const auto& instructions){
    for(const auto& i : instructions){
        fold(points, i);
    }
};


int main(){
    auto [points, instructions] = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto visibleDotsAfterOneFold = getVisibleDotsAfterOneFold(points, instructions[0]);
    std::cout << "After one fold, there are " << visibleDotsAfterOneFold << " dots visible.\n";

    //Task 2
    finishFolding(points, instructions | std::views::drop(1));
    std::cout << "After finishing folding, the following letters are readable:\n";
    printPaper(points);
}