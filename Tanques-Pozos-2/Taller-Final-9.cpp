//============================================================================
// Name        : Taller-Final-9.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>

//1- se itera un maximo de 10000 veces
//2- por iteracion se avanza 0.01 en x o en y

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int radio;
}pozo;

typedef struct{
	string id;
	pair<float,float> posicion;
	string direccion;
	float distancia;
	bool colisionado;
}tanque;

typedef struct{
	pozo unPozo;
	tanque unTanque;
	bool encontrada;
}colision;

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}
float distancia(pair<float,float> pto1,pair<float,float> pto2){
	return sqrt(pow(pto1.first-pto2.first,2)+pow(pto1.second-pto2.second,2));
}
vector<tanque> cargarTanques(){
	vector<tanque> tanques;
	ifstream archivo("/home/jorge/Taller-Finales/Tanques-Pozos-2/Source/tanques.txt");
	if(archivo.is_open()){
		string linea;
		tanque unTanque;
		while(getline(archivo,linea)){
			 vector<string> tokens = tokenize(linea,' ');
			 unTanque.id = *tokens.begin();
			 unTanque.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			 unTanque.direccion = *(tokens.begin()+3);
			 unTanque.distancia = 0;
			 unTanque.colisionado = false;
			 tanques.push_back(unTanque);
		}
	}else{
		cout <<"no se pudo abrir el archivo de tanques"<<endl;
	}
	return tanques;
}
vector<pozo> cargarPozos(){
	vector<pozo> pozos;
	ifstream archivo("/home/jorge/Taller-Finales/Tanques-Pozos-2/Source/pozos.txt");
	if(archivo.is_open()){
		string linea;
		pozo unPozo;
		while(getline(archivo,linea)){
			 vector<string> tokens = tokenize(linea,' ');
			 unPozo.id = *tokens.begin();
			 unPozo.posicion =  make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			 unPozo.radio = atoi((*(tokens.begin()+3)).c_str());
			 pozos.push_back(unPozo);
		}
	}else{
		cout <<"no se pudo abrir el archivo de pozos"<<endl;
	}
	return pozos;
}
colision encontrarColision(tanque* unTanque,vector<pozo> pozos){
	colision unaColision;
	unaColision.encontrada = false;
	for(vector<pozo>::iterator itPozo = pozos.begin(); itPozo != pozos.end(); ++itPozo){
		if(distancia(unTanque->posicion,itPozo->posicion) < itPozo->radio ){
			unTanque->colisionado = true;
			unaColision.encontrada = true;
			unaColision.unPozo = *itPozo;
			unaColision.unTanque = *unTanque;
		}
	}
	return unaColision;
}
void desplazarTanque(tanque* unTanque){
	float desplazamientoMinimo = 0.01;
	if(unTanque->direccion == "H"){
		unTanque->posicion.first += desplazamientoMinimo;
	}else if(unTanque->direccion == "V"){
		unTanque->posicion.second += desplazamientoMinimo;
	}
	unTanque->distancia += desplazamientoMinimo;
}
vector<colision> encontrarColisiones(vector<tanque>* tanques,vector<pozo> pozos){
	vector<colision> colisiones;
	int iteracion = 0;
	while(iteracion <= 10000){
		for(vector<tanque>::iterator itTanque = tanques->begin(); itTanque != tanques->end(); ++itTanque){
			if(!itTanque->colisionado){
				colision unaColision = encontrarColision(&(*itTanque),pozos);

				if(unaColision.encontrada){
					colisiones.push_back(unaColision);
				}else{
					desplazarTanque(&(*itTanque));
				}
			}
		}
		iteracion++;
	}

	return colisiones;
}
bool compararDistanciasRecorridas(const colision&a,const colision&b){
	return a.unTanque.distancia>b.unTanque.distancia;
}
void imprimirResultado(vector<colision> colisiones){
	for(vector<colision>::iterator itColision = colisiones.begin(); itColision != colisiones.end(); ++itColision){
		cout << "id tanque:"<<itColision->unTanque.id<<" ,id pozo:"<<itColision->unPozo.id<<" ,distancia:"<<itColision->unTanque.distancia<<endl;
	}
}
int main() {
	vector<tanque> tanques = cargarTanques();
	vector<pozo> pozos = cargarPozos();
	vector<colision> colisiones = encontrarColisiones(&tanques,pozos);
	sort(colisiones.begin(),colisiones.end(),compararDistanciasRecorridas);
	imprimirResultado(colisiones);
	return 0;
}
