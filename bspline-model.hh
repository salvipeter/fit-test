#pragma once

#include "model.hh"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

class BSplineModel : public Model {
public:
  BSplineModel();
  virtual ~BSplineModel();
  virtual bool open(std::string filename) override;
  virtual void draw() const override;
  void toggleControlNet();
  void setMeanMapRange(double range);

private:
  struct MyTraits : public OpenMesh::DefaultTraits {
    VertexTraits {
      double mean;
    };
  };
  using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;

  void generateMesh();

  MyMesh mesh;
  bool show_control_net;
  double mean_map_range;
};
