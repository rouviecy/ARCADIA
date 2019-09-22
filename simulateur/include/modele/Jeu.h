#ifndef JEU_H
#define JEU_H

#include <map>
#include <vector>

#include "params.h"
#include "Robot.h"
#include "Tresor.h"

#include "../../../modele/etat.pb.h"

class Jeu{

public:
	Jeu();
	void Initialiser();
	void UpdateProto();

	void Avancer(float dt);

	void DirigerRobot(int idRobot, int sensX, int sensY);
	void AttraperNearest(int idRobot);
	void GuiderRobot(int idRobot, int xGoal, int yGoal);
	void DeployerPince(int idRobot, int idTresor);
	void LibererPince(int idRobot);

	jdd::Jeu* GetProto();

private:
	std::map<int, Robot> verts, rouges, allBots;
	std::map<int, Tresor> tresors;
	int scoreVert, scoreRouge;
	float t;
	jdd::Jeu proto;
	jdd::Equipe *protoVerts, *protoRouges;
	void AddVert(int id, std::string nom, int x, int y);
	void AddRouge(int id, std::string nom, int x, int y);
	void AddTresor(int id, int x, int y);
};

#endif
