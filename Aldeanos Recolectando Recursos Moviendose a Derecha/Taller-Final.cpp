//============================================================================
// Name        : Taller-Final.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
//el mapa es de 7 x 14

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int fuerza;
	int cantidadRecolectada;
}aldeano;

typedef struct{
	string id;
	pair<float,float> posicion;
	int cantidadDisponible;
	vector<pair<float,float> > rangoDeRecoleccion;
}recurso;

void cargarRangoDeRecoleccion(recurso* unRecurso){
	unRecurso->rangoDeRecoleccion.clear();
	//norte
	if(unRecurso->posicion.second-1 >= 0){
		unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first,unRecurso->posicion.second-1));
	}
	//noroeste
	if(unRecurso->posicion.first-1 >= 0 && unRecurso->posicion.second-1 >= 0){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first-1,unRecurso->posicion.second-1));
	}
	//oeste
	if(unRecurso->posicion.first-1 >= 0){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first-1,unRecurso->posicion.second));
	}
	//suroeste
	if(unRecurso->posicion.first-1 >= 0 && unRecurso->posicion.second+1 <= 14){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first-1,unRecurso->posicion.second+1));
	}
	//sur
	if(unRecurso->posicion.second+1 <= 14){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first,unRecurso->posicion.second+1));
	}
	//sureste
	if(unRecurso->posicion.first+1 <= 7 && unRecurso->posicion.second+1 <= 14){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first+1,unRecurso->posicion.second+1));
	}
	//este
	if(unRecurso->posicion.first+1 <= 7){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first+1,unRecurso->posicion.second));
	}
	//noreste
	if(unRecurso->posicion.first+1 <= 7 && unRecurso->posicion.second-1 >= 0){
			unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first+1,unRecurso->posicion.second-1));
	}
}

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

vector<aldeano> cargarAldeanos(){
	vector<aldeano> aldeanos;
	ifstream archivo("/home/jorge/Taller-Finales/Aldeanos Recolectando Recursos Moviendose a Derecha/Source/aldeanos.txt");
	if(archivo.is_open()){
		string linea;
		aldeano unAldeano;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unAldeano.id = *tokens.begin();
			unAldeano.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unAldeano.fuerza = atoi((*(tokens.begin()+3)).c_str());
			unAldeano.cantidadRecolectada = 0;
			aldeanos.push_back(unAldeano);
		}
	}else{
		cout << "no se pudo abrir el archivo de aldeanos"<<endl;
	}
	return aldeanos;
}

vector<recurso> cargarRecursos(){
	vector<recurso> recursos;
	ifstream archivo("/home/jorge/Taller-Finales/Aldeanos Recolectando Recursos Moviendose a Derecha/Source/recursos.txt");
	if(archivo.is_open()){
		string linea;
		recurso unRecurso;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unRecurso.id = *tokens.begin();
			unRecurso.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unRecurso.cantidadDisponible = atoi((*(tokens.begin()+3)).c_str());
			cargarRangoDeRecoleccion(&unRecurso);
			recursos.push_back(unRecurso);
		}
	}else{
		cout << "no se pudo abrir el archivo de recursos"<<endl;
	}
	return recursos;
}

bool aldeanoTrabajando(aldeano unAldeano,vector<recurso> recursos){
	for(vector<recurso>::iterator itRecurso = recursos.begin(); itRecurso != recursos.end(); ++itRecurso){
		if(itRecurso->cantidadDisponible > 0){
			for(vector<pair<float,float> >::iterator itRango = itRecurso->rangoDeRecoleccion.begin(); itRango != itRecurso->rangoDeRecoleccion.end(); ++itRango){
				if(unAldeano.posicion == *itRango){
					return true;
				}
			}

		}
	}
	return false;
}

//el juego no se termina si:
//1- hay algún aldeano trabado pero recolectando recursos
//2- el aldeano se puede mover
bool juegoTerminado(vector<aldeano> aldeanos,vector<recurso> recursos){
	for(vector<aldeano>::iterator itAldeano = aldeanos.begin(); itAldeano != aldeanos.end(); ++itAldeano){
		if(itAldeano->posicion.first < 7){
			return false;
		}else if( itAldeano->posicion.first == 7 && aldeanoTrabajando(*itAldeano,recursos)){
			return false;
		}
	}
	return true;
}

void intentarRecolectar(aldeano* unAldeano,vector<recurso>* recursos){
	int cantidadARecolectar = 0;
	for(vector<recurso>::iterator itRecurso = recursos->begin(); itRecurso != recursos->end(); ++itRecurso){
		if(itRecurso->cantidadDisponible > 0){
			for(vector<pair<float,float> >::iterator itRango = itRecurso->rangoDeRecoleccion.begin(); itRango != itRecurso->rangoDeRecoleccion.end(); ++itRango){
				if(unAldeano->posicion == *itRango){
					if(unAldeano->fuerza >= itRecurso->cantidadDisponible){
						cantidadARecolectar += itRecurso->cantidadDisponible;
						itRecurso->cantidadDisponible = 0;
					}else{
						cantidadARecolectar += unAldeano->fuerza;
						itRecurso->cantidadDisponible -= unAldeano->fuerza;
					}
				}
			}
		}
	}
	unAldeano->cantidadRecolectada += cantidadARecolectar;
}

void intentarMoverse(aldeano* unAldeano,vector<recurso> recursos){
	if(unAldeano->posicion.first < 7){
		pair<float,float> proximaPosicion = make_pair(unAldeano->posicion.first+1,unAldeano->posicion.second);
		for(vector<recurso>::iterator itRecurso = recursos.begin(); itRecurso != recursos.end(); ++itRecurso){
			if(itRecurso->cantidadDisponible > 0 && itRecurso->posicion == proximaPosicion){
				return;
			}
		}
		unAldeano->posicion = proximaPosicion;
	}
}

void recolectarRecursos(vector<aldeano>* aldeanos,vector<recurso>* recursos){
	while(!juegoTerminado(*aldeanos,*recursos)){
		for(vector<aldeano>::iterator itAldeano = aldeanos->begin(); itAldeano != aldeanos->end(); ++itAldeano){
			intentarRecolectar(&(*itAldeano),recursos);
			intentarMoverse(&(*itAldeano),*recursos);
		}

	}
}

bool compararCantidadRecolectada(const aldeano&a , const aldeano&b){
	return a.cantidadRecolectada > b.cantidadRecolectada;
}

void imprimirResultado(vector<aldeano> aldeanos){
	for(vector<aldeano>::iterator itAldeano = aldeanos.begin(); itAldeano != aldeanos.end(); ++itAldeano){
		cout << itAldeano->id << " "<<itAldeano->cantidadRecolectada<<endl;
	}
}

int main() {
	vector<aldeano> aldeanos = cargarAldeanos();
	vector<recurso> recursos = cargarRecursos();
	recolectarRecursos(&aldeanos,&recursos);
	sort(aldeanos.begin(),aldeanos.end(),compararCantidadRecolectada);
	imprimirResultado(aldeanos);
	return 0;
}
