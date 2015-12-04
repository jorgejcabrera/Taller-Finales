//============================================================================
// Name        : Taller-Final-Tanques.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

using namespace std;

typedef struct{
	pair<float,float> posicion;
	float radio;
	string id;
}pozo;

typedef struct{
	pair<float,float> posicion;
	string id;
	string direccion;
}tanque;

typedef struct{
	tanque tanqueC;
	pozo pozoC;
	float distancia;
}colision;

std::vector<std::string> split(const std::string &linea,char delim){
	std::vector<std::string> tokens;
	std::string token;
	std::stringstream ss(linea);
	while(std::getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

list<pozo> cargarPozos( ){
	list<pozo> pozos;
	ifstream archivoPozos ("/home/jorge/workspace/Taller-Final-Tanques/pozos.txt");
	string linea;
	pozo pozo;
	string sX;
	string sY;
	string sRadio;
	if (archivoPozos.is_open()){
		while(getline(archivoPozos,linea)){
			vector<string> tokens = split(linea,' ');
			sX = *(tokens.begin()+1);
			sY = *(tokens.begin()+2);
			sRadio = *(tokens.begin()+3);
			pozo.id = *tokens.begin();
			pozo.posicion = make_pair(strtof(sX.c_str(),NULL),strtof(sY.c_str(),NULL));
			pozo.radio = strtof(sRadio.c_str(),NULL);
			pozos.push_back(pozo);
		}
	}else{
		cout << "no se pudo abrir el archivo de tanques"<<endl;
	}
	return pozos;
}

list<tanque> cargarTanques(){
	list<tanque> tanques;
	ifstream archivoTanques ("/home/jorge/workspace/Taller-Final-Tanques/tanques.txt");
	tanque tanque;
	string linea;
	string sX;
	string sY;
	if (archivoTanques.is_open()){
		while(getline(archivoTanques,linea)){
			vector<string> tokens = split(linea,' ');
			sX = *(tokens.begin()+1);
			sY = *(tokens.begin()+2);
			tanque.id = *(tokens.begin());
			tanque.posicion = make_pair(strtof(sX.c_str(),NULL),strtof(sY.c_str(),NULL));
			tanque.direccion = *(tokens.begin()+3);
			tanque.direccion.erase(std::remove(tanque.direccion.begin(),tanque.direccion.end(),'\r'),tanque.direccion.end());
			tanques.push_back(tanque);
		}
	}else{
		cout << "no se pudo abrir el archivo de tanques"<<endl;
	}
	return tanques;
}

float resolverCuadratica(float a,float b, float c, float d){
	float a1 = sqrt( pow(a,2) - pow(b-c,2) );
	float a2 = - sqrt( pow(a,2) - pow(b-c,2) );
	float resultado1 = a1 + d;
	float resultado2 = a2 + d;
	if( resultado1 > resultado2 ){
		return resultado2;
	}else{
		return resultado1;
	}
}

float calcularDistancia(pozo pozo,tanque tanque){
	pair<float,float> posicionDeColision;
	//colision horizontal
	if(tanque.direccion == "H"){
		posicionDeColision = make_pair(0,tanque.posicion.second);
		float xColision = resolverCuadratica(pozo.radio,tanque.posicion.second,pozo.posicion.second,pozo.posicion.first);
		posicionDeColision.first = xColision;
		return sqrt(pow(pozo.posicion.first - posicionDeColision.first,2) + pow(pozo.posicion.second - posicionDeColision.second,2));
	//colision vertical
	}else if(tanque.direccion == "V"){
		posicionDeColision = make_pair(tanque.posicion.first,0);
		float yColision = resolverCuadratica(pozo.radio,tanque.posicion.first,pozo.posicion.first,pozo.posicion.second);
		posicionDeColision.second = yColision;
		return sqrt(pow(pozo.posicion.first - posicionDeColision.first,2) + pow(pozo.posicion.second - posicionDeColision.second,2));
	}
	return 0;
}

bool compararDistancias(const colision &a,const colision &b){
	return a.distancia < b.distancia;
}

//POST: devuelve de la lista de pozos, el pozo que está más cerca de colisionar con el tanque
colision encontrarColision(tanque tanque,list<pozo> pozos){
	colision colisionMenorDistancia;
	colisionMenorDistancia.distancia = 0;
	vector<colision> colisiones;
	for(list<pozo>::iterator itPozo = pozos.begin(); itPozo != pozos.end();++itPozo){
		if( tanque.direccion == "H" && tanque.posicion.second <= itPozo->posicion.second + itPozo->radio &&
			tanque.posicion.second >= itPozo->posicion.second - itPozo->radio){
			pozo pozoAuxiliar = *(itPozo);
			colisionMenorDistancia.pozoC = pozoAuxiliar;
			colisionMenorDistancia.tanqueC = tanque;
			colisionMenorDistancia.distancia = calcularDistancia(pozoAuxiliar,tanque);
			colisiones.push_back(colisionMenorDistancia);
		}else if(tanque.direccion == "V" && tanque.posicion.first <= itPozo->posicion.first + itPozo->radio &&
			tanque.posicion.first >= itPozo->posicion.first - itPozo->radio){
			colisionMenorDistancia.pozoC = *itPozo;
			colisionMenorDistancia.tanqueC = tanque;
			colisionMenorDistancia.distancia = calcularDistancia(*itPozo,tanque);
			colisiones.push_back(colisionMenorDistancia);
		}else if(tanque.direccion != "V" && tanque.direccion != "H"){
			cout << "la direccion del tanque es inválida"<<endl;
		}
	}
	std::sort(colisiones.begin(),colisiones.end(),compararDistancias);
	if(!colisiones.empty())
		colisionMenorDistancia = *(colisiones.begin());
	return colisionMenorDistancia;
}

vector<colision> encontrarColisiones(list<tanque> tanques,list<pozo> pozos){
	vector<colision> tanquesColisionados;
	colision colision;
	for(list<tanque>::iterator itTanque = tanques.begin(); itTanque!= tanques.end();++itTanque){
		colision  = encontrarColision(*itTanque,pozos);
		if(colision.distancia != 0){
			tanquesColisionados.push_back(colision);
		}
	}
	return tanquesColisionados;
}

void imprimirColisiones(vector<colision> tanques){
	for(vector<colision>::iterator it = tanques.begin(); it != tanques.end();++it)
		cout << it->tanqueC.id <<" "<<it->pozoC.id<<" "<<it->distancia<<endl;
}

int main() {
	list<pozo> pozos = cargarPozos();
	list<tanque> tanques = cargarTanques();
	vector<colision> tanquesColisionados = encontrarColisiones(tanques,pozos);
	std::sort(tanquesColisionados.begin(),tanquesColisionados.end(),compararDistancias);
	imprimirColisiones(tanquesColisionados);
	return 0;
}
