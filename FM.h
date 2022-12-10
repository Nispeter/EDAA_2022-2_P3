#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

int lastSp;
int lastEp;
int* suffix_arr;
char* bwt_arr;
char* input_text;
int len_text;
map<char,int> C;
map<char,vector<int>> Occ;

//////////////////////////////////INICIO DE SECCION OBTENIDA DE RECURSO EXTERNO/////////////////////////////////////////
//Estructura que almacena las rotaciones del texto para construir el suffix array 
struct rotation {
	int index;
	char* suffix;
};

//funcion que compara las rotaciones del string y las ordena acendentemente 
int cmpfunc(const void* x, const void* y)
{
	struct rotation* rx = (struct rotation*)x;
	struct rotation* ry = (struct rotation*)y;
	return strcmp(rx->suffix, ry->suffix);
}

/*
Funcion que recive el texto y lo rota  un espacio a la vez en cada iteracion.
Luego se ordena segun el valor ascii de sus caracteres y se guardan las posiciones respecto 
al texto original
texto: ACTA$
rotaciones:
[0] ACTA$
[1] CTA$A
[2] TA$AC
[3] A$ACT
[4] $ACTA
Orden:
[4] $ACTA
[3] A$ACT
[0] ACTA$
[1] CTA$A
[2] TA$AC
 
SA: {4,3,0,1,2}
Que coincide con el inicio del prefixo en el texto original (conciderando rotacion)
*/

int* computeSuffixArray(char* input_text, int len_text)
{

	struct rotation *suff;
	suff = (rotation*)malloc(len_text * sizeof(rotation));

	for (int i = 0; i < len_text; i++) {
		suff[i].index = i;
		suff[i].suffix = (input_text + i);
	}

	qsort(suff, len_text, sizeof(struct rotation),
		cmpfunc);


	int* suffix_arr
		= (int*)malloc(len_text * sizeof(int));

	for (int i = 0; i < len_text; i++){
		suffix_arr[i] = suff[i].index;

        }

	return suffix_arr;
}

/*
Itera por el arreglo de sufijos buscando la utlima columna del arreglo orden del ejemplo anterior 
por medio de computar input_text[(suffix_arr[i] + n - 1) % n]
*/
char* findLastChar(char* input_text,
				int* suffix_arr, int n)
{

	char* bwt_arr = (char*)malloc(n * sizeof(char));
	int i;
	for (i = 0; i < n; i++) {

		int j = suffix_arr[i] - 1;
		if (j < 0)
			j = j + n;

		bwt_arr[i] = input_text[j];
	}

	bwt_arr[i] = '\0';

	// Returns the computed Burrows - Wheeler Transform
	return bwt_arr;
}

//////////////////////////////////FIN DE SECCION OBTENIDA DE RECURSO EXTERNO/////////////////////////////////////////

/*
Se obtiene el arreglo C para cada caracter c que representa las ocurrencias totales de ese caracter en el texto.
Luego se busca el arreglo Occ que representa el proceso de ocurrencias de cada caracter en la BWT.
este se calcula de la siguiente forma:  se itera por cada valor de la BWT,
para cada caracter nuevo (no ingresado anteriormente) se rellenan las ocurrencias hasta la posicion acual - 1 en 0 y la actual en 1 
Si es que ya se ha calculado las apariciones del caracter previamente entonces para cada ingreso de un nuevo caracter
se repite el mismo valor (si no es el valor incrementado) y si es el valor incrementado entonces se repite el valor anterior 
y se le suma 1.
*/
void buildArrays(){
	
	char prev;

	for (int i = 0; i < len_text; ++i)
	{
		if(input_text[suffix_arr[i]]  != prev){
            C[input_text[suffix_arr[i]]] = i;
            prev = input_text[suffix_arr[i]];
        }

        if(Occ[bwt_arr[i]].size()==0){
            for (int j = 0; j <= i; j++)
            {
                Occ[bwt_arr[i]].push_back(0);
            }
            for (map<char,vector<int>>::iterator itr = Occ.begin(); itr != Occ.end(); itr++)
            {
                char a = itr->first;
                if(i!=0 and a != bwt_arr[i]){
                    Occ[itr->first].push_back(Occ[itr->first][i-1]);
                }
            }
            Occ[bwt_arr[i]][i] = 1; 
        }
        else{
            for (map<char,vector<int>>::iterator itr = Occ.begin(); itr != Occ.end(); itr++)
            {
                if(i!=0){
                    Occ[itr->first].push_back(Occ[itr->first][i-1]);
                }
            }
            Occ[bwt_arr[i]][i] = Occ[bwt_arr[i]][i-1] + 1;
        }
			
	}

}

/*
Aritmetica entre el SA y la BWT representado en los arreglos C y Occ
calcula el rango del arreglo de sufijos donde se encuentra el patron buscado, se representa de la siguiente forma:
Para cada caracter  c del  patron 
	rango superior = C[c] + BWT,rank(c, rango superior)
	rango inferior = C[c] + BWT.rank(c,rango inferior)
la diferencia entre estos bounds es el rango donde se encuentra el patron al final de las iteraciones 
*/

int countMatch(string &pat) {
    int i = pat.size()-1;			//tamaño del patron
    char c = pat[i];				//caracter actual
    int sp = C[c]-1;				//upper bound
    int ep = len_text-1;			//lower bound
    //primera iteracion se verifica la existencia de C[c]
    map<char,int>::iterator itr = C.find(c);
    itr++;
    ep = itr->second-1;
    i--;
    if(Occ[c][ep] > len_text or Occ[c][ep] < 0 or ep < 0 or ep > len_text)
        ep = len_text-1;

    while ((sp < ep) and (i >= 0) ) 
    {
        if(C[c]+Occ[c][ep] > len_text or C[c]+Occ[c][ep] < 0)
            break;
        c = pat[i];
        sp = C[c]+Occ[c][sp]-1;
        ep = C[c]+Occ[c][ep]-1;
        i--;
    }
    lastSp = sp;
    lastEp = ep;
    return abs(ep - sp);
}

//Encuentra la posicion del patron en el texto
vector<int> locateMatch(string &pat){
    countMatch(pat);
    int sp = lastSp;
    int ep = lastEp;
    vector<int>foundAt;
    for (int i = sp+1; i <= ep; i++)
    {
        foundAt.push_back(suffix_arr[i]);
    }
    sort(foundAt.begin(),foundAt.end());
    return foundAt;     
}

//fso representa el vector de tamaños de cada file y estan en orden de concatenacion
void doc_locate(vector<int> &fso, string &pat){
    vector<int> lm = locateMatch(pat);
    long int textCount = 0;
    long int sizeCount = 0;
    int ocurCount = 0;
    for (int i = 0; i <= lm.size(); i++)
    {
        if(i != lm.size() and lm[i] < sizeCount + fso[textCount] )
            ocurCount++;
        else {
            cout<<"Ocurrencias de patron '"<<pat<<"' en documento "<<textCount<<" = "<<ocurCount<<endl;
            ocurCount = 1;
            sizeCount += fso[textCount]+1;
            textCount++;
        }
    }       
}

void FM(string &text)
{
	if(text[text.size()-1]!='$')//adaptacion al parseo original de la experimentacion 
		text.append("$");
	int size = text.size();
	input_text = (char*)malloc((size+1) * sizeof(char));
	strcpy(input_text, text.c_str());
	len_text = strlen(input_text);
	suffix_arr = computeSuffixArray(input_text, len_text);//construccion del SA
	bwt_arr = findLastChar(input_text, suffix_arr, len_text);//construccion del BWT
	buildArrays();//Construccion de C y Occ
}
