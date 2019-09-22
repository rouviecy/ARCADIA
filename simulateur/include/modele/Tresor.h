#ifndef TRESOR_H
#define TRESOR_H

#include "Entite.h"

#include "../../../modele/etat.pb.h"

class Tresor : public Entite{

public:
	Tresor();
	Tresor(int id, int x, int y, jdd::Tresor* proto);
	void UpdateProto();
	bool IsCarried();
	void Deplacer(Entite* porteur);
	void Deplacer(int x, int y, Entite* porteur);

private:
	Entite* porteur;
	jdd::Tresor* proto;
};

#endif
