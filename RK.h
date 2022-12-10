#include <bits/stdc++.h>
using namespace std;

class rabinKarp{
	private:
		int primeValue = 457;       //numero primo por el cual calcular el hash
		int charAlf = 256;          //tamaño del diccionario 
		int tSize;                  //tamaño del texto
		int pSize;                  //tamaño del patron 
		string text;                //texto
		string pattern;             //patron
        int base = 1;               //base de calculo de valor de hash
        int tHV = 0;                //valor de hash del texto por el cual se aplicara el rolling hash
        int pHV = 0;                //valor de hash del patron 

        /*
        Se realiza hash inicial de la primera ventana de texto y se realiza un hash del patron 
        Se calcula la potencia de base que sera la que distinga en la suma de caracteres de una ventana de texto, es decir
        para un hash basico de la suma en ASCII, dado un string del texto (la ventana de comparacion)
            AABA = 65+65+66+65 = 261  hash value
            BAAA = 65+65+65+66 = 261  same hash value
        No hace diferencia entre las posiciones de los caracteres en el string
        Por eso se calcula la base que se le aplicara a cada caracter, siendo esto base^(pSize)
        */
		void hashValues(){
            for(int i = 0; i < pSize; i++ ){
                tHV = (charAlf * tHV + text[i]) % primeValue;
                pHV = (charAlf * pHV + pattern[i]) % primeValue;
                if(i != 0){
                    base *= charAlf;
                    base %= primeValue;
                }
            }
        }

        /*
        Si no coinciden los valores hash del patron y la ventana de texto entonces  o no coincidela comparacion de los substrings se 
        hace un roll hash que inculye el siguiente caracter del texto y quita el primero de la ventana:

            patron = CAT
            texto & ventana = [AAC]ATT   --(rollHash)--> A[ACA]TT

        */
        void rollHash(int i){
           if(i < tSize - pSize){
                tHV = (charAlf * (tHV - text[i] * base) + text[i + pSize]);
                tHV %= primeValue;
                if(tHV < 0)
                    tHV += primeValue; 
            }
        }

	public: 
		rabinKarp(string &t, string &p){
			text = t;
			pattern = p;
			tSize = text.size();
            pSize = pattern.size();
            hashValues();
		}

        /*
        Metodo principal del rabinKarp que busca la cantidad de coincidencias de un patron en el texto (ambos dados en el constructor)
        Itera por sobre todo el texto comparando una ventana (substring) del texto de tamaño pSize y el patron, 
        si sus valores hash coinciden se requiere hacer una comparacion directa entre los caracteres de ambos substrings
        sino coincide se realiza un rollHash 
        */

        int count(){
            int cont = 0;
            int j = 0;
            for(int i = 0; i < tSize - pSize + 1; i++ ){
                if(tHV == pHV){
                    for(j = 0; j < pSize; j++){
                        if(text[i+j] != pattern[j])
                            break;  
                        if(j == pSize-1){
                            cont ++ ;
                            //cerr<<"pos: "<< i <<endl;
                        }
                    }
                    
                }
                rollHash(i);
            }
            return cont;
        }

};



/*
Formula hash rescatada de: https://www.programiz.com/dsa/rabin-karp-algorithm
*/