import sys
import os.path

pathRessources = os.path.join('..', 'ressources')
pathModele = os.path.join('..','modele')

sys.path.append(pathModele)

import zmq
import pygame
from pygame.locals import *
from etat_pb2 import *
from commun_pb2 import *

COORD_MAX = (10000, 10000)
SIZE_ARENE = (1000, 1000)
SIZE_ROBOT = 20

class Ihm:

	def __init__(self, ip, port, topic):
		self.initialiserAffichage()
		self.fond = self.chargerRessource('fond.png', SIZE_ARENE)
		self.bVerte = self.chargerRessource('bVerte.png', (SIZE_ROBOT, SIZE_ROBOT))
		self.bRouge = self.chargerRessource('bRouge.png', (SIZE_ROBOT, SIZE_ROBOT))
		self.bBlanche = self.chargerRessource('bBlanche.png', (SIZE_ROBOT, SIZE_ROBOT))
		self.initialiserTopic(ip, port, topic)
	
	def initialiserAffichage(self):
		pygame.init()
		self.fenetre = pygame.display.set_mode(SIZE_ARENE)
		self.ratioX = float(SIZE_ARENE[0]) / float(COORD_MAX[0])
		self.ratioY = float(SIZE_ARENE[1]) / float(COORD_MAX[1])
		
	def initialiserTopic(self, ip, port, topic):
		self.context = zmq.Context()
		self.socket = self.context.socket(zmq.SUB)
		self.socket.connect('tcp://%s:%s' % (ip, str(port)))
		self.socket.setsockopt(zmq.SUBSCRIBE, topic)

	def chargerRessource(self, nom, size):
		image = pygame.image.load(os.path.join(pathRessources, nom)).convert()
		return pygame.transform.scale(image, size)

	def positionnerImage(self, image, x, y):
		xCorner = int(self.ratioX * x) - image.get_width() / 2
		yCorner = int(self.ratioY * y) - image.get_height() / 2
		self.fenetre.blit(image, (xCorner, yCorner))

	def majAffichage(self, data):
		self.fenetre.blit(self.fond, (0,0))
		if data is None:
			pygame.display.flip()
			return
		for equipe in data.equipes:
			if equipe.camp == Camp.VERT:
				image = self.bVerte
			elif equipe.camp == Camp.ROUGE:
				image = self.bRouge
			else:
				continue
			for robot in equipe.membres:
				self.positionnerImage(image, robot.pos.x, robot.pos.y)
				pygame.draw.line(self.fenetre, (255, 255, 255), (robot.pos.x, robot.pos.y), (robot.pince.x, robot.pince.y))
		for tresor in data.tresors:
			self.positionnerImage(self.bBlanche, tresor.pos.x, tresor.pos.y)
		pygame.display.flip()

	def job(self):
		while True:
			for event in pygame.event.get():
				if event.type == QUIT:
					self.socket.close()
					self.context.close()
					break
			topic, msg = self.socket.recv_multipart()
			data = Jeu()
			data.ParseFromString(msg)
			self.majAffichage(data)

ihm = Ihm('127.0.0.1', 4242, b'etat')
ihm.job()
