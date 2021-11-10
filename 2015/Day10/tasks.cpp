

#include <iostream>
#include <string>
#include <algorithm>


auto playLookAndSay = [](auto number, const size_t n){
    auto lookAndSay = [](auto& number){
        std::string newNumber{};
        auto it1 = std::begin(number);
        auto it2 = it1;
        do{
            it2 = std::mismatch(it2, std::end(number), it2+1).second;
            newNumber += std::to_string(std::distance(it1, it2));
            newNumber += *it1;
            it1=it2;
        }while(it2 != std::end(number));
        number = newNumber;
    };

    for(auto i = 0u; i < n; i++){
        lookAndSay(number);
    }
    return number;
};

int main(){
    const std::string input = "1321131112";

    //Task 1
    auto result = playLookAndSay(input, 40);
    std::cout << "The input after 40 iterations has length " << result.size() << ".\n";

    //Task 2
    result = playLookAndSay(result, 10);
    std::cout << "The input after 50 iterations has length " << result.size() << ".\n";
}