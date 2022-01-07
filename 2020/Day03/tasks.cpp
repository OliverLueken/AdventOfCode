
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <vector>
#include <ranges>

auto parseInput(const auto& input){
    return Matrix::Matrix<char>(input.size(), input[0].size(), input | std::views::join);
}

auto countTreesOnSlope(const auto& forest, const auto dx, const auto dy){
    auto count = 0ul;
    auto y = dy;
    auto x = dx;
    while(y < forest.rows()){
        count+=bool(forest(y, x%forest.cols()) == '#' );
        y+=dy;
        x+=dx;
    }
    return count;
}

auto multiplyTreeCountOfRemainingSlopes(const auto& forest){
    auto product  = countTreesOnSlope(forest, 1u, 1u);
         product *= countTreesOnSlope(forest, 5u, 1u);
         product *= countTreesOnSlope(forest, 7u, 1u);
         product *= countTreesOnSlope(forest, 1u, 2u);
    return product;
}

int main(){
    const auto forest = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto treeCount = countTreesOnSlope(forest, 3u, 1u);
    std::cout << "On slope (3,1) are " << treeCount << " trees.\n";

    //Task 2
    const auto allTreeSlopesProduct = treeCount * multiplyTreeCountOfRemainingSlopes(forest);
    std::cout << "The product of the trees on all slopes is " << allTreeSlopesProduct << " trees.\n";

}