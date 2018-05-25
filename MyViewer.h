// -*- mode: c++ -*-
#pragma once

#include <memory>
#include <string>

#include <QGLViewer/qglviewer.h>

class MeshModel;
class BSplineModel;

class MyViewer : public QGLViewer {
  Q_OBJECT

public:
  explicit MyViewer(QWidget *parent);
  virtual ~MyViewer();

  bool openMesh(std::string filename);
  bool openBSpline(std::string filename);
  void setRanges(size_t resolution, double mean_range, double deviation_range);
  void fit();

signals:
  void startComputation(QString message);
  void midComputation(int percent);
  void endComputation();

protected:
  virtual void init() override;
  virtual void draw() override;
  virtual void keyPressEvent(QKeyEvent *e) override;

private:
  void setupCamera();

  std::unique_ptr<MeshModel> points;
  std::unique_ptr<BSplineModel> nominal, fitted;
  GLuint isophote_texture;
};
