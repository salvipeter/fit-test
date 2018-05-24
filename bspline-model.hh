#pragma once

#include "model.hh"

#include <memory>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

class Geom_BSplineSurface;

class BSplineModel : public Model {
public:
  BSplineModel();
  virtual ~BSplineModel();
  virtual bool open(std::string filename) override;
  virtual void draw() const override;
  void toggleControlNet();
  void setMeanMapRange(double range);
  double distance(const Vec &p) const;

private:
  struct MyTraits : public OpenMesh::DefaultTraits {
    using Point  = OpenMesh::Vec3d;
    using Normal = OpenMesh::Vec3d;
    VertexTraits {
      double mean;
    };
  };
  using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;

  void generateMesh();

  std::unique_ptr<Geom_BSplineSurface> surface;
  MyMesh mesh;
  bool show_control_net;
  double mean_map_range;
};
