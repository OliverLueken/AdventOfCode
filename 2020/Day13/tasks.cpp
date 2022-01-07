#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>


void earliestbus(std::vector<std::string>& input, int& result){
  int t=stoi(input[0]);
  std::vector<int> ids;
  size_t a=0, b=0;
  while(b!=std::string::npos){
    b=input[1].find(",",a);
    try{
      ids.push_back(stoi(input[1].substr(a,b-a)));
    }
    catch (...){}
    a=b+1;
  }
  int min=t, minid;
  for(auto id:ids){
    int a=id-t%id;
    if(a<min){
      min=a;
      minid=id;
    }
  }
  result=minid*min;
}

void besttime(std::vector<std::string>& input, long& result){

  size_t a=0, b=0;
  std::vector<int> ni, bi;
  int i=0;
  long N=1;
  while(b!=std::string::npos){
    b=input[1].find(",",a);
    try{
      ni.push_back(stoi(input[1].substr(a,b-a)));
      bi.push_back(ni.back()-i);
      N*=ni.back();
      std::cout << ni.back() << ", " << bi.back() << std::endl;
    }
    catch (...){}
    i++;
    a=b+1;
  }
  std::cout << "N=" << N << std::endl;
  //chinese remainder theorem
  long x;
  result=0;
  for(int i=0; i<ni.size(); i++){
    long Ni=N/ni[i];
    int tempNi = Ni%ni[i];

    int x=1;
    for(; x<ni[i]; x++){
      if(x*tempNi%ni[i]==1){
        break;
      }
    }
    std::cout << Ni << ", " << x << std::endl;
    result+=x*Ni*bi[i];
  }
  result%=N;
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

  int result1; long result2=1;
  earliestbus(input, result1);
  besttime(input, result2);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
