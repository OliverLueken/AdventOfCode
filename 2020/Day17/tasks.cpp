
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

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

template<size_t N>
using Population = std::unordered_set<std::array<int,N>>;

template<size_t N>
using VisibleNeighbors = std::unordered_map<std::array<int,N>, int>;

template<size_t N>
using getVisibleNeighborsLambda = std::function<VisibleNeighbors<N>(const Population<N>&)>;


void printcubes(const auto& c) {
    const auto n = std::ranges::minmax(c | std::views::elements<0>);
    const auto m = std::ranges::minmax(c | std::views::elements<1>);
    const auto k = std::ranges::minmax(c | std::views::elements<2>);
    std::cout << n.min << ' ' << n.max << '\n';
    std::cout << m.min << ' ' << m.max << '\n';
    std::unordered_map<int, Matrix::Matrix<int>> slices{};
    for(auto i=k.min; i<=k.max; i++){
        slices[i] = Matrix::Matrix<int>(n.max-n.min+1, m.max-m.min+1);
    }
    for (const auto& p : c) {
        auto& currSlice = slices[p[2]];
        currSlice(p[0]-n.min, p[1]-m.min)++;
    }
    for(const auto& [i, slice] : slices){
        std::cout << i << '\n';
        print(slice);
    }
    std::cout << "\n\n";
}

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
        std::ranges::for_each(visibleNeighbors, populateNextPopulation);
    }

public:
    GameOfLife(getVisibleNeighborsLambda getVisibleNeighbors_, const Population& population_)
      : getVisibleNeighbors{getVisibleNeighbors_},
        population{population_},
        nextPopulation{} {
    }

    void play(){
        for(auto i=0; i<6; ++i){
            playOneRound();
            std::swap(nextPopulation, population);
            nextPopulation.clear();
        }
    }

    auto getOccupiedSeats(){
        return population.size();
    }
};

template <typename L1, typename L2>
auto make_GameOfLife(L1 &&l1, L2 &&l2) {
    return GameOfLife<std::decay_t<L1>, std::decay_t<L2>>
        (std::forward<L1>(l1), std::forward<L2>(l2));
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


template<size_t N>
auto parseInput(const auto& input) {
    std::unordered_set<std::array<int, N>> activeCubes;
    for (auto y = 0; y < static_cast<int>(input.size()); y++) {
        for (auto x = 0; x < static_cast<int>(input[y].size()); x++) {
            if (input[y][x] == '#') {
                std::array<int,N> obj{};
                obj[0] = y;
                obj[1] = x;
                activeCubes.insert(obj);
            }
        }
    }
    return activeCubes;
}


template<size_t N>
auto getNumberOfActiveCubes = [](){
    const auto population = parseInput<N>(readFile::vectorOfStrings());
    auto gameOfLife = make_GameOfLife( getVisibleNeighbors<N>, population );
    gameOfLife.play();
    return gameOfLife.getOccupiedSeats();
};

int main(){

    //Task 1
    const auto result1 = getNumberOfActiveCubes<3>();
    std::cout << "Result1: " << result1 << ".\n";

    //Task 2
    const auto result2 = getNumberOfActiveCubes<4>();
    std::cout << "Result2: " << result2 << ".\n";

    VerifySolution::verifySolution(result1, result2);
}
