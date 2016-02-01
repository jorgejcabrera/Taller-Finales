// Soldados Atacando Centros Civicos.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

//el mapa es de 7x14

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int ataque;
	int salud;
	string movimientos;
	string equipo;
	int ciclos;										//son los ciclos de ataque
}soldado;

typedef struct{
	pair<float,float> posicion;						//es el vertice inferior izquierdo
	int salud;
	int ataque;
	int ancho;
	int alto;
	string equipo;
	vector<pair<float,float> > rangoDeAtaque;
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

//PRE: la posicion de la torr siempre cae dentro del mapa
void cargarRangoDeAtaque(torre* unaTorre){
	unaTorre->rangoDeAtaque.clear();
	//lado izquierdo
	for(int j = unaTorre->posicion.second-unaTorre->alto; j <= unaTorre->posicion.second+1 ; j++){
		pair<float,float> rangoDeAtaque = make_pair(unaTorre->posicion.first-1,j);
		if( rangoDeAtaque.first >= 0 && rangoDeAtaque.second >= 0 && rangoDeAtaque.second <= 14){
			unaTorre->rangoDeAtaque.push_back(rangoDeAtaque);
		}
	}
	//lado derecho
	for(int j = unaTorre->posicion.second-unaTorre->alto; j <= unaTorre->posicion.second+1; j++){
		pair<float,float> rangoDeAtaque = make_pair(unaTorre->posicion.first+unaTorre->ancho,j);
		if(rangoDeAtaque.first <= 7 && rangoDeAtaque.second >= 0 && rangoDeAtaque.second <= 14){
			unaTorre->rangoDeAtaque.push_back(rangoDeAtaque);
		}
	}
	//lado de arriba
	for(int i = unaTorre->posicion.first; i < unaTorre->posicion.first+unaTorre->ancho; i++){
		pair<float,float> rangoDeAtaque = make_pair(i,unaTorre->posicion.second-unaTorre->alto);
		if(rangoDeAtaque.second >= 0){
			unaTorre->rangoDeAtaque.push_back(rangoDeAtaque);
		}
	}
	//lado de abajo
	for(int i = unaTorre->posicion.first; i < unaTorre->posicion.first+unaTorre->ancho; i++){
		pair<float,float> rangoDeAtaque = make_pair(i,unaTorre->posicion.second+1);
		if(rangoDeAtaque.second <= 14){
			unaTorre->rangoDeAtaque.push_back(rangoDeAtaque);
		}
	}
}

vector<soldado> cargarSoldados(){
	vector<soldado> soldados;
	ifstream archivo("soldados.txt");
	if(archivo.is_open()){
		string linea;
		soldado unSoldado;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unSoldado.id = *tokens.begin();
			unSoldado.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unSoldado.ataque = atoi((*(tokens.begin()+3)).c_str());
			unSoldado.salud = atoi((*(tokens.begin()+4)).c_str());
			unSoldado.movimientos = *(tokens.begin()+5);
			unSoldado.equipo = *(tokens.begin()+6);
			unSoldado.ciclos = 0;
			soldados.push_back(unSoldado);
		}
	}else{
		cout<<"el archivo de soldados no se pudo abrir"<<endl;
	}
	return soldados;
}

vector<torre> cargarTorres(){
	vector<torre> torres;
	ifstream archivo("torres.txt");
	if(archivo.is_open()){
		string linea;
		torre unaTorre;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unaTorre.posicion = make_pair(atof((*tokens.begin()).c_str()),atof((*(tokens.begin()+1)).c_str()));
			unaTorre.salud = atoi((*(tokens.begin()+2)).c_str());
			unaTorre.ataque = atoi((*(tokens.begin()+3)).c_str());
			unaTorre.ancho = atoi((*(tokens.begin()+4)).c_str());
			unaTorre.alto = atoi((*(tokens.begin()+5)).c_str());
			unaTorre.equipo = *(tokens.begin()+6);
			cargarRangoDeAtaque(&unaTorre);
			torres.push_back(unaTorre);
		}
	}else{
		cout<<"el archivo de torres no se pudo abrir"<<endl;
	}
	return torres;
}

//POST: el juego termina si se destruye una de las dos torres, o si algún jugador se queda sin soldados.
//si se quedan sin movimientos pero están vivos y ambos mantienen sus centros civicos el juego no termina
bool juegoTerminado(vector<soldado> soldados,vector<torre> torres){
	bool perdioEquipoAzul = true;
	bool perdioEquipoRojo = true;
	//chequeo si destruyeron las torres
	for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
		if(itTorre->salud <= 0){
			return true;
		}
	}
	//cheque si alguien se quedo sin soldados
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if( itSoldado->salud > 0 && itSoldado->equipo == "Rojo"){
			perdioEquipoRojo = false;
		}else if( itSoldado->salud > 0 && itSoldado->equipo == "Azul"){
			perdioEquipoAzul = false;
		}
	}
	if(perdioEquipoAzul || perdioEquipoRojo){
		return true;
	}
	return false;
}

