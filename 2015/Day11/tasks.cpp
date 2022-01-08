
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

auto getNewPassword = [](auto& pw){
    auto hasThreeLettersInIncreasingOrder = [](const auto& password){
        for(auto it = std::begin(password)+2; it!=std::end(password); it++){
            if( *(it-2)+2 == *(it-1)+1 && *(it-1)+1 == *it ) return true;
        }
        return false;
    };
    auto hasTwoPairs = [](const auto& password){
        auto it = std::ranges::adjacent_find(password);
        it = std::adjacent_find(it+2, std::end(password));
        return it < std::end(password);
    };
    auto incrementPW = [](auto& password){
        auto isNotZ = [](const auto& c){return c != 'z'; };
        auto isForbiddenChar = [](const auto& c){
            return c=='i' || c=='o' || c=='l';
        };

        auto it = std::ranges::find_if( password | std::views::reverse, isNotZ );
        *it+=1;
        if(isForbiddenChar(*it)) *it+=1;
        std::fill(std::rbegin(password), it, 'a');
    };

    do{
        incrementPW(pw);
    }while(!(hasThreeLettersInIncreasingOrder(pw) && hasTwoPairs(pw)));
};

int main(){
    std::string password = "vzbxkghb";

    //Task 1
    getNewPassword(password);
    const auto password1 = password;
    std::cout << "The new password should be " << password1 << ".\n";

    //Task 2
    getNewPassword(password);
    const auto password2 = password;
    std::cout << "The new password should be " << password2 << ".\n";

    VerifySolution::verifySolution(password1, password2);
}