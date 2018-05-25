#include "fitter.hh"

namespace ReFitter {

  SurfaceHandle fit(const SurfaceHandle &nominal, const PointArray &points,
                    double tolerance) {
    // TODO
    PointMatrix cnet(1, 1, 1, 1);
    RealArray knots_u, knots_v;
    IntArray mult_u, mult_v;
    Int deg_u, deg_v;
    return new Geom_BSplineSurface(cnet,
                                   knots_u, knots_v, mult_u, mult_v,
                                   deg_u, deg_v);
  }

};
