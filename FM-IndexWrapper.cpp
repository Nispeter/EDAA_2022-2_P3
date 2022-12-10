#include "FM-IndexWrapper.hpp"
#include <sdsl/suffix_arrays.hpp>
#include <vector>
#include <string>
#include <algorithm>

using namespace sdsl;
using namespace std;

FMIndexWrapper::FMIndexWrapper(string file_name){
	construct(fm_index, file_name,1);
}


vector<int> FMIndexWrapper::doc_locate(const string &pattern, vector<int> posList){
	auto posiciones = sdsl::locate(fm_index, pattern.begin(), pattern.end());
    sort(posiciones.begin(), posiciones.end());
    int posNum = 0;
    vector<int> docIndex;
    for (int i = 0; i < posList.size(); ++i)
    {
    	if(posiciones[posNum]<posList[i]){
    		docIndex.push_back(i+1);
    		posNum++;
    		if(posNum==posiciones.size())
    			break;
    	}
    }
    return docIndex;
}
