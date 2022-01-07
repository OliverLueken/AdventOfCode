#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>

void rambictus(std::vector<int>& n, int& result, int num){
  std::map<int, int> lastseenat;
  for(int i=0; i<n.size()-1; i++) lastseenat[n[i]]=i+1;

  int last=n.back();
  int next;
  for(int i=n.size()+1; i<=num; i++){
    auto it=lastseenat.find(last);
    if(it==lastseenat.end()){
      //std::cout << "new\n";
      lastseenat[last]=i-1;
      next=0;
    }
    else{
      //std::cout << "repeat\n";
      next=i-1-lastseenat[last];
      lastseenat[last]=i-1;

    }
    last=next;
    //std::cout << last << std::endl;
  }
  result=last;
}


int main(){
  //std::vector<int> input = {0,3,6};
   std::vector<int> input = {1,20,8,12,0,14};//{0,3,6};

  int result1=0, result2=0;
  rambictus(input, result1, 2020);
  rambictus(input, result2, 30000000);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
