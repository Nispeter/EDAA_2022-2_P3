// Construcción del suffix array y la BWT de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o sa sa.cpp -lsdsl -ldivsutamrt -ldivsutamrt64
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

void doc_locate(string filename, string patron){
  // Leemos el archivo de entrada y guardamos el contenido en 'seq'
  string infile(filename);
  
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
  
  int_vector<> sa(1, 0, bits::hi(n)+1);
  sa.resize(n);
  algorithm::calculate_sa((const unsigned char*)seq.data(), n, sa);

  char* pat = (char*) patron.c_str();
  
  vector<int> pos = sa_locate((char*)seq.data(), pat, sa);
  vector<int> tam;
  tam.push_back(n);

  long int textCount = 0;
  long int sizeCount = 0;
  int ocurCount = 0;
  for (int i = 0; i <= pos.size(); i++) {
    	if(i != pos.size() and pos[i] < sizeCount + tam[textCount] )
            ocurCount++;
        else {
            cout<<"Ocurrencias de patron '"<<pat<<"' en documento "<<textCount<<" = "<<ocurCount<<endl;
            ocurCount = 1;
            sizeCount += tam[textCount]+1;
            textCount++;
        }
    }       
}

double promedio(const vector<double> &v) {
  double aux = 0;

  for (double i : v) {
    aux += i;
  }

  return (aux / (double) v.size()); 
}

double varianza(const vector<double> &v, double prom) {
  double var = 0;

  for (double i : v) {
    var += (i - prom) * (i - prom);
  }

  return (var / (double) v.size());
}


int main(int argc, char** argv) { 
  string filename = "test";
  string patron = "test";

  doc_locate(filename, patron);
  
  return 0;
}