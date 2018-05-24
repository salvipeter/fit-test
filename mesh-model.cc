#include "mesh-model.hh"

MeshModel::MeshModel() :
  deviation_range(0.0)
{
}

MeshModel::~MeshModel() {
}

bool
MeshModel::open(std::string filename) {
  if (!OpenMesh::IO::read_mesh(mesh, filename) || mesh.n_vertices() == 0)
    return false;

  auto min = mesh.point(*mesh.vertices_begin()), max = min;
  for (auto v : mesh.vertices()) {
    min.minimize(mesh.point(v));
    max.maximize(mesh.point(v));
  }
  box_min = Vec(min.data());
  box_max = Vec(max.data());

  mesh.request_vertex_normals();
  mesh.request_face_normals();
  mesh.update_normals();        // needs face normals
  mesh.release_face_normals();

  return true;
}

void
MeshModel::draw() const {
  glPolygonMode(GL_FRONT_AND_BACK, !show_solid && show_wireframe ? GL_LINE : GL_FILL);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1, 1);

  if (visualization == Visualization::DEVIATION)
    glDisable(GL_LIGHTING);

  glColor3d(1.0, 1.0, 1.0);
  if (show_solid || show_wireframe) {
    for (auto f : mesh.faces()) {
      glBegin(GL_POLYGON);
      for (auto v : mesh.fv_range(f)) {
        if (visualization == Visualization::DEVIATION)
          glColor3dv(blendColor(deviation_range, mesh.data(v).deviation));
        else
          glNormal3fv(mesh.normal(v).data());
        glVertex3fv(mesh.point(v).data());
      }
      glEnd();
    }
  }

  if (visualization == Visualization::DEVIATION)
    glEnable(GL_LIGHTING);

  if (show_solid && show_wireframe) {
    glPolygonMode(GL_FRONT, GL_LINE);
    glColor3d(0.0, 0.0, 0.0);
    glDisable(GL_LIGHTING);
    for (auto f : mesh.faces()) {
      glBegin(GL_POLYGON);
      for (auto v : mesh.fv_range(f))
        glVertex3fv(mesh.point(v).data());
      glEnd();
    }
    glEnable(GL_LIGHTING);
  }
}

void
MeshModel::setDeviationFn(double (*fn)(const Vec &p)) {
  deviation = fn;
}

void
MeshModel::setDeviationRange(double range) {
  deviation_range = range;
}

void
MeshModel::updateDeviation() {
  for (auto v : mesh.vertices())
    mesh.data(v).deviation = deviation(Vec(mesh.point(v).data()));
}
