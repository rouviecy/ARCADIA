#ifndef ENTITE_H
#define ENTITE_H

#include <cmath>

class Entite{

public:
	Entite();
	Entite(int id, int x, int y);
	int GetId();
	int GetX();
	int GetY();
	float DistanceTo(Entite* cible);
	float DistanceTo(int x, int y);

protected:
	int id;
	int x;
	int y;

private:

};

#endif
