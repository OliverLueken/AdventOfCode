#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(){
  std::string line;
  std::ifstream input("input.txt");
  std::vector<int> yesanswers(26,0);

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

  int total1=0;
  int total2=0;
  int grpsize=0;
  for(int i=0; i<=lines.size(); i++){
    if(i==lines.size() || lines[i].empty()){
      for(auto i:yesanswers){
        if(i>0) total1++;
        if(i==grpsize) total2++;
      }
      grpsize=0;
      yesanswers.assign(26,0);
    }
    else{
      grpsize++;
      for(auto c:lines[i]){
        yesanswers[c-'a']++;
      }
    }
  }

  std::cout << total1 << "\n";
  std::cout << total2 << "\n";
}
