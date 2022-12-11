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

  if(mid == n-m-1)
    return mid;
  if (l >= mid or r <= mid )
    return mid;
  //Modo 1 obtiene el limite inferior 
  if(mode){
    if(strncmp(pat, text+sa[mid] , m) <= 0)
      return get_bound(l,mid,text,pat,sa,mode);
    else
      return get_bound(mid,r,text,pat,sa,mode);
  }
  //Modo 0 obtiene el limite superior 
  else {
    if(strncmp(pat,text+sa[mid] , m) < 0)
      return get_bound(l,mid,text,pat,sa,mode);
    else
      return get_bound(mid,r,text,pat,sa,mode);
  }
  return -1;
}

vector<int> sa_locate(char *text, char* pat, int_vector<> &sa){
  //sa_locate retorna un vector de enteros que contiene 
  //la posiciond e cada ocurrencia del patron en la concatenacion de textos 
  vector<int> pos;
  int n = strlen(text);
  int m = strlen(pat);
  //Rangos del suffix array dados por ub y lb
  int l = 0;
  int r = n-m-1;
  int lb = get_bound(l,r,text,pat,sa,1)+1;
  int ub = get_bound(l,r,text,pat,sa,0);

  //cerr<<"lb: "<<lb<<" ub:"<<ub<<endl;
  //cerr<<"salb:"; for (int i = 0; i < m; i++) cerr<<text[sa[lb]+i];
  //cerr<<",saub:";for (int i = 0; i < m; i++) cerr<<text[sa[ub]+i];cerr<<endl;
  
  //push de todos los hits encontrados por la busqueda binaria 
  for(int i = lb;  i <= ub; i++){
    pos.push_back(sa[i]);
  }
  sort(pos.begin(),pos.end());
  cerr<<"size:"<<pos.size()<<endl;
  return pos;
}

int_vector<> saCalculate(string filename, string patron) {
  // Leemos el archivo de entrada y guardamos el contenido en 'seq'
  string infile(filename);
  
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
  /*DOCUMENTADO EN FM-INDEXWRAPPER.CPP*/
  for(int i = 0; i < posList.size(); i++){
    while(pos[posCount] > lowRange and pos[posCount] <= upRange){
      posCount++;
      ocurCount++;
    }
      if(ocurCount != 0) {
        docIndex.push_back(i+1);
      }
      ocurCount = 0;

    lowRange = upRange;
    if(i != posList.size()-1)upRange = posList[i+1];
    else upRange += posList[i];
  }

  return docIndex;  
}
//Promedio de tiempo experimental
double promedio(const vector<double> &v) {
  double aux = 0;

  for (double i : v) {
    aux += i;
  }

  return (aux / (double) v.size()); 
}
//varianza de tiempo experimental 
double varianza(const vector<double> &v, double prom) {
  double var = 0;

  for (double i : v) {
    var += (i - prom) * (i - prom);
  }

  return (var / (double) v.size());
}


int main(int argc, char** argv) { 
  /*
  parseo de argimentos:
    Nombre del archivo de testeo
    Tamaño del patron
    Cantidad de documentos
    Tamaño de documentos 
    Documento de salida para logs
    Iteraciones de prueba para obtener promedio 
  */
  string original_filename(argv[1]);
  string patron_size(argv[2]);
  string nDoc(argv[3]);
  string lenDoc(argv[4]);
  string logfile(argv[5]);
  string reps(argv[6]);

  // ./a.out nombreArchivo tamaño_patron nDocs largoDocs log repeticiones

  ifstream t(original_filename);
  stringstream buffer;
  buffer << t.rdbuf();

  random_device rnd_device;
  mt19937 mersenne_engine {rnd_device()};
  uniform_int_distribution<long unsigned int> dist {0, (buffer.str()).size() - stoi(patron_size) - 1};
  auto gen = [&dist, &mersenne_engine](){return dist(mersenne_engine);};

  vector<int> vec(1);

  string w;

  auto start = chrono::high_resolution_clock::now();
  vector<double> tBusSA, tBusFM, tConsSA, tConsFM; 
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> diff = end - start;

  vector<int> posList = parser(original_filename, stoi(nDoc), stoi(lenDoc));

  string filename = original_filename + ".parsed";
  
  vector<int> docIndexFM;
  vector<int> docIndexSA; 

  for (int i = 0; i < stoi(reps); ++i) {
    generate(vec.begin(), vec.end(), gen);
    w = (buffer.str()).substr(vec[0], stoi(patron_size));
    //CONSTRUCTOR DE SUFFIX ARRAY
    start = chrono::high_resolution_clock::now();
    int_vector<> sa = saCalculate(filename, w);
    end = chrono::high_resolution_clock::now();
    diff = end - start;
    tConsSA.push_back(diff.count());
    //DOC LOCATE DE FM INDEX 
    start = chrono::high_resolution_clock::now();
    docIndexSA = doc_locate(filename, w, posList, sa);
    end = chrono::high_resolution_clock::now();
    int_vector<> aux;
    seq = aux;
    diff = end - start;
    tBusSA.push_back(diff.count());
    //CONSTRUCTOR DE FM INDEX
    start = chrono::high_resolution_clock::now();
    FMIndexWrapper FMIndex(filename);
    end = chrono::high_resolution_clock::now();
    diff = end - start;
    tConsFM.push_back(diff.count());
    //DOC LOCATE DE FM INDEX
    start = chrono::high_resolution_clock::now();
    docIndexFM = FMIndex.doc_locate(w, posList);
    end = chrono::high_resolution_clock::now();
    diff = end - start;
    tBusFM.push_back(diff.count());  
  }

  double prom_tBusSA = promedio(tBusSA);
  double prom_tBusFM = promedio(tBusFM);
  double prom_tConsSA = promedio(tConsSA);
  double prom_tConsFM = promedio(tConsFM);

  double var_tBusSA = varianza(tBusSA, prom_tBusSA);
  double var_tBusFM = varianza(tBusFM, prom_tBusFM);
  double var_tConsSA = varianza(tConsSA, prom_tConsSA);
  double var_tConsFM = varianza(tConsFM, prom_tConsFM);


  ofstream log(logfile, ios_base::app | ios_base::out);


  log << stoi(nDoc) * stoi(lenDoc) << "," << prom_tBusSA << "," << prom_tBusFM << "," << var_tBusSA << "," << var_tBusFM << "," << prom_tConsSA << "," << prom_tConsFM << "," << var_tConsSA << "," << var_tConsFM << endl;

  cout << w << endl;

  cout << "FM" << endl;

  for (int i : docIndexFM) {
    cout << i << " ";
  }

  cout << endl;

  cout << "SA" << endl;

  for (int i : docIndexSA) {
    cout << i << " ";
  }

  cout << endl;

  return 0;
}