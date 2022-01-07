#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(){
  std::string line;
  std::ifstream input("input.txt");
  std::vector<bool> occupiedseats(2048,0);

  int biggest=0;
  if(input.is_open()){
  	while(getline(input,line)){
        unsigned int id=0;
        for(int i=0; i<7; i++){
          id<<=1;
          if(line[i]!='F') id++;
        }
        for(int i=7; i<10; i++){
          id<<=1;
          if(line[i]!='L') id++;
        }
        if(id>biggest) biggest=id;
        occupiedseats[id]=true;
        std::cout << line << ", " << id << std::endl;
    	}
      input.close();
    }
  else
    std::cout << "Unable to open file\n";

  // unsigned int a=1;
  // for(int i=0; i<10; i++){
  //   std::cout << a << std::endl;
  //   a<<1;
  // }
  int firstocc=0;
  for(int i=0; i<occupiedseats.size(); i++)
    if(occupiedseats[i]){
      firstocc=i;
      break;
    }
  int myseat=0;
  for(int i=firstocc; i<occupiedseats.size(); i++){
    if(!occupiedseats[i]){
      myseat=i;
      break;
    }
  }

    std::cout << biggest << "\n";
      std::cout << myseat << "\n";
}
