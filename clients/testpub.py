import sys
import os.path

pathRessources = os.path.join('..', 'ressources')
pathModele = os.path.join('..','modele')

sys.path.append(pathModele)

import zmq
import time
from ordre_pb2 import *
from commun_pb2 import *

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.connect('tcp://127.0.0.1:4243')

for i in [1, 2, 5]:
	time.sleep(1)
	data = Ordre(
		idRobot = i,
		objectifMouvementPos = Position(
			x = 420,
			y = 4250
		),
#		objectifPinceId = 10,
#		libererPince = False
	)
	socket.send_multipart((b'ordre', data.SerializeToString()))
