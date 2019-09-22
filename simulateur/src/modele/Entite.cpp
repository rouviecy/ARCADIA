#include "../../include/modele/Entite.h"

using namespace std;

Entite::Entite(){
	Entite(-1, 0, 0);
}

Entite::Entite(int id, int x, int y){
	this->id = id;
	this->x = x;
	this->y = y;
}

int Entite::GetId(){
	return id;
}

int Entite::GetX(){
	return x;
}

int Entite::GetY(){
	return y;
}

float Entite::DistanceTo(int x, int y){
	int dx = x - this->x;
	int dy = y - this->y;
	return sqrt((float) (dx * dx + dy * dy));
}

float Entite::DistanceTo(Entite* cible){
	return DistanceTo(cible->x, cible->y);
}
