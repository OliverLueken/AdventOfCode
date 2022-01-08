
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct reindeerData{
    unsigned int maxSpeed;
    unsigned int maxSpeedDuration;
    unsigned int restDuration;
};

auto getReindeerData = [](const auto& input){
    std::vector<reindeerData> data;
    for(const auto& s : input){
        auto split = Utilities::split(s);
        reindeerData thisReindeer{(unsigned int)std::stoi(split[3]),
                                  (unsigned int)std::stoi(split[6]),
                                  (unsigned int)std::stoi(split[13])
        };
        data.push_back(thisReindeer);
    }
    return data;
};

auto getFurthestDistance = [](const auto& data, const auto totalRunDuration){
    auto maxDistance = 0ul;
    for(const auto& reindeer : data){
        const auto segmentDuration = reindeer.maxSpeedDuration+reindeer.restDuration;
        const auto segments        = totalRunDuration/segmentDuration;
              auto distance        = segments*reindeer.maxSpeed*reindeer.maxSpeedDuration;
        if(segments*segmentDuration + reindeer.maxSpeedDuration <= totalRunDuration)
            distance+=reindeer.maxSpeed*reindeer.maxSpeedDuration;
        else
            distance+=reindeer.maxSpeed*(totalRunDuration - segments*segmentDuration);
        if(distance > maxDistance) maxDistance = distance;
    }
    return maxDistance;
};

auto getMostStars = [](const auto& data, const auto totalRunDuration){
    auto move = [](auto& distanceTraveled, auto& second, const auto& reindeer){
        distanceTraveled[second] = distanceTraveled[second-1]+reindeer.maxSpeed;
        second++;
    };

    auto rest = [](auto& distanceTraveled, auto& second){
        distanceTraveled[second] = distanceTraveled[second-1];
        second++;
    };

    auto logTravel = [&move, &rest, totalRunDuration](auto& distanceTraveled, const auto& reindeer){
        auto segmentDuration = reindeer.maxSpeedDuration+reindeer.restDuration;
        auto segments = totalRunDuration/segmentDuration;
        auto second = 1u;

        //run the first segments unhindered
        for(auto i=0u; i<segments; i++){
            for(auto j=0u; j<reindeer.maxSpeedDuration; j++)
                move(distanceTraveled, second, reindeer);
            for(auto j=0u; j<reindeer.restDuration; j++)
                rest(distanceTraveled, second);
        }
        //check with totalRunDuration for the last few meters
        for(auto j=0u; j<reindeer.maxSpeedDuration && second<=totalRunDuration; j++)
            move(distanceTraveled, second, reindeer);
        for(auto j=0u; j<reindeer.restDuration && second<=totalRunDuration; j++)
            rest(distanceTraveled, second);

    };

    auto awardStars = [totalRunDuration](const auto& distanceTraveled, const auto N){
        auto awardStarToWinner = [N, distanceTraveled](auto& stars, const auto s){
            auto winner = 0u;
            for(auto reindeer=1u; reindeer<N; reindeer++){
                if(distanceTraveled[reindeer][s] > distanceTraveled[winner][s]) winner=reindeer;
            }
            stars[winner]++;
        };

        std::vector<unsigned int> stars(N);
        for(auto s=1u; s<totalRunDuration; s++){
            awardStarToWinner(stars, s);
        }
        return stars;
    };

    std::vector<std::vector<unsigned int>> distanceTraveled(data.size(), std::vector<unsigned int>(totalRunDuration+1));
    for(size_t reindeer=0u; reindeer < data.size(); reindeer++){
        logTravel(distanceTraveled[reindeer], data[reindeer]);
    }

    const auto stars = awardStars(distanceTraveled, data.size());
    return std::ranges::max(stars);
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    const auto reindeerData = getReindeerData(input);

    //Task 1
    const auto furthestDistance = getFurthestDistance(reindeerData, 2503u);
    std::cout << "The fastest reindeer traveled " << furthestDistance << "km.\n";

    //Task 2
    const auto mostStars = getMostStars(reindeerData, 2503u);
    std::cout << "The reindeer with the most stars accumulated " << mostStars << " stars.\n";

    VerifySolution::verifySolution(furthestDistance, mostStars);
}