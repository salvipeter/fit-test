#include "bspline-model.hh"

#include <Geom_BSplineSurface.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>

BSplineModel::BSplineModel() :
  show_control_net(true), resolution(30), mean_map_range(0.0)
{
}

BSplineModel::~BSplineModel() {
}

void
BSplineModel::generateMesh() {
  mesh.clear();
  mesh.request_vertex_normals();

  std::vector<MyMesh::VertexHandle> handles, tri;

  double ulo, uhi, vlo, vhi;
  surface->Bounds(ulo, uhi, vlo, vhi);
  for (size_t i = 0; i <= resolution; ++i) {
    double u = (double)i / resolution;
    u = ulo + (uhi - ulo) * u;
    for (size_t j = 0; j <= resolution; ++j) {
      double v = (double)j / resolution;
      v = vlo + (vhi - vlo) * v;
      gp_Pnt p;
      gp_Vec d1u, d1v, d2u, d2v, d2uv;
      surface->D2(u, v, p, d1u, d1v, d2u, d2v, d2uv);

      // Add point
      auto q = MyMesh::Point(p.XYZ().GetData());
      auto new_vertex = mesh.add_vertex(q);
      handles.push_back(new_vertex);

      // Compute normal
      auto n = (d1u ^ d1v).Normalized();
      q = MyMesh::Normal(n.XYZ().GetData());
      mesh.set_normal(new_vertex, q);

      // Compute mean curvature
      double E = d1u.SquareMagnitude();
      double F = d1u * d1v;
      double G = d1v.SquareMagnitude();
      double L = n * d2u;
      double M = n * d2uv;
      double N = n * d2v;
      mesh.data(new_vertex).mean = (N*E-2*M*F+L*G)/(2*(E*G-F*F));
    }
  }

  size_t len = resolution + 1;
  for (size_t i = 0; i < resolution; ++i)
    for (size_t j = 0; j < resolution; ++j) {
      tri.clear();
      tri.push_back(handles[(i + 1) * len + j]);
      tri.push_back(handles[i * len + j + 1]);
      tri.push_back(handles[i * len + j]);
      mesh.add_face(tri);
      tri.clear();
      tri.push_back(handles[(i + 1) * len + j + 1]);
      tri.push_back(handles[i * len + j + 1]);
      tri.push_back(handles[(i + 1) * len + j]);
      mesh.add_face(tri);
    }
}

bool
BSplineModel::open(std::string filename) {
  size_t deg_u, deg_v, nknots_u, nknots_v;
  std::vector<double> knots_u, knots_v;
  std::vector<int> mult_u, mult_v;
  try {

    std::ifstream f(filename.c_str());

    f >> deg_u >> deg_v;

    // U knot vector
    f >> nknots_u;
    for (size_t i = 0; i < nknots_u; ++i) {
      double u;
      f >> u;
      if (i > 0 && knots_u.back() == u)
        mult_u.back()++;
      else {
        knots_u.push_back(u);
        mult_u.push_back(1);
      }
    }
    TColStd_Array1OfReal knots_u_occ(knots_u[0], 1, knots_u.size());
    TColStd_Array1OfInteger mult_u_occ(mult_u[0], 1, mult_u.size());

    // V knot vector
    f >> nknots_v;
    for (size_t i = 0; i < nknots_v; ++i) {
      double v;
      f >> v;
      if (i > 0 && knots_v.back() == v)
        mult_v.back()++;
      else {
        knots_v.push_back(v);
        mult_v.push_back(1);
      }
    }
    TColStd_Array1OfReal knots_v_occ(knots_v[0], 1, knots_v.size());
    TColStd_Array1OfInteger mult_v_occ(mult_v[0], 1, mult_v.size());

    // Control net
    size_t nrow = nknots_u - deg_u - 1, ncol = nknots_v - deg_v - 1;
    TColgp_Array2OfPnt cnet(1, nrow, 1, ncol);

    for (size_t i = 1; i <= nrow; ++i)
      for (size_t j = 1; j <= ncol; ++j) {
        double x, y, z;
        f >> x >> y >> z;
        cnet(i, j) = gp_Pnt(x, y, z);
      }

    surface = std::make_unique<Geom_BSplineSurface>(cnet, knots_u_occ, knots_v_occ,
                                                    mult_u_occ, mult_v_occ, deg_u, deg_v);

  } catch(std::ifstream::failure) {
    return false;
  }

  generateMesh();

  // TODO: would be faster to compute this by the control net
  auto min = mesh.point(*mesh.vertices_begin()), max = min;
  for (auto v : mesh.vertices()) {
    min.minimize(mesh.point(v));
    max.maximize(mesh.point(v));
  }
  box_min = Vec(min.data());
  box_max = Vec(max.data());

  return true;
}

