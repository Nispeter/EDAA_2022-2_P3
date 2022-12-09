#include <bits/stdc++.h>
#include "FM.h"

using namespace std;

int main(int argc, char const *argv[])
{
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