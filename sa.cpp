#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include "parser.hpp"
#include "FM-IndexWrapper.hpp"

using namespace sdsl;
using namespace std;

// Esto es un hack terrible, pero necesitamos separar la construccion de la busqueda. 
int_vector<> seq;


int get_bound(int l, int r, char *text, char* pat, int_vector<> &sa, int mode){
  /*
  Funcion recursiva que realiza busqueda binaria sobre el arreglo de 
  sufijos, retorna el lowerBound y Upperbound del suffix array ordenado 
  donde se encuentran los patrones 
  */
  int n = strlen(text);
  int m = strlen(pat);
  long mid = (l+r)/2;

  if (l > r )
    return -1;
  else if(l == r or mid == 0 or mid == n-1)
    return mid;
  //Modo 1 obtiene el limite inferior 
  if(mode){
    if(strncmp(text+sa[mid], pat , m) >= 0)
      return get_bound(l,mid-1,text,pat,sa,mode);
    else
      return get_bound(mid+1,r,text,pat,sa,mode);
  }
  //Modo 2 obtiene el limite superior 
  else {
    if(strncmp(text+sa[mid], pat , m) > 0)
      return get_bound(l,mid-1,text,pat,sa,mode);
    else
      return get_bound(mid+1,r,text,pat,sa,mode);
  }
  return -1;
}

vector<int> sa_locate(char *text, char* pat, int_vector<> &sa){
  //sa_locate retorna un vector de enteros que contiene 
  //la posiciond e cada ocurrencia del patron en la concatenacion de textos 
  vector<int> pos;
  int n = strlen(text);
  int m = strlen(pat);

  int l = 0;
  int r = n-m-1;
  int lb = get_bound(l,r,text,pat,sa,1);
  int ub = get_bound(l,r,text,pat,sa,0);

  for(int i = lb;  i < ub; i++){
    pos.push_back(sa[i]);
  }
  sort(pos.begin(),pos.end());
  return pos;
}

int_vector<> saCalculate(string filename, string patron) {
  // Leemos el archivo de entrada y guardamos el contenido en 'seq'
  string infile(filename);
  
  //int_vector<> seq;
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
  //funcion base de algorithm que calcula el arreglo de sufijos y lo almacena en sa
  algorithm::calculate_sa((const unsigned char*)seq.data(), n, sa);

  return sa;
}

vector<int> doc_locate(string filename, string patron, vector<int> posList, int_vector<> sa){
  /*doc_locate obtiene el archivo del cual aparece una ocurrenica del patron*/
  //sa_locate 
  char* pat = (char*) patron.c_str();
  vector<int> pos = sa_locate((char*)seq.data(), pat, sa);
  vector<int> docIndex;
  if(!pos.size())
    return docIndex;

  int ocurCount = 0;
  int lowRange = 0;
  int upRange = posList[0];
  int posCount = 0;
  
  for(int i = 0; i < posList.size(); i++){
    while(pos[posCount] > lowRange and pos[posCount] <= upRange){
      posCount++;
      ocurCount++;
    }
      if(ocurCount != 0) {
        //cout<<":Ocurrencias de patron '"<<pat<<"' en documento "<<i+1<<" = "<<ocurCount<<endl;
        docIndex.push_back(i+1);
      }
      ocurCount = 0;

    lowRange = upRange;
    if(i != posList.size()-1)upRange = posList[i+1];
    else upRange += posList[i];
  }

  return docIndex;  
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
  string original_filename(argv[1]);
  string patron(argv[2]);
  string nDoc(argv[3]);
  string lenDoc(argv[4]);

  vector<int> posList = parser(original_filename, stoi(nDoc), stoi(lenDoc));

  string filename = original_filename + ".parsed";
  
  FMIndexWrapper FMIndex(filename);
  int_vector<> sa = saCalculate(filename, patron);

  cout << "FM" << endl;
  vector<int> docIndexFM = FMIndex.doc_locate(patron, posList);
  for (int i : docIndexFM) {
    cout << i << " ";
  }
  cout << endl;

  cout << "SA" << endl;
  vector<int> docIndexSA = doc_locate(filename, patron, posList, sa);
  for (int i : docIndexSA) {
    cout << i << " ";
  }

  cout << endl;

  // Recordar limpiar seq, si se hacen 30 reps.
  int_vector<> aux;
  seq = aux;

  return 0;
}