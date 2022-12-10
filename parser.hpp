#include <bits/stdc++.h>

using namespace std;

vector<int> parser(string file_name, int nDocuments, int lenDoc){
	vector<int> posList;
	ifstream file(file_name);
	char *input = new char [nDocuments*lenDoc];
	file.read(input,nDocuments*lenDoc);
	file.close();
	string Sinput(input);
	delete[] input;
	int pos = lenDoc;
	for(int i = 0; i<nDocuments-1; i++){
		posList.push_back(pos);
		Sinput.insert(pos,"$");
		pos += lenDoc + 1;
	}
	ofstream out(file_name + ".parsed");
	out << Sinput;
	out.close();
	return posList;
}