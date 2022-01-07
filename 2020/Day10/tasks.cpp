#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

long long f(int n){
  return std::pow(2,std::max(n-1,0))-pow(2,std::max(n-3,0))+1;
}

void joltagedifference(std::vector<int>& input, int& result1, long long& result2){
  input.push_back(0);
  std::sort(input.begin(),input.end());


  int ones=0, threes=1, diff, n=0;
  for(int i=0; i<input.size()-1; i++){
    diff=input[i+1]-input[i];
    if(diff==1){
      ones++;
      n++;
    }
    if(diff==3){
      threes++;

      result2*=f(n);
      n=0;
    }
  }

  result2*=f(n);
  result1=ones*threes;


}

std::vector<int> readfile(std::string file){
  std::string line;
  std::ifstream input(file);
  std::vector<int> lines;

  if(input.is_open()){
  	while(getline(input,line)){
        lines.push_back(stol(line));
    	}
      input.close();
    }
  else{
    std::cout << "Unable to open file\n";
  }
  return lines;
}

int main(){
  std::vector<int> input=readfile("input.txt");

  int result1; long long result2=1;
  joltagedifference(input, result1, result2);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
