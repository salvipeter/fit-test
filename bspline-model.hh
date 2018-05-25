#pragma once

#include "model.hh"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <Geom_BSplineSurface.hxx>

class BSplineModel : public Model {
public:
  BSplineModel();
  virtual ~BSplineModel();
  virtual bool open(std::string filename) override;
  virtual void draw() const override;
  const opencascade::handle<Geom_BSplineSurface> &getSurface() const;
  void setSurface(const opencascade::handle<Geom_BSplineSurface> &s);
  void toggleControlNet();
  size_t getResolution() const;
  void setResolution(size_t res);
  void setMeanMapRange(double range);
  double distance(const Vec &p) const;
  void generateMesh();

private:
  struct MyTraits : public OpenMesh::DefaultTraits {
    using Point  = OpenMesh::Vec3d;
    using Normal = OpenMesh::Vec3d;
    VertexTraits {
      double mean;
    };
  };
  using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;

  opencascade::handle<Geom_BSplineSurface> surface;
  MyMesh mesh;
  bool show_control_net;
  size_t resolution;
  double mean_map_range;
};
