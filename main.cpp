#include <bits/stdc++.h>
#include "FM.h"
#include "parse.h"

using namespace std;

int main(int argc, char const *argv[])
{

    // int total_lines = 10'000;
    // int random_lines = 10;
    // int pattern_len = 5;
    // std::string line;
    // std::ifstream file("test.txt");
    // int line_number = 0;
    // string text = "";
    // while (std::getline(file, line)) {
    //     text.append(" "+line);
    //     if(line_number > total_lines)
    //         break;
    //     ++line_number;
    // }
    // text += "$";
    // set <string>  stringSet = getStaticPattern(total_lines,random_lines);
    // int valSet = rand()%stringSet.size();
    // set<string>::iterator iter = stringSet.begin();
    // advance(iter,valSet);
    // string pattern = *iter;

    string a = "alabaalaalabarda";
    string c = "alalarabanalara";
    string d = "alabamala";
    string joined = a + "$" + c + "$" +d;           //Distintos documentos concatenados con un $
    string b = "ala";

    ////////////////////////STRING MATCH COUNT////////////////////////
    // FM(joined);
    // cout<<"match count: "<<endl;
    // cout<<countMatch(b)<<endl;
    // vector<int>m = locateMatch(b);
    // cout<<"match pos: "<<endl;
    // for (int i = 0; i < m.size(); i++)
    // {
    //     cout<<m[i]<<endl;
    // }

    ////////////////////////FILE MATCH COUNT////////////////////////
    FM(joined);
    vector<int> textSize;               //vector de tamaño de documentos 
    textSize.push_back(a.size());
    textSize.push_back(c.size());
    textSize.push_back(d.size());
    doc_locate(textSize,b);          //recive textSize y el patron, no se puede usar luego de un countMatch por que este lo llama internamente.

    return 0;
}