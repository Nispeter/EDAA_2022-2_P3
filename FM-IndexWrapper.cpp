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
	auto pos = sdsl::locate(fm_index, pattern.begin(), pattern.end());			//hits
    sort(pos.begin(), pos.end());
    vector<int> docIndex;




	if(!pos.size())
    return docIndex;

  // for (int i = 0; i < posList.size(); i++)
  // {
  //   cerr<<posList[i]<<endl;
  // }
  

  int ocurCount = 0;
  int lowRange = 0;
  int upRange = posList[0];
  int posCount = 0;

  // for (int i = 0; i < pos.size(); i++)
  // {
  //   cerr<<"pos:"<<pos[i]<<endl;
  // }
  
  
  for(int i = 0; i < posList.size(); i++){
    while(pos[posCount] > lowRange and pos[posCount] <= upRange){
      //cerr<<pos[posCount] <<"<"<<upRange<<endl;
      posCount++;
      ocurCount++;
    }
      if(ocurCount != 0)docIndex.push_back(i+1);;
      ocurCount = 0;

    lowRange = upRange;
    if(i != posList.size()-1)upRange = posList[i+1];
    else upRange += posList[i];
  }
    return docIndex;
}