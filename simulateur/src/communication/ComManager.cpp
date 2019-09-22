#include "../../include/communication/ComManager.h"

using namespace std;

ComManager::ComManager(string ip, int portPub, int portSub, string topicEtat, string topicOrdre){
	this->ip = ip;
	this->portPub = portPub;
	this->portSub = portSub;
	this->topicEtat = topicEtat;
	this->topicOrdre = topicOrdre;
	this->isInitialized = false;
}

void ComManager::Initialiser(){
	if(isInitialized){
		return;
	}
	zItems.clear();
	dicoItemToZSub.clear();
	stringstream ssAdressePub, ssAdresseSub;
	ssAdressePub << "tcp://" << ip << ":" << portPub;
	ssAdresseSub << "tcp://" << ip << ":" << portSub;
	zContext = zmq::context_t(1);
	zPub = new zmq::socket_t(zContext, ZMQ_PUB);
	zSub = new zmq::socket_t(zContext, ZMQ_SUB);
	zPub->bind(ssAdressePub.str().c_str());
	zSub->bind(ssAdresseSub.str().c_str());
	zSub->setsockopt(ZMQ_SUBSCRIBE, topicOrdre.c_str(), 0);
	zItems.push_back(zmq::pollitem_t{*zSub, 0, ZMQ_POLLIN, 0});
	dicoItemToZSub[&(zItems.back())] = zSub;
	isInitialized = true;
}

void ComManager::Ecouter(Jeu* jeu){
	mutexEcoute.unlock();
	threadEcoute = thread(ComManager::BouclerEcoute, this, jeu);
}

void ComManager::BouclerEcoute(ComManager* com, Jeu* jeu){
	while(com->isInitialized){
		com->mutexEcoute.lock();
		if(!com->isInitialized){
			break;
		}
		zmq::poll(&com->zItems[0], com->zItems.size(), -1);
		for(vector<zmq::pollitem_t>::iterator it = com->zItems.begin(); it != com->zItems.end(); ++it){
			if(!(it->revents & ZMQ_POLLIN)){
				continue;
			}
			zmq::socket_t *zSocket = (zmq::socket_t*)(&it->socket);
			if(zSocket == NULL || com->dicoItemToZSub.find(&*it) == com->dicoItemToZSub.end()){
				continue;
			}
			zmq::socket_t *zSubNow = com->dicoItemToZSub[&*it];
			zmq::message_t msgTopic, msgData;
			int hasMore;
			size_t nbMoreParts = sizeof(hasMore);
			zSocket->recv(&msgTopic, ZMQ_SNDMORE);
			zSocket->getsockopt(ZMQ_RCVMORE, &hasMore, &nbMoreParts);
			if(!hasMore){
				continue;
			}
			zSocket->recv(&msgData, 0);
			const std::string nomTopicReception(static_cast<const char*>(msgTopic.data()), msgTopic.size());
			const std::string msg(static_cast<const char*>(msgData.data()), msgData.size());
			if(zSubNow == com->zSub){
				if(nomTopicReception.compare(com->topicOrdre) == 0){
					jdd::Ordre protoOrdre;
					protoOrdre.ParseFromString(msg);
					int id = protoOrdre.idrobot();
					if(id == 0){
						continue;
					}
					if(protoOrdre.has_objectifmouvementpos()){
						jeu->GuiderRobot(id, protoOrdre.objectifmouvementpos().x(), protoOrdre.objectifmouvementpos().y());
					}
					if(protoOrdre.objectifpinceid() != 0){
						jeu->DeployerPince(id, protoOrdre.objectifpinceid());
					}
					if(protoOrdre.libererpince()){
						jeu->LibererPince(id);
					}
					if(protoOrdre.mouvementsimple() != jdd::Ordre::NOTHING){
						switch(protoOrdre.mouvementsimple()){
							case jdd::Ordre::HAUT:
								jeu->DirigerRobot(id, 0, -1); break;
							case jdd::Ordre::BAS:
								jeu->DirigerRobot(id, 0, +1); break;
							case jdd::Ordre::GAUCHE:
								jeu->DirigerRobot(id, -1, 0); break;
							case jdd::Ordre::DROITE:
								jeu->DirigerRobot(id, +1, 0); break;
							case jdd::Ordre::STOP:
								jeu->DirigerRobot(id, 0, 0); break;
							case jdd::Ordre::GRAB_SOMETHING:
								jeu->AttraperNearest(id); break;
							default:
								break;
						}
					}
				}
			}
		}
		com->mutexEcoute.unlock();
	}
}

void ComManager::Fermer(){
	if(!isInitialized){
		return;
	}
	mutexEcoute.lock();
	zPub->close();
	zSub->close();
	zContext.close();
	isInitialized = false;
	mutexEcoute.unlock();
	threadEcoute.join();
}

void ComManager::EnvoyerEtat(string msg){
	if(!isInitialized){
		return;
	}
	zmq::message_t zTopic(topicEtat.size());
	zmq::message_t zMsg(msg.size());
	memcpy(zTopic.data(), (void*) topicEtat.c_str(), topicEtat.size());
	memcpy(zMsg.data(), (void*) msg.c_str(), msg.size());	
	zPub->send(zTopic, ZMQ_SNDMORE);
	zPub->send(zMsg, 0);
}
