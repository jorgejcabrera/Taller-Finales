//============================================================================
// Name        : Taller-Final-1.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

//el mapa en este caso es de 8 x 14

using namespace std;

typedef struct{
	string id;
	pair<float,float> posicion;
	int salud;
	int ataque;
	string movimientos;
	string equipo;
	vector<pair<float,float> > rangoDeAtaque;
}soldado;

vector<string> tokenize(string linea,char delim){
	vector<string> tokens;
	string token;
	stringstream ss(linea);
	while(getline(ss,token,delim)){
		tokens.push_back(token);
	}
	return tokens;
}

void actualizarRangoDeAtaque(soldado* unSoldado){
	unSoldado->rangoDeAtaque.clear();
	//norte
	if(unSoldado->posicion.second-1 >= 0){
		unSoldado->rangoDeAtaque.push_back(make_pair(unSoldado->posicion.first,unSoldado->posicion.second-1));
	}
	//oeste
	if(unSoldado->posicion.first-1 >= 0){
		unSoldado->rangoDeAtaque.push_back(make_pair(unSoldado->posicion.first-1,unSoldado->posicion.second));
	}
	//sur
	if(unSoldado->posicion.second+1 <= 14){
		unSoldado->rangoDeAtaque.push_back(make_pair(unSoldado->posicion.first,unSoldado->posicion.second+1));
	}
	//este
	if(unSoldado->posicion.first+1 <= 7){
		unSoldado->rangoDeAtaque.push_back(make_pair(unSoldado->posicion.first+1,unSoldado->posicion.second));
	}
}

vector<soldado> cargarSoldados(){
	vector<soldado> soldados;
	ifstream archivo("/home/jorge/Taller-Finales/Soldados combatiendo/Source/soldados.txt");
	if(archivo.is_open()){
		string linea;
		soldado unSoldado;
		while(getline(archivo,linea)){
			vector<string> tokens = tokenize(linea,' ');
			unSoldado.id = *tokens.begin();
			unSoldado.posicion = make_pair(atof((*(tokens.begin()+1)).c_str()),atof((*(tokens.begin()+2)).c_str()));
			unSoldado.salud = atoi((*(tokens.begin()+3)).c_str());
			unSoldado.ataque = atoi((*(tokens.begin()+4)).c_str());
			unSoldado.movimientos = *(tokens.begin()+5);
			unSoldado.equipo = *(tokens.begin()+6);
			actualizarRangoDeAtaque(&unSoldado);
			soldados.push_back(unSoldado);
		}
	}else{
		cout << "el archivo de soldados no se pudo abrir de manera correcta"<<endl;
	}
	return soldados;
}

bool soldadoLuchando(soldado unSoldado,vector<soldado> soldados){
	for(vector<soldado>::iterator itEnemigo = soldados.begin(); itEnemigo != soldados.end(); ++itEnemigo){
		if(itEnemigo->id != unSoldado.id && itEnemigo->equipo != unSoldado.equipo && itEnemigo->salud > 0){
			for(vector<pair<float,float> >::iterator itRango = itEnemigo->rangoDeAtaque.begin(); itRango != itEnemigo->rangoDeAtaque.end(); ++itRango){
				if(unSoldado.posicion == *itRango){
					return true;
				}
			}
		}
	}
	return false;
}

bool juegoTerminado(vector<soldado> soldados){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud > 0){
			//si le quedan movimientos por realizar el juego no termina
			if(!itSoldado->movimientos.empty()){
				return false;
			//si no le quedan movimientos por realizar puede ser que esté peleando con alguien
			}else if(soldadoLuchando(*itSoldado,soldados)){
				return false;
			}
		}
	}
	return true;
}

void intentarAtacar(soldado* unSoldado,vector<soldado>* soldados){
	for(vector<soldado>::iterator itEnemigo = soldados->begin(); itEnemigo != soldados->end(); ++itEnemigo){
		if(itEnemigo->id != unSoldado->id && itEnemigo->equipo != unSoldado->equipo && unSoldado->salud > 0 && itEnemigo->salud > 0){
			for(vector<pair<float,float> >::iterator itRango = unSoldado->rangoDeAtaque.begin(); itRango != unSoldado->rangoDeAtaque.end();++itRango){
				//atacamos
				if(itEnemigo->posicion == *itRango){
					unSoldado->movimientos.clear();
					itEnemigo->movimientos.clear();
					if(unSoldado->ataque >= itEnemigo->salud){
						itEnemigo->salud = 0;
					}else{
						itEnemigo->salud -= unSoldado->ataque;
					}
				}
			}
		}
	}
}

void intentarMoverse(soldado* unSoldado,vector<soldado> soldados){
	if(unSoldado->salud > 0 && !unSoldado->movimientos.empty()){
		char proximoMovimiento = *unSoldado->movimientos.begin();
		unSoldado->movimientos.erase(unSoldado->movimientos.begin());
		pair<float,float> proximaPosicion = unSoldado->posicion;
		if(proximoMovimiento == 'U' && proximaPosicion.second-1 >= 0 ){
			proximaPosicion.second--;
		}else if(proximoMovimiento == 'L' && proximaPosicion.first-1 >= 0){
			proximaPosicion.first--;
		}else if(proximoMovimiento == 'D' && proximaPosicion.second+1 <= 14){
			proximaPosicion.second++;
		}else if(proximaPosicion.first+1 <= 8){
			proximaPosicion.first++;
		}
		unSoldado->posicion = proximaPosicion;
		actualizarRangoDeAtaque(unSoldado);
	}
}

void atacar(vector<soldado>* soldados){
	while(!juegoTerminado(*soldados)){
		for(vector<soldado>::iterator itSoldado = soldados->begin(); itSoldado != soldados->end(); ++itSoldado){
			intentarAtacar(&(*itSoldado),soldados);
			intentarMoverse(&(*itSoldado),*soldados);
		}
	}
}

void imprimirResultados(vector<soldado> soldados){
	for(vector<soldado>::iterator itSoldado = soldados.begin(); itSoldado != soldados.end(); ++itSoldado){
		if(itSoldado->salud <= 0){
			cout << itSoldado->id<<endl;
		}
	}
}
int main() {
	vector<soldado> soldados = cargarSoldados();
	atacar(&soldados);
	imprimirResultados(soldados);
	return 0;
}
