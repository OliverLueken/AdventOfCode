
#include "../../readFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

auto getNewPassword = [](auto& pw){
    auto hasThreeLettersInIncreasingOrder = [](const auto& pw){
        for(auto it = std::begin(pw)+2; it!=std::end(pw); it++){
            if( *(it-2)+2 == *(it-1)+1 && *(it-1)+1 == *it ) return true;
        }
        return false;
    };
    auto hasTwoPairs = [](const auto& pw){
        auto it = std::ranges::adjacent_find(pw);
        it = std::adjacent_find(it+2, std::end(pw));
        return it < std::end(pw);
    };
    auto incrementPW = [](auto& pw){
        auto isNotZ = [](const auto& c){return c != 'z'; };
        auto isForbiddenChar = [](const auto& c){
            return c=='i' || c=='o' || c=='l';
        };

        auto it = std::ranges::find_if( pw | std::views::reverse, isNotZ );
        *it+=1;
        if(isForbiddenChar(*it)) *it+=1;
        std::fill(std::rbegin(pw), it, 'a');
    };

    do{
        incrementPW(pw);
    }while(!(hasThreeLettersInIncreasingOrder(pw) && hasTwoPairs(pw)));
};

int main(){
    std::string password = "vzbxkghb";

    //Task 1
    getNewPassword(password);
    std::cout << "The new password should be " << password << ".\n";

    //Task 2
    getNewPassword(password);
    std::cout << "The new password should be " << password << ".\n";
}