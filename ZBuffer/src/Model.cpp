#include <Eigen/Dense>
#include "Model.hpp"
#include "Logger.hpp"
#include <algorithm>

Model::Model(const char *filename)
  : m_filename(filename)
{
  std::string err = tinyobj::LoadObj(m_shapes, filename);
  ASSERT_MSG(err.empty(), "%s", err.c_str());

  for ( size_t i=0; i < m_shapes.size(); i++ ) {
    if ( m_shapes[i].mesh.normals.empty() )
      calculate_normal(i);
  }
}

Model::~Model()
{
}

void Model::debug() const
{
  for ( size_t i=0; i < m_shapes.size(); i++ ) {
    const tinyobj::shape_t & shape = m_shapes[i];
    const std::vector<unsigned int> & indices = shape.mesh.indices;
    const std::vector<float> & positions = shape.mesh.positions;
    const std::vector<float> & normals = shape.mesh.normals;

    INFO("Shape %lu: %s", i, shape.name.c_str());
    ASSERT(indices.size() % 3 == 0);
#if 0
    for ( size_t j=0; j < indices.size(); j+=3 ) {
      printf("triangle: (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)  (%.2f, %.2f, %.2f)\n",
          positions[3*indices[j  ]], positions[3*indices[j  ]+1], positions[3*indices[j  ]+2],
          positions[3*indices[j+1]], positions[3*indices[j+1]+1], positions[3*indices[j+1]+2],
          positions[3*indices[j+2]], positions[3*indices[j+2]+1], positions[3*indices[j+2]+2]);

      if ( !normals.empty() ) {
        printf("normals: (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)  (%.2f, %.2f, %.2f)\n",
            normals[3*indices[j  ]], normals[3*indices[j  ]+1], normals[3*indices[j  ]+2],
            normals[3*indices[j+1]], normals[3*indices[j+1]+1], normals[3*indices[j+1]+2],
            normals[3*indices[j+2]], normals[3*indices[j+2]+1], normals[3*indices[j+2]+2]);
      }
    }
#endif
    INFO("number of triangles = %lu", indices.size() / 3);
    INFO("number of vertices = %lu", positions.size() / 3);
    INFO("indices.size() = %lu", indices.size());
    INFO("positions.size() = %lu", positions.size());
    INFO("normals.size() = %lu", normals.size());
    INFO("min of index = %u", *(std::min_element(indices.begin(), indices.end())));
    INFO("max of index = %u", *(std::max_element(indices.begin(), indices.end())));
  }
}

size_t Model::numShapes() const
{
  return m_shapes.size();
}

size_t Model::vertexSize(size_t i) const
{
  return m_shapes[i].mesh.positions.size();
}

size_t Model::normalSize(size_t i) const
{
  return m_shapes[i].mesh.normals.size();
}

size_t Model::indexSize(size_t i) const
{
  return m_shapes[i].mesh.indices.size();
}

void *Model::vertexData(size_t i)
{
  return (void*)&(m_shapes[i].mesh.positions[0]);
}

void *Model::normalData(size_t i)
{
  return (void*)&(m_shapes[i].mesh.normals[0]);
}

void *Model::indexData(size_t i)
{
  return (void*)&(m_shapes[i].mesh.indices[0]);
}

void Model::calculate_normal(size_t idx)
{
  // Index is assumed
  ASSERT(!m_shapes[idx].mesh.indices.empty());

  const std::vector<unsigned int> & indices = m_shapes[idx].mesh.indices;
  const std::vector<float> & positions = m_shapes[idx].mesh.positions;

  // To store normal for each vertex
  std::vector<Vector3> new_normals(positions.size()/3, Vector3::Zero());

  // For each face, calculate face normal, add it to each vertex of the face
  for ( size_t i=0; i < indices.size(); i += 3 )
  {
    Vector3 a(positions[3*indices[i+1]  ]-positions[3*indices[i]  ],
                      positions[3*indices[i+1]+1]-positions[3*indices[i]+1],
                      positions[3*indices[i+1]+2]-positions[3*indices[i]+2]);
    Vector3 b(positions[3*indices[i+2]  ]-positions[3*indices[i]  ],
                      positions[3*indices[i+2]+1]-positions[3*indices[i]+1],
                      positions[3*indices[i+2]+2]-positions[3*indices[i]+2]);
    Vector3 n = a.cross(b).normalized();
    new_normals[indices[i  ]] += n;
    new_normals[indices[i+1]] += n;
    new_normals[indices[i+2]] += n;
  }
  for ( size_t i=0; i < new_normals.size(); i++ )
  {
    new_normals[i].normalize();
  }

  std::vector<float> & normals = m_shapes[idx].mesh.normals;
  if ( !normals.empty() )
    WARN("Overwriting exisiting normals...");
  normals.resize(positions.size());

  for ( size_t i=0; i < indices.size(); i++ )
  {
    normals[3*indices[i]  ] = new_normals[indices[i]].x();
    normals[3*indices[i]+1] = new_normals[indices[i]].y();
    normals[3*indices[i]+2] = new_normals[indices[i]].z();
  }
}

