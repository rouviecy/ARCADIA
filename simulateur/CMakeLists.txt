cmake_minimum_required(VERSION 2.8)
project(jdd)

find_package(PkgConfig)
find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
	NAMES zmq.hpp
	PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
)

find_library(ZeroMQ_LIBRARY
	NAMES zmq
	PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
)

add_library(jdd_LIBS
	../modele/commun.pb.h				../modele/commun.pb.cc
	../modele/etat.pb.h					../modele/etat.pb.cc
	../modele/ordre.pb.h				../modele/ordre.pb.cc
	include/communication/ComManager.h	src/communication/ComManager.cpp
	include/modele/Jeu.h				src/modele/Jeu.cpp
	include/modele/Entite.h				src/modele/Entite.cpp
	include/modele/Robot.h				src/modele/Robot.cpp
	include/modele/Tresor.h				src/modele/Tresor.cpp
	include/modele/params.h
)

add_executable(arena src/arena.cpp)

target_include_directories(arena PUBLIC PUBLIC ${ZeroMQ_INCLUDE_DIR})

target_link_libraries(arena jdd_LIBS ${ZeroMQ_LIBRARY} ${Protobuf_LIBRARIES})

