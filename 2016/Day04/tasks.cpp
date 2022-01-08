
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <numeric>

class Room{
    unsigned int id{0};
    std::string name{};
    std::string checksum{};

public:
    unsigned int getID() const { return id;}

    void parseInput(std::string&& input){
        auto lastDash = std::ranges::find(input | std::views::reverse, '-').base()-1;
        name = input.substr(0, std::distance(std::begin(input), lastDash));
        auto split = Utilities::split(lastDash+1, std::end(input), '[');
        id = std::stoi(split[0]);
        split[1].pop_back(); //removes the trailing ']'
        checksum = std::move(split[1]);
    }

    bool isValid() const {
        auto roomName = this->name;
        std::erase(roomName, '-');
        std::unordered_map<char, int> quantityMap{};
        for(const auto c : roomName) quantityMap[c]++;

        std::vector<std::pair<char, int>> quantityVec{};
        std::ranges::move(std::begin(quantityMap),
                          std::end(quantityMap),
                          std::back_inserter(quantityVec)
        );

        std::ranges::sort(quantityVec,
            [](const auto& lhs, const auto& rhs){
                const auto [c1, n1] = lhs;
                const auto [c2, n2] = rhs;
                if(n1==n2) return c1<c2;
                return n1 > n2;
            }
        );

        const auto matchedUntilIt = std::ranges::mismatch(quantityVec, checksum, {},
            [](const auto& quantity){ return quantity.first; }
        ).in2;
        return matchedUntilIt == std::end(checksum);
    }

    std::string decrypt() const{
        const auto rot = id%26;
        std::string decryptedName{};
        std::ranges::transform(name, std::back_inserter(decryptedName),
            [rot](const auto c) -> char {
                return c == '-' ? ' ' : (char) (c+rot-'a')%26 + 'a';
            }
        );
        return decryptedName;
    }
};

auto parseInput = [](auto&& input){
    std::vector<Room> rooms{};
    std::ranges::transform(input, std::back_inserter(rooms),
        [](auto& roomData){
            Room room{};
            room.parseInput(std::move(roomData));
            return room;
        }
    );
    return rooms;
};

auto sumValidRoomIDs = [](const auto& rooms){
    return std::transform_reduce(std::begin(rooms), std::end(rooms), 0u, std::plus<>(),
        [](const auto& room){return room.isValid() ? room.getID() : 0u;}
    );
};

auto getNorthPoleObjectStorageID = [](const auto& rooms){
    const auto storageIt = std::ranges::find(rooms, "northpole object storage",
        [](const auto& room){return room.decrypt();}
    );
    return storageIt->getID();
};

int main(){
    const std::vector<Room> rooms = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto sumOfValidRoomIDs = sumValidRoomIDs(rooms);
    std::cout << "The sum of all valid room IDs is " << sumOfValidRoomIDs << ".\n";

    //Task 2
    const auto storageID = getNorthPoleObjectStorageID(rooms);
    std::cout << "The ID of the northpole object storage is " << storageID << ".\n";

    VerifySolution::verifySolution(sumOfValidRoomIDs, storageID);
}