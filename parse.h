#include <bits/stdc++.h>

using namespace std;

int pattern_len = 5;
set<std::string> tokens;

void staticPattern(string line){
    //vector <string> tokens;
    stringstream check1(line);
    string intermediate;
    while(getline(check1, intermediate, ' ')){
        if(intermediate.size() == pattern_len)
            tokens.insert(intermediate);
    }
}

void cleanLine(string &line){
    for (int i = 0; i < line.size(); ++i)
    {
        if(line[i]!= ' ' and ((line[i] < 65 or line[i] > 90) and ( line[i] < 97 or line[i] > 122))){
            line[i] = ' ';
        }
    }
}

void printTokens(set<string> &t){
    cerr<<"tokens: "<<endl;
    for (set<string>::iterator itr = t.begin() ; itr != t.end() ; itr++)
    {
        cerr<<*itr<<" ;";
    }
    cerr<<endl;
}

set <string> getStaticPattern(int number_of_lines,int  number_of_lines_to_select)
{

  vector<int> line_indices(number_of_lines);
  iota(begin(line_indices), end(line_indices), 0); // init line_indices

  random_device r;
  seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
  mt19937 eng(seed);

  shuffle(begin(line_indices), end(line_indices), eng);

  assert(number_of_lines_to_select <= number_of_lines);

  string line;
  std::ifstream file("test.txt");

  int line_number = 0;
  while (getline(file, line)) {
    for (int i = 0; i < number_of_lines_to_select; ++i) {
      if (line_number == line_indices[i]) {
        
        cleanLine(line);
        staticPattern(line);
      }
    }
    if(line_number > number_of_lines)
        break;
    ++line_number;
   }
    return tokens;
}

