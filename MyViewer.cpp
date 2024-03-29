#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <QtGui/QKeyEvent>

#include "MyViewer.h"
#include "mesh-model.hh"
#include "bspline-model.hh"
#include "fitter.hh"

#ifdef _WIN32
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_BGRA 0x80E1
#endif

MyViewer::MyViewer(QWidget *parent) :
  QGLViewer(parent)
{
}

MyViewer::~MyViewer() {
  glDeleteTextures(1, &isophote_texture);
}

bool
MyViewer::openMesh(std::string filename) {
  points = std::make_unique<MeshModel>();
  if (!points->open(filename)) {
    points.reset();
    return false;
  }
  if (nominal) {
    emit startComputation("Computing deviation...");
    emit midComputation(50);
    points->updateDeviation([&](const Vec &p) { return nominal->distance(p); });
    emit endComputation();
  }
  setupCamera();
  return true;
}

bool
MyViewer::openBSpline(std::string filename) {
  nominal = std::make_unique<BSplineModel>();
  if (!nominal->open(filename)) {
    nominal.reset();
    return false;
  }
  if (points) {
    emit startComputation("Computing deviation...");
    emit midComputation(50);
    points->updateDeviation([&](const Vec &p) { return nominal->distance(p); });
    emit endComputation();
  }
  nominal->setIsophoteTexture(isophote_texture);
  setupCamera();
  return true;
}

void
MyViewer::setRanges(size_t resolution, double mean_range, double deviation_range) {
  if (points) {
    points->setDeviationRange(deviation_range);
  }
  if (nominal) {
    if (nominal->getResolution() != resolution) {
      nominal->setResolution(resolution);
      nominal->generateMesh();
    }
    nominal->setMeanMapRange(mean_range);
  }
  if (fitted) {
    if (fitted->getResolution() != resolution) {
      fitted->setResolution(resolution);
      fitted->generateMesh();
    }
    fitted->setMeanMapRange(mean_range);
  }
  update();
}

void
MyViewer::fit(double tolerance) {
  if (!nominal || !points)
    return;
  fitted = std::make_unique<BSplineModel>();
  auto surf_handle = ReFitter::fit(nominal->getSurface(), points->getPoints(), tolerance);
  fitted->setSurface(surf_handle);
  update();
}

void MyViewer::setupCamera() {
  Vec min, max;
  if (nominal)
    nominal->boundingBox(min, max);
  else if (points)
    points->boundingBox(min, max);
  else
    return;
  camera()->setSceneBoundingBox(min, max);
  camera()->showEntireScene();
  update();
}

void MyViewer::init() {
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  QImage img(":/isophotes.png");
  glGenTextures(1, &isophote_texture);
  glBindTexture(GL_TEXTURE_2D, isophote_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, img.convertToFormat(QImage::Format_ARGB32).bits());
}

void MyViewer::draw() {
  if (points)
    points->draw();
  if (nominal)
    nominal->draw();
  if (fitted)
    fitted->draw();
}

void MyViewer::keyPressEvent(QKeyEvent *e) {
  Model *current = nullptr;
  if (e->modifiers() == Qt::NoModifier)
    current = nominal.get();
  else if (e->modifiers() == Qt::ShiftModifier)
    current = fitted.get();
  else if (e->modifiers() == Qt::AltModifier)
    current = points.get();
  if (current)
    switch (e->key()) {
    case Qt::Key_P:
      current->setVisualization(Model::Visualization::PLAIN);
      update();
      break;
    case Qt::Key_M:
      current->setVisualization(Model::Visualization::MEAN);
      update();
      break;
    case Qt::Key_I:
      current->setVisualization(Model::Visualization::ISOPHOTES);
      update();
      break;
    case Qt::Key_D:
      current->setVisualization(Model::Visualization::DEVIATION);
      update();
      break;
    case Qt::Key_C:
      {
        BSplineModel *bsp = dynamic_cast<BSplineModel *>(current);
        if (bsp) {
          bsp->toggleControlNet();
          update();
        }
      }
      break;
    case Qt::Key_S:
      current->toggleSolid();
      update();
      break;
    case Qt::Key_W:
      current->toggleWireframe();
      update();
      break;
    default:
      QGLViewer::keyPressEvent(e);
    }
  else
    QGLViewer::keyPressEvent(e);
}
