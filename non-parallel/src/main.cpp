/*Calculates the JW distance for all pairs of strings in an input file.

Input format is ./sjw [inputfile] [outputfile]. Each line of input file is
treated as a unique string and (should) accept and character value.

Running complexity ~O(N^2) due to cartesian product of input (and itself) -
thus is unavoidable. Although, this program will (unnecessarily) compute d(a,b)
and d(b,a). The expectation is that this will lead into comparing 2 different
input files. Also, this is a proof of concept and is used for testing algorithm
speed on a single thread - thus reducing the number of comparisons will
artificially reduce computation time. Optimizations will be applied in the final
version.

Original JW implementation done by Stefan Hamminga (s@stefanhamminga.com) which
has been (very slightly) adjusted for speed.*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <time.h>

#include "jwdist.h"
#include "csv.h"

int main( int argc, char *argv[] ){

  if( argc != 3){
    return 1;
  }
  std::string FILEIN = argv[1];
  std::string FILEOUT = argv[2];


  std::cout<<"Reading names...";
  std::cout.flush();
  std::vector<std::string> nameso = load_name_list(FILEIN);
  std::vector<std::string_view> names;
  std::string_view tt;
  for(int a = 0; a < nameso.size() ; a++){
    tt = nameso[a];
    names.push_back(tt);
  }
  std::cout<<"Done"<<std::endl;

  ///*
  std::cout<<"Calculating distances...";
  std::cout.flush();
  clock_t begin = clock();
  std::vector<double> vout = jw_self_outer(names);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  double ips = ((double)(names.size()*names.size()))/time_spent;
  std::cout<<"Done"<<std::endl;

  //write to file
  std::vector<std::string> sout;
  int i = 0;
  for(int a = 0; a < names.size() ; a++){
    for(int b = 0; b < names.size() ; b++){
      sout.push_back(nameso[a]+","+nameso[b]+","+std::to_string(vout[i]));
      i++;
    }
  }
  std::cout<<"Writing file...";
  std::cout.flush();
  write_lines(FILEOUT,sout);
  std::cout<<"Done"<<std::endl;



  std::cout<< "Number of comparisons per second: " << ips <<std::endl;
  //*/

  /*

  std::cout<< edit_distance::jaro_winkler("xBnrtOJ8bpD5","mEI8XSn747Th") <<std::endl;*/

  return 0;
}
