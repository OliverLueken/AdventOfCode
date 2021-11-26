
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <memory>
#include <numeric>

class Display{
    std::array<std::array<int, 50>, 6> display{};

public:
    void rectangle(const unsigned int rows, const unsigned int cols){
        std::ranges::for_each_n(std::begin(display), cols,
            [rows](auto& row){ std::ranges::fill_n(row.begin(), rows, 1); }
        );
    }

    void rotateRow(const int row, const int shift){
        const auto rowLength = display[0].size();
        auto rowRange = display | std::views::join | std::views::drop(rowLength*row) | std::views::take(rowLength);
        std::ranges::rotate(rowRange, std::views::drop(rowRange, rowLength-shift).begin() );
    }

    void rotateCol(const int col, const int shift){
        auto nthElement = [&col, this](const auto i) -> int& { return this->display[i][col]; };
        const auto colLength = display.size();
        auto colRange = std::views::iota(0u, colLength) | std::views::transform(nthElement);
        std::ranges::rotate( colRange, std::views::drop(colRange, colLength-shift).begin() );
    }

    void print() const {
        for(const auto& col : display){
            for(const auto val : col){
                std::cout << (val == 0 ? ' ' : '#');
            }
            std::cout << '\n';
        }
        std::cout<< '\n';
    }

    unsigned int countOn() const {
        auto range = display | std::views::join;
        return std::accumulate(range.begin(), range.end(), 0u);
    }
};

class Operation{
public:
    virtual void operator()(Display*) const = 0;
};

class rectOperation : public Operation{
    int rows{}, cols{};
public:
    rectOperation(const int rows, const int cols) : rows{rows}, cols{cols}{}
    void operator()(Display* display) const override {
        display->rectangle(rows, cols);
    }
};

class rotRowOperation : public Operation{
    int row{}, shift{};
public:
    rotRowOperation(const int row, const int shift) : row{row}, shift{shift}{}
    void operator()(Display* display) const override {
        display->rotateRow(row, shift);
    }
};

class rotColOperation : public Operation{
    int col{}, shift{};
public:
    rotColOperation(const int col, const int shift) : col{col}, shift{shift}{}
    void operator()(Display* display) const override {
        display->rotateCol(col, shift);
    }
};

auto parseOperations = [](const auto& input){
    std::vector<std::unique_ptr<Operation>> operations{};
    for(const auto& op : input){
        const auto split = Utilities::splitOnEach(op, " x=");
        if(split[0]=="rect"){
            operations.emplace_back( std::make_unique<rectOperation>  (std::stoi(split[1]), std::stoi(split[2])) );
        }
        else if(split[1] == "row"){
            operations.emplace_back( std::make_unique<rotRowOperation>(std::stoi(split[3]), std::stoi(split[5])) );
        }
        else{ //col
            operations.emplace_back( std::make_unique<rotColOperation>(std::stoi(split[2]), std::stoi(split[4])) );
        }
    }
    return operations;
};

int main(){
    const auto operations = parseOperations(readFile::vectorOfStrings("input.txt"));

    Display display{};
    std::ranges::for_each(operations,
        [&display](const auto& opPtr){
            (*opPtr)(&display);
        }
    );

    //Task 1
    std::cout << "There are a total of " << display.countOn() << " pixels lit.\n\n";

    //Task 2
    display.print();
}