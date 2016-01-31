//============================================================================
// Name        : Taller-Final-3.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

//el mapa es de 7x14

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int salud;
	int ciclos;										//cantidad de ciclos que vivio
	int distancia;
}soldado;

typedef struct{
	pair<float,float> posicion;
	int ataque;
	vector<pair<float,float> > rangoDeAtaque;
}obstaculo;

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

void cargarRangoDeAtaque(obstaculo* unObstaculo){
	unObstaculo->rangoDeAtaque.clear();
	//norte
	if(unObstaculo->posicion.second-1 >= 0){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first,unObstaculo->posicion.second-1));
	}
	//noroeste
	if(unObstaculo->posicion.first-1 >= 0 && unObstaculo->posicion.second-1 >= 0){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first-1,unObstaculo->posicion.second-1));
	}
	//oeste
	if(unObstaculo->posicion.first-1 >= 0){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first-1,unObstaculo->posicion.second));
	}
	//suroeste
	if(unObstaculo->posicion.first-1 >= 0 && unObstaculo->posicion.second+1 <= 14){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first-1,unObstaculo->posicion.second+1));
	}
	//sur
	if(unObstaculo->posicion.second+1 <= 14){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first,unObstaculo->posicion.second+1));
	}
	//sureste
	if(unObstaculo->posicion.first+1 <= 7 && unObstaculo->posicion.second+1 <= 14){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first+1,unObstaculo->posicion.second+1));
	}
	//este
	if(unObstaculo->posicion.first+1 <= 7){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first+1,unObstaculo->posicion.second));
	}
	//noreste
	if(unObstaculo->posicion.first+1 <= 7 && unObstaculo->posicion.second-1 >= 0){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first+1,unObstaculo->posicion.second-1));
	}
}

vector<soldado> cargarSoldados(){
	vector<soldado> soldados;
	ifstream archivo("/home/jorge/workspace/Taller-Final-3/Source/soldados.txt");
	if(archivo.is_open()){
		string linea;
		soldado unSoldado;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unSoldado.id = *tokens.begin();
			unSoldado.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unSoldado.salud = atoi((*(tokens.begin()+3)).c_str());
			unSoldado.ciclos = 0;
			unSoldado.distancia = 0;
			soldados.push_back(unSoldado);
		}
	}else{
		cout << "no se pudo abrir el archivo de soldados"<<endl;
	}
	return soldados;
}

vector<obstaculo> cargarObstaculos(){
	vector<obstaculo> obstaculos;
	ifstream archivo("/home/jorge/workspace/Taller-Final-3/Source/obstaculos.txt");
	if(archivo.is_open()){
		string linea;
		obstaculo unObstaculo;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unObstaculo.posicion = make_pair(atof((*(tokens.begin())).c_str()),atof((*(tokens.begin()+1)).c_str()));
			unObstaculo.ataque = atoi((*(tokens.begin()+2)).c_str());
			cargarRangoDeAtaque(&unObstaculo);
			obstaculos.push_back(unObstaculo);
		}
	}else{
		cout << "no se pudo abrir el archivo de obstaculos"<<endl;
	}
	return obstaculos;
}

void intentarDesplazarse(soldado* unSoldado,vector<obstaculo> obstaculos){
	if(unSoldado->salud > 0){
		pair<float,float> proximaPosicion = make_pair(unSoldado->posicion.first+1,unSoldado->posicion.second);
		if(proximaPosicion.first > 7)
			return;
		for(vector<obstaculo>::iterator itObstaculo = obstaculos.begin(); itObstaculo != obstaculos.end(); ++itObstaculo){
			if( itObstaculo->posicion == proximaPosicion){
				return;
			}
		}
		unSoldado->distancia++;
		unSoldado->posicion = proximaPosicion;
	}
}

void recibirDanio(soldado* unSoldado,vector<obstaculo> obstaculos){
	if(unSoldado->salud > 0){
		int danioAAplicar = 0;
		for(vector<obstaculo>::iterator itObstaculo = obstaculos.begin(); itObstaculo != obstaculos.end(); ++itObstaculo){
			for(vector<pair<float,float> >::iterator itRango = itObstaculo->rangoDeAtaque.begin(); itRango != itObstaculo->rangoDeAtaque.end(); ++itRango){
				if(unSoldado->posicion == *itRango){
					danioAAplicar += itObstaculo->ataque;
				}
			}
		}

		if(danioAAplicar >= unSoldado->salud){
			unSoldado->salud = 0;
		}else{
			unSoldado->salud -= danioAAplicar;
		}
	}

}

bool soldadoLuchando(soldado unSoldado,vector<obstaculo> obstaculos){
	for(vector<obstaculo>::iterator itObstaculo = obstaculos.begin(); itObstaculo != obstaculos.end(); ++itObstaculo){
		for(vector<pair<float,float> >::iterator itRango = itObstaculo->rangoDeAtaque.begin(); itRango != itObstaculo->rangoDeAtaque.end(); ++itRango){
			if(unSoldado.posicion == *itRango){
				return true;
			}
		}
	}
	return false;
}

bool juegoTerminado(vector<soldado> soldados,vector<obstaculo> obstaculos){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud > 0 && itSoldado->posicion.first < 7){
			return false;
		}else if( itSoldado->salud > 0 && itSoldado->posicion.first == 7 && soldadoLuchando(*itSoldado,obstaculos)){
			return false;
		}
	}
	return true;
}

void desplazarSoldados(vector<soldado>* soldados,vector<obstaculo> obstaculos){
	while(!juegoTerminado(*soldados,obstaculos)){
		for(vector<soldado>::iterator itSoldado = soldados->begin(); itSoldado != soldados->end(); ++itSoldado){
			if(itSoldado->salud > 0){
				itSoldado->ciclos++;
				recibirDanio(&(*itSoldado),obstaculos);
				intentarDesplazarse(&(*itSoldado),obstaculos);
			}
		}
	}
}

bool compararDistancias(const soldado&a, const soldado&b){
	return a.distancia < b.distancia;
}

void imprimirResultado(vector<soldado> soldados){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud <= 0){
			cout << itSoldado->id << " distancia: "<<itSoldado->distancia<<" ciclos: "<<itSoldado->ciclos<<endl;
		}
	}
}

int main() {
	vector<soldado> soldados = cargarSoldados();
	vector<obstaculo> obstaculos = cargarObstaculos();
	desplazarSoldados(&soldados,obstaculos);
	sort(soldados.begin(),soldados.end(),compararDistancias);
	imprimirResultado(soldados);
	return 0;
}
