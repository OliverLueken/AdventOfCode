
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <numeric>

auto parseInput = [](const auto& input){
    std::vector<std::pair<std::string, std::string>> replacements{};
    for(const auto& s : input){
        auto split = Utilities::split(s);
        if(split.size()==1) break;
        replacements.emplace_back(std::move(split[0]), std::move(split[2]));
    }

    return std::make_pair(replacements, input.back());
};

auto iterateMolecules = [](auto& molecules, const auto& replacements){
    auto addNewMolecules = [](const auto& molecule, auto& newMolecules, const auto& fromReplacement, const auto& toReplacement){
        const auto toReplaceRanges = Utilities::searchAll(molecule, fromReplacement);
        for(const auto& subrange : toReplaceRanges){
            const auto first = std::distance(std::begin(molecule), subrange.begin());
            const auto last  = std::distance(std::begin(molecule), subrange.end());
            newMolecules.emplace(molecule.substr(0, first) + toReplacement + molecule.substr(last));
        }
    };

    std::unordered_set<std::string> newMolecules{};
    for(const auto& molecule : molecules){
        for(const auto& [fromReplacement, toReplacement] : replacements){
            addNewMolecules(molecule, newMolecules, fromReplacement, toReplacement);
        }
    }
    std::swap(molecules, newMolecules);
};


auto countDistinctMolecules = [](const auto& molecule, const auto& replacements){

    std::unordered_set<std::string> molecules{molecule};
    iterateMolecules(molecules, replacements);

    return molecules.size();
};


auto countStepsToCreateMolecule = [](auto& goalMolecule, auto& replacements){
    std::unordered_set<std::string> elements{"CRn"};
    for(const auto& [fromReplacement, toReplacement] : replacements){
        elements.insert(fromReplacement);
    }

    auto getElementCount = [&goalMolecule](const auto& element){
        return Utilities::searchAll(goalMolecule, element).size();
    };

    const auto countElements = std::transform_reduce(std::begin(elements), std::end(elements),
                                               0u, std::plus<>(), getElementCount);
    const auto countY = std::ranges::count(goalMolecule, 'Y');
    return countElements-countY-1;
};

int main(){
    auto [replacements, molecule] = parseInput(readFile::vectorOfStrings("input.txt"));

    const auto distinctMolecules = countDistinctMolecules(molecule, replacements);
    std::cout << "There are " << distinctMolecules << " distinct molecules that can be created.\n";

    const auto stepsToCreateMolecule = countStepsToCreateMolecule(molecule, replacements);
    std::cout << "It will take at least " << stepsToCreateMolecule << " steps to create the desired molecule.\n";

    VerifySolution::verifySolution(distinctMolecules, stepsToCreateMolecule);
}