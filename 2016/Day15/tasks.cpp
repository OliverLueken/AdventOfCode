
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

struct Parameter{
    int n{};
    int b{};
};

auto parseInput = [](const auto& input){
    std::vector<Parameter> parameter{};
    auto i=1;
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        auto n = std::stoi(split[3]);
        auto p = std::stoi(split[11]);
        auto b = (n-i-p)%n;
        parameter.emplace_back(n, b);
        i++;
    }
    return parameter;
};

auto solveCRM = [](const auto& parameter){
    const auto getXi = [](const auto N, const auto n){
        for(auto x=0u; std::cmp_less(x,n) ; x++){
            if((x*N)%n==1) return x;
        }
        return 0u;
    };
    const auto N = std::transform_reduce(std::begin(parameter), std::end(parameter), 1u, std::multiplies<>(),
        [](const auto& param){ return param.n;}
    );
    auto x = 0u;
    for(const auto& p : parameter){
        const auto N_i = N/p.n;
        const auto x_i = getXi(N_i, p.n);
        x+=x_i*N_i*p.b;
    }
    return x%N;
};

int main(){
    auto parameter = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto timeToDrop = solveCRM(parameter);
    std::cout << "The time to drop the ball is at t=" <<timeToDrop <<  "\n";

    //Task 2
    auto n = 11;
    auto p = 0;
    auto b = (n-parameter.size()-1-p)%n;
    parameter.emplace_back(n, b);
    const auto timeToDrop2 = solveCRM(parameter);
    std::cout << "The time to drop the ball is at t=" << timeToDrop2 <<  "\n";

    VerifySolution::verifySolution(timeToDrop, timeToDrop2);
}