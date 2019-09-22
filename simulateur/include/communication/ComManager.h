#ifndef COMMANAGER_H
#define COMMANAGER_H

#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

#include <zmq.hpp>

#include "../modele/Jeu.h" // a supprimer
#include "../../../modele/ordre.pb.h" // a supprimer

class ComManager{

public:
	ComManager(std::string ip, int portPub, int portSub, std::string topicEtat, std::string topicOrdre);
	void Initialiser();
	void Ecouter(Jeu* jeu);
	void EnvoyerEtat(std::string msg);
	void Fermer();

private:
	zmq::context_t zContext;
	zmq::socket_t *zPub, *zSub;
	std::vector<zmq::pollitem_t> zItems;
	std::map<zmq::pollitem_t*, zmq::socket_t*> dicoItemToZSub;
	std::string ip;
	int portPub, portSub;
	std::string topicEtat, topicOrdre;
	bool isInitialized;
	static void BouclerEcoute(ComManager* com, Jeu* jeu);
	std::thread threadEcoute;
	std::mutex mutexEcoute;
};

#endif
