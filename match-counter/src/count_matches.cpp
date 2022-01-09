/*TODO      Write input parsing*/
/*TODO      Write csv reader*/

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits.h>


std::vector<std::string> load_name_list(std::string FILENAME){
  std::vector<std::string> namesstr;
  std::ifstream file(FILENAME);
  if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        namesstr.push_back(line);
      }
      file.close();
  }
  return namesstr;
}

std::vector<std::vector<int>> load_csv(std::string FILENAME){
	std::vector<std::string> filestr = load_name_list(FILENAME);
	std::vector<std::vector<int>>  out;
	std::vector<int> in;


	std::string split = "";
	for(int i = 0; i < filestr.size() ; i++){
		in.clear();
		std::stringstream lineStream(filestr[i]);
		while(std::getline(lineStream,split,' ')){
			in.push_back(std::stoi(split));
		}
		out.push_back(in);
	}
	return out;
}

std::vector<std::vector<std::vector<int>>> read_all_csvs(std::vector<std::string> FILENAMES){
	std::vector<std::vector<std::vector<int>>> out;
	std::vector<std::vector<int>> in;
	for(int f = 0; f<FILENAMES.size(); f++){
		in = load_csv(FILENAMES[f]);
		out.push_back(in);
	}
	return out;
}


std::vector<int> matchlist2int(std::vector<std::vector<int>> matchlist,int nr){
	std::vector<int> out(matchlist.size(),0);
	int score = 0;
	for(int a = 0; a < matchlist.size() ; a++){
		score = nr*matchlist[a][0] + matchlist[a][1];
		out[a] = score;
	}
	return out;
}

std::vector<int> matchlist_level(std::vector<std::vector<int>> matchlist){
	std::vector<int> out(matchlist.size(),0);
	for(int a = 0; a < matchlist.size() ; a++){
		out[a] = matchlist[a][2];
	}
	return out;
}


std::vector<int> get_levels(std::vector<std::vector<int>> matches){
	std::vector<int> out(matches.size(),0);
	for(int i = 0; i < matches.size() ; i++){
		std::sort(matches[i].begin(),matches[i].end());
		matches[i].erase(std::unique(matches[i].begin(),matches[i].end()),matches[i].end());
		out[i] = matches[i].size();
	}
	return out;
}

std::vector<std::vector<int>> convert_levels(std::vector<std::vector<int>> matches){
    std::vector<std::vector<int>> matches2 = matches;
	std::vector<std::vector<int>> out;
	std::vector<int> in;
	std::vector<std::vector<int>> u_levels;
	for(int i = 0; i < matches.size() ; i++){
		std::sort(matches[i].begin(),matches[i].end());
		matches[i].erase(std::unique(matches[i].begin(),matches[i].end()),matches[i].end());
		u_levels.push_back(matches[i]);
	}
	for(int i = 0; i < matches2.size() ; i++){
	    in.clear();
		for(int j = 0; j < matches2[i].size() ; j++){
		    for(int k = 0; k < u_levels[i].size(); k++){
		        if(u_levels[i][k]==matches2[i][j]){
		            in.push_back(k);
		        }
		    }
	    }
	    out.push_back(in);
	}
	return out;
}


bool is_active(std::vector<int> counter,std::vector<std::vector<int>> matches){
	for(int a = 0; a < counter.size() ; a++){
		if(counter[a] < (matches[a].size()-1)){
			return true;
		}
	}
	return false;
}

int get_min(std::vector<int> count,std::vector<std::vector<int>> vals,std::vector<bool> is_end){
	int minv = INT_MAX;
	for(int i = 0; i < vals.size() ; i++){
		if(!is_end[i]){
			minv = std::min(minv,vals[i][count[i]]);
		}
	}
	return minv;
}

int get_match_level(std::vector<int> match, std::vector<int> levels){
	int cprod = 1;
	int clevel = 0;
	for(int a = 0 ; a < match.size() ; a++){
		clevel += match[a]*cprod;
		cprod *= levels[a];
	}
	return clevel;
}

