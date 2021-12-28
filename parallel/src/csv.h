
#ifndef CSV_A

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>



std::vector<std::string> load_name_list(std::string FILENAME){
  std::vector<std::string> namesstr;
  std::ifstream file(FILENAME);
  if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        line.pop_back();
        line.erase(line.begin());//removing quotations
        namesstr.push_back(line);
      }
      file.close();
  }
  return namesstr;
}


void write_lines(std::string FILENAME, std::vector<std::string> vout){
  std::ofstream myfile(FILENAME);
  if(myfile.is_open())
  {
      for(int a = 0;a<vout.size();a++){
          myfile<<vout[a]<< std::endl;
      }
      myfile.close();
  }
}

void write_lines(std::string FILENAME, std::vector<double> vout){
  std::ofstream myfile(FILENAME);
  if(myfile.is_open())
  {
      for(int a = 0;a<vout.size();a++){
          myfile<<vout[a]<< std::endl;
      }
      myfile.close();
  }
}
#endif