void
BSplineModel::draw() const {
  if (show_control_net) {
    const auto &cpts = surface->Poles();

    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glColor3d(0.3, 0.3, 1.0);
    int nrowcol[2];
    nrowcol[0] = cpts.UpperRow() - cpts.LowerRow() + 1;
    nrowcol[1] = cpts.UpperCol() - cpts.LowerCol() + 1;
    for (int k = 0; k < 2; ++k)
      for (int i = 0; i < nrowcol[k]; ++i) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < nrowcol[1-k]; ++j) {
          if (k == 0) {
            const auto &p = cpts(cpts.LowerRow() + i, cpts.LowerCol() + j);
            glVertex3dv(p.XYZ().GetData());
          } else {
            const auto &p = cpts(cpts.LowerRow() + j, cpts.LowerCol() + i);
            glVertex3dv(p.XYZ().GetData());
          }
        }
        glEnd();
      }

    glLineWidth(1.0);
    glPointSize(8.0);
    glColor3d(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = cpts.LowerRow(); i <= cpts.UpperRow(); ++i)
      for (int j = cpts.LowerCol(); j <= cpts.UpperCol(); ++j) {
        const auto &p = cpts(i, j);
        glVertex3dv(p.XYZ().GetData());
      }
    glEnd();
    glPointSize(1.0);
    glEnable(GL_LIGHTING);
  }

  glPolygonMode(GL_FRONT_AND_BACK, !show_solid && show_wireframe ? GL_LINE : GL_FILL);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1, 1);

  if (show_solid || show_wireframe) {
    if (visualization == Visualization::PLAIN)
      glColor3d(1.0, 1.0, 1.0);
    else if (visualization == Visualization::ISOPHOTES) {
      glBindTexture(GL_TEXTURE_2D, *isophote_texture);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      glEnable(GL_TEXTURE_2D);
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
    }
    for (auto f : mesh.faces()) {
      glBegin(GL_POLYGON);
      for (auto v : mesh.fv_range(f)) {
        if (visualization == Visualization::MEAN)
          glColor3dv(blendColor(mean_map_range, mesh.data(v).mean));
        glNormal3dv(mesh.normal(v).data());
        glVertex3dv(mesh.point(v).data());
      }
      glEnd();
    }
    if (visualization == Visualization::ISOPHOTES) {
      glDisable(GL_TEXTURE_GEN_S);
      glDisable(GL_TEXTURE_GEN_T);
      glDisable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
  }

  if (show_solid && show_wireframe) {
    glPolygonMode(GL_FRONT, GL_LINE);
    glColor3d(0.0, 0.0, 0.0);
    glDisable(GL_LIGHTING);
    for (auto f : mesh.faces()) {
      glBegin(GL_POLYGON);
      for (auto v : mesh.fv_range(f))
        glVertex3dv(mesh.point(v).data());
      glEnd();
    }
    glEnable(GL_LIGHTING);
  }
}

void
BSplineModel::toggleControlNet() {
  show_control_net = !show_control_net;
}

size_t
BSplineModel::getResolution() const {
  return resolution;
}

void
BSplineModel::setResolution(size_t res) {
  resolution = res;
}

void
BSplineModel::setMeanMapRange(double range) {
  mean_map_range = range;
}

double
BSplineModel::distance(const Vec &p) const {
  // return GeomAPI_ProjectPointOnSurf(gp_Pnt(p[0], p[1], p[2]), surface.get());
  gp_Pnt q(p[0], p[1], p[2]);
  GeomAPI_ProjectPointOnSurf projection(q, surface.get());
  return projection.LowerDistance();
}

