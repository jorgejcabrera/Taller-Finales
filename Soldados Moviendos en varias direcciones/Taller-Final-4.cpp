//============================================================================
// Name        : Taller-Final-4.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;
//el mapa es de 7x14

typedef struct{
	string id;
	pair<float,float> posicion;
	int salud;
	string movimientos;
	int ciclos;
	int distancia;
}soldado;

typedef struct{
	pair<float,float> posicion;
	int ataque;
	vector<pair<float,float> > rangoDeAtaque;
}obstaculo;

void cargarRangoDeAtaque(obstaculo* unObstaculo){
	unObstaculo->rangoDeAtaque.clear();
	//norte
	if(unObstaculo->posicion.second-1 >= 0){
		unObstaculo->rangoDeAtaque.push_back(make_pair(unObstaculo->posicion.first,unObstaculo->posicion.second-1));
	}
	//noreste
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

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

vector<soldado> cargarSoldados(){
	vector<soldado> soldados;
	ifstream archivo("/home/jorge/Taller-Finales/Soldados Moviendos en varias direcciones/Source/soldados.txt");
	if(archivo.is_open()){
		string linea;
		soldado unSoldado;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unSoldado.id = *tokens.begin();
			unSoldado.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unSoldado.salud = atoi((*(tokens.begin()+3)).c_str());
			unSoldado.movimientos = *(tokens.begin()+4);
			unSoldado.ciclos = 0;
			unSoldado.distancia = 0;
			soldados.push_back(unSoldado);
		}
	}else{
		cout << "el archivo de soldados no se pudo abrir"<<endl;
	}
	return soldados;
}

vector<obstaculo> cargarObstaculos(){
	vector<obstaculo> obstaculos;
	ifstream archivo("/home/jorge/Taller-Finales/Soldados Moviendos en varias direcciones/Source/obstaculos.txt");
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
		cout << "el archivo de obstaculos no se pudo abrir"<<endl;
	}
	return obstaculos;
}

bool juegoTerminado(vector<soldado> soldados,vector<obstaculo> obstaculos){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud > 0 && !itSoldado->movimientos.empty()){
			return false;
		}
	}
	return true;
}

void aplicarAtaque(soldado* unSoldado,vector<obstaculo> obstaculos){
	int danioAAplicar = 0;
	for(vector<obstaculo>::iterator itObstaculo = obstaculos.begin(); itObstaculo != obstaculos.end(); ++itObstaculo){
		for(vector<pair<float,float> >::iterator itRango = itObstaculo->rangoDeAtaque.begin(); itRango != itObstaculo->rangoDeAtaque.end();++itRango){
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

void moverSoldado(soldado* unSoldado,vector<obstaculo> obstaculos){
	if(unSoldado->salud > 0 && !unSoldado->movimientos.empty()){
		char proximoMovimiento = *unSoldado->movimientos.begin();
		unSoldado->movimientos.erase(unSoldado->movimientos.begin());
		pair<float,float> proximaPosicion = unSoldado->posicion;
		//up
		if(unSoldado->posicion.second-1 >= 0 && proximoMovimiento == 'U'){
			proximaPosicion.second--;
		//left
		}else if(unSoldado->posicion.first-1 >= 0 && proximoMovimiento == 'L'){
			proximaPosicion.first--;
		//down
		}else if(unSoldado->posicion.second+1 <= 14 && proximoMovimiento == 'D'){
			proximaPosicion.second++;
		//right
		}else if(unSoldado->posicion.first+1 <= 7 && proximoMovimiento == 'R')
			proximaPosicion.first++;
		for(vector<obstaculo>::iterator itObstaculo = obstaculos.begin(); itObstaculo != obstaculos.end(); ++itObstaculo){
			if(itObstaculo->posicion == proximaPosicion){
				return;
			}
		}
		unSoldado->posicion = proximaPosicion;
		unSoldado->distancia++;
	}
}

void realizarMovimientos(vector<soldado>* soldados,vector<obstaculo> obstaculos){
	while(!juegoTerminado(*soldados,obstaculos)){
		for(vector<soldado>::iterator itSoldado = soldados->begin(); itSoldado != soldados->end(); ++itSoldado){
			if(itSoldado->salud > 0){
				itSoldado->ciclos = 0;
				aplicarAtaque(&(*itSoldado),obstaculos);
				moverSoldado(&(*itSoldado),obstaculos);
			}
		}
	}
}

bool compararDistancias(const soldado&a,const soldado&b){
	return a.distancia < b.distancia;
}

void imprimirResultado(vector<soldado> soldados){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud > 0){
			cout << "id: "<<itSoldado->id <<",distancia: "<<itSoldado->distancia<<",salud: "<<itSoldado->salud<<endl;
		}
	}
}
int main() {
	vector<soldado> soldados = cargarSoldados();
	vector<obstaculo> obstaculos = cargarObstaculos();
	realizarMovimientos(&soldados,obstaculos);
	sort(soldados.begin(),soldados.end(),compararDistancias);
	imprimirResultado(soldados);
	return 0;
}
