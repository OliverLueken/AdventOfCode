#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>

//#define triple std::tuple<int,int,int>
#define triple std::tuple<int,int,int,int>

void printcubes(std::set<triple>& c){
  for(auto it:c){
    std::cout << "(" << std::get<0>(it) << ", " << std::get<1>(it) << ", " << std::get<2>(it) << ")\n";
  }
  std::cout << "\n\n";
}

int countActiveNeighbors(triple pos, std::set<triple>& activeCubes){
  int neighbors=0;
  int posx=std::get<0>(pos);
  int posy=std::get<1>(pos);
  int posz=std::get<2>(pos);
  int posw=std::get<3>(pos);
  for(int x=posx-1; x<=posx+1; x++){
    for(int y=posy-1; y<=posy+1; y++){
      for(int z=posz-1; z<=posz+1; z++){
        for(int w=posw-1; w<=posw+1; w++){
          triple pos2={x,y,z,w};
          if(activeCubes.find(pos2)!=activeCubes.end()){
            neighbors++;
          }
        }
      }
    }
  }
  if(activeCubes.find(pos)!=activeCubes.end()) neighbors--;
  return neighbors;
}

void update(std::set<triple>& activeCubes, triple boundlower, triple boundupper){
  std::set<triple> cubesToActivate, cubesToDeactivate;

  //check status of cubes
  for(int x=std::get<0>(boundlower); x<std::get<0>(boundupper); x++){
    for(int y=std::get<1>(boundlower); y<std::get<1>(boundupper); y++){
      for(int z=std::get<2>(boundlower); z<std::get<2>(boundupper); z++){
        for(int w=std::get<3>(boundlower); w<std::get<3>(boundupper); w++){
          //std::cout << "checking (" << x <<", " << y << ", " << z << ")\n";
          triple pos = std::make_tuple(x,y,z,w);
          int neighbors = countActiveNeighbors(pos, activeCubes);
          //std::cout << neighbors << std::endl;
          if(activeCubes.find(pos)!=activeCubes.end()) { //active
            if(neighbors!=2 && neighbors!=3){
              cubesToDeactivate.insert(pos);
            }
          } else { //inactive
            if(neighbors==3)
              cubesToActivate.insert(pos);
          }
        }
      }
    }
  }
  //printcubes(cubesToDeactivate);
  //deactivate cubes
  for(auto pos:cubesToDeactivate){
    activeCubes.erase(pos);
  }

  //printcubes(cubesToActivate);
  //activate cubes
  for(auto pos:cubesToActivate){
    activeCubes.insert(pos);
  }
}

std::set<triple> initializeCubes(std::vector<std::string>& input){
  std::set<triple> activeCubes;
  for(int y=0; y<input.size(); y++){
    //std::cout << input[y] << std::endl;
    for(int x=0; x<input[y].size(); x++){
      if(input[y][x]=='#'){
        activeCubes.insert(std::make_tuple(x,y,0,0));
      }
    }
  }
  return activeCubes;
}

void conwayCubes(std::vector<std::string>& input, int& result){
  std::set<triple> activeCubes = initializeCubes(input);

  //printcubes(activeCubes);

  int u = input[0].size()+6;
  triple boundlower = {-6,-6,-6,-6},
         boundupper = {u,u,u,u};

  for(int i=0; i<6; i++){
    update(activeCubes, boundlower, boundupper);
    //printcubes(activeCubes);
  }

  result=activeCubes.size();
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

  int result;
  conwayCubes(input, result);

  std::cout << result << "\n";
}
