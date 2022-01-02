/*Calculates the JW distance for all pairs of strings in an input file and
outputs the ids for full matches and partial matches. A given match is
considered full if the JW distance is equal to or greater than HI_THRES. It is
considered only partial if the distance is less the HI_THRES but greater than
LO_THRES.

Input format is ./sjw [input1] [input2] [outputfile]. Each line of input file is
treated as a unique string.

Running complexity ~O(N^2) due to cartesian product of inputs - thus is
unavoidable. Optimizations still need to be applied. Testing has shown that the
speed running on a GTX1660S is roughly 20x faster than 15 threads on a 3700X.
Currently if there are too many strings to compare the GPU will likely run out
of memory.

TODO: Calculate memory requirements and subdivide task into mini-batches.

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
#include "oclfuncs.h"

int main( int argc, char *argv[] ){

  const int MAX_BATCH_SIZE = 10000;
  int lws = 4;


  if( argc != 4){
    return 1;
  }
  std::string FILEIN1 = argv[1];
  std::string FILEIN2 = argv[2];
  std::string FILEOUT = argv[3];

  std::vector<std::string> av = load_name_list(FILEIN1);
  std::vector<std::string> bv = load_name_list(FILEIN2);

  //Data preparation
  const int len1 = av.size();
  const int len2 = bv.size();
  std::vector<float> FLOAT_BUFF(MAX_BATCH_SIZE*MAX_BATCH_SIZE,0.0);
  int times = ceil((float)(len1)/(float)(MAX_BATCH_SIZE))*ceil((float)(len2)/(float)(MAX_BATCH_SIZE));

  std::string astr;
  std::string bstr;
  int aloc[len1];
  int bloc[len2];
  char apv[len1];
  char bpv[len2];
  std::vector<int> asi;
  std::vector<int> bsi;

  int cind = 0;
  for(int i = 0; i < len1 ; i++) {
    astr += av[i];
    asi.push_back(av[i].length());
    aloc[i] = cind;
    cind+=av[i].length();
  }
  cind = 0;
  for(int i = 0; i < len2 ; i++){
    bstr += bv[i];
    bsi.push_back(bv[i].length());
    bloc[i] = cind;
    cind+=bv[i].length();
  }

  //Pointers for GPU
  char *a = &astr[0];
  char *b = &bstr[0];
  int *as = &asi[0];
  int *bs = &bsi[0];
  int *al = &aloc[0];
  int *bl = &bloc[0];
  float *res = FLOAT_BUFF.data();

  std::cout<<"Calculating distances...";
  std::cout.flush();
  auto start = std::chrono::steady_clock::now();

  std::vector<std::vector<std::vector<int>>> vout = do_gpu(a,b,as,bs,al,bl,len1,len2,res,MAX_BATCH_SIZE,lws);

  auto end = std::chrono::steady_clock::now();
  double time_spent = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000000;
  double ips = ((double)(times*av.size()*av.size()))/time_spent;
  std::cout<<"Done"<<std::endl;
  std::cout<<"Number of full matches: "<<vout[1].size()<<std::endl;
  std::cout<<"Number of partial matches: "<<vout[0].size()<<std::endl;
  std::cout<< "Number of comparisons per second: " << ips <<std::endl;

  //write to file
  std::cout<<"Writing file...";
  std::cout.flush();
  write_gpu_out(FILEOUT,vout,MAX_BATCH_SIZE);
  std::cout<<"Done"<<std::endl;

  return 0;
}
