#ifndef ROBOT_H
#define ROBOT_H

#include <string>

#include "params.h"
#include "Entite.h"
#include "Tresor.h"

#include "../../../modele/etat.pb.h"

class Robot : public Entite{

public:
	Robot();
	Robot(int id, std::string nom, int x, int y, jdd::Robot* proto);
	void UpdateProto();
	
	void MouvementDirect(int sensX, int sensY);
	void AssignerObjectif(int x, int y);
	void Attraper(Tresor* cible);
	void Relacher();
	
	void ExeMouvement(float dt);
	void ExePince(float dt);
		
	std::string GetNom();
	Tresor* GetCharge();

private:
	std::string nom;
	Tresor* charge;
	bool pinceDeployee;
	bool pinceRelacher;
	int xPince, yPince;
	int xGoal, yGoal;
	Tresor* pinceGoal;
	jdd::Robot* proto;
	
	void Retracter(float dt);
};

#endif
