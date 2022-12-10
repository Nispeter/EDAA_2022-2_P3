#ifndef FMINDEX
#define FMINDEX

#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <vector>
#include <algorithm>

using namespace sdsl;
using namespace std;

class FMIndexWrapper{
private:
	csa_wt<wt_int<>> fm_index;
public:
	FMIndexWrapper(string file_name);
	vector<int> doc_locate(const string &pattern, vector<int> posList);
};

#endif