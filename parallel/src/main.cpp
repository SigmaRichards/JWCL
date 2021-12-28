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
#include <chrono>

#include "jwdist.h"
#include "csv.h"
#include "parops.h"

int main( int argc, char *argv[] ){

  int NUM_THREADS = 15;


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

  std::cout<<"Calculating distances...";
  std::cout.flush();
  auto start = std::chrono::steady_clock::now();
  std::vector<std::string> vout = do_par(names,NUM_THREADS);;
  auto end = std::chrono::steady_clock::now();
  double time_spent = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000000;
  double ips = ((double)(names.size()*names.size()))/time_spent;
  std::cout<<"Done"<<std::endl;

  //write to file
  std::cout<<"Writing file...";
  std::cout.flush();
  write_lines(FILEOUT,vout);
  std::cout<<"Done"<<std::endl;



  std::cout<< "Number of comparisons per second: " << ips <<std::endl;
  /*
  for(int a = 1; a < 20; a++){
    auto start = std::chrono::steady_clock::now();
    do_par(names,a);
    auto end = std::chrono::steady_clock::now();
    std::cout<< (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/1000 <<std::endl;
  }*/
  //std::cout<< edit_distance::jaro_winkler("xBnrtOJ8bpD5","mEI8XSn747Th") <<std::endl;

  return 0;
}
