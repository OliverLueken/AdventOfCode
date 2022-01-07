#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <regex>
#include <boost/algorithm/string.hpp>

typedef std::map<std::string, std::vector<std::pair<int,int>>> type_rules;


int result1=0; long result2=0;

void print(const std::map<std::string, std::vector<bool>>& possibleFieldsForKey){
  for(const auto& [key, v]: possibleFieldsForKey){
    std::cout << key << ": ";
    for(auto b:v) std::cout << b << ", ";
    std::cout << std::endl;
  }
}

void extractInfo(std::vector<std::string> input, std::map<std::string,std::vector<std::pair<int, int>>>& rules,
                                                 std::vector<std::vector<int>>& tickets){
  int mode=0;
  std::regex replace(" or ");
  for(auto lineit=input.begin(); lineit!=input.end(); lineit++){
    if(lineit->empty()){
      lineit++;
      mode++;
      continue;
    }
    if(mode==0){
      size_t a=lineit->find(":");
      std::string field=lineit->substr(0,a);
      std::string rule = lineit->substr(a+2);
      rule=std::regex_replace(rule, replace, ",");
      std::vector<std::string> temprule;
      boost::split(temprule, rule, boost::is_any_of(",-"));

      std::vector<std::pair<int,int>> valueoffield;
      for(int i=0; i<temprule.size(); i+=2){
        valueoffield.push_back({stoi(temprule[i]), stoi(temprule[i+1])});
      }

      rules[field]=valueoffield;
    }
    else{
      std::vector<std::string> temp;
      std::vector<int> fieldvalues;

      boost::split(temp, *lineit, boost::is_any_of(","));
      std::transform(temp.begin(), temp.end(), std::back_inserter(fieldvalues),
        [](std::string s) -> int {return std::stoi(s);});

      tickets.push_back(fieldvalues);
      //for(auto i:fieldvalues) std::cout << i << std::endl;
    }
  }
}

bool isValueValidWithField( const int value, const std::string& key, const type_rules& rules){
  for(const auto& pair:rules.at(key)){
    if(pair.first <= value && value <= pair.second)
      return true;
  }
  return false;
}

bool isValidValue(const int value, const type_rules& rules){
  for(const auto& [key,rule]:rules){
    if(isValueValidWithField(value, key, rules)){
      return true;
    }
  }
  return false;
}

bool isValidTicket(const std::vector<int>& ticket, const type_rules& rules ){
  bool isValidTicket = true;
  for(const auto value:ticket){
    if(!isValidValue(value, rules)){
      isValidTicket=false;
      result1+=value;
    }
  }
  return isValidTicket;
}


void filterInvalidTickets(const type_rules& rules, std::vector<std::vector<int>>& tickets){
  // for(auto& [k,v]:rules){
  //   std::cout << k << ":";
  //   for(auto& p:v){
  //     std::cout <<"\t" << p.first << ", " << p.second << std::endl;
  //   }
  // }
  for(auto ticketit=tickets.begin(); ticketit!=tickets.end(); ){
    if(!isValidTicket(*ticketit, rules)){
      ticketit=tickets.erase(ticketit);
    }
    else {
      ticketit++;
    }
  }
  //
  // for(const auto& t:tickets){
  //   for(const auto& v:t){
  //     std::cout << v << ", ";
  //   }
  //   std::cout << std::endl;
  // }


}

std::map<std::string, std::vector<bool>> buildBoolTable(const type_rules& rules, const std::vector<std::vector<int>>& tickets){
  std::map<std::string, std::vector<bool>> table;
  for(const auto& [key, rule]: rules){
    std::vector<bool> boolvecforkey;
    for(int i_value=0; i_value<tickets[0].size(); i_value++){
      bool allValuesPossible = true;
      for(const auto& ticket:tickets){
        if(!isValueValidWithField(ticket[i_value], key, rules)){
          allValuesPossible=false;
        }
      }
      boolvecforkey.push_back(allValuesPossible);
    }
    table[key]=boolvecforkey;
  }
  return table;
}

int countPossibleFields(const std::vector<bool>& vec){
  int n=0;
  for(const auto& b:vec) if(b) n++;
  return n;
}

int findOnlyTrue(const std::vector<bool> v){
  for(int i=0; i<v.size(); i++) if(v[i]) return i;
  return -1;
}

void updateOtherKeys(int i, std::map<std::string, std::vector<bool>>& table){
  for(auto& [key, vec]:table){
    vec[i]=false;
  }
}

std::map<std::string, int> evaluateTable(std::map<std::string, std::vector<bool>>& table){
  std::map<std::string, int> a;
  //std::cout << std::endl;
  int n=table.size();
  while(a.size()<n){
    for(auto tableit=table.begin(); tableit!=table.end();){
      const std::string& key = tableit->first;
      std::vector<bool> vec = tableit->second;
      int n = countPossibleFields(vec);
      if(n==1){

        int i=findOnlyTrue(vec);
        a[key]=i;
        //std::cout << key << ", " << i << std::endl;
        updateOtherKeys(i, table);
        tableit=table.erase(tableit);
        break;
      }
      else{
        tableit++;
      }
    }
    //print(table);
  }
  return a;
}

void determineFields(const type_rules& rules, const std::vector<std::vector<int>>& tickets){
  std::map<std::string, std::vector<bool>> possibleFieldsForKey;
  possibleFieldsForKey = buildBoolTable(rules, tickets);

  //print(possibleFieldsForKey);

  std::map<std::string, int> keyToField;
  keyToField = evaluateTable(possibleFieldsForKey);

  // for(const auto& [key, v]:keyToField){
  //   std::cout << key << ": " << v << std::endl;
  // }

  result2=1;
  for(const auto& [key,v]:keyToField){
    if(key.find("departure")==0){
      result2*=tickets[0][v];
    }
  }
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

  type_rules rules;
  std::vector<std::vector<int>> tickets;
  extractInfo(input, rules, tickets);

  filterInvalidTickets(rules, tickets);
  // besttime(input, result2);
  determineFields(rules, tickets);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
}
