#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>

void dock(std::vector<std::string>& input, uint64_t& result){
  uint64_t mask1 = 0, mask0=0;

  std::vector<uint64_t> mem(1,0);
  for(auto lineit=input.begin(); lineit!=input.end(); lineit++){
    if(lineit->at(1)=='a'){
      mask1 = 0;
      mask0 = 0;
      for(size_t i=lineit->find('=')+2; i<lineit->size(); i++){
        mask1<<=1;
        mask0<<=1;
        if((*lineit)[i]=='1') mask1+=1;
        if((*lineit)[i]!='0') mask0+=1;
      }
    }
    else{
      int pos=stoi(lineit->substr(lineit->find('[')+1,lineit->find(']')));
      if(pos>=mem.size()) mem.resize(pos+1,0);
      mem[pos] = stoll(lineit->substr(lineit->find('=')+2));
      mem[pos]|=mask1;
      mem[pos]&=mask0;
      }
  }

  for(auto i:mem){
    result+=i;
  }
}

void dock2(std::vector<std::string>& input, uint64_t& result){
  std::string mask;
  std::vector<uint64_t> allpos(0);
  std::map<uint64_t,int> mem;
  for(auto lineit=input.begin(); lineit!=input.end(); lineit++){
    // std::cout << *lineit << std::endl;
    if(lineit->at(1)=='a'){
      mask=lineit->substr(lineit->find('=')+2);
    }
    else{
      int data = stol(lineit->substr(lineit->find('=')+2));

      allpos.resize(0);
      allpos.push_back(stol(lineit->substr(lineit->find('[')+1,lineit->find(']'))));
      int p=0;
      for(int i=mask.size()-1; i>=0; i--){
        int currcount = allpos.size();
        uint64_t m=1ul<<p;
        for(int pos=0; pos<currcount; pos++){
          if(mask[i]=='X') {
            allpos.push_back(allpos[pos]);
            allpos[pos]^=m;
          }
          if(mask[i]=='1') allpos[pos]|=m;
        }
        p++;
      }

      for(auto& pos:allpos){
        mem[pos]=data;
      }
    }
  }

  for(const auto& [key,value]:mem){
    result+=value;
  }
}

std::vector<std::string> readfile(std::string file){
  std::string line;
  std::ifstream input(file);
  std::vector<std::string> lines;

  if(input.is_open()){
  	while(getline(input,line)){
        lines.push_back(line);
    	}
      input.close();
    }
  else{
    std::cout << "Unable to open file\n";
  }
  return lines;
}

int main(){
  std::vector<std::string> input=readfile("input.txt");

  uint64_t result1=0, result2=0;
  dock(input, result1);
  dock2(input, result2);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
