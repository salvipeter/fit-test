# -*- mode: Makefile -*-

TARGET = fit-test
CONFIG = c++14 qt opengl debug
QT = gui widgets opengl xml

HEADERS = MyWindow.h MyViewer.h model.hh mesh-model.hh bspline-model.hh options-window.hh \
          fitter.hh
SOURCES = MyWindow.cpp MyViewer.cpp main.cpp model.cc mesh-model.cc bspline-model.cc \
          options-window.cc fitter.cc

OPENCASCADE = /home/salvi/project/opencascade-7.2.0/build

INCLUDEPATH += $$OPENCASCADE/include/opencascade

LIBS += -lQGLViewer-qt5 -L/usr/lib/OpenMesh -lOpenMeshCored -lGL -lGLU \
        -L$$OPENCASCADE/lin64/gcc/libd -lTKernel -lTKG3d -lTKGeomBase -lTKGeomAlgo -lTKMath

RESOURCES = fit-test.qrc
