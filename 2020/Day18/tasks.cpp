#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <regex>

unsigned long long result1=0, result2=0;

std::string innerExpressionltr(std::string ex){
  size_t a, b;

  a=ex.find_first_of("+*");
  while(a!=std::string::npos){
    b=ex.find_first_of("+*",a+1);
    long x,y;
    x=stol(ex);
    y=stol(ex.substr(a+1));
    if(ex[a]=='+'){
      x=x+y;
    }
    else{
      x=x*y;
    }
    ex.replace(0, b, std::to_string(x));
    a=ex.find_first_of("+*");
  }
  return ex;
}


std::string innerExpressionPlus(std::string ex){
  size_t a,b,c;
  b=ex.find("+");
  while(b!=std::string::npos){
    a=ex.find_last_of("+*", b-1);
    if(a==std::string::npos) a=0;
    else a=a+2;
    c=ex.find_first_of("+*",b+1);

    long x,y;

    x=stol(ex.substr(a));
    y=stol(ex.substr(b+1));
    x+=y;
    ex.replace(a,c-1-a, std::to_string(x));

    b=ex.find("+");
  }
  return innerExpressionltr(ex);
}

unsigned long long outerExpression(std::string ex, int mode){
  const std::regex regex("([(][^()]+[)])"); //matches brackets with stuff in between that's not brackets
  std::smatch match;
  while(std::regex_search(ex, match, regex)){

    std::string eval;
    if(mode==1)
      eval = innerExpressionltr(match.str().substr(1,match.length()-2));
    else
      eval = innerExpressionPlus(match.str().substr(1,match.length()-2));
    ex.replace(match.position(), match.length(), eval);
  }
  unsigned long long result;
  if(mode==1) result = stoll(innerExpressionltr(ex));
  else        result = stoll(innerExpressionPlus(ex));
  return result;
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

void evalAll(std::vector<std::string>& input){

  unsigned long long x;
  for(auto& ex:input){x=outerExpression(ex,1);
    result1+=x;
    x=outerExpression(ex,2);
    result2+=x;
  }
}

int main(){
  std::vector<std::string> input=readfile("input.txt");

  evalAll(input);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
