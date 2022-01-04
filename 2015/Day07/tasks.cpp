
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>



class andGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string wire1{};
    std::string wire2{};

public:
    andGate(std::unordered_map<std::string, uint16_t>* _signals, std::string&& _out, std::string&& _in1, std::string&& _in2)
        : signals{_signals}, outWire{std::move(_out)}, wire1{std::move(_in1)}, wire2{std::move(_in2)}{}
    void passSignal() const {
        (*signals)[outWire] = (*signals)[wire1]&(*signals)[wire2];
    }
    bool isReady() const {
        auto isready = signals->contains(wire1) && signals->contains(wire2);
        return isready;
    }
};

class orGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string wire1{};
    std::string wire2{};

public:
    orGate(std::unordered_map<std::string, uint16_t>* _signals, std::string&& _out, std::string&& _in1, std::string&& _in2)
        : signals{_signals}, outWire{std::move(_out)}, wire1{std::move(_in1)}, wire2{std::move(_in2)}{}
    void passSignal() const {
        (*signals)[outWire] = (*signals)[wire1]|(*signals)[wire2];
    }
    bool isReady() const {
        return signals->contains(wire1) && signals->contains(wire2);
    }
};

class lshiftGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string wire{};
    size_t shift{};

public:
    lshiftGate(std::unordered_map<std::string, uint16_t>* _signals, std::string&& _out, std::string&& _in1, std::string&& _in2)
        : signals{_signals}, outWire{std::move(_out)}, wire{std::move(_in1)}, shift{std::stoul(_in2)} {}
    void passSignal() const {
        (*signals)[outWire] = (*signals)[wire]<<shift;
    }
    bool isReady() const {
        return signals->contains(wire);
    }
};

class rshiftGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string wire{};
    size_t shift{};

public:
    rshiftGate(std::unordered_map<std::string, uint16_t>* _signals, std::string&& _out, std::string&& _in1, std::string&& _in2)
        : signals{_signals}, outWire{std::move(_out)}, wire{std::move(_in1)}, shift{std::stoul(_in2)} {}
    void passSignal() const {
        (*signals)[outWire] = (*signals)[wire]>>shift;
    }
    bool isReady() const {
        return signals->contains(wire);
    }
};

class notGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string wire{};

public:
    notGate(std::unordered_map<std::string, uint16_t>* _signals, std::string&& _out, std::string&& _in)
        : signals{_signals}, outWire{std::move(_out)}, wire{std::move(_in)}{}
    void passSignal() const {
        (*signals)[outWire] = ~(*signals)[wire];
    }
    bool isReady() const {
        return signals->contains(wire);
    }
};

class defaultGate{
    std::unordered_map<std::string, uint16_t>* signals{nullptr};
    std::string outWire{};
    std::string inWire{};

public:
    defaultGate(std::unordered_map<std::string, uint16_t>* _signals, auto&& _out, auto&& _in)
        : signals{_signals}, outWire{std::move(_out)}, inWire{std::move(_in)}{}
    void passSignal() const {
        (*signals)[outWire] = (*signals)[inWire];
    }
    bool isReady() const {
        return signals->contains(inWire);
    }
};


class Circuit{
    std::unordered_map<std::string, uint16_t> signals{};
    std::unordered_map<std::string, uint16_t> presetSignals{};
    std::vector<std::variant<andGate, orGate, lshiftGate, rshiftGate, notGate, defaultGate>> gates{};

public:
    void createGates(const auto& input){

        for(const auto& inputGate : input){
            auto parts = Utilities::split(inputGate);
            std::string& out = parts[parts.size()-1];
            if(parts[0] == "NOT"){
                gates.emplace_back(notGate(&signals, std::move(out), std::move(parts[1]) ));
            }
            else if(parts[1] == "->"){
                auto& inWire = parts[0];
                if( std::isdigit(inWire[0]) ){
                    presetSignals[out] = (uint16_t) std::stoul( inWire );
                    continue;
                }
                else{
                    gates.emplace_back(defaultGate(&signals, std::move(out), std::move(inWire) ));
                }

            }
            else if(parts[1] == "AND"){
                auto& inWire1 = parts[0];
                auto& inWire2 = parts[2];
                if(std::isdigit(inWire1[0])) presetSignals[inWire1] = (uint16_t) std::stoul(inWire1);
                if(std::isdigit(inWire2[0])) presetSignals[inWire2] = (uint16_t) std::stoul(inWire2);
                gates.emplace_back(andGate(&signals, std::move(out), std::move(inWire1), std::move(inWire2) ));
            }
            else if(parts[1] == "OR"){
                auto& inWire1 = parts[0];
                auto& inWire2 = parts[2];
                if(std::isdigit(inWire1[0])) presetSignals[inWire1] = (uint16_t) std::stoul(inWire1);
                if(std::isdigit(inWire2[0])) presetSignals[inWire2] = (uint16_t) std::stoul(inWire2);
                gates.emplace_back(orGate(&signals, std::move(out), std::move(inWire1), std::move(inWire2) ));
            }
            else if(parts[1] == "LSHIFT"){
                auto& inWire = parts[0];
                auto& shift = parts[2];
                if(std::isdigit(inWire[0])) presetSignals[inWire] = (uint16_t)std::stoul(inWire);
                gates.emplace_back(lshiftGate(&signals, std::move(out), std::move(inWire), std::move(shift) ));
            }
            else{
                auto& inWire = parts[0];
                auto& shift = parts[2];
                if(std::isdigit(inWire[0])) presetSignals[inWire] = (uint16_t)std::stoul(inWire);
                gates.emplace_back(rshiftGate(&signals, std::move(out), std::move(inWire), std::move(shift) ));
            }
        }
    }


    auto getSignalAtWire(const std::string& s){
        for(const auto& [wire, val] : presetSignals){
            signals[wire] = val;
        }

        auto signalPassedThrough = [this](auto& gate){
            const auto isReady = std::visit([](const auto& _gate) -> bool {
                return _gate.isReady();
            }, gate);

            if(isReady){
                std::visit([](const auto& _gate){ _gate.passSignal(); }, gate);
                return true;
            }
            return false;
        };

        while(!signals.contains(s))
        {
            std::ranges::for_each(gates, signalPassedThrough);
        }

        return signals[s];
    }

    auto get(const std::string& s) noexcept {
        return signals[s];
    }

    auto set(const std::string& s, const auto& val){
        presetSignals[s] = val;
    }

    void reset(){
        signals.clear();
    }
};


int main(){
    Circuit circuit{};
    circuit.createGates(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto signalAtWireA = circuit.getSignalAtWire("a");
    std::cout << "Wire a provides signal " << signalAtWireA << ".\n";

    //Task 2
    circuit.reset();
    circuit.set("b", signalAtWireA);
    const auto newSignalAtWireA = circuit.getSignalAtWire("a");
    std::cout << "Wire a provides signal " << newSignalAtWireA << " now.\n";

}