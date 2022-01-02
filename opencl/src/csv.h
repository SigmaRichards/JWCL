
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

std::vector<std::string> parse_gpu(std::vector<std::vector<std::vector<int>>> vin,int MBS){
  std::vector<std::string> vout;
  std::string row;
  std::string col;
  std::string score;
  std::string f;
  for(int i = 0; i<2 ;i++){
    for(int j = 0; j<vin[i].size() ;j++){
      row = std::to_string( vin[i][j][2]/MBS + vin[i][j][0]*MBS);
      col = std::to_string( vin[i][j][2]%MBS + vin[i][j][1]*MBS);
      score = std::to_string(i);
      f = row+","+col+","+score;
      vout.push_back(f);
    }
  }
  return vout;
}

void write_gpu_out(std::string FILENAME,std::vector<std::vector<std::vector<int>>> vin,int MBS){
  std::vector<std::string> vout = parse_gpu(vin,MBS);
  write_lines(FILENAME,vout);
}
#endif
