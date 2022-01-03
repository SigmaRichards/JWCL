
#ifndef CSV_A

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

std::vector<std::string> unique_strings(std::vector<std::string> in){
  std::vector<std::string> out = in;
  std::stable_sort(out.begin(), out.end());
  out.erase(std::unique(out.begin(), out.end()), out.end());
  return out;
}

/* OLD
std::vector<std::vector<int>> inds_each_unique(std::vector<std::string> uqs,std::vector<std::string> ref,std::vector<int> sind){
  std::vector<std::vector<int>> out(uqs.size());

  for(int i = 0; i < sind.size();i++){
    for(int j = 0; j < uqs.size() ; j++){
      if(ref[sind[i]]==uqs[j]){
        out[j].push_back(sind[i]);
        break;
      }
    }
  }
  return out;
}*/

std::vector<std::vector<int>> inds_each_unique(std::vector<std::string> uqs,std::vector<std::string> ref,std::vector<int> sind){
  std::vector<std::vector<int>> out(uqs.size());

  int i = 0;
  int j = 0;
  while(i < ref.size()){
    if(ref[sind[i]]==uqs[j]){
      out[j].push_back(sind[i]);
      i++;
    }else{
      j++;
    }
  }
  return out;
}

template <typename T>
std::vector<int> sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<int> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);
  std::stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

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


std::vector<std::vector<int>> parse_gpu(std::vector<std::vector<std::vector<int>>> vin,int MBS){
  std::vector<std::vector<int>> vout;
  int row;
  int col;
  int score;
  for(int i = 0; i<2 ;i++){
    for(int j = 0; j<vin[i].size() ;j++){
      row = vin[i][j][2]/MBS + vin[i][j][0]*MBS;
      col = vin[i][j][2]%MBS + vin[i][j][1]*MBS;
      score = i;
      std::vector<int> f = {row,col,score};
      vout.push_back(f);
    }
  }
  return vout;
}

std::vector<std::vector<int>> copy_vout(std::vector<std::vector<int>> vin,
                                   std::vector<std::vector<int>> aref,
                                   std::vector<std::vector<int>> bref){
  std::vector<int> in;
  std::vector<std::vector<int>> vout;
  for(int i = 0; i < vin.size() ; i++){
    for(int j = 0; j < aref[vin[i][0]].size() ; j++){
      for(int k = 0; k < bref[vin[i][1]].size() ; k++){
        in.push_back(aref[vin[i][0]][j]);
        in.push_back(bref[vin[i][1]][k]);
        in.push_back(vin[i][2]);
        vout.push_back(in);
        in.clear();
      }
    }
  }
  return vout;
}

std::vector<std::string> convert_vout(std::vector<std::vector<int>> vin){
  std::vector<std::string> vout;
  std::string temp;
  for(int i = 0; i < vin.size() ; i++){
    temp = "";
    temp = std::to_string(vin[i][0]) + "," +std::to_string(vin[i][1]) + "," +std::to_string(vin[i][2]);
    vout.push_back(temp);
  }
  return vout;
}

void write_lines(std::string FILENAME,
                 std::vector<std::vector<int>> vout,
                 std::vector<std::vector<int>> aref,
                 std::vector<std::vector<int>> bref){
  std::ofstream myfile(FILENAME);
  std::vector<std::string> l2w;
  std::vector<std::vector<int>> vt;
  if(myfile.is_open())
  {
    for(int a = 0;a<vout.size();a++){
      vt.clear();
      l2w.clear();

      vt.push_back(vout[a]);
      l2w = convert_vout(copy_vout(vt,aref,bref));
      for(int i = 0; i < l2w.size(); i++){
        myfile<<l2w[i]<< std::endl;
      }
    }
    myfile.close();
  }
}

void write_gpu_out(std::string FILENAME,int MBS,
                   std::vector<std::vector<std::vector<int>>> vin,
                   std::vector<std::vector<int>> aref,
                   std::vector<std::vector<int>> bref){
  std::vector<std::vector<int>> vout = parse_gpu(vin,MBS);
  write_lines(FILENAME,vout,aref,bref);
}
#endif
