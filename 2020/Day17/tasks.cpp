
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>

template<size_t N>
struct std::hash<std::array<int, N>>{
    size_t operator()(const std::array<int, N>& position) const noexcept{
        size_t hash_=0;
        for(const auto i : position){
            hash_=hash_^(i<<1);
        }
        return hash_;
    }
};



template<typename getVisibleNeighborsLambda, typename Population>
class GameOfLife{
    getVisibleNeighborsLambda getVisibleNeighbors{};
    Population population{};
    Population nextPopulation{};

    auto playOneRound(){
        auto populateNextPopulation = [this](const auto& visibleNeighborsPair){
            const auto& [currentObject, numberOfVisibleNeighbors] = visibleNeighborsPair;
            if(population.contains(currentObject)){
                if(Utilities::isBetween(numberOfVisibleNeighbors, 2, 4)){
                    nextPopulation.emplace(currentObject);
                }
            }
            else if(numberOfVisibleNeighbors == 3){
                nextPopulation.emplace(currentObject);
            }
        };

        const auto visibleNeighbors = getVisibleNeighbors(population);
        nextPopulation.reserve(visibleNeighbors.size());
        std::ranges::for_each(visibleNeighbors, populateNextPopulation);
    }

public:
    GameOfLife(getVisibleNeighborsLambda& getVisibleNeighbors_, Population&& population_)
      : getVisibleNeighbors{ std::move(getVisibleNeighbors_) },
        population{ std::move(population_) },
        nextPopulation{} {
    }

    void play(){
        for(auto i=0; i<6; ++i){
            playOneRound();
            std::swap(nextPopulation, population);
            nextPopulation.clear();
        }
    }

    auto getPopulationSize(){
        return population.size();
    }
};

template <typename L1, typename L2>
auto make_GameOfLife(L1 &&l1, L2 &&l2) {
    return GameOfLife<std::decay_t<L1>, std::decay_t<L2>>(
        std::forward<L1>(l1),
        std::forward<L2>(l2)
    );
}

auto incrementNeighbors = [](const auto& pos, auto& visibleNeighbors){
    auto incrementNeighbors_ = [&visibleNeighbors](const auto incrementNeighbors__, const auto& pos_, auto it, auto& neighbor){
        if(it == neighbor.size()){
            visibleNeighbors[neighbor]++;
            return;
        }
        neighbor[it] = pos_[it]-1;
        incrementNeighbors__(incrementNeighbors__, pos_, it+1, neighbor);
        neighbor[it]++;
        incrementNeighbors__(incrementNeighbors__, pos_, it+1, neighbor);
        neighbor[it]++;
        incrementNeighbors__(incrementNeighbors__, pos_, it+1, neighbor);
    };

    auto neighbor{pos};
    incrementNeighbors_(incrementNeighbors_, pos, 0lu, neighbor);
    visibleNeighbors[pos]--;
};

template<size_t N>
auto getVisibleNeighbors = [](const auto& population) {
    std::unordered_map<std::array<int,N>, int> visibleNeighbors{};

    for(const auto& pos : population){
        incrementNeighbors(pos, visibleNeighbors);
    }
    return visibleNeighbors;
};


auto parseInput(const auto& input) {
    std::unordered_set<std::array<int, 2>> activeCubes;
    for (auto y = 0; y < static_cast<int>(input.size()); y++) {
        for (auto x = 0; x < static_cast<int>(input[y].size()); x++) {
            if (input[y][x] == '#') {
                std::array<int,2> obj{};
                obj[0] = y;
                obj[1] = x;
                activeCubes.insert(obj);
            }
        }
    }
    return activeCubes;
}

template<size_t N>
auto getPopulation(const auto& input){
    std::unordered_set<std::array<int, N>> population{};
    std::ranges::transform(input, std::inserter(population, std::begin(population)), [](const auto& arr){
        return std::array<int, N>{arr[0], arr[1]};
    });
    return population;
}

template<size_t N>
auto getNumberOfActiveCubes = [](const auto& input){
    auto population = getPopulation<N>(input);
    auto gameOfLife = make_GameOfLife( getVisibleNeighbors<N>, std::move(population) );
    gameOfLife.play();
    return gameOfLife.getPopulationSize();
};

int main(){
    const auto input = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto numberOfActiveCubesInThreeDimensions = getNumberOfActiveCubes<3>(input);
    std::cout << "There are " << numberOfActiveCubesInThreeDimensions << " active cubes after six cycles.\n";

    //Task 2
    const auto numberOfActiveCubesInFourDimensions = getNumberOfActiveCubes<4>(input);
    std::cout << "In four dimensions, there are " << numberOfActiveCubesInFourDimensions << " active cubes after six cycles.\n";

    VerifySolution::verifySolution(numberOfActiveCubesInThreeDimensions, numberOfActiveCubesInFourDimensions);
}
