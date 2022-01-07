#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(){
    std::string line;
    std::ifstream input("input.txt");
    std::vector<std::pair<int,int>> slopes;
   	slopes.push_back(std::make_pair(1,1));
   	slopes.push_back(std::make_pair(3,1));
   	slopes.push_back(std::make_pair(5,1));
   	slopes.push_back(std::make_pair(7,1));
   	slopes.push_back(std::make_pair(1,2));
    std::vector<int> count_trees(5,0);
    std::vector<int> x(5,0);
    std::vector<int> y(5,0);

    if(input.is_open()){
	while(getline(input,line)){
	    for(int i=0; i<slopes.size(); i++){
		if(y[i]==0){
		    y[i]-=slopes[i].second;
	            if(line[x[i]]=='#')
	   	        count_trees[i]++;
	            x[i]=(x[i]+slopes[i].first)%line.size();
		}
		y[i]++;
    	    }
	}
	input.close();
    }
    else
	std::cout << "Unable to open file\n";
    //std::sort(numbers.begin(), numbers.end());
    long m=1;
    for(auto it=count_trees.begin(); it!=count_trees.end(); it++)
	m*=*it;
    std::cout << m << "\n";
}
