
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <set>

auto getOccupiedSeats(const auto& boardingPasses){
    std::set<unsigned int> seatIDs{};
    for(const auto& boardingPass : boardingPasses){
        unsigned int id=0;
        for(int i=0; i<10; i++){
            id<<=1;
            if(boardingPass[i]=='B' || boardingPass[i]=='R') id++;
        }
        seatIDs.insert(id);
    }
    return seatIDs;
}

auto getMySeat(const auto& seatIDs, const auto maxSeatID){
    for(auto id = maxSeatID; id>=0; id--){
        if(!seatIDs.contains(id)) return id; //find first id not included in the set
    }
    return 0u;
}

int main(){
    const auto boardingPasses = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto occupiedSeats = getOccupiedSeats(boardingPasses);
    const auto maxSeatID = std::ranges::max(occupiedSeats);
    std::cout << "The highest seat ID on a boarding pass is " << maxSeatID << ".\n";

    //Task 2
    const auto mySeatID = getMySeat(occupiedSeats, maxSeatID);
    std::cout << "The ID of my seat is " << mySeatID << ".\n";

    VerifySolution::verifySolution(maxSeatID, mySeatID);
}