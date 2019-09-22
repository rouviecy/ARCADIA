#include <iostream>
#include <cmath>
#include <unistd.h> // à remplacer par timer

#include "../include/modele/params.h"
#include "../include/modele/Jeu.h"
#include "../include/communication/ComManager.h"

#include "../../modele/etat.pb.h"
#include "../../modele/ordre.pb.h"

using namespace std;

int main(int argc, char *argv[]){
	Jeu jeu;
	ComManager com("*", 4242, 4243, "etat", "ordre");
	jeu.Initialiser();
	com.Initialiser();
	jdd::Jeu* proto = jeu.GetProto();
	jeu.UpdateProto();
	com.Ecouter(&jeu);
	int nbStepBetweenPub = (int) round(DT_PUB / DT_SIMU);
	int microSecondsBetweenStep = (int) (1000000. * DT_SIMU);
	for(int i = 1; true; i++){
		jeu.Avancer(DT_SIMU);
		if(i == nbStepBetweenPub){
			string msg;
			jeu.UpdateProto();
			proto->SerializeToString(&msg);
			com.EnvoyerEtat(msg);
			i = 0;
		}
		usleep(microSecondsBetweenStep);
	}
	
	com.Fermer();

}
