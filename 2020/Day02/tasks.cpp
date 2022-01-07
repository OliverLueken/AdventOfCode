#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

int main(){
    std::string line;
    std::ifstream input("input.txt");
    int count_valid=0;
    int min, max;
    char letter;
    std::string pw;

    if(input.is_open()){
	while(getline(input,line)){
	    std::string::size_type n, m;
	    n = line.find("-");
	    min = stoi(line.substr(0,n));
	    m = line.find(" ");
	    max = stoi(line.substr(n+1, m-1-n));
	    m=line.find(":");
	    letter = line.substr(m-1,1).front();
	    pw = line.substr(m+2,line.size());

	    /* //part 1
	    int count = 0;
	    for(auto pos=0; pos<=pw.size()-1; pos++){
		if(letter==pw[pos])
			count++;
	    }
	    if(min<= count && count <= max)
		    count_valid++;
	    */

	    //part 2
	    if((pw[min-1]==letter) ^ (pw[max-1]==letter))
		    count_valid++;

	}
	input.close();
    }
    else
	std::cout << "Unable to open file\n";
    //std::sort(numbers.begin(), numbers.end());

    std::cout << count_valid << "\n";
}
