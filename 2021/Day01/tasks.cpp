
#include "../../lib/readFile.hpp"

#include <iostream>
#include <vector>


auto getLargerThanPreviousSlideCount(const auto& v, const unsigned int n = 1u){
    auto count=0u;
    for(auto i=n; i<v.size(); i++){
        if(v[i] < v[i+n]) count++;
    }
    return count;
}

int main(){
    const auto measurements = readFile::vectorOfInts("input.txt");

    //Task 1
    const auto largerThanPreviousMeasurementsCount = getLargerThanPreviousSlideCount(measurements);
    std::cout << "There are a total of " << largerThanPreviousMeasurementsCount << " measurements larger than the previous.\n";

    //Task 2
    const auto largerThanPreviousThreeSlideCount   = getLargerThanPreviousSlideCount(measurements, 3);
    std::cout << "There are a total of " << largerThanPreviousThreeSlideCount << " three-sums lager than the previous.\n";

}