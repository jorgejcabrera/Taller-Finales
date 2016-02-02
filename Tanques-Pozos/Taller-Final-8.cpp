//============================================================================
// Name        : Taller-Final-8.cpp
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
#include <math.h>

using namespace std;

//1- se hardcodea un x maximo 1000
//2- se avanza de a 0.001 en x por cada iteracion
//3- los tanques se mueven en direccion horizontal

 typedef struct{
	 string id;
	 pair<float,float> posicion;
	 int radio;
 }pozo;

 typedef struct{
	 string id;
	 pair<float,float> infIzq;
	 pair<float,float> supDer;
	 float distancia;										//distancia recorrida hasta caer
	 bool colisionado;
 }tanque;

 typedef struct{
	 tanque unTanque;
	 pozo unPozo;
	 bool encontrada;
 }colision;

 float distancia(pair<float,float> pos1,pair<float,float> pos2){
	 return sqrt(pow(pos1.first-pos2.first,2)+pow(pos1.second-pos2.second,2));
 }
 vector<string> tokenize(string linea,char delim){
	 vector<string> tokens;
	 stringstream ss(linea);
	 string token;
	 while(getline(ss,token,delim)){
		 tokens.push_back(token);
	 }
	 return tokens;
 }
 vector<pozo> cargarPozos(){
	 vector<pozo> pozos;
	 ifstream archivo("/home/jorge/Taller-Finales/Tanques-Pozos/Source/pozos.txt");
	 if(archivo.is_open()){
		 string linea;
		 pozo unPozo;
		 while(getline(archivo,linea)){
			 vector<string> tokens = tokenize(linea,' ');
			 unPozo.id = *tokens.begin();
			 unPozo.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			 unPozo.radio = atoi((*(tokens.begin()+3)).c_str());
			 pozos.push_back(unPozo);
		 }
	 }else{
		 cout <<"no se pudo abrir el archivo de pozos"<<endl;
	 }
	 return pozos;
 }
 vector<tanque> cargarTanques(){
	 vector<tanque> tanques;
	 ifstream archivo("/home/jorge/Taller-Finales/Tanques-Pozos/Source/tanques.txt");
	 if(archivo.is_open()){
		 string linea;
		 tanque unTanque;
		 while(getline(archivo,linea)){
			 vector<string> tokens = tokenize(linea,' ');
			 unTanque.id = *tokens.begin();
			 unTanque.infIzq = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			 unTanque.supDer = make_pair(atof((*(tokens.begin()+3)).c_str()),atof((*(tokens.begin()+4)).c_str()));
			 unTanque.distancia = 0;
			 unTanque.colisionado = false;
			 tanques.push_back(unTanque);
		 }
	 }else{
		 cout <<"no se pudo abrir el archivo de tanques"<<endl;
	 }
	 return tanques;
 }
 colision encontrarColision(tanque* unTanque,vector<pozo> pozos){
	 colision colisionEncontrada;
	 colisionEncontrada.encontrada = false;
	 for(vector<pozo>::iterator itPozo = pozos.begin(); itPozo != pozos.end(); ++itPozo){
		 pair<float,float> infDer = make_pair(unTanque->supDer.first,unTanque->infIzq.second);
		 pair<float,float> ptoMedio = make_pair(unTanque->supDer.first, unTanque->infIzq.second+distancia(infDer,unTanque->supDer));
		 if(distancia(infDer,itPozo->posicion) <= itPozo->radio || distancia(ptoMedio,itPozo->posicion) <= itPozo->radio
				 || distancia(unTanque->supDer,itPozo->posicion) <= itPozo->radio){
			 unTanque->colisionado = true;
			 colisionEncontrada.unPozo = *itPozo;
			 colisionEncontrada.unTanque = *unTanque;
			 colisionEncontrada.encontrada = true;
		 }
	 }
	 return colisionEncontrada;
 }
 vector<colision> encontrarColisiones(vector<tanque>* tanques,vector<pozo> pozos){
	 vector<colision> colisiones;
	 int iteraciones = 0;
	 while(iteraciones <= 10000 ){
		 for(vector<tanque>::iterator itTanque = tanques->begin(); itTanque != tanques->end();++itTanque){
			 if(!itTanque->colisionado){
				 colision unaColision = encontrarColision(&(*itTanque),pozos);;
				 //el tanque colisiona
				 if(unaColision.encontrada){
					 colisiones.push_back(unaColision);
				 //el tanque no colisiona asi que lo movemos
				 }else{
					 itTanque->infIzq.first += 0.01;
					 itTanque->supDer.first += 0.01;
					 itTanque->distancia += 0.01;
				 }
			 }
		 }
		 iteraciones++;
	 }
	 return colisiones;
 }
 bool compararDistanciasRecorridas(const colision&a,const colision&b){
	 return a.unTanque.distancia > b.unTanque.distancia;
 }
 void imprimirResultado(vector<colision> colisiones){
	 for(vector<colision>::iterator itColision = colisiones.begin(); itColision != colisiones.end(); ++itColision){
		 cout << "tanque:"<<itColision->unTanque.id <<", pozo:"<<itColision->unPozo.id<<", distancia:"<<itColision->unTanque.distancia<<endl;
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
