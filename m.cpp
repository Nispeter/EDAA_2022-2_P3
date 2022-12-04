#include <bits/stdc++.h>
#include "FM.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string a = "alabaalaalabarda";
    string b = "ala";
    FM(a);
    cout<<"match count: "<<endl;
    cout<<countMatch(b)<<endl;
    vector<int>m = locateMatch(b);
    cout<<"match pos: "<<endl;
    sort(m.begin(),m.end());
    for (int i = 0; i < m.size(); i++)
    {
        cout<<m[i]<<endl;
    }
    
    return 0;
}
