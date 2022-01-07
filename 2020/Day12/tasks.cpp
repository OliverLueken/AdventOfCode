#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
const double pi = std::acos(-1);

void navigate(std::vector<std::string> input, int& result1){
  int dir=0;
  int x=0, y=0;
  for(auto it:input){
    //std::cout << it << std::endl;
    char c = it[0];
    int val = stoi(it.substr(1,50));
    if(c=='F'){
      switch(dir){
        case 0:{
          c='E';
          break;
        }
        case 90:{
          c='S';
          break;
        }
        case 180:{
          c='W';
          break;
        }
        case 270:{
          c='N';
          break;
        }
      }
    }

    switch(c){
      case 'E':{
        x+=val;
        break;
      }
      case 'N':{
        y+=val;
        break;
      }
      case 'W':{
        x-=val;
        break;
      }
      case 'S':{
        y-=val;
        break;
      }
      case 'L':{
        dir=360+dir-val;
        dir%=360;
        break;
      }
      case 'R':{
        dir+=val;
        dir%=360;
      }

    }
    //std::cout << x << ", " << y << ", " << dir << std::endl << std::endl;
  }
  result1=std::abs(x)+std::abs(y);
}
void navigate2(std::vector<std::string> input, double& result2){
  int dir=0;
  double x=0, y=0;
  double wx=10, wy=1;

  for(auto it:input){
    std::cout << it << std::endl;
    char c = it[0];
    int val = stoi(it.substr(1,50));

    switch(c){
      case 'E':{
        wx+=val;
        break;
      }
      case 'N':{
        wy+=val;
        break;
      }
      case 'W':{
        wx-=val;
        break;
      }
      case 'S':{
        wy-=val;
        break;
      }
      case 'L':{
        double a=wx, b=wy;
        wx=std::cos(val*pi/180)*a+std::sin(-val*pi/180)*b;
        wy=std::sin(val*pi/180)*a+std::cos(val*pi/180)*b;
        break;
      }
      case 'R':{
        double a=wx, b=wy;
        wx=std::cos(-val*pi/180)*a+std::sin(val*pi/180)*b;
        wy=std::sin(-val*pi/180)*a+std::cos(-val*pi/180)*b;
        break;
      }
      case 'F':{
        x+=val*wx;
        y+=val*wy;
      }

    }
    std::cout << x << ", " << y << std::endl;
    std::cout << wx << ", " << wy << std::endl<<std::endl;
  }
  result2=std::abs(x)+std::abs(y);
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

  int result1; double result2=1;
  navigate(input, result1);
  navigate2(input, result2);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
