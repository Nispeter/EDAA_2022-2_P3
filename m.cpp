#include "SA.h"
#include "RK.h"
#include "FM.h"

#include "parse.h"
#include <cstdlib>
#include <time.h> 

using namespace std;

int main(int argc, char const *argv[])
{

	string op_mode = "t";

	if(argc > 1){
		string argv1 = argv[1];
		cerr<<"arg"<<argv1<<endl;
		if(argv1 == "-p")
			op_mode  = "p";
	}

	int reps = 50;
	
if(op_mode == "t"){
	int max = 100'000;
	int base = 100;
	int incr = 10;

	srand (time(NULL));
				
				for (auto size = base; size <= max; size *= incr) {

					cerr<<size<<endl;
					int total_lines = size;
    				int random_lines = 10;
    				pattern_len = 5;

					std::string line;
					std::ifstream file("test.txt");
					int line_number = 0;
					string text = "";
					while (std::getline(file, line)) {
						text.append(" "+line);
						if(line_number > total_lines)
							break;
						++line_number;
					}

					cerr<<"Suffix array"<<endl;
					double cont = 0;
					ofstream myfile; 
		        	myfile.open ("outT.txt",ios::app);
		        	for (int i = 0; i < reps; ++i)
		        	{
		        		
    					set <string>  stringSet = getStaticPattern(total_lines,random_lines);
    					int valSet = rand()%stringSet.size();
    					set<string>::iterator iter = stringSet.begin();
    					advance(iter,valSet);
    					string pattern = *iter;

					   	SuffixArray SA(text);

		        		auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		int c = SA.count(pattern);
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
						cerr<<pattern<<" app "<<c<<endl;
			        	chrono::duration<double> diff = end - start;
		        		cont += diff.count();
		        	}
					cont /= reps;
 					myfile <<cont <<"," ; 
  					myfile.close();

					cerr<<"Rabin karp"<<endl;
  					cont = 0;
		        	myfile.open ("outT.txt",ios::app);
		        	for (int i = 0; i < reps; ++i)
		        	{
    					set <string>  stringSet = getStaticPattern(total_lines,random_lines);
    					int valSet = rand()%stringSet.size();
    					set<string>::iterator iter = stringSet.begin();
    					advance(iter,valSet);
    					string pattern = *iter;

						rabinKarp RK(text,pattern);

		        		auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		int c = RK.count();
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
						cerr<<pattern<<" app "<<c<<endl;
			        	chrono::duration<double> diff = end - start;
		        		cont += diff.count();
		        	}
					cont /= reps;
 					myfile <<cont <<","; 
  					myfile.close();

					cerr<<"FM index"<<endl;
  					cont = 0;
		        	myfile.open ("outT.txt",ios::app);
					FM(text);

		        	for (int i = 0; i < reps; ++i)
		        	{
						set <string>  stringSet = getStaticPattern(total_lines,random_lines);
						int valSet = rand()%stringSet.size();
						set<string>::iterator iter = stringSet.begin();
						advance(iter,valSet);
						string pattern = *iter;

						auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		count(pattern);
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
			        	chrono::duration<double> diff = end - start;
		        		cont += diff.count();
		        	}
					cont /= reps;
 					myfile <<cont <<","; 
  					myfile.close();

		        }

			
		    ofstream myfile;
		    myfile.open ("outT.txt",ios::app);
 			myfile <<endl<<endl; 
  			myfile.close();

			}

else if(op_mode == "p"){
	srand (time(NULL));
				int max = 8;
				int base = 2;
				int incr = 1;

				int total_lines = 10'000;
				int random_lines = 10;
    					
				std::string line;
				std::ifstream file("test.txt");
				int line_number = 0;
				string text = "";

				while (std::getline(file, line)) {
					text.append(" "+line);
					if(line_number > total_lines)
						break;
					++line_number;
				}

				for (auto size = base; size <= max; size += incr) {
					cerr<<size<<endl;

					double cont = 0;
					ofstream myfile;
					pattern_len = size;

		        	cerr<<"Suffix array PatSize"<<endl;	

		        	for (int i = 0; i < reps; ++i)
		        	{
    					set <string>  stringSet = getStaticPattern(total_lines,random_lines);
    					int valSet = rand()%stringSet.size();
    					set<string>::iterator iter = stringSet.begin();
    					advance(iter,valSet);
    					string pattern = *iter;

						SuffixArray SA(text);

					   	auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		int c = SA.count(pattern);
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
					   	chrono::duration<double> diff = end - start;
			        	diff = end - start;
		        		cont += diff.count();

		        	}
		        	myfile.open ("outP.txt",ios::app);
					cont /= reps;
					myfile <<cont <<"," ; 
					myfile.close();

					cerr<<"RK PatSize"<<endl;	
  					cont = 0;
		        	myfile.open ("outP.txt",ios::app);
		        	for (auto size = base; size <= max; size += incr)
		        	{
    					pattern_len = size;
    					set <string>  stringSet = getStaticPattern(total_lines,random_lines);
    					int valSet = rand()%stringSet.size();
    					set<string>::iterator iter = stringSet.begin();
    					advance(iter,valSet);
    					string pattern = *iter;

						rabinKarp RK(text,pattern);

		        		auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		RK.count();
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
			        	chrono::duration<double> diff = end - start;
		        		cont += diff.count();

		        	}
					cont /= reps;
 					myfile <<cont <<","; 
  					myfile.close();

					cont = 0;
		        	myfile.open ("outP.txt",ios::app);
					cerr<<"FM index PatSize"<<endl;	

		        	for (auto size = base; size <= max; size += incr)
		        	{

						pattern_len = size;
						set <string>  stringSet = getStaticPattern(total_lines,random_lines);
						int valSet = rand()%stringSet.size();
						set<string>::iterator iter = stringSet.begin();
						advance(iter,valSet);
						string pattern = *iter;

						FM(text);

		        		auto start = chrono::high_resolution_clock::now();
			        	//####################INICIO DEL CLOCK####################
		        		count(pattern);
			        	//#####################FIN DEL CLOCK#####################
			        	auto end = chrono::high_resolution_clock::now();
			        	chrono::duration<double> diff = end - start;
		        		cont += diff.count();

		        	}
					cont /= reps;
 					myfile <<cont <<","; 
  					myfile.close();

		        }
			
		    ofstream myfile;
		    myfile.open ("outP.txt",ios::app);
 			myfile << "\n"; 
  			myfile.close();

			}

	

	return 0;
}

