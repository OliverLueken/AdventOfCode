
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <algorithm>
#include <ranges>


auto parseInput = [](const auto& input){
    std::vector<std::pair<char, size_t>> instructions;
    std::vector<std::pair<size_t, size_t>> coords{};

    for(const auto& row : input){
        const auto split = Utilities::splitOnEach(row, " ,=");
        if(split[0]!= "fold"){
            const auto j = std::stoul(split[0]);
            const auto i = std::stoul(split[1]);
            coords.emplace_back(i, j);
        }
        else{
            instructions.emplace_back( split[2][0], std::stoul(split[3]) );
        }
    }
    auto max_i=std::ranges::max(coords | std::views::elements<0>);
    auto max_j=std::ranges::max(coords | std::views::elements<1>);

    Matrix::Matrix<int> paper{max_i+1,max_j+1};
    for(const auto& [i,j] : coords){
        paper(i,j) = 1;
    }

    return std::make_pair(paper, instructions);
};

auto foldHorizontal(auto& paper, auto iFoldingLine){
    for(auto j=0u; j<paper.cols(); j++){
        for(auto i=1; 0<=iFoldingLine-i && iFoldingLine+i<paper.rows(); i++){
            paper(iFoldingLine-i, j) += paper(iFoldingLine+i, j);
        }

        // works but very inefficient for now
        // auto range1 = paper.col(j) | std::views::take(iFoldingLine) | std::views::reverse;
        // auto range2 = paper.col(j) | std::views::drop(iFoldingLine+1);
        // std::ranges::transform( range1, range2, std::begin(range1), std::plus<>() );
    }
    paper.resize(iFoldingLine, paper.cols());
}

auto foldVertical(auto& paper, auto jFoldingLine){
    for(auto i=0u; i<paper.rows(); i++){
        auto range1 = paper.row(i) | std::views::take(jFoldingLine) | std::views::reverse;
        auto range2 = paper.row(i) | std::views::drop(jFoldingLine+1);
        std::ranges::transform( range1, range2, std::begin(range1), std::plus<>() );
    }
    paper.resize(paper.rows(), jFoldingLine);
}

auto printPaper(const auto& paper){
    for(auto i=0u; i<paper.rows(); i++){
        const auto row = paper.row(i);
        for(const auto i : row){
            std::cout << ( i>0 ? '#' : ' ' );
        }
        std::cout << '\n';
    }
}

auto fold(auto& paper, const auto& instructions){
    if(instructions.first=='y'){
        foldHorizontal(paper, instructions.second);
    }
    else{
        foldVertical(paper, instructions.second);
    }
}

auto getVisibleDotsAfterOneFold = [](auto& paper, const auto& instruction){
    fold(paper, instruction);
    return Utilities::sum(paper.data());
};


auto finishFolding = [](auto& paper, const auto& instructions){
    for(const auto& i : instructions){
        fold(paper, i);
    }
};

int main(){
    auto [paper, instructions] = parseInput(readFile::vectorOfStrings("input.txt"));

    const auto visibleDotsAfterOneFold = getVisibleDotsAfterOneFold(paper, instructions[0]);
    std::cout << "After one fold, there are " << visibleDotsAfterOneFold << " dots visible.\n";


    finishFolding(paper, instructions | std::views::drop(1));
    std::cout << "After finishing folding, the following letters are readable:\n";
    printPaper(paper);
}