void Model::getTriangles(std::vector<Triangle> &triangles, const Matrix4 &transform)
{
  const Matrix4 normal_transform = transform.inverse().transpose();

  float x[2] = {99999.f, -99999.f};
  float y[2] = {99999.f, -99999.f};
  float z[2] = {99999.f, -99999.f};

  size_t n_filtered = 0;
  size_t n_remained = 0;

  for ( size_t i=0; i < m_shapes.size(); i++ )
  {
    const std::vector<unsigned int> & indices = m_shapes[i].mesh.indices;
    const std::vector<float> & positions = m_shapes[i].mesh.positions;
    const std::vector<float> & normals = m_shapes[i].mesh.normals;

    for ( size_t j=0; j < indices.size(); j += 3 )
    {
      Triangle t;

      // do the transformation
      for ( size_t k=0; k < 3; k++ )
      {
        t.vertices[k] = Vector4(positions[3*indices[j+k]], positions[3*indices[j+k]+1], positions[3*indices[j+k]+2], 1.0);
        t.vertices[k] = transform * t.vertices[k];
        t.vertices[k] /= t.vertices[k].w();
      }

      // do statistics about vertex info
      for ( size_t k=0; k < 3; k++ )
      {
        x[0] = std::min(x[0], (float)(t.vertices[k].x()));
        x[1] = std::max(x[1], (float)(t.vertices[k].x()));
        y[0] = std::min(y[0], (float)(t.vertices[k].y()));
        y[1] = std::max(y[1], (float)(t.vertices[k].y()));
        z[0] = std::min(z[0], (float)(t.vertices[k].z()));
        z[1] = std::max(z[1], (float)(t.vertices[k].z()));
      }

      // filter out this triangle if all three vertices are outside of the viewing volume
      if ( (t.vertices[0].x() < -1.0f || t.vertices[0].x() > 1.0f ||
            t.vertices[0].y() < -1.0f || t.vertices[0].y() > 1.0f ||
            t.vertices[0].z() < -1.0f || t.vertices[0].z() > 1.0f )
        && (t.vertices[1].x() < -1.0f || t.vertices[1].x() > 1.0f ||
            t.vertices[1].y() < -1.0f || t.vertices[1].y() > 1.0f ||
            t.vertices[1].z() < -1.0f || t.vertices[1].z() > 1.0f )
        && (t.vertices[2].x() < -1.0f || t.vertices[2].x() > 1.0f ||
            t.vertices[2].y() < -1.0f || t.vertices[2].y() > 1.0f ||
            t.vertices[2].z() < -1.0f || t.vertices[2].z() > 1.0f ) )
      {
        n_filtered++;
        continue;
      }

      // transform the normals as well
      for ( size_t k=0; k < 3; k++ )
      {
        t.normals[k] = Vector4(normals[3*indices[j+k]], normals[3*indices[j+k]+1], normals[3*indices[j+k]+2], 1.0);
        t.normals[k] = normal_transform * t.normals[k];
        Vector3 n(t.normals[k].x()/t.normals[k].w(),
                  t.normals[k].y()/t.normals[k].w(),
                  t.normals[k].z()/t.normals[k].w());
        n.normalize();
        t.normals[k] = Vector4(n.x(), n.y(), n.z(), 1.0);
      }

      // filter out this triangle if it's facing backward to viewer
      if (1)
      if ( t.normals[0].z() < 0 || t.normals[1].z() < 0 || t.normals[2].z() < 0 )
      {
        n_filtered++;
        continue;
      }
      if (0)
      {
        Vector4 n4 = t.normals[0] + t.normals[1] + t.normals[2];
        Vector3 n(n4.x()/n4.w(), n4.y()/n4.w(), n4.z()/n4.w());
        n.normalize();
        INFO("normal = (%.2f, %.2f, %.2f)", n.x(), n.y(), n.z());
      }

      triangles.push_back(t);
      n_remained++;
    }
  }
  INFO("range of x (before clip): (%.2f, %.2f)", x[0], x[1]);
  INFO("range of y (before clip): (%.2f, %.2f)", y[0], y[1]);
  INFO("range of z (before clip): (%.2f, %.2f)", z[0], z[1]);
  INFO("filtered: %.2f%% (%lu/%lu)", 100.0f*n_filtered/(n_filtered+n_remained), n_filtered, n_filtered+n_remained);
}
