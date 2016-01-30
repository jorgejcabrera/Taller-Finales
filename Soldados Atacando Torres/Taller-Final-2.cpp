//============================================================================
// Name        : Taller-Final-2.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

//el mapa es de 7x14

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int salud;
	int fuerza;
}soldado;

typedef struct{
	string id;
	pair<float,float> posicion;
	int salud;
	int fuerza;
	vector<pair<float,float> >rangoDeAtaque;
}torre;

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

void cargarRangoDeAtaque(torre* unaTorre){
	unaTorre->rangoDeAtaque.clear();
	//norte
	if(unaTorre->posicion.second-1 >= 0){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first,unaTorre->posicion.second-1));
	}
	//noroeste
	if(unaTorre->posicion.first-1 >= 0 && unaTorre->posicion.second-1 >= 0 ){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first-1,unaTorre->posicion.second-1));
	}
	//oeste
	if(unaTorre->posicion.first-1 >= 0){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first-1,unaTorre->posicion.second));
	}
	//suroeste
	if(unaTorre->posicion.first-1 >= 0 && unaTorre->posicion.second+1 <= 14){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first-1,unaTorre->posicion.second+1));
	}
	//sur
	if(unaTorre->posicion.second+1 <= 14){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first,unaTorre->posicion.second+1));
	}
	//sureste
	if(unaTorre->posicion.first+1 <= 7 && unaTorre->posicion.second+1 <= 14){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first+1,unaTorre->posicion.second+1));
	}
	//este
	if(unaTorre->posicion.first+1 <= 7){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first+1,unaTorre->posicion.second));
	}
	//noreste
	if(unaTorre->posicion.first+1 <= 7 && unaTorre->posicion.second-1 >= 0){
		unaTorre->rangoDeAtaque.push_back(make_pair(unaTorre->posicion.first+1,unaTorre->posicion.second-1));
	}
}

vector<soldado> cargarSoldados(){
	vector<soldado> soldados;
	ifstream archivo("/home/jorge/workspace/Taller-Final-2/Source/soldados.txt");
	if(archivo.is_open()){
		string linea;
		soldado unSoldado;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unSoldado.id = *tokens.begin();
			unSoldado.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unSoldado.salud = atoi((*(tokens.begin()+3)).c_str());
			unSoldado.fuerza = atoi((*(tokens.begin()+4)).c_str());
			soldados.push_back(unSoldado);
		}
	}else{
		cout << "no se pudo abrir el archivo de soldados"<<endl;
	}
	return soldados;
}

vector<torre> cargarTorres(){
	vector<torre> torres;
	ifstream archivo("/home/jorge/workspace/Taller-Final-2/Source/torres.txt");
	if(archivo.is_open()){
		string linea;
		torre unaTorre;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unaTorre.id = *tokens.begin();
			unaTorre.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unaTorre.salud = atoi((*(tokens.begin()+3)).c_str());
			unaTorre.fuerza = atoi((*(tokens.begin()+4)).c_str());
			cargarRangoDeAtaque(&unaTorre);
			torres.push_back(unaTorre);
		}
	}else{
		cout << "no se pudo abrir el archivo de torres"<<endl;
	}
	return torres;
}

bool soldadoLuchando(soldado unSoldado,vector<torre> torres){
	for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
		if(itTorre->salud > 0){
			for(vector<pair<float,float> >::iterator itRango = itTorre->rangoDeAtaque.begin(); itRango != itTorre->rangoDeAtaque.end(); ++itRango){
				if( unSoldado.posicion == *itRango){
					return true;
				}
			}
		}
	}
	return false;
}

bool juegoTerminado(vector<soldado> soldados,vector<torre> torres){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud > 0){
			if( itSoldado->posicion.first < 7){
				return false;
			}else if(itSoldado->posicion.first == 7 && soldadoLuchando(*itSoldado,torres)){
				cout << "el soldado esta luchando"<<endl;
				return false;
			}
		}
	}
	return true;
}

void intentarAtacarTorres(soldado* unSoldado,vector<torre>* torres){
	if(unSoldado->salud > 0){
		for(vector<torre>::iterator itTorre = torres->begin(); itTorre != torres->end(); ++itTorre){
			if(itTorre->salud > 0){
				for(vector<pair<float,float> >::iterator itRango = itTorre->rangoDeAtaque.begin(); itRango != itTorre->rangoDeAtaque.end(); ++itRango){
					//atacamos
					if(unSoldado->posicion == *itRango){
						if(unSoldado->fuerza >= itTorre->salud){
							itTorre->salud = 0;
						}else{
							itTorre->salud -= unSoldado->fuerza;
						}
					}
				}
			}
		}
	}
}

void recibirDanioDeTorres(soldado* unSoldado,vector<torre> torres){
	if(unSoldado->salud > 0){
		int danioCausado = 0;
		for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
			if(itTorre->salud > 0){
				for(vector<pair<float,float> >::iterator itRango = itTorre->rangoDeAtaque.begin(); itRango != itTorre->rangoDeAtaque.end(); ++itRango){
					if(unSoldado->posicion == *itRango){
						danioCausado += itTorre->fuerza;
					}
				}
			}
		}
		if(danioCausado >= unSoldado->salud){
			unSoldado->salud = 0;
		}else{
			unSoldado->salud -= danioCausado;
		}
	}
}

void intentarMoverse(soldado* unSoldado,vector<torre> torres){
	if( unSoldado->salud > 0){
		pair<float,float> proximaPosicion = make_pair(unSoldado->posicion.first+1,unSoldado->posicion.second);
		if(proximaPosicion.first > 7) return;
		for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
			if(itTorre->salud > 0 && itTorre->posicion == proximaPosicion){
				return;
			}
		}
		unSoldado->posicion = proximaPosicion;
	}
}

void atacar(vector<soldado>* soldados,vector<torre>* torres){
	while(!juegoTerminado(*soldados,*torres)){
		for(vector<soldado>::iterator itSoldado = soldados->begin(); itSoldado != soldados->end(); ++itSoldado){
			recibirDanioDeTorres(&(*itSoldado),*torres);
			intentarAtacarTorres(&(*itSoldado),torres);
			intentarMoverse(&(*itSoldado),*torres);
		}
	}
}

void imprimirResultado(vector<soldado> soldados,vector<torre> torres){
	cout << "los soldados caidos son: "<<endl;
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud <= 0){
			cout << itSoldado->id<<endl;
		}
	}
	cout << "las torres caidas son: "<<endl;
	for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
		if(itTorre->salud <= 0){
			cout << itTorre->id<<endl;
		}
	}
}

int main() {
	vector<soldado> soldados = cargarSoldados();
	vector<torre> torres = cargarTorres();
	atacar(&soldados,&torres);
	imprimirResultado(soldados,torres);
	return 0;
}
