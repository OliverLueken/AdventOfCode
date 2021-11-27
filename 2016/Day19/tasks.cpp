
#include <iostream>
#include <cmath>

auto getFirstGameWinner(const auto n){
    const auto i = (int) std::log2(n);
    const auto b = n-(1<<i);
    const auto c = b>>1;
    return 4*c+3;
}

auto getSecondGameWinner(const auto n){
    const auto i = (int) (log(n-1)/log(3));
    const auto x = (int) std::pow(3,i);
    if(n>2*x+1){
        return 2*n-3*x;
    }
    else if(n == 2*x+1){
        return n-x+1;
    }
    return n-x;
}

int main(){
    const auto input = 3017957;

    //Task 1
    const auto firstGameWinner = getFirstGameWinner(input);
    std::cout << "The elf at position " << firstGameWinner << " gets all the presents on the first game.\n";

    //Task 2
    const auto secondGameWinner = getSecondGameWinner(input);
    std::cout << "The elf at position " << secondGameWinner << " gets all the presents on the second game.\n";
}