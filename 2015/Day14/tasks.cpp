
#include "../../readFile.h"
#include "../../utilities.h"

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

auto getFurthestDistance = [](const auto& data, const unsigned int seconds){
    auto maxDistance = 0ul;
    for(const auto& reindeer : data){
        auto segmentDuration = reindeer.maxSpeedDuration+reindeer.restDuration;
        auto segments = seconds/segmentDuration;
        auto distance = segments*reindeer.maxSpeed*reindeer.maxSpeedDuration;
        if(segments*segmentDuration + reindeer.maxSpeedDuration <= seconds)
            distance+=reindeer.maxSpeed*reindeer.maxSpeedDuration;
        else
            distance+=reindeer.maxSpeed*(seconds - segments*segmentDuration);
        if(distance > maxDistance) maxDistance = distance;
    }
    return maxDistance;
};

auto getMostStars = [](const auto& data, const unsigned int seconds){

    auto move = [](auto& distanceTraveled, auto& second, const auto& reindeer){
        distanceTraveled[second] = distanceTraveled[second-1]+reindeer.maxSpeed;
        second++;
    };

    auto rest = [](auto& distanceTraveled, auto& second){
        distanceTraveled[second] = distanceTraveled[second-1];
        second++;
    };

    auto logTravel = [&](auto& distanceTraveled, const auto& reindeer, const auto seconds){
        auto segmentDuration = reindeer.maxSpeedDuration+reindeer.restDuration;
        auto segments = seconds/segmentDuration;
        auto second = 1u;

        //run unhindered the first segments
        for(auto i=0u; i<segments; i++){
            for(auto j=0u; j<reindeer.maxSpeedDuration; j++)
                move(distanceTraveled, second, reindeer);
            for(auto j=0u; j<reindeer.restDuration; j++)
                rest(distanceTraveled, second);
        }
        //check with seconds for the last few meters
        for(auto j=0u; j<reindeer.maxSpeedDuration && second<=seconds; j++)
            move(distanceTraveled, second, reindeer);
        for(auto j=0u; j<reindeer.restDuration && second<=seconds; j++)
            rest(distanceTraveled, second);

    };

    auto awardStars = [](const auto& distanceTraveled, const auto N, const auto seconds){
        auto awardStarToWinner = [](auto& stars, const auto& distanceTraveled, const auto s, const auto N){
            auto winner = 0u;
            for(auto reindeer=1u; reindeer<N; reindeer++){
                if(distanceTraveled[reindeer][s] > distanceTraveled[winner][s]) winner=reindeer;
            }
            stars[winner]++;
        };

        std::vector<unsigned int> stars(N);
        for(auto s=1u; s<seconds; s++){
            awardStarToWinner(stars, distanceTraveled, s, N);
        }
        return stars;
    };

    std::vector<std::vector<unsigned int>> distanceTraveled(data.size(), std::vector<unsigned int>(seconds+1));
    for(size_t reindeer=0u; reindeer < data.size(); reindeer++){
        logTravel(distanceTraveled[reindeer], data[reindeer], seconds);
    }

    const auto stars = awardStars(distanceTraveled, data.size(), seconds);
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
}