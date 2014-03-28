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
    printf("number of triangles = %lu\n", indices.size() / 3);
    printf("number of vertices = %lu\n", positions.size() / 3);
    printf("indices.size() = %lu\n", indices.size());
    printf("positions.size() = %lu\n", positions.size());
    printf("normals.size() = %lu\n", normals.size());
    printf("min of index = %u\n", *(std::min_element(indices.begin(), indices.end())));
    printf("max of index = %u\n", *(std::max_element(indices.begin(), indices.end())));
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
  std::vector<Eigen::Vector3f> new_normals(positions.size()/3, Eigen::Vector3f::Zero());

  // For each face, calculate face normal, add it to each vertex of the face
  for ( size_t i=0; i < indices.size(); i += 3 )
  {
    Eigen::Vector3f a(positions[3*indices[i+1]  ]-positions[3*indices[i]  ],
                      positions[3*indices[i+1]+1]-positions[3*indices[i]+1],
                      positions[3*indices[i+1]+2]-positions[3*indices[i]+2]);
    Eigen::Vector3f b(positions[3*indices[i+2]  ]-positions[3*indices[i]  ],
                      positions[3*indices[i+2]+1]-positions[3*indices[i]+1],
                      positions[3*indices[i+2]+2]-positions[3*indices[i]+2]);
    Eigen::Vector3f n = a.cross(b).normalized();
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
