#include <bits/stdc++.h>

using namespace std;

void* operator new(size_t c){
	return malloc(c);
}

class SuffixArray{											//void* almacena cualquier typecast y puede ser casteado a cualquiera
	private:												//las minusculas son mayores que las mayusculas
	string _t;
	string_view _sv;
	vector<unsigned> _arr;

	bool compareSuffix(unsigned l, unsigned r){				//string solo de vista
															//true si esque el a va antes que r
		return _sv.substr(l) < _sv.substr(r);				//se realiza una copia en vez de mirarlo 
			
	}

	public:
	SuffixArray(const string &s) : _t(s) , _arr(s.size()+1) {
		
		_t.push_back('$');
		_sv  = string_view(_t);
		iota(_arr.begin(),_arr.end(),0);

		sort(_arr.begin(), _arr.end(), 
//alternativa 1
		/*
				WRAPPER#########################################################
				normalmente se le pasa todos los parametros a los funciones
				el bind efectua de weapper
				auto wrapper = bind()
				que recive menos parametros 
				sirve para mantener estaticos algunos parametros de funciones
				ademas las funciones normalmente usan 1 parametro mas que 
				es el puntero a si mismo.
					
				Las funciones dentro de otras no se pueden llamar a si mismas si 
				no poseen la referencias propias.
			*/

			[this](unsigned l, unsigned r) -> bool{
				return this->_sv.substr(l)<this->_sv.substr(r);
			} 
			/*
					FUNICON LAMBDA#################################################
					funcion que se declara in-line  
					su implementacion tiene objetivo similar al wapper
				
					[this](unsigned l, unsigned r) -> bool {
						this-> _sv.substr(l) < this->_sv.substr(r);
					}
				*/
//alternativa 2
/*
			bind(&SuffixArray::compareSuffix,
				this,placeholders::_1, placeholders::_2)	*/
				); 


	}

	unsigned count(const string &s){

		auto lower = lower_bound(_arr.begin(),_arr.end(),s,			//usar iterador en vez de auto
			[this](unsigned l, const string &s) -> bool{			//lower bound busqueda binaria encuentra el match
				// true si es que el string s no esta antes
				return _sv.substr(l) < s;
			} );

		auto upper = upper_bound(_arr.begin(), _arr.end(), s,

			[this](const string &s,unsigned l ) -> bool{
				// true si es que el string s no esta antes			//se llama a size para definir el inicio
				return _sv.substr(l,s.size()) > s;
				} );

		return distance(lower,upper);
	}

	void printArray(){
		for(unsigned i : _arr){
			cout << i << " " <<_sv.substr(i) <<endl;
		}
	}

};


