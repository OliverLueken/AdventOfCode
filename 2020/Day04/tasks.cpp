#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool fieldIsValid(int type, std::string info){
  try{
    switch(type){
      case 0: {//byr
        int birthyear = stoi(info);
        if(birthyear <1920 || birthyear > 2002 )
          return false;
        break;
      }
      case 1:{ //iyr
        int issueyear = stoi(info);
        if(issueyear <2010 || issueyear > 2020)
          return false;
        break;
      }
      case 2:{ //eyr
        int expiratioinyear = stoi(info);
        if(expiratioinyear < 2020 || expiratioinyear >2030)
          return false;
        break;
      }
      case 3:{ //hgt
        int height = stoi(info.substr(0,info.size()-2));
        if(info.substr(info.size()-2,2)=="cm"){
          if(height<150 || height >193)
            return false;
        } else if(info.substr(info.size()-2,2)=="in"){
          if(height<59|| height > 76)
            return false;
        } else
          return false;
        break;
      }
      case 4:{ //hcl
        if(info[0]!='#')   return false;
        if(info.size()!=7) return false;
        for(int i=1; i<info.size(); i++)
          if(isxdigit(info[i])==0 || isupper(info[i])!=0)
            return false;
        break;
      }
      case 5:{ //ecl
        if(info.size()!=3) return false;
        if(info == "amb" || info == "blu" || info == "brn" || info == "gry" || info == "grn" || info == "hzl" || info == "oth") return true;
        return false;
        break;
      }
      case 6:{ //pid
        if(info.size()!=9) return false;
        int n = stoi(info);
        break;
      }
    }
  }
  catch(const std::exception& ex){
    return false;
  }
  return true;
}

int main(){
    std::string line;
    std::ifstream input("input.txt");
    std::vector<std::string> passports;
    std::vector<std::string> fields;
      fields.push_back("byr");
      fields.push_back("iyr");
      fields.push_back("eyr");
      fields.push_back("hgt");
      fields.push_back("hcl");
      fields.push_back("ecl");
      fields.push_back("pid");
    std::vector<bool> hasvalidfield(fields.size(),0);

  if(input.is_open()){
  	while(getline(input,line)){
        passports.push_back(line);
    	}
      input.close();
    }
  else
    std::cout << "Unable to open file\n";

  int validpassports=0;
  for(int line=0; line<=passports.size(); line++){
    if(line==passports.size() || passports[line].empty()){
      bool isvalid=true;
      for(auto it=hasvalidfield.begin(); it!=hasvalidfield.end(); it++){
        if(!*it){
          isvalid=false;
        }
        *it=false;
      }
      if(isvalid) validpassports++;
    }
    else{
      for(int i=0; i<fields.size(); i++){
        size_t a = passports[line].find(fields[i]+":");
        if(a!=std::string::npos){
          size_t b = passports[line].find(" ", a);
          hasvalidfield[i]=fieldIsValid(i,passports[line].substr(a+4,b-a-4));
        }
      }
    }

  }

  std::cout << validpassports << "\n";
}
