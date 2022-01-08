
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class IP{
    std::vector<std::string> outsideBracket{};
    std::vector<std::string> insideBracket {};

public:
    void parseInput(const auto& input){
        auto split = Utilities::splitOnEach(input, "[]");
        auto it = std::begin(split);
        while(it!=std::end(split)-1){
            outsideBracket.emplace_back(std::move(*it));
            it++;
            insideBracket.emplace_back(std::move(*it));
            it++;
        }
        outsideBracket.emplace_back(std::move(*it));
    }

    bool supportsTLS() const {
        const auto hasABBA = [](const auto& s){
            if(s.size()<4) return false;
            auto it = std::begin(s);
            do{
                if(*it == *(it+3) && *(it+1) == *(it+2) && *it!=*(it+1)) return true;
                it++;
            }while(it != std::end(s)-3);
            return false;
        };
        return std::ranges::any_of(outsideBracket, hasABBA) && std::ranges::none_of(insideBracket, hasABBA);
    }

    auto getBABs() const {
        std::vector<std::string> BABs{};
        const auto addBABs = [&BABs](const auto& s){
            for(auto i=0u; i<s.size()-2; i++){
                if(s[i]==s[i+2] && s[i]!=s[i+1]){
                    BABs.emplace_back(std::string{s[i+1], s[i], s[i+1]});
                }
            }
        };
        std::ranges::for_each(outsideBracket, addBABs);
        return BABs;
    }

    bool supportsSSL() const {
        const std::vector<std::string> BABs = getBABs();
        const auto hasAnyBAB = [&BABs](const auto& s){
            const auto insideS = [&s](const auto& bab){return Utilities::contains(s, bab);};
            return std::ranges::any_of(BABs, insideS);
        };
        return std::ranges::any_of(insideBracket, hasAnyBAB);
    }
};

auto parseIPs = [](const auto& input){
    std::vector<IP> ips{};
    std::ranges::transform(input, std::back_inserter(ips),
        [](const auto& in){
            IP ip{};
            ip.parseInput(in);
            return ip;
        }
    );
    return ips;
};

int main(){
    const auto ips = parseIPs(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto supportsTLS = [](const auto& ip){return ip.supportsTLS();};
    const auto supportTLSCount = std::ranges::count_if(ips, supportsTLS);
    std::cout << "There are " << supportTLSCount << " IPs that support TLS.\n";

    //Task 1
    const auto supportsSSL = [](const auto& ip){return ip.supportsSSL();};
    const auto supportSSLCount = std::ranges::count_if(ips, supportsSSL);
    std::cout << "There are " << supportSSLCount << " IPs that support SSL.\n";

    VerifySolution::verifySolution(supportTLSCount, supportSSLCount);
}