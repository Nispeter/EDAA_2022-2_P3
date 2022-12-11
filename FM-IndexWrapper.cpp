#include "FM-IndexWrapper.hpp"
#include <sdsl/suffix_arrays.hpp>
#include <vector>
#include <string>
#include <algorithm>

using namespace sdsl;
using namespace std;

FMIndexWrapper::FMIndexWrapper(string file_name){
	/*
	Constructor que llama a cosntruct de sdsl
	Impelemntado con un wavelet tree, almacenado en la variable csa_wt
	osea un compressed suffix array con wavelet tree
	*/
	construct(fm_index, file_name,1);
}

/*
Funcion que encuentra que documentos contienen una coincidencia con el patron.
*/
vector<int> FMIndexWrapper::doc_locate(const string &pattern, vector<int> posList){
	auto pos = sdsl::locate(fm_index, pattern.begin(), pattern.end());			
    sort(pos.begin(), pos.end());
    vector<int> docIndex;

	/*
	Busqeuda de documentos  similar a la se suffix array
	dado un vector de posiciones que indica la concatenacion de cada documento 
	un patron y el fm-index
	*/
	if(!pos.size())
    return docIndex;

	int ocurCount = 0;
	int lowRange = 0;
	int upRange = posList[0];
	int posCount = 0;
  
	/*
	Se itera por todos los hits encontrados con locate 
	y se compara con los rangos de los tamaños de los documentos.
	lowRange = lower bound del rango de tamaño de documento 
	upRange = Upper bound del rango de tamaño de documento 
	ocurrCount = contador de hits en cada documento 
	posCount = iterador de hits
	i = iterador de documento
	*/
	for(int i = 0; i < posList.size(); i++){
		while(pos[posCount] > lowRange and pos[posCount] <= upRange){
		posCount++;
		ocurCount++;
		}
		if(ocurCount != 0)docIndex.push_back(i+1);;
		ocurCount = 0;

		lowRange = upRange;
		if(i != posList.size()-1)upRange = posList[i+1];
		else upRange += posList[i];
	}
	/*
	En caso de encontrar hits en el rango se almacenan en un vector y se retorna
	*/
	return docIndex;
}

/*
OBSERVACION: la seccion de identificacion de documento de doc_locate de SA es similar a el de FM-index 
por lo que solo se documenta en este codigo.
*/