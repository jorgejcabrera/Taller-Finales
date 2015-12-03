//============================================================================
// Name        : Taller-Final-Rectangulos.cpp
// Author      : Jorge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>
#include <math.h>

using namespace std;

typedef struct{
  string id;
  pair<float,float> inferiorIzq;
  pair<float,float> superiorDer;
  float area;
  float distancia;
}rectangulo;

typedef struct {
  rectangulo rect1;
  rectangulo rect2;
  rectangulo rect3;
  float areaTotal;      //es el area total que queda definida por la interseccion de los 3 rectangulos
}colision;

std::vector<std::string> split(const std::string &linea, char delim) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(linea);
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

float calcularArea(rectangulo rect){
  float base = sqrt(pow(rect.superiorDer.first,2)) - sqrt(pow(rect.inferiorIzq.first,2));
  float altura = sqrt(pow(rect.superiorDer.second,2)) - sqrt(pow(rect.inferiorIzq.second,2));
  return base * altura;
}

float calcularDistancia(rectangulo rect){
  float ancho = sqrt(pow(rect.superiorDer.first,2)) - sqrt(pow(rect.inferiorIzq.first,2));
	pair<float,float> superiorIzq = make_pair(rect.superiorDer.first - ancho,rect.superiorDer.second);
  return sqrt(pow(superiorIzq.first - 0,2) + pow(superiorIzq.second - 0,2));
}

bool compararDistancias(const rectangulo &a,const rectangulo &b){
	return a.distancia < b.distancia;
}

//no estoy seguro si esto hace falta hacerlo
float calcularInterseccion(colision nuevaColision){
	return 0;
}

bool hayInterseccion(rectangulo rect1,rectangulo rect2){
  pair<float,float> rect1InfDer = make_pair(rect1.superiorDer.first, rect1.inferiorIzq.second);
  pair<float,float> rect1SupIzq = make_pair(rect1.inferiorIzq.first, rect1.superiorDer.second);
  pair<float,float> rect2SupIzq = make_pair(rect2.inferiorIzq.first, rect2.superiorDer.second);

  //rect 2 dentro de rect 1
  if((rect2SupIzq.first >= rect1.inferiorIzq.first && rect2SupIzq.first <= rect1InfDer.first) &&
	(rect2SupIzq.second >= rect1.inferiorIzq.second && rect2SupIzq.second <= rect1.superiorDer.second) ){
	  return true;
  }else if((rect2.superiorDer.first >= rect1.inferiorIzq.first && rect2.superiorDer.first <= rect1.superiorDer.first) &&
		  (rect2.superiorDer.second >= rect1.inferiorIzq.second && rect2.superiorDer.second <= rect1.superiorDer.second)){
	  return true;
  //rect 1 dentro de rect 2
  }else if((rect1SupIzq.first >= rect2.inferiorIzq.first && rect1SupIzq.first <= rect2.superiorDer.first) &&
		  (rect1SupIzq.second >= rect2.inferiorIzq.second && rect1SupIzq.second <= rect2.superiorDer.second)){
	  return true;
  }else if((rect1.superiorDer.first >= rect2.inferiorIzq.first && rect1.superiorDer.first <= rect2.superiorDer.first) &&
		  (rect1.superiorDer.second >= rect2.inferiorIzq.second && rect1.superiorDer.second <= rect2.superiorDer.second)){
	  return true;
  }
  return false;
}

list<colision> encontrarColisiones(list<rectangulo>* rectangulos){
  list<colision> colisiones;
  rectangulo rect1;
  rectangulo rect2;
  rectangulo rect3;
  bool huboColision = false;
  list<rectangulo>::iterator anteUltimoIt = --rectangulos->end();
  for(list<rectangulo>::iterator itPivote = rectangulos->begin(); std::distance(rectangulos->begin(),itPivote) <= rectangulos->size()-2; ++itPivote){
	  rect1 = *itPivote;
	  for(list<rectangulo>::iterator it = ++itPivote; it != rectangulos->end();++it){
		  //encuentro la primer colision
		  if( hayInterseccion(rect1,*it) && !huboColision){
			  rect2 = *it;
			  huboColision = true;
		  //encuentro las demás colisiones
		  }else if( hayInterseccion(rect1,*it) && huboColision && hayInterseccion(rect2,*it)){
			  rect3 = *it;
			  colision nuevaColision;
			  nuevaColision.rect1 = rect1;
			  nuevaColision.rect2 = rect2;
			  nuevaColision.rect3 = rect3;
			  nuevaColision.areaTotal = rect1.area + rect2.area + rect3.area - calcularInterseccion(nuevaColision) * 2;
			  colisiones.push_front(nuevaColision);
			  huboColision = false;
		  }
	  }
	  huboColision = false;
  }
  return colisiones;
}

void imprimirColisiones(vector<rectangulo> rectangulosColisionados){
	cout << "imprimiendo colision"<<endl;
	for(vector<rectangulo>::iterator it = rectangulosColisionados.begin(); it!=rectangulosColisionados.end();++it){
		cout << it->distancia << " " << it->area << " " << it->id<<endl;
	}
}

int main () {
  string linea;
  ifstream archivo ("../Taller-Final-Rectangulos/Source/sample.txt");
  list<rectangulo> rectangulos;

  if ( archivo.is_open() ){
  	string sX;
  	string sY;
  	string sId;
  	rectangulo rect;
  	//leemos el archivo y cargamos los datos en la lista
    while ( getline (archivo,linea) ){
    	vector<string> tokens = split(linea,' ');
    	sX = *(tokens.begin()+1);
    	sY = *(tokens.begin()+2);
    	rect.id = *tokens.begin();
    	rect.inferiorIzq= make_pair(strtof(sX.c_str(),NULL),strtof(sY.c_str(),NULL));
    	sX = *(tokens.begin()+3);
    	sY = *(tokens.begin()+4);
    	rect.superiorDer = make_pair(strtof(sX.c_str(),NULL),strtof(sY.c_str(),NULL));
    	rect.distancia = calcularDistancia(rect);
    	rect.area = calcularArea(rect);
    	rectangulos.push_front(rect);
    }

    //buscamos las colisiones e imprimimos por stdout
    list<colision> colisiones = encontrarColisiones(&rectangulos);
    for(list<colision>::iterator it = colisiones.begin(); it != colisiones.end(); ++it){
    	vector<rectangulo> rectangulosColisionados;
    	rectangulosColisionados.push_back(it->rect1);
    	rectangulosColisionados.push_back(it->rect2);
    	rectangulosColisionados.push_back(it->rect3);
    	std::sort(rectangulosColisionados.begin(),rectangulosColisionados.end(),compararDistancias);
    	imprimirColisiones(rectangulosColisionados);
    }
    archivo.close();
  }else{
	  cout << "No se pudo abrir el archivo";
  }
  return 0;
}
