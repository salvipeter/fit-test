#pragma once

#include "model.hh"

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <TColgp_Array1OfPnt.hxx>

class MeshModel : public Model {
public:
  MeshModel();
  virtual ~MeshModel();
  virtual bool open(std::string filename) override;
  virtual void draw() const override;
  TColgp_Array1OfPnt getPoints() const;
  void setDeviationRange(double range);
  void updateDeviation(const std::function<double(const Vec &p)> &fn);

private:
  struct MyTraits : public OpenMesh::DefaultTraits {
    VertexTraits {
      double deviation;
    };
  };
  using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;

  MyMesh mesh;
  std::function<double(const Vec &p)> deviation;
  double deviation_range;
};