void intentarRecibirDanio(soldado* unSoldado,vector<torre> torres){
	if(unSoldado->salud > 0){
		int danioARecibir = 0;
		for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
			for(vector<pair<float,float> >::iterator itRango = itTorre->rangoDeAtaque.begin(); itRango != itTorre->rangoDeAtaque.end();++itRango){
				if(itTorre->salud > 0 && unSoldado->posicion == *itRango && unSoldado->equipo != itTorre->equipo){
					danioARecibir+= itTorre->ataque;
				}
			}
		}
		if(danioARecibir >= unSoldado->salud){
			unSoldado->salud = 0;
		}else{
			unSoldado->salud -= danioARecibir;
		}
	}
}

void intentarAtacarTorres(soldado* unSoldado,vector<torre>* torres){
	if(unSoldado->salud > 0){
		for(vector<torre>::iterator itTorre = torres->begin(); itTorre != torres->end(); ++itTorre){
			for(vector<pair<float,float> >::iterator itRango = itTorre->rangoDeAtaque.begin(); itRango != itTorre->rangoDeAtaque.end();++itRango){
				//atacamos
				if( unSoldado->posicion == *itRango && unSoldado->equipo != itTorre->equipo){
					if(!unSoldado->movimientos.empty()){
						unSoldado->movimientos.erase(unSoldado->movimientos.begin(),unSoldado->movimientos.end());
					}
					unSoldado->ciclos++;
					if( unSoldado->ataque >= itTorre->salud){
						itTorre->salud = 0;
					}else{
						itTorre->salud -= unSoldado->ataque;
					}
				}
			}
		}
	}
}

void intentarMover(soldado* unSoldado,vector<torre> torres,vector<soldado> soldados){
	if(unSoldado->salud > 0 && !unSoldado->movimientos.empty()){
		char proximoMovimiento = *unSoldado->movimientos.begin();
		unSoldado->movimientos.erase(unSoldado->movimientos.begin());
		pair<float,float> proximaPosicion = unSoldado->posicion;
		//up
		if(proximoMovimiento == 'U' && proximaPosicion.second-1 >= 0){
			proximaPosicion.second--;
		//left
		}else if(proximoMovimiento == 'L' && proximaPosicion.first-1 >= 0){
			proximaPosicion.first--;
		//down
		}else if(proximoMovimiento == 'D' && proximaPosicion.second+1 <= 14){
			proximaPosicion.second++;
		//right
		}else if(proximoMovimiento == 'R' && proximaPosicion.first+1 <= 7){
			proximaPosicion.first++;
		}
		//me fijo si en la proxima posicion hay una torre
		for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
			pair<float,float> verticeSuperiorDerecho = make_pair(itTorre->posicion.first + itTorre->ancho - 1,itTorre->posicion.second - (itTorre->alto-1));
			if(proximaPosicion.first >= itTorre->posicion.first && proximaPosicion.first <= verticeSuperiorDerecho.first && 
				proximaPosicion.second >= verticeSuperiorDerecho.second && proximaPosicion.second <= itTorre->posicion.second){
				return;
			}
		}
		//me fijo si en la proxima posicion hay algún soldado atacando
		for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
			if(itSoldado->id != unSoldado->id && itSoldado->posicion == proximaPosicion){
				return;
			}
		}
		unSoldado->posicion = proximaPosicion;
	}
}

void atacar(vector<soldado>* soldados,vector<torre>* torres){
	while(!juegoTerminado(*soldados,*torres)){
		for(vector<soldado>::iterator itSoldado = soldados->begin(); itSoldado != soldados->end(); ++itSoldado){
			intentarRecibirDanio(&(*itSoldado),*torres);
			intentarAtacarTorres(&(*itSoldado),torres);
			intentarMover(&(*itSoldado),*torres,*soldados);
		}
	}
}

void imprimirResultado(vector<soldado> soldados,vector<torre> torres){
	int ganoAzul = false;
	int ganoRojo = false;
	for(vector<torre>::iterator itTorre = torres.begin(); itTorre != torres.end(); ++itTorre){
		if(itTorre->salud <= 0 && itTorre->equipo == "Azul"){
			ganoRojo = true;
		}
		if(itTorre->salud <= 0 && itTorre->equipo == "Rojo"){
			ganoAzul = true;
		}
	}
	if(!ganoAzul && !ganoRojo){
		for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
			if(itSoldado->salud > 0 && itSoldado->equipo == "Azul"){
				ganoAzul = true;
			}
			if(itSoldado->salud > 0 && itSoldado->equipo == "Rojo"){
				ganoRojo = true;
			}
		}
	}
	if(ganoAzul){
		cout << "gano el equipo azul"<<endl;
	}
	if(ganoRojo){
		cout << "gano el equipo rojo"<<endl;
	}
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		cout <<"id:"<<itSoldado->id <<",salud:"<<itSoldado->salud<<",ciclos:"<<itSoldado->ciclos<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[]){
	vector<soldado> soldados = cargarSoldados();
	vector<torre> torres = cargarTorres();
	atacar(&soldados,&torres);
	imprimirResultado(soldados,torres);
	return 0;
}

