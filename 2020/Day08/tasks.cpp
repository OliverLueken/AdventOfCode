#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool run(std::vector<std::string>& lines, int& acc){

    std::vector<bool> linevisited(lines.size(),0);
    int currentline =0, count=0;
    char com;
    do{
      count++;
      std::cout << currentline << ", " << lines[currentline] << ", "<< linevisited[currentline] << std::endl;
      linevisited[currentline]=true;
      com=lines[currentline][0];
      switch(com){
        case 'n':{
          currentline++;
          break;
        }
        case 'j':{
          currentline+=stoi(lines[currentline].substr(4,50));
          break;
        }
        case 'a':{
          acc+=stoi(lines[currentline].substr(4,50));
          currentline++;
        }
      }
    }while(linevisited[currentline]==false && currentline <lines.size() && currentline >=0 && count <lines.size());

    return currentline==lines.size();
}

int main(){
  std::string line;
  std::ifstream input("input.txt");
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
  std::string a;
  int acc;
  bool terminatesuccessfully;
  for(auto it=lines.begin(); it!=lines.end(); it++){
    acc=0;
    if((*it)[0]=='n'){
      (*it)[0]='j';
      terminatesuccessfully = run(lines, acc);
      (*it)[0]='n';
    }
    else if((*it)[0]=='j'){
      (*it)[0]='n';
      terminatesuccessfully = run(lines, acc);
      (*it)[0]='j';
    }
    //std::getline(std::cin, a);
    if(terminatesuccessfully) break;
  }

  std::cout << acc << "\n";
}
