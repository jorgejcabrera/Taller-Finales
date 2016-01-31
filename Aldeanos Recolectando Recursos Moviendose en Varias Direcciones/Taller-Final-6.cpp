//============================================================================
// Name        : Taller-Final-6.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

//el mapa es de 7x13

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int fuerza;
	string movimientos;
	int cantidadRecolectada;
	int distancia;
}aldeano;

typedef struct{
	string id;
	pair<float,float> posicion;
	int cantidadDisponible;
	vector<pair<float,float> > rangoDeRecoleccion;
}recurso;

void cargarRangoDeRecoleccion(recurso* unRecurso){
	unRecurso->rangoDeRecoleccion.clear();
	//norte.
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
	if(unRecurso->posicion.first-1 >= 0 && unRecurso->posicion.second+1 <= 13){
		unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first-1,unRecurso->posicion.second+1));
	}
	//sur
	if(unRecurso->posicion.second+1 <= 13){
		unRecurso->rangoDeRecoleccion.push_back(make_pair(unRecurso->posicion.first,unRecurso->posicion.second+1));
	}
	//sureste
	if(unRecurso->posicion.first+1 <= 7 && unRecurso->posicion.second+1 <= 13){
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
	ifstream archivo("/home/jorge/Taller-Finales/Aldeanos Recolectando Recursos Moviendose en Varias Direcciones/Source/aldeanos.txt");
	if(archivo.is_open()){
		string linea;
		aldeano unAldeano;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unAldeano.id = *tokens.begin();
			unAldeano.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unAldeano.fuerza = atoi((*(tokens.begin()+3)).c_str());
			unAldeano.movimientos = *(tokens.begin()+4);
			unAldeano.cantidadRecolectada = 0;
			unAldeano.distancia = 0;
			aldeanos.push_back(unAldeano);
		}

	}else{
		cout << "no se pudo abrir el archivo de aldeanos"<<endl;
	}
	return aldeanos;
}

vector<recurso> cargarRecursos(){
	vector<recurso> recursos;
	ifstream archivo("/home/jorge/Taller-Finales/Aldeanos Recolectando Recursos Moviendose en Varias Direcciones/Source/recursos.txt");
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
		cout << "no se pudo abrir el archivo de aldeanos"<<endl;
	}
	return recursos;
}

bool aldeanoRecolectando(aldeano unAldeano,vector<recurso> recursos){
	if(unAldeano.fuerza > 0){
		for(vector<recurso>::iterator itRecurso = recursos.begin(); itRecurso != recursos.end(); ++itRecurso){
			if(itRecurso->cantidadDisponible > 0){
				for(vector<pair<float,float> >::iterator itRango = itRecurso->rangoDeRecoleccion.begin(); itRango != itRecurso->rangoDeRecoleccion.end(); ++itRango){
					if(unAldeano.posicion == *itRango){
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool juegoTerminado(vector<aldeano> aldeanos,vector<recurso> recursos){
	for(vector<aldeano>::iterator itAldeano = aldeanos.begin(); itAldeano != aldeanos.end(); ++itAldeano){
		if(itAldeano->fuerza > 0 && !itAldeano->movimientos.empty()){
			return false;
		}else if(itAldeano->fuerza > 0 && itAldeano->movimientos.empty() && aldeanoRecolectando(*itAldeano,recursos)){
			return false;
		}
	}
	return true;
}

void recolectarRecursos(aldeano* unAldeano,vector<recurso>* recursos){
	if(unAldeano->fuerza > 0){
		for(vector<recurso>::iterator itRecurso = recursos->begin(); itRecurso != recursos->end(); ++itRecurso){
			if(itRecurso->cantidadDisponible > 0){
				for(vector<pair<float,float> >::iterator itRango = itRecurso->rangoDeRecoleccion.begin(); itRango != itRecurso->rangoDeRecoleccion.end(); ++itRango){
					if(unAldeano->posicion == *itRango){
						if(unAldeano->fuerza >= itRecurso->cantidadDisponible){
							unAldeano->cantidadRecolectada += itRecurso->cantidadDisponible;
							itRecurso->cantidadDisponible = 0;
						}else{
							unAldeano->cantidadRecolectada += unAldeano->fuerza;
							itRecurso->cantidadDisponible -= unAldeano->fuerza;
						}
					}
				}
			}
		}
	}
}


void moverAldeano(aldeano* unAldeano,vector<recurso> recursos){
	if(!unAldeano->movimientos.empty()){
		char proximoMovimiento = *unAldeano->movimientos.begin();
		unAldeano->movimientos.erase(unAldeano->movimientos.begin());
		pair<float,float> proximaPosicion = unAldeano->posicion;
		//up
		if(proximoMovimiento == 'U' && unAldeano->posicion.second-1 >= 0){
			proximaPosicion.second--;
		//left
		}else if(proximoMovimiento == 'L' && unAldeano->posicion.first-1 >= 0){
			proximaPosicion.first--;
		//down
		}else if(proximoMovimiento == 'D' && unAldeano->posicion.second+1 <= 13){
			proximaPosicion.second++;
		//right
		}else if(proximoMovimiento == 'R' && unAldeano->posicion.first+1 <= 7){
			proximaPosicion.first++;
		}
		for(vector<recurso>::iterator itRecurso = recursos.begin(); itRecurso != recursos.end(); ++itRecurso){
			if(itRecurso->posicion == proximaPosicion){
				return;
			}
		}
		if(unAldeano->posicion != proximaPosicion)
			unAldeano->distancia++;
		unAldeano->posicion = proximaPosicion;
	}
}

void recolectarRecursos(vector<aldeano>* aldeanos,vector<recurso>* recursos){
	while(!juegoTerminado(*aldeanos,*recursos)){
		for(vector<aldeano>::iterator itAldeano = aldeanos->begin(); itAldeano != aldeanos->end();++itAldeano){
			recolectarRecursos(&(*itAldeano),recursos);
			moverAldeano(&(*itAldeano),*recursos);
		}
	}
}

bool compararCantidadRecolectada(const aldeano&a,const aldeano&b){
	return a.cantidadRecolectada > b.cantidadRecolectada;
}

void imprimirResultado(vector<aldeano> aldeanos){
	for(vector<aldeano>::iterator itAldeano = aldeanos.begin(); itAldeano != aldeanos.end(); ++itAldeano){
		cout <<"id:"<<itAldeano->id<<", cantidad recolectada:"<<itAldeano->cantidadRecolectada<<", distancia:"<<itAldeano->distancia<<endl;
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
