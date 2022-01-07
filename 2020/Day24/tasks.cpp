#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <queue>
#include <set>

typedef std::set<std::string> strset;
typedef std::vector<strset> strsetvec;
typedef std::vector<std::string> strvec;
typedef std::pair<int,int> point;

unsigned long result1=0, result2=0;

void updateCoords(int direction, point& x){
  switch(direction){
    case 1:{
      x.first +=1;
      x.second+=0;
      break;
    }
    case 2:{
      x.first +=0;
      x.second+=-1;
      break;
    }
    case 3:{
      x.first +=-1;
      x.second+=-1;
      break;
    }
    case 4:{
      x.first +=-1;
      x.second+=0;
      break;
    }
    case 5:{
      x.first +=0;
      x.second+=1;
      break;
    }
    case 6:{
      x.first +=1;
      x.second+=1;
      break;
    }
  }
}

int getNextDirection(std::string& line){
  if(line.empty()) return 0;
  char a= line.front();
  line.erase(0,1);
  if(a == 'e') return 1;
  if(a == 'w') return 4;

  char b = line.front();
  line.erase(0,1);
  if(a == 'n'){
    if(b == 'w') return 5;
    if(b == 'e') return 6;
  }
    if(b == 'w') return 3;
    //if(b == 'e')
      return 2;
}

point obtainTileCoords(std::string& line){
  point x={0,0};
  int nextDirection;
  do{
    nextDirection = getNextDirection(line);
    updateCoords(nextDirection, x);
  }while(nextDirection >0);
  return x;
}

void flipTile(std::set<point>& Tiles, const point& x){
  auto it = Tiles.insert(x);
  if(!it.second) Tiles.erase(x);
}

int countBlackNeighbors(const std::set<point>& blackTiles, const point& tile){
  int n=0;
  for(int i=-1; i<=1; i++){
    for(int j=-1; j<=1; j++){
      if(i==-1 && j==1 || i==0 && j==0 || i==1 && j==-1) continue;
      point x({tile.first+i, tile.second+j});
      if(blackTiles.find(x) != blackTiles.end()) n++;
    }
  }
  return n;
}

void checkWhiteTile(const std::set<point>& blackTiles, const point& x, std::set<point>& tilesToFlip){
  if(blackTiles.find(x)!=blackTiles.end()) return;

  int n = countBlackNeighbors(blackTiles, x);
  if(n==2) tilesToFlip.insert(x);
}

void checkNeighbors(const std::set<point>& blackTiles, const point& tile, std::set<point>& tilesToFlip){
  for(int i=-1; i<=1; i++){
    for(int j=-1; j<=1; j++){
      if(i==-1 && j==1 || i==0 && j==0 || i==1 && j==-1) continue;
      point x = {tile.first+i, tile.second+j};
      checkWhiteTile(blackTiles, x, tilesToFlip);
    }
  }
}

std::set<point> getTilesToFlip(const std::set<point>& blackTiles){
  std::set<point> tilesToFlip;
  for(auto& tile:blackTiles){
    int n=countBlackNeighbors(blackTiles, tile);
    if(n==0 || n>2) tilesToFlip.insert(tile);

    checkNeighbors(blackTiles, tile, tilesToFlip);
  }
  return tilesToFlip;
}

void flipTiles(std::set<point>& blackTiles, const std::set<point>& tilesToFlip){
  for(auto& tile:tilesToFlip){
    flipTile(blackTiles, tile);
  }
}

void doADay(std::set<point>& blackTiles){
  std::set<point> tilesToFlip = getTilesToFlip(blackTiles);
  flipTiles(blackTiles, tilesToFlip);
}

void doStuff(strvec& input){
  std::set<point> blackTiles;
  for(auto& line:input){
    point x = obtainTileCoords(line);
    flipTile(blackTiles, x);
  }
  result1 = blackTiles.size();


  for(int i=0; i<100; i++){
    doADay(blackTiles);
    std::cout << i+1 << ": " << blackTiles.size() << std::endl;
  }
  result2 = blackTiles.size();
}

strvec readfile(std::string file){
  std::string line;
  std::ifstream input(file);
  strvec lines;

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
  strvec input=readfile("input.txt");

  doStuff(input);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
