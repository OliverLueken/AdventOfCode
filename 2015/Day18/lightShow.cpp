
#include "lightShow.h"

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <ranges>

unsigned int lightShow::getNeighborLights(const unsigned int x, const unsigned int y) const{
    const auto N = (unsigned int)lights.size();

    auto lightsOn = 0u;
    for(auto xx = std::max(1u,x)-1; xx<=std::min(N-1,x+1); xx++){
        for(auto yy = std::max(1u,y)-1; yy<=std::min(N-1,y+1); yy++){
            if(xx != x || yy != y){
                lightsOn+=lights[yy][xx];
            }
        }
    }
    return lightsOn;
}

size_t lightShow::getOn() const {
    const auto range = lights | std::views::join;
    return std::accumulate( std::begin(range), std::end(range), 0 );
}

lightShow::lightShow(std::vector<std::string> input) : lights{}{
    for(auto y = 0u; y<input.size(); y++){
        std::vector<bool> temp{};
        for(auto x = 0u; x<input[0].size(); x++){
            temp.emplace_back(input[y][x] == '#');
        }
        lights.push_back(temp);
    }
}

void lightShow::print() const{
    for(auto y = 0u; y<lights.size(); y++){
        for(auto x = 0u; x<lights.size(); x++){
            std::cout << (lights[y][x] ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void lightShow::update(){
    grid newLights{lights};
    const auto N = lights.size();
    for(auto y=0u; y<N; y++){
        for(auto x=0u; x<N; x++){
            const auto onAmount = getNeighborLights(x,y);
            if     (lights[y][x] == true  && (onAmount < 2 || onAmount > 3)) newLights[y][x]=false;
            else if(lights[y][x] == false && onAmount == 3)                  newLights[y][x]=true;
        }
    }
    std::swap(lights, newLights);
}

void lightShow::turnCornersOn(){
    const auto N=lights.size()-1;
    lights[0][0]=true;
    lights[0][N]=true;
    lights[N][0]=true;
    lights[N][N]=true;
}