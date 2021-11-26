
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

class Factory;

class ChipReceiver{
protected:
    std::vector<int> chips;
    int id{-1};

public:
    ChipReceiver(int id) : id{id}{}

    void addChip(int chip){
        chips.push_back(chip);
    }

    int getFirstChip() const {
        return chips[0];
    }
};

class Output : public ChipReceiver{
public:
    Output() : ChipReceiver{-1}{}
    Output(int id) : ChipReceiver{id}{}
};

class Robot : public ChipReceiver{
    ChipReceiver* lowPtr{nullptr};
    ChipReceiver* highPtr{nullptr};

public:
    Robot() : ChipReceiver{-1}{};
    void initialize(ChipReceiver* lowPtr, ChipReceiver* highPtr, int id){
        this->lowPtr = lowPtr;
        this->highPtr = highPtr;
        this->id = id;
    }

    void passChips(){
        const auto [min, max] = std::ranges::minmax_element(chips);
        lowPtr->addChip(*min);
        highPtr->addChip(*max);
        chips.clear();
        //Task 1
        if(*min == 17 && *max == 61){
            std::cout << "The robot handling chips 17 and 61 has the id " << id << ".\n";
        }
    }

    bool hasTwoChips() const {
        return chips.size() == 2;
    }
};

class Factory{
    std::unordered_map<int, Output> output{};
    std::unordered_map<int, Robot> robots{};

    void parseInstruction(const auto& instruction){
        for(int i=1; i<=20; i++){
            output.emplace(std::make_pair(i, Output(i)));
        }
        const auto split = Utilities::split(instruction);
        if(split[0] == "value"){
            const auto chipID  = std::stoi(split[1]);
            const auto robotID = std::stoi(split[5]);
            robots[robotID].addChip(chipID);
        }
        else{
            const auto robotID = std::stoi(split[1]);
            const auto lowID   = std::stoi(split[6]);
            const auto highID  = std::stoi(split[11]);
            ChipReceiver* lowReceiver  = split[5]  == "bot" ? (ChipReceiver*) &robots[lowID]  : (ChipReceiver*) &output[lowID];
            ChipReceiver* highReceiver = split[10] == "bot" ? (ChipReceiver*) &robots[highID] : (ChipReceiver*) &output[highID];
            robots[robotID].initialize(lowReceiver, highReceiver, robotID);
        }
    }

    bool update(){
        bool passedChips = false;
        std::ranges::for_each(robots,
            [&passedChips](auto& pair){
                auto& [id, robot] = pair;
                if(robot.hasTwoChips()){
                    robot.passChips();
                    passedChips = true;
                }
            }
        );
        return passedChips;
    }

public:
    Factory() = default;

    void parseInstructions(const auto& instructions){
        for(const auto& instruction : instructions){
            parseInstruction(instruction);
        }
    }

    void passAllChips(){
        while(update()){}
    }

    const auto& getOutput() const {
        return output;
    }
};

auto getChipProduct(const auto& factory){
    const auto& output = factory.getOutput();
    return output.at(0).getFirstChip()*output.at(1).getFirstChip()*output.at(2).getFirstChip();
}

int main(){
    const auto instructions = readFile::vectorOfStrings("input.txt");

    Factory factory{};
    factory.parseInstructions(instructions);

    //Task 1 & 2
    factory.passAllChips();
    std::cout << "The chips in the first three outputs have a product of " << getChipProduct(factory) << ".\n";
}