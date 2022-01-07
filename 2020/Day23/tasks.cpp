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

class node{
public:
  int n;
  node* next;
};

class circle{
  int maxvalue;
  std::vector<node*> nodepos;

public:
  node* current;

  circle(std::string s, bool bigmode=false);
  void doNMoves(int n);
  void doOneMove();
  node* removeNextThree();
  bool isInCircle(node* d);
  node* getDestinationCup();
  node* findCup(int n);
  void insertCups(node* cups, node* destination);
  void printOrder(int from=1, int amount=9);
};

circle::circle(std::string s, bool bigmode){
  if(bigmode) nodepos.resize(10000000);
  else        nodepos.resize(9);

  node* c = new node;
  c->n = s[0]-'0';
  nodepos[c->n-1] = c;
  //std::cout << c->n;
  maxvalue = c->n;
  current = c;


  for(int i=1; i<s.size(); i++){
    node* b = new node;
    b->n=s[i]-'0';
    nodepos[b->n-1] = b;
    //std::cout << b->n;
    if(b->n>maxvalue) maxvalue = b->n;
    c->next = b;
    c=b;
    //std::cout << current->n;
  }
  if(bigmode){
    for(int i = maxvalue+1; i<=1000000; i++){
      node* b = new node;
      b->n=i;
      nodepos[b->n-1] = b;
      //std::cout << b->n << ", ";
      c->next = b;
      c=b;
    }
    maxvalue = 1000000;
  }
  c->next = current;
  std::cout << "\nConstruction complete\n";
}

void circle::doNMoves(int n){
  for(int i=0; i<n; i++){
    doOneMove();
  }
}

void  circle::doOneMove(){
  node* removedCups = removeNextThree();
  node* destinationCup = getDestinationCup();
  insertCups(removedCups, destinationCup);
  current = current->next;
  //printOrder();
}

node* circle::removeNextThree(){
  node* removedCups = current->next;
  current->next = removedCups->next->next->next;
  removedCups->next->next->next = NULL;
  return removedCups;
}

bool circle::isInCircle(node* d){
  if(d->next == NULL) return false;
  d=d->next;
  if(d->next == NULL) return false;
  d=d->next;
  if(d->next == NULL) return false;
  return true;
}

node* circle::getDestinationCup(){
  int n = current->n;
  node* destinationCup;
  bool isInC;
  do{
    n=n+maxvalue-2;
    n=n%maxvalue+1;
    destinationCup = findCup(n);
    isInC = isInCircle(destinationCup);
    //std::cout << "Destination Cup " << destinationCup->n << std::endl;
  }while(!isInC);
  return destinationCup;
}

node* circle::findCup(int n){
  // node* c = current->next;
  // do{
  //   //std::cout << "Looking at " << c->n << std::endl;
  //   if(c->n == n) return c;
  //   c=c->next;
  // }while(c!=current);
  // return c;
  //std::cout << "Find " << n <<std::endl;
  return nodepos[n-1];
}

void  circle::insertCups(node* cups, node* destination){
  cups->next->next->next = destination->next;
  destination->next = cups;
}

void circle::printOrder(int from, int amount){
  node* one = findCup(from);
  node* n = one->next;
  int i=0;
  while(i<amount){
    std::cout << n->n << ", ";
    n = n->next;
    i++;
  }
  std::cout << std::endl;
}

void doStuff(){
  std::string s= "135468729";
  circle c(s);
  // std::cout << c.current->n << std::endl;
  //c.printOrder();
  c.doNMoves(100);
  c.printOrder();

  circle c2(s, true);
  c2.doNMoves(10000000);
  c2.printOrder(1, 2);
}

// strvec readfile(std::string file){
//   std::string line;
//   std::ifstream input(file);
//   strvec lines;
//
//   if(input.is_open()){
//   	while(getline(input,line)){
//         lines.push_back(line);
//     	}
//       input.close();
//     }
//   else{
//     std::cout << "Unable to open file\n";
//   }
//   return lines;
// }

int main(){
  //strvec input=readfile("input.txt");

  doStuff();

  //std::cout << result1 << "\n";
  //std::cout << result2 << "\n";
}
