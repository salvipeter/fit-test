# -*- mode: Makefile -*-

TARGET = fit-test
CONFIG = c++14 qt opengl debug
QT = gui widgets opengl xml

HEADERS = MyWindow.h MyViewer.h model.hh mesh-model.hh bspline-model.hh
SOURCES = MyWindow.cpp MyViewer.cpp main.cpp model.cc mesh-model.cc bspline-model.cc

OPENCASCADE = ../opencascade-7.2.0/build

INCLUDEPATH += $$OPENCASCADE/include/opencascade

LIBS += -lQGLViewer -L/usr/lib/OpenMesh -lOpenMeshCored -lGL -lGLU \
        -L$$OPENCASCADE/lin32/gcc/libd -lTKernel -lTKG3d -lTKGeomAlgo

RESOURCES = fit-test.qrc
