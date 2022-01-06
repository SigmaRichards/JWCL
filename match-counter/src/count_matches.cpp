/*TODO      Fix unique levels vs out index*/
/*TODO      Write input parsing*/
/*TODO      Write csv reader*/

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <limits.h>

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

int vec_prod(std::vector<int> v){
	int out = 1;
	for(int i = 0; i < v.size() ; i++){
		out*=v[i];
	}
	return out;
}

std::vector<int> match_count(std::vector<std::vector<int>> matches,std::vector<std::vector<int>> mlevel){
	int nfeatures = matches.size();

	std::vector<int> levels = get_levels(matches);
	std::vector<int> out(vec_prod(levels),0);
	std::cout<<vec_prod(levels)<<std::endl;
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
		std::cout<<tlevel<<std::endl;
		out[tlevel]++;
	}
	return out;
}

int main(){
	std::vector<std::string> allnames;//~~~~~~~~~~~~~~~~~~~###################

	std::vector<std::vector<std::vector<int>>> match_lists = {{{0,0,2},{0,1,1},{0,2,0},{1,0,2},{1,1,1},{1,2,0}},
																														{{0,0,1},{0,2,2},{0,3,0}},
																														{{0,0,2},{0,3,1},{0,8,0}}};//read_all_csvs(allnames);//~~~~~~~~~~~~~~~~~~~###################

	std::vector<std::vector<int>> match_lists_num;
	std::vector<std::vector<int>> match_lists_lev;
	for(int i = 0; i < match_lists.size(); i++) {
		match_lists_num.push_back(matchlist2int(match_lists[i],9));
		match_lists_lev.push_back(matchlist_level(match_lists[i]));
	}

	std::vector<int> counts = match_count(match_lists_num,match_lists_lev);//Currently breaks if number of unique levels differs
																																				 //from the maximum level size. E.g. if only levels 2
																																				 //and 3 appear then there are only 2 unique levels
																																				 //however the will score it 3*etc..

	//write_out(counts);//~~~~~~~~~~~~~~~~~~~###################

	return 0;
}
