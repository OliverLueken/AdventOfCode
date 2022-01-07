#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

int main(){
    std::string line;
    std::ifstream input("input.txt");
    std::set<int> numbers;
    if(input.is_open()){
	while(getline(input,line)){
		numbers.insert(stoi(line));
		//std::cout << numbers.back() << "\n";
	}
	input.close();
    }
    else
	std::cout << "Unable to open file\n";
    //std::sort(numbers.begin(), numbers.end());


    for(auto n=numbers.begin(); n!=numbers.end(); n++){
	    //std::cout << *n <<"\n";
	    int a=*n, b=2020-a;
	    if(numbers.find(b)!=numbers.end()){
		    std::cout << a*b << "\n";
		    break;
	    }
    }
    for(auto n=numbers.begin();n!=numbers.end(); n++){
	    int a=*n;
	    for(auto m=next(n); m!=numbers.end(); m++){
		int b=*m, c=2020-a-b;
		if(numbers.find(c)!=numbers.end()){
			std::cout << a*b*c << "\n";
			goto endloop;
		}
	    }
    }
endloop: ;
}
