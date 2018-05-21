#pragma once

#include <QGLViewer/qglviewer.h>

using qglviewer::Vec;

class Model {
public:
  enum class Visualization { PLAIN, MEAN, ISOPHOTES, DEVIATION };

  Model();
  virtual ~Model();
  virtual bool open(std::string filename) = 0;
  virtual void draw() const = 0;
  void boundingBox(Vec &min, Vec &max) const;
  void setVisualization(Visualization vis);
  void setIsophoteTexture(GLuint &id);
  void toggleSolid();
  void toggleWireframe();

protected:
  static Vec blendColor(double range, double x);

  Vec box_min, box_max;
  Visualization visualization;
  bool show_solid, show_wireframe;
  GLuint *isophote_texture;
};
