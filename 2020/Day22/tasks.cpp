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
typedef std::deque<int> deck;

unsigned long result1=0, result2=0;

void dealDeck(const strvec& input, deck& deck1, deck& deck2){
  int i;
  for(i=1; i<input.size(); i++){
    if(input[i].empty()) break;
    deck1.push_back(stoi(input[i]));
  }
  i+=2;
  for(;i<input.size();i++){
    deck2.push_back(stoi(input[i]));
  }
}

int updateDeck(deck& deck1, deck& deck2, int roundWonBy, int a, int b){
  if(roundWonBy == 1){
    deck1.push_back(a);
    deck1.push_back(b);
    if(deck2.empty()){
      return 1;
    }
  }
  else{
    deck2.push_back(b);
    deck2.push_back(a);
    if(deck1.empty()){
      return 2;
    }
  }
  return 0;
}


bool deckAlreadyExisted(deck& deck1, deck& deck2, std::set<std::pair<deck,deck>>& existingDecks){
  std::pair<deck,deck> p = {deck1, deck2};

  auto it = existingDecks.insert(p);

  return !it.second;
}

deck firstNcards(deck d, int n){

  while(d.size()>n) d.pop_back();

  return d;
}

void printdecks(deck d1, deck d2){
  std::cout << "Deck 1: ";
  while(d1.size()>0){
    std::cout << d1.front() << ", ";
    d1.pop_front();
  }
  std::cout << std::endl;

  std::cout << "Deck 2: ";
  while(d2.size()>0){
    std::cout << d2.front() << ", ";
    d2.pop_front();
  }
  std::cout << std::endl;
}

int playGame2(deck deck1, deck deck2, int depth = 0){
  std::set<std::pair<deck,deck>> existingDecks;
  int a, b;
  int gameWonBy = 0;
  int round = 1;
  while(gameWonBy==0){
    std::cout << "Round " << round << " (Game " << depth+1 << ")\n";
    printdecks(deck1, deck2);

    if(deckAlreadyExisted(deck1, deck2, existingDecks)){
      gameWonBy = 1;
      break;
    }

    //Draw
    a=deck1.front();
    b=deck2.front();
    deck1.pop_front();
    deck2.pop_front();

    std::cout << a << std::endl << b << std::endl;

    int roundWonBy;
    //Do recursive call?
    if(deck1.size() >= a && deck2.size() >= b){
      deck deck1copy = firstNcards(deck1, a);
      deck deck2copy = firstNcards(deck2, b);
      roundWonBy = playGame2(deck1copy, deck2copy, depth+1);
    }
    else{
      if(a>b) roundWonBy = 1;
      else    roundWonBy = 2;
    }
    std::cout << "Round won by " << roundWonBy << std::endl << std::endl;

    gameWonBy = updateDeck(deck1, deck2, roundWonBy, a, b);
  }

  if(depth==0) {
    if(gameWonBy==2) deck1=deck2;
    for(int i=deck1.size(); i>0; i--){
      result2+=i*deck1.front();
      deck1.pop_front();
    }
  }
  return gameWonBy;
}

void playGame1(deck deck1, deck deck2){
  int a, b;
  int gameWonBy = 0;
  while(gameWonBy==0){
    a=deck1.front();
    b=deck2.front();
    deck1.pop_front();
    deck2.pop_front();

    bool player1wonRound = a>b;
    gameWonBy = updateDeck(deck1, deck2, player1wonRound, a, b);
  }

  if(gameWonBy==2) deck1=deck2;
  for(int i=deck1.size(); i>0; i--){
    result1+=i*deck1.front();
    deck1.pop_front();
  }
}

void doStuff(const strvec& input){
  deck deck1, deck2;
  dealDeck(input, deck1, deck2);
  playGame1(deck1, deck2);
  playGame2(deck1, deck2);

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