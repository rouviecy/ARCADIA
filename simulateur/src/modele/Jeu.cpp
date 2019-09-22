#include "../../include/modele/Jeu.h"

using namespace std;

Jeu::Jeu(){
	t = 0;
	scoreRouge = 0;
	scoreVert = 0;
	rouges.clear();
	verts.clear();
	proto = jdd::Jeu();
	protoVerts = proto.add_equipes();
	protoVerts->set_camp(jdd::Camp::VERT);
	protoVerts->set_score(0);
	protoRouges = proto.add_equipes();
	protoRouges->set_camp(jdd::Camp::ROUGE);
	protoRouges->set_score(0);
}

void Jeu::Initialiser(){
	AddVert(5, "toto", 0, 0);
	AddVert(2, "tutu", 1000, 1000);
	AddRouge(1, "tata", 2000, 2000);
	AddTresor(10, 420, 5000);
}

void Jeu::AddVert(int id, string nom, int x, int y){
	jdd::Robot* protoNouveau = protoVerts->add_membres();
	verts[id] = Robot(id, nom, x, y, protoNouveau);
	allBots[id] = verts[id];
}

void Jeu::AddRouge(int id, string nom, int x, int y){
	jdd::Robot* protoNouveau = protoRouges->add_membres();
	rouges[id] = Robot(id, nom, x, y, protoNouveau);
	allBots[id] = rouges[id];
}

void Jeu::AddTresor(int id, int x, int y){
	jdd::Tresor* protoNouveau = proto.add_tresors();
	tresors[id] = Tresor(id, x, y, protoNouveau);
}

void Jeu::Avancer(float dt){
	t += dt;
	for(map<int, Robot>::iterator it = allBots.begin(); it != allBots.end(); ++it){
		it->second.ExePince(dt);
	}
	for(map<int, Robot>::iterator it = allBots.begin(); it != allBots.end(); ++it){
		it->second.ExeMouvement(dt);
	}
}

void Jeu::DirigerRobot(int idRobot, int sensX, int sensY){
	if(allBots.find(idRobot) == allBots.end()){
		return;
	}
	allBots[idRobot].MouvementDirect(sensX, sensY);
}

void Jeu::AttraperNearest(int idRobot){
	//TODO
}

void Jeu::GuiderRobot(int idRobot, int xGoal, int yGoal){
	if(allBots.find(idRobot) == allBots.end()){
		return;
	}
	if(xGoal < 0){xGoal = 0;}
	else if(xGoal > X_MAX){xGoal = X_MAX;}
	if(yGoal < 0){yGoal = 0;}
	else if(yGoal > Y_MAX){yGoal = Y_MAX;}
	allBots[idRobot].AssignerObjectif(xGoal, yGoal);
}

void Jeu::DeployerPince(int idRobot, int idTresor){
	if(allBots.find(idRobot) == allBots.end()){
		return;
	}
	if(tresors.find(idTresor) == tresors.end()){
		return;
	}
	allBots[idRobot].Attraper(&(tresors[idTresor]));
}

void Jeu::LibererPince(int idRobot){
	if(allBots.find(idRobot) == allBots.end()){
		return;
	}
	allBots[idRobot].Relacher();
}

void Jeu::UpdateProto(){
	for(map<int, Robot>::iterator it = allBots.begin(); it != allBots.end(); ++it){
		it->second.UpdateProto();
	}
	for(map<int, Tresor>::iterator it = tresors.begin(); it != tresors.end(); ++it){
		it->second.UpdateProto();
	}
}

jdd::Jeu* Jeu::GetProto(){
	return &proto;
}
