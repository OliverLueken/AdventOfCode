
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "lightShow.h"

#include <iostream>

auto animate(auto& lightshow, const auto N){
    for(auto i=0; i<N; i++){
        lightshow.update();
    }
    return lightshow.getOn();
}

auto animateWithCornersStuckOn(auto& lightshow, const auto N){
    for(auto i=0; i<N; i++){
        lightshow.update();
        lightshow.turnCornersOn();
    }
    return lightshow.getOn();
}

int main(){
    lightShow lightshow(readFile::vectorOfStrings("input.txt"));
    const auto onLights = animate(lightshow, 100);
    std::cout << "There are a total of " << onLights << " lights on.\n";

    lightShow lightshowWithStuckCorners(readFile::vectorOfStrings("input.txt"));
    const auto onLightsWithStuckCorners = animateWithCornersStuckOn(lightshowWithStuckCorners, 100);
    std::cout << "There are a total of " << onLightsWithStuckCorners << " lights on with the corner lights stuck on on.\n";

    VerifySolution::verifySolution(onLights, onLightsWithStuckCorners);
}