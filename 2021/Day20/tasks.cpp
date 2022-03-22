
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ranges>

using Position = Utilities::Position<int>;

auto parseInput = [](const auto& input){
    std::vector<bool> decoder{};
    std::ranges::transform(input[0], std::back_inserter(decoder),
        [](const auto c){return c=='#';}
    );

    std::unordered_map<Position, bool> image{};
    for(auto i=1; std::less{}(i, input.size()); i++){
        for(auto j=0; std::less{}(j, input[i].size()); j++){
            image[Position{i,j}] = input[i][j]=='#';
        }
    }
    return std::make_pair(image, decoder);
};

auto getNewValue(const auto& pos, const auto& image, const auto defaultVal){
    int val = 0;
    for(auto i=-1; i<=1; i++){
        for(auto j=-1; j<=1; j++){
            const auto neighborPos = Position{pos.first+i, pos.second+j};
            val<<=1;
            val+= (image.contains(neighborPos) ? image.at(neighborPos) : defaultVal );
        }
    }
    return val;
}

auto getBounds(const auto& image){
    const auto [leftBound, rightBound]  = std::ranges::minmax(image | std::views::keys | std::views::transform(&Position::first));
    const auto [upperBound, lowerBound] = std::ranges::minmax(image | std::views::keys | std::views::transform(&Position::second));
    return std::make_tuple(leftBound, rightBound, upperBound, lowerBound);
}

auto enhanceImage = [](const auto& decoder, auto& image, const auto defaultVal, const auto& bounds){
    std::unordered_map<Position, bool> newImage{};
    const auto& [leftBound, rightBound, upperBound, lowerBound] = bounds;
    for(auto i=leftBound; i<=rightBound; i++){
        for(auto j=upperBound; j<=lowerBound; j++){
            const auto pos = Position{i,j};
            const auto newValue = getNewValue(pos, image, defaultVal);
            newImage[pos] = decoder[newValue];
        }
    }
    std::swap(image, newImage);
};

auto updateBounds(auto& bounds){
    auto& [leftBound, rightBound, upperBound, lowerBound] = bounds;
    leftBound--;
    rightBound++;
    upperBound--;
    lowerBound++;
}

auto getLitPixelsAfterEnhancements(auto image, const auto& decoder, const int n){
    auto bounds = getBounds(image);
    const auto& [leftBound, rightBound, upperBound, lowerBound] = bounds;
    image.reserve( ((rightBound-leftBound)+2*n)*((lowerBound-upperBound)+2*n) );
    auto defaultVal = false;
    for(auto i=0; i<n; i++){
        updateBounds(bounds);
        enhanceImage(decoder, image, defaultVal, bounds);
        defaultVal = defaultVal ? decoder[251] : decoder[0];
    }
    return std::ranges::count(image | std::views::values, true);
}


int main(){
    const auto [image, decoder] = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto litPixelsAfterTwoEnhancements = getLitPixelsAfterEnhancements(image, decoder, 2);
    std::cout << "There are " << litPixelsAfterTwoEnhancements << " pixels lit after two enhancements.\n";

    //Task 2
    const auto litPixelsAfterFiftyEnhancements = getLitPixelsAfterEnhancements(image, decoder, 50);
    std::cout << "There are " << litPixelsAfterFiftyEnhancements << " pixels lit after fifty enhancements.\n";

    VerifySolution::verifySolution(litPixelsAfterTwoEnhancements, litPixelsAfterFiftyEnhancements);
}