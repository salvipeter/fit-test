#pragma once

#include <Geom_BSplineSurface.hxx>

namespace ReFitter {

  // Human-readable type names
  using Int           = Standard_Integer;
  using IntArray      = TColStd_Array1OfInteger;
  using Real          = Standard_Real;
  using RealArray     = TColStd_Array1OfReal;
  using Point         = gp_Pnt;
  using PointArray    = TColgp_Array1OfPnt;
  using PointMatrix   = TColgp_Array2OfPnt;
  using Vector        = gp_Vec;
  using Surface       = Geom_BSplineSurface;
  using SurfaceHandle = opencascade::handle<Surface>;

  SurfaceHandle fit(const SurfaceHandle &nominal, const PointArray &points,
                    double tolerance);

};