std::vector<int> reverse_gml(int match, std::vector<int> levels){
    std::vector<int> out(levels.size(),0);
    std::vector<int> prods = {1};
    for(int i = 1; i<levels.size() ; i++) prods.push_back(prods[i-1]*levels[i-1]);
    std::reverse(prods.begin(), prods.end());
    for(int i = 0; i<levels.size() ; i++){
        if(match >= prods[i]){
            out[levels.size()-i-1] = match/prods[i];
            match = match - prods[i]*out[levels.size()-i-1];
        }
    }
    //std::reverse(out.begin(), out.end());
    return out;

}

int vec_prod(std::vector<int> v){
	int out = 1;
	for(int i = 0; i < v.size() ; i++){
		out*=v[i];
	}
	return out;
}

std::vector<int> match_count(std::vector<std::vector<int>> matches,std::vector<std::vector<int>> mlevel){
	int nfeatures = matches.size();

	std::vector<int> levels = get_levels(mlevel);
	std::cout<<std::endl;
	std::vector<int> out(vec_prod(levels),0);
	std::vector<int> cmatch(nfeatures,0);
	std::vector<int> counter(nfeatures,0);
	std::vector<bool> is_end(nfeatures,false);
	int minv = 0;
	int tlevel = 0;
	while(is_active(counter,matches)){
		minv = get_min(counter,matches,is_end);
		for(int i = 0; i < nfeatures ; i++) {
			if(matches[i][counter[i]]==minv) {
				cmatch[i]=mlevel[i][counter[i]];
				if(counter[i] < (matches[i].size()-1)){
					counter[i]++;
				}else{
					is_end[i] = true;
				}
			}
		}

		tlevel = get_match_level(cmatch,levels);
		//for(int t=0; t<cmatch.size(); t++) std::cout<<" "<<cmatch[t];
		//std::cout<<": "<<tlevel<<std::endl;


		out[tlevel]++;
		for(int i = 0; i < cmatch.size() ; i++) cmatch[i] = 0;
	}
	return out;
}

int main(){
	std::vector<std::string> allnames = {
		"../data/rand_match_1_4.csv",
		"../data/rand_match_2_4.csv",
		"../data/rand_match_3_4.csv",
		"../data/rand_match_4_4.csv"
	};//~~~~~~~~~~~~~~~~~~~###################

	/*std::vector<std::vector<std::vector<int>>> match_lists = {{{0,0,2},{0,1,1},{0,2,0},{1,0,2},{1,1,1},{1,2,0}},
															  {{0,0,3},{0,2,2},{0,3,0}},
															  {{0,0,2},{0,3,3},{0,8,0}}};//read_all_csvs(allnames);//~~~~~~~~~~~~~~~~~~~###################*/

	std::cout<<"Reading files...";
	std::cout.flush();
	std::vector<std::vector<std::vector<int>>> match_lists = read_all_csvs(allnames);
	std::cout<<"Done"<<std::endl;

	std::cout<<"Converting vectors...";
	std::cout.flush();
	std::vector<std::vector<int>> match_lists_num;
	std::vector<std::vector<int>> match_lists_lev;
	for(int i = 0; i < match_lists.size(); i++) {
		match_lists_num.push_back(matchlist2int(match_lists[i],10000));
		match_lists_lev.push_back(matchlist_level(match_lists[i]));
	}
	match_lists_lev = convert_levels(match_lists_lev);
	std::cout<<"Done"<<std::endl;


	std::cout<<"Counting matches...";
	std::cout.flush();
	std::vector<int> counts = match_count(match_lists_num,match_lists_lev);
	std::cout<<"Done"<<std::endl;

	for(int i = 0; i < counts.size() ; i++){
	    if(counts[i]>0){
	        std::vector<int> cmat = reverse_gml(i, get_levels(match_lists_lev));
	        for(int j = 0; j < cmat.size() ; j++){
	            std::cout<< " " << cmat[j];
	        }
	        std::cout<< ": " <<counts[i]<< std::endl;
	    }
	}

	//write_out(counts);//~~~~~~~~~~~~~~~~~~~###################

	return 0;
}
