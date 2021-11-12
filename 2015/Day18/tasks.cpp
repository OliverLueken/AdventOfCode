
#include "../../readFile.h"
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
    auto onLights = animate(lightshow, 100);
    std::cout << "There are a total of " << onLights << " lights on.\n";

    lightShow lightshow2(readFile::vectorOfStrings("input.txt"));
    onLights = animateWithCornersStuckOn(lightshow2, 100);
    std::cout << "There are a total of " << onLights << " lights on with the corner lights stuck on on.\n";

}