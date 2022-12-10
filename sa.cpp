// Construcción del suffix array y la BWT de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o sa sa.cpp -lsdsl -ldivsufsort -ldivsufsort64
#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <iostream>
#include <algorithm>

using namespace sdsl;
using namespace std;

int get_bound(int l, int r, char *text, char* pat, int_vector<> &sa, int mode){
  int n = strlen(text);
  int m = strlen(pat);
  long mid = (l+r)/2;
  //cerr<<"size "<<l<<" "<<mid<<" "<<r<<endl;
  if (l > r )
    return -1;
  else if(l == r or mid == 0 or mid == n-1)
    return mid;
  
  if(mode){
    if(strncmp(text+sa[mid], pat , m) >= 0)
      return get_bound(l,mid-1,text,pat,sa,mode);
    else
      return get_bound(mid+1,r,text,pat,sa,mode);
  }
  else {
    if(strncmp(text+sa[mid], pat , m) > 0)
      return get_bound(l,mid-1,text,pat,sa,mode);
    else
      return get_bound(mid+1,r,text,pat,sa,mode);
  }
  return -1;
}

vector<int> sa_locate(char *text, char* pat, int_vector<> &sa){
  vector<int> pos;
  int n = strlen(text);
  int m = strlen(pat);
  //n size of text
  //m size of pat 

  int l = 0;
  int r = n-m-1;
  int lb = get_bound(l,r,text,pat,sa,1)+1;
  int ub = get_bound(l,r,text,pat,sa,0);

  //cerr<<"lower_bound "<<lb<<endl;
  //cerr<<"upper_bound "<<ub<<endl;
  for(int i = lb;  i < ub; i++){
    pos.push_back(sa[i]);
    //cerr<<sa[i]<<" ";
  }//cerr<<endl;
  sort(pos.begin(),pos.end());
  return pos;
}

void doc_locate(vector<int> &fso, vector<int> pos, char* pat){
    long int textCount = 0;
    long int sizeCount = 0;
    int ocurCount = 0;
    for (int i = 0; i <= pos.size(); i++)
    {
        if(i != pos.size() and pos[i] < sizeCount + fso[textCount] )
            ocurCount++;
        else {
            cout<<"Ocurrencias de patron '"<<pat<<"' en documento "<<textCount<<" = "<<ocurCount<<endl;
            ocurCount = 1;
            sizeCount += fso[textCount]+1;
            textCount++;
        }
    }       
}

int main(int argc, char** argv) {
  if (argc !=  2) {
    cout << "Uso: " << argv[0] << " <archivo entrada>" << endl;
    return 1;
  }
  // Leemos el archivo de entrada y guardamos el contenido en 'seq'
  string infile(argv[1]);
  
  int_vector<> seq;
  int32_t n;
  {
    load_vector_from_file(seq, infile, 1);
    n = seq.size();
    
    seq.resize(n+1);
    n = seq.size();
    seq[n-1] = 0; // Representa el final de texto. Suele representarse por el
                  // símbolo $ 
  }
  cout << "Construyendo el Suffix array ..." << endl;
  
  int_vector<> sa(1, 0, bits::hi(n)+1);
  sa.resize(n);
  algorithm::calculate_sa((const unsigned char*)seq.data(), n, sa);

  // char * a = (char*)"nalnalnalnalnalnalnalnallnlanlnalnlanalhsdfg";
  // int_vector<> sa(1, 0, bits::hi(strlen(a))+1);
  // sa.resize(strlen(a));
  //n=sterlen(a);
  // algorithm::calculate_sa((const unsigned char*)a, strlen(a), sa);

  char* pat = (char*)"number";
  // cerr<<"a->"<<strlen(a)<<endl;
  // for (int i = 0; i < strlen(a); i++)
  // {
  //   cerr<<"["<<i<<"]"<<sa[i]<<":"<<a[sa[i]]<<" ";
  // }cerr<<endl;
  
  vector<int> pos =  sa_locate((char*)seq.data(), pat, sa);
  //vector<int> pos =  sa_locate(a, pat, sa);
  vector<int> tam;
  tam.push_back(n);
  doc_locate(tam,pos,pat);
    
  cout << "Tamaño del SA " << size_in_mega_bytes(sa) << " MB." << endl;
  
  return 0;
}