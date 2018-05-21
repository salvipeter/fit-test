#pragma once

#include "model.hh"

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

class MeshModel : public Model {
public:
  MeshModel();
  virtual ~MeshModel();
  virtual bool open(std::string filename) override;
  virtual void draw() const override;
  void setDeviationFn(double (*fn)(const Vec &p));
  void setDeviationRange(double range);
  void updateDeviation();

private:
  struct MyTraits : public OpenMesh::DefaultTraits {
    VertexTraits {
      double deviation;
    };
  };
  using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;

  MyMesh mesh;
  double (*deviation)(const Vec &p);
  double deviation_range;
};
