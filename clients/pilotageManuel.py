import sys, time
import os.path

pathRessources = os.path.join('..', 'ressources')
pathModele = os.path.join('..','modele')

sys.path.append(pathModele)

import zmq
import pygame
from pygame.locals import *
from ordre_pb2 import *
from commun_pb2 import *

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.connect('tcp://127.0.0.1:4243')

idRobot = sys.argv[1]
print(idRobot)

pygame.init()
fenetre = pygame.display.set_mode((100, 100))

while True:
	for event in pygame.event.get():
		if event.type == QUIT:
			break
		data = Ordre(idRobot = int(idRobot))
		if event.type == KEYDOWN:
			if event.key == K_UP:
				data.mouvementSimple = Ordre.ZSQD.HAUT
			elif event.key == K_DOWN:
				data.mouvementSimple = Ordre.ZSQD.BAS
			elif event.key == K_LEFT:
				data.mouvementSimple = Ordre.ZSQD.GAUCHE
			elif event.key == K_RIGHT:
				data.mouvementSimple = Ordre.ZSQD.DROITE
			elif event.key == K_SPACE:
				data.mouvementSimple = Ordre.ZSQD.STOP
			elif event.key == K_LCTRL or event.key == K_RCTRL:
				data.mouvementSimple = Ordre.ZSQD.GRAB_SOMETHING
			elif event.key == K_TAB:
				data.libererPince = True
			else:
				continue
			socket.send_multipart((b'ordre', data.SerializeToString()))

socket.close()
context.close()
