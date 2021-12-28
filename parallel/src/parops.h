#pragma once

#include <thread>
#include <future>
#include <vector>
#include <string_view>



std::vector<std::vector<std::string_view>> split_v(std::vector<std::string_view> v, int s){
  std::vector<std::vector<std::string_view>> out;

  int len = v.size();

  int bsize = len/s;
  int nextra = len - s*bsize;

  int i = 0;
  int e = 0;
  for(int a = 0 ; a < s ; a++){
    std::vector<std::string_view> vin;
    e = (int)(nextra > 0);
    nextra--;
    for(int b = 0 ; b < bsize + e; b++){
      vin.push_back(v[i]);
      i++;
    }
    out.push_back(vin);
  }
  return out;
}

std::vector<std::string> do_par(std::vector<std::string_view> v,int s){
  std::vector<std::string> out;

  std::vector<std::vector<std::string_view>>  ns1 = split_v(v,s);
  //auto ns2 = split_v(v,s);

  std::vector<std::future<std::vector<std::string>>> futures(s);
  std::vector<std::vector<std::string>> joined;
  std::vector<std::thread> vecOfThreads;
  for(int a = 0; a < s;a++){
    std::packaged_task<std::vector<std::string>(std::vector<std::string_view>,std::vector<std::string_view>)> task{jw_outer};
    futures[a] = task.get_future();
    std::thread th(std::move(task),v,ns1[a]);
    vecOfThreads.push_back(std::move(th));
  }

  for(int a = 0; a < s; a++){
    vecOfThreads[a].join();
    joined.push_back(futures[a].get());
  }

  std::string t;
  for(int a = 0; a<v.size() ; a++){
    t = "";
    for(int b = 0; b<s ; b++){
      t+= joined[b][a];
    }
    out.push_back(t);
  }
  return out;
}
