
#include "../../lib/readFile.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Keypad{
protected:
    int x{0}, y{0};

public:
    Keypad() = default;
    Keypad(int x, int y) : x{x}, y{y}{}

    virtual std::string getCurrentButton() const = 0;
    virtual void moveRight()=0;
    virtual void moveLeft()=0;
    virtual void moveDown()=0;
    virtual void moveUp()=0;

    void updatePosition(const char instruction){
        switch (instruction) {
            case 'R':
                moveRight();
                break;
            case 'L':
                moveLeft();
                break;
            case 'D':
                moveDown();
                break;
            case 'U':
                moveUp();
                break;
        }
    }

    void followInstructions(const std::string instructionSet){
        for(const auto instruction : instructionSet){
            updatePosition(instruction);
        }
    }
};

class SquareKeypad : public Keypad{
public:
    void moveRight() override {
        x = std::clamp(x+1, -1, 1);
    }
    void moveLeft() override {
        x = std::clamp(x-1, -1, 1);
    }
    void moveDown() override {
        y = std::clamp(y+1, -1, 1);
    }
    void moveUp() override {
        y = std::clamp(y-1, -1, 1);
    }

    std::string getCurrentButton() const override {
        return std::to_string(x+3*y+5);
    };
};


class DiamondKeypad : public Keypad{
public:
    DiamondKeypad() : Keypad{-2, 0}{}
    void moveRight() override {
        x = std::clamp(x+1 , -2+std::abs(y), 2-std::abs(y));
    }
    void moveLeft() override {
        x = std::clamp(x-1 , -2+std::abs(y), 2-std::abs(y));
    }
    void moveDown() override {
        y = std::clamp(y+1 , -2+std::abs(x), 2-std::abs(x));
    }
    void moveUp() override {
        y = std::clamp(y-1 , -2+std::abs(x), 2-std::abs(x));
    }

    std::string getCurrentButton() const override {
        std::stringstream stream{};
        stream << std::hex << 7+x+(13-y*y)*y/3;
        return stream.str();
    };
};

auto getCode = [](const auto& instructions, auto&& keypad){
    std::string code{""};
    for(const auto& instructionSet : instructions){
        keypad.followInstructions(instructionSet);
        code += keypad.getCurrentButton();
    }
    return code;
};

int main(){
    const auto instructions = readFile::vectorOfStrings("input.txt");

    //Task 1
    auto code = getCode(instructions, SquareKeypad{});
    std::cout << "The code is " << code << ".\n";

    //Task 2
    code = getCode(instructions, DiamondKeypad{});
    std::cout << "The actual code is " << code << ".\n";
}