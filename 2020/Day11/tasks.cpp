#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>

//#define triple std::tuple<int,int,int>
#define pair std::tuple<int,int>

void printseats(std::vector<std::string>& c){
  for(auto it:c){
    std::cout << it << std::endl;
  }
  std::cout << "\n\n";
}

int countActiveNeighbors(int posx, int posy, std::vector<std::string>& seatingplan){
  int neighbors=0;
  int maxx =seatingplan.size()-1;
  int maxy = seatingplan[0].size()-1;
  for(int x=std::max(posx-1,0); x<=std::min(posx+1,maxx); x++){
    for(int y=std::max(posy-1,0); y<=std::min(posy+1,maxy); y++){

          if(seatingplan[x][y]=='#'){
            neighbors++;
      }
    }
  }
  if(seatingplan[posx][posy]=='#') neighbors--;
  return neighbors;
}

int countNeighbors(int posx, int posy, std::vector<std::string>& seatingplan){
  int neighbors=0;
  int maxx = seatingplan.size();
  int maxy = seatingplan[0].size();

  //nach oben links
  for(int i=1; i<=std::min(posx,posy); i++){
      if(seatingplan[posx-i][posy-i]=='#'){ neighbors++; break;}
      if(seatingplan[posx-i][posy-i]=='L') break;
  }
  //nach unten links
  for(int i=1; i<=std::min(posx,maxy-posy-1); i++){
      if(seatingplan[posx-i][posy+i]=='#'){ neighbors++; break;}
      if(seatingplan[posx-i][posy+i]=='L') break;
  }
  //nach links
  for(int x=posx-1; x>=0; x--){
    if(seatingplan[x][posy]=='#'){ neighbors++; break;}
    if(seatingplan[x][posy]=='L') break;
  }
  //nach oben
  for(int y=posy-1; y>=0; y--){
    if(seatingplan[posx][y]=='#'){ neighbors++; break;}
    if(seatingplan[posx][y]=='L') break;
  }
  //nach unten
  for(int y=posy+1; y<maxy; y++){
    if(seatingplan[posx][y]=='#'){ neighbors++; break;}
    if(seatingplan[posx][y]=='L') break;
  }
  //nach oben rechts
  for(int i=1; i<=std::min(maxx-posx-1,posy); i++){
      if(seatingplan[posx+i][posy-i]=='#'){ neighbors++; break;}
      if(seatingplan[posx+i][posy-i]=='L') break;
  }
    //nach unten rechts
  for(int i=1; i<=std::min(maxx-posx-1,maxy-posy-1); i++){
      if(seatingplan[posx+i][posy+i]=='#'){ neighbors++; break;}
      if(seatingplan[posx+i][posy+i]=='L') break;
  }
  //nach rechts
  for(int x=posx+1; x<maxx; x++){
    if(seatingplan[x][posy]=='#'){ neighbors++; break;}
    if(seatingplan[x][posy]=='L') break;
  }
  return neighbors;
}

bool update(std::vector<std::string>& seatingplan, int mode){
  std::set<pair> seatsToOccupie, seatsToFree;
  //check status of cubes
  for(int x=0; x<seatingplan.size(); x++){
    for(int y=0; y<seatingplan[x].size(); y++){
          //std::cout << "checking (" << x <<", " << y << ", " << z << ")\n";
          //triple pos = std::make_tuple(x,y,z,w);
      if(seatingplan[x][y]!='.'){
        if(mode==1){
          int neighbors = countActiveNeighbors(x, y, seatingplan);
          if(seatingplan[x][y]=='#') { //occupied
            if(neighbors>=4){
              seatsToFree.insert({x,y});
            }
          } else { //is a free seat
            if(neighbors==0)
              seatsToOccupie.insert({x,y});
          }
        }
        if(mode==2){
          int neighbors = countNeighbors(x,y, seatingplan);
          if(seatingplan[x][y]=='#'){
            if(neighbors>=5)
              seatsToFree.insert({x,y});
          }
          else if(neighbors==0)
            seatsToOccupie.insert({x,y});
        }
      }
    }
  }


  //printcubes(cubesToDeactivate);
  //deactivate cubes
  for(auto pos:seatsToFree){
    seatingplan[std::get<0>(pos)][std::get<1>(pos)]='L';
  }

  //printcubes(cubesToActivate);
  //activate cubes
  for(auto pos:seatsToOccupie){
    seatingplan[std::get<0>(pos)][std::get<1>(pos)]='#';
  }

  return (seatsToFree.size()>0 || seatsToOccupie.size()>0);
}


void seatingsystem(std::vector<std::string>& seatingplan, int& result, int mode){
  int i;
  for(i=0; i<1000; i++){
    bool hadchanges = update(seatingplan, mode);
    if(!hadchanges) break;
  }

  int occupiedseats=0;
  for(int i=0; i<seatingplan.size(); i++)
    for(int j=0; j<seatingplan[i].size(); j++){
      if(seatingplan[i][j]=='#') occupiedseats++;
    }
  result=occupiedseats;
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
  std::vector<std::string> seatingplan2 = input;

  int result1, result2;
  seatingsystem(input, result1, 1);
  seatingsystem(seatingplan2, result2, 2);
  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
