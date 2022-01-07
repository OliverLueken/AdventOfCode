#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void encodingerror(std::vector<long>& input, long& result1, long& result2){
  for(int i=25; i<input.size(); i++){
    bool isvalid=false;
    for(int a=i-25; a<i-1; a++){
      for(int b=a+1; b<i; b++){
        if(input[a]+input[b]==input[i]){
          isvalid=true;
          goto endcheckvalid;
        }
      }
    }
    endcheckvalid:
    if(!isvalid){
      result1=input[i];
      break;
    }
  }

  for(int i=0; i<input.size(); i++){
    long sum=input[i];
    long mini=result1, maxi=0;
    for(int j=i+1; sum<result1; j++){
      sum+=input[j];
      if( mini > input[j]) mini=input[j];
      if( maxi < input[j]) maxi=input[j];
    }
    if(sum==result1){
      result2=mini+maxi;
      break;
    }
  }
}

std::vector<long> readfile(std::string file){
  std::string line;
  std::ifstream input(file);
  std::vector<long> lines;

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
  std::vector<long> input=readfile("input.txt");

  long result1, result2;
  encodingerror(input, result1, result2);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
