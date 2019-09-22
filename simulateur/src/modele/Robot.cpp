#include "../../include/modele/Robot.h"

using namespace std;

Robot::Robot() : Robot(-1, "undefined", 0, 0, NULL){

}

Robot::Robot(int id, string nom, int x, int y, jdd::Robot* proto): Entite(id, x, y){
	this->nom = nom;
	this->charge = NULL;
	this->proto = proto;
	this->pinceDeployee = false;
	this->xPince = x;
	this->yPince = y;
	this->xGoal = x;
	this->yGoal = y;
	this->pinceGoal = NULL;
	this->pinceRelacher = false;
	if(proto == NULL){
		return;
	}
	proto->set_id(id);
	proto->set_nom(nom);
	UpdateProto();
}

void Robot::MouvementDirect(int sensX, int sensY){
	xGoal = (sensX == 0 ? x : (sensX > 0 ? X_MAX : 0));
	yGoal = (sensY == 0 ? y : (sensY > 0 ? Y_MAX : 0));
}

void Robot::AssignerObjectif(int x, int y){
	xGoal = x;
	yGoal = y;
}

void Robot::Attraper(Tresor* cible){
	pinceGoal = cible;
}

void Robot::Relacher(){
	pinceRelacher = true;
}

void Robot::ExeMouvement(float dt){
	if(xPince != x or yPince != y){
		return;
	}
	if(xGoal == x and yGoal == y){
		return;
	}
	float portee = dt * V_MAX;
	float distance = DistanceTo(xGoal, yGoal);
	if(distance <= portee){
		x = xGoal;
		y = yGoal;
	}
	float ratio = portee / distance;
	float dx = (float) (xGoal - x) * ratio;
	float dy = (float) (yGoal - y) * ratio;
	x += (int) trunc(dx);
	y += (int) trunc(dy);
	xPince = x;
	yPince = y;
}

void Robot::ExePince(float dt){
	if(pinceRelacher){
		pinceRelacher = false;
		if(charge != NULL){
			charge->Deplacer(x, y, NULL);
			charge = NULL;
		}
	}
	if(charge != NULL){
		Retracter(dt);
	}
	else if(pinceGoal != NULL){
		if(pinceGoal->IsCarried()){
			Retracter(dt);
		}
		else{
			float distanceRobotTresor = DistanceTo(pinceGoal);
			if(distanceRobotTresor <= PORTEE_PINCE){
				pinceGoal = NULL;
				Retracter(dt);
			}
			else{
				float distancePinceTresor = pinceGoal->DistanceTo(xPince, yPince);
				float portee = dt * V_PINCE_MAX;
				if(distancePinceTresor <= portee){
					xPince = pinceGoal->GetX();
					yPince = pinceGoal->GetY();
					pinceGoal->Deplacer(this);
					charge = pinceGoal;
				}
				else{
					float ratio = portee / distancePinceTresor;
					float dx = (float) (xGoal - xPince) * ratio;
					float dy = (float) (yGoal - yPince) * ratio;
					xPince += (int) trunc(dx);
					yPince += (int) trunc(dy);
				}
				pinceGoal = NULL;
			}
		}
		
	}
	else{
		xPince = x;
		yPince = y;
		if(charge != NULL){
			charge->Deplacer(x, y, (Entite*) this);
		}
	}
}

void Robot::Retracter(float dt){
	pinceGoal = NULL;
	float distanceRobotPince = DistanceTo(xPince, yPince);
	float portee = dt * V_PINCE_MAX;
	if(distanceRobotPince <= portee){
		xPince = x;
		yPince = y;
	}
	else{
		float ratio = portee / distanceRobotPince;
		float dx = (float) (xPince - x) * ratio;
		float dy = (float) (yPince - y) * ratio;
		xPince += (int) trunc(dx);
		yPince += (int) trunc(dy);
	}
}

void Robot::UpdateProto(){
	proto->mutable_pos()->set_x(x);
	proto->mutable_pos()->set_y(y);
	proto->set_pincedeployee(pinceDeployee);
	proto->mutable_pince()->set_x(xPince);
	proto->mutable_pince()->set_y(yPince);
}

string Robot::GetNom(){
	return nom;
}

Tresor* Robot::GetCharge(){
	return charge;
}
