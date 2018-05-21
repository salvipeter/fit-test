# -*- mode: Makefile -*-

TARGET = fit-test
CONFIG *= c++14 qt opengl debug
QT += gui widgets opengl xml

HEADERS = MyWindow.h MyViewer.h MyViewer.hpp
SOURCES = MyWindow.cpp MyViewer.cpp main.cpp

LIBS *= -lQGLViewer -L/usr/lib/OpenMesh -lOpenMeshCored -lGL -lGLU

RESOURCES = fit-test.qrc
