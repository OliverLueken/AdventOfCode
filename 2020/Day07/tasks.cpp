#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

long sumallbags(std::string bag, std::vector<std::string>* rules, int depth){
  long bagcount=0;
  for(auto& rule:*rules){
    if(rule.find(bag)==0){
      size_t a;
      std::string thisrule=rule;
      a=thisrule.find("contain ")+6;
      do{
        thisrule=thisrule.substr(a+2,rule.size());
        // std::cout << thisrule << std::endl;
        try{
          int num=std::stoi(thisrule);

          a=thisrule.find("bag");
          std::string thisbag = thisrule.substr(2,a-3);
          std::cout << std::string(depth,' ') << num << " " << thisbag << std::endl;
          bagcount+=num+num*sumallbags(thisbag,rules, depth+2);
            // std::cout << bagcount << std::endl;
        }
        catch (...){
        }
        a=thisrule.find(", ",a);

      }while(a!=std::string::npos);

    }
  }
    std::cout << std::string(depth,' ') << bagcount << std::endl;
  return bagcount;
}

void findallouter(std::vector<std::string>* outerbags, std::vector<std::string>* rules){  //std::unordered_map<std::string,std::vector<std::string>>* bagchildren){

  for(auto bagit=outerbags->begin(); bagit!=outerbags->end(); bagit++){
    size_t a;
    for(auto it=rules->begin(); it!=rules->end(); ){
      a=it->find("bag");
      if(it->find(*bagit,a)!=std::string::npos){
        std::string newbag=it->substr(0,a-1);
        outerbags->push_back(newbag);
        it=rules->erase(it);
        continue;
      }
      it++;
    }
  }
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

  long int bagcount = sumallbags("shiny gold", &lines,0);


  std::vector<std::string> outerbags;
  outerbags.reserve(lines.size());
  outerbags.push_back("shiny gold");
  findallouter(&outerbags, &lines);

  std::cout << outerbags.size()-1 << "\n";
  std::cout << bagcount << "\n";
}
