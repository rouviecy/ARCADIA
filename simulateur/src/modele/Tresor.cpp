#include "../../include/modele/Tresor.h"

using namespace std;

Tresor::Tresor() : Tresor(-1, 0, 0, NULL){

}

Tresor::Tresor(int id, int x, int y, jdd::Tresor* proto) : Entite(id, x, y){
	this->proto = proto;
	this->porteur = NULL;
	if(proto == NULL){
		return;
	}
	proto->set_id(id);
	UpdateProto();
}

bool Tresor::IsCarried(){
	return porteur != NULL;
}

void Tresor::Deplacer(Entite* porteur){
	this->porteur = porteur;
}

void Tresor::Deplacer(int x, int y, Entite* porteur){
	this->x = x;
	this->y = y;
	Deplacer(porteur);
}

void Tresor::UpdateProto(){
	proto->mutable_pos()->set_x(x);
	proto->mutable_pos()->set_y(y);
}
