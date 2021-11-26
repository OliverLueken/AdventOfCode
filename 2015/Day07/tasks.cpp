
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <memory>

class gate{
protected:
    std::unordered_map<std::string, uint16_t>& signals;
    std::string outWire{};

public:
    gate(std::unordered_map<std::string, uint16_t>& sig) : signals{sig}{}
    gate(std::unordered_map<std::string, uint16_t>& sig, std::string&& out)
        : signals{sig}, outWire{std::move(out)}{}
    virtual void passSignal() = 0;
    virtual bool isReady() = 0;
};

class andGate : public gate{
public:
    std::string wire1{};
    std::string wire2{};

    andGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in1, std::string in2)
        : gate{signals, std::move(out)}, wire1{std::move(in1)}, wire2{std::move(in2)}{
            if(std::isdigit(wire1[0])) signals[wire1] = (uint16_t)std::stoul(wire1);
            if(std::isdigit(wire2[0])) signals[wire2] = (uint16_t)std::stoul(wire2);
        }
    void passSignal(){
        signals[outWire] = signals.at(wire1)&signals.at(wire2);
    }
    bool isReady(){
        return signals.contains(wire1) && signals.contains(wire2);
    }
};

class orGate : public gate{
public:
    std::string wire1{};
    std::string wire2{};

    orGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in1, std::string in2)
        : gate{signals, std::move(out)}, wire1{std::move(in1)}, wire2{std::move(in2)}{
            if(std::isdigit(wire1[0])) signals[wire1] = (uint16_t)std::stoul(wire1);
            if(std::isdigit(wire2[0])) signals[wire2] = (uint16_t)std::stoul(wire2);
        }
    void passSignal(){
        signals[outWire] = signals.at(wire1)|signals.at(wire2);
    }
    bool isReady(){
        return signals.contains(wire1) && signals.contains(wire2);
    }
};

class lshiftGate : public gate{
public:
    std::string wire{};
    size_t shift{};

    lshiftGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in1, std::string in2)
        : gate{signals, std::move(out)}, wire{std::move(in1)}, shift{std::stoul(in2)} {
            if(std::isdigit(wire[0])) signals[wire] = (uint16_t)std::stoul(wire);
        }
    void passSignal(){
        signals[outWire] = signals.at(wire)<<shift;
    }
    bool isReady(){
        return signals.contains(wire);
    }
};

class rshiftGate : public gate{
public:
    std::string wire{};
    size_t shift{};

    rshiftGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in1, std::string in2)
        : gate{signals, std::move(out)}, wire{std::move(in1)}, shift{std::stoul(in2)} {
            if(std::isdigit(wire[0])) signals[wire] = (uint16_t)std::stoul(wire);
        }
    void passSignal(){
        signals[outWire] = signals.at(wire)>>shift;
    }
    bool isReady(){
        return signals.contains(wire);
    }
};

class notGate : public gate{
public:
    std::string wire{};

    notGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in)
        : gate{signals, std::move(out)}, wire{std::move(in)}{
            if(std::isdigit(wire[0])) signals[wire] = (uint16_t)std::stoul(wire);
        }
    void passSignal(){
        signals[outWire] = ~signals.at(wire);
    }
    bool isReady(){
        return signals.contains(wire);
    }
};

class defaultGate : public gate{
public:
    std::string wire{};

    defaultGate(std::unordered_map<std::string, uint16_t>& signals, std::string out, std::string in)
        : gate{signals, std::move(out)}, wire{std::move(in)}{
            if(std::isdigit(wire[0])) signals[wire] = (uint16_t)std::stoul(wire);
        }
    void passSignal(){
        signals[outWire] = signals.at(wire);
    }
    bool isReady(){
        return signals.contains(wire);
    }
};


auto parseInput = [](const auto& input, auto& signals){
    auto parse = [&signals](const auto& input) -> std::unique_ptr<gate>{
        auto parts = Utilities::split(input);

        std::string out = *std::rbegin(parts);
        if(parts[0] == "NOT"){
            return std::make_unique<notGate>(signals, out, parts[1]);
        }
        else if(parts[1] == "->"){
            return std::make_unique<defaultGate>(signals, out, parts[0]);
        }
        else if(parts[1] == "AND"){
            return std::make_unique<andGate>(signals, out, parts[0], parts[2]);
        }
        else if(parts[1] == "OR"){
            return std::make_unique<orGate>(signals, out, parts[0], parts[2]);
        }
        else if(parts[1] == "LSHIFT"){
            return std::make_unique<lshiftGate>(signals, out, parts[0], parts[2]);
        }
        return std::make_unique<rshiftGate>(signals, out, parts[0], parts[2]);

    };

    std::vector<std::unique_ptr<gate>> gates;
    std::ranges::transform(input, std::back_inserter(gates), parse);
    return gates;
};

auto propagateSignals = [](auto& signals, auto& gates){
    auto signalPassedThrough = [&signals](auto& gatePtr){
        if(gatePtr->isReady()){
            gatePtr->passSignal();
            return true;
        }
        return false;
    };

    while(!gates.empty())
    {
        std::erase_if(gates,signalPassedThrough);
    }
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    std::unordered_map<std::string, uint16_t> signals;
    auto gates = parseInput(input, signals);

    //Task 1
    propagateSignals(signals, gates);
    std::cout << "Wire a provides signal " << signals["a"] << ".\n";

    //Task 2
    uint16_t newB = signals["a"];
    signals.clear();
    gates = parseInput(input, signals);
    signals["44430"] = newB;

    propagateSignals(signals, gates);
    std::cout << "Wire a provides signal " << signals["a"] << " now.\n";

}