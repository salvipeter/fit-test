#include "model.hh"

Model::Model() :
  visualization(Visualization::PLAIN), show_solid(true), show_wireframe(false)
{
}

Model::~Model() {
}

void
Model::boundingBox(Vec &min, Vec &max) const {
  min = box_min;
  max = box_max;
}

void
Model::setVisualization(Visualization vis) {
  visualization = vis;
}

void
Model::setIsophoteTexture(GLuint &id) {
  isophote_texture = &id;
}

void
Model::toggleSolid() {
  show_solid = !show_solid;
}

void
Model::toggleWireframe() {
  show_wireframe = !show_wireframe;
}

Vec
Model::blendColor(double range, double x) {
  static const Vec red(1,0,0), green(0,1,0), blue(0,0,1);
  if (range == 0.0)
    return x < 0 ? blue : red;
  if (x < 0) {
    double alpha = std::min(x / (-range), 1.0);
    return green * (1 - alpha) + blue * alpha;
  }
  double alpha = std::min(x / range, 1.0);
  return green * (1 - alpha) + red * alpha;
}
