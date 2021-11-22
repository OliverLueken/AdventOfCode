
#include "../../readFile.h"
#include "../../utilities.h"

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
        split[1].pop_back();
        checksum = std::move(split[1]);
    }

    bool isValid() const {
        auto name = this->name;
        std::erase(name, '-');
        std::unordered_map<char, int> quantityMap{};
        for(const auto c : name) quantityMap[c]++;

        std::vector<std::pair<char, int>> quantityVec{};
        for(const auto [c, n] : quantityMap){
            quantityVec.emplace_back(std::make_pair(c, n));
        }

        std::ranges::sort(quantityVec, [](const auto& lhs, const auto& rhs){
            const auto [c1, n1] = lhs;
            const auto [c2, n2] = rhs;
            if(n1==n2) return c1<c2;
            return n1 > n2;
        });

        bool isValid = true;
        for(int letter=0; letter<5; letter++){
            isValid &= quantityVec[letter].first == checksum[letter];
        }
        return isValid;
    }

    std::string decrypt() const{
        auto rot = id%26;
        std::string decryptedName{};
        for(const auto c : name){
            if(c == '-') decryptedName+=' ';
            else decryptedName += (char) (c+rot-'a')%26 + 'a';
        }
        return decryptedName;
    }
};

auto parseInput = [](auto input){
    std::vector<Room> rooms{};
    for(auto& roomData : input){
        Room room{};
        room.parseInput(std::move(roomData));
        rooms.emplace_back(std::move(room));
    }
    return rooms;
};

auto sumValidroomIDs = [](const auto& rooms){
    return std::transform_reduce(std::begin(rooms), std::end(rooms),
                                 0u, std::plus<>(),
                                 [](const auto& room){return room.isValid() ? room.getID() : 0u;});
};

auto getNorthPoleObjectStorageID = [](const auto& rooms){

    for(const auto& room : rooms){
        if(room.decrypt() == "northpole object storage")
            return room.getID();
    }
    return 0u;
};

int main(){
    const std::vector<Room> rooms = parseInput(readFile::vectorOfStrings("input.txt"));

    const auto sumOfValidRoomIDs = sumValidroomIDs(rooms);
    std::cout << "The sum of all valid room IDs is " << sumOfValidRoomIDs << ".\n";

    const auto storageID = getNorthPoleObjectStorageID(rooms);
    std::cout << "The ID of the northpole object storage is " << storageID << ".\n";
}