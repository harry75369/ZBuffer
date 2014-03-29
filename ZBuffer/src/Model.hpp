#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <string>
#include <vector>
#include <Eigen/Eigen>
#include "tiny_obj_loader.h"

struct Triangle {
  typedef Eigen::Vector4d Vector4;

  Vector4 vertices[3];
  Vector4 normals[3];
};

class Model {
public:
  typedef Eigen::Vector3d Vector3;
  typedef Eigen::Vector4d Vector4;
  typedef Eigen::Matrix4d Matrix4;

public:
  Model(const char *filename);
  ~Model();

public:
  void debug() const;
  size_t numShapes() const;
  size_t vertexSize(size_t i) const;
  size_t normalSize(size_t i) const;
  size_t indexSize(size_t i) const;
  void *vertexData(size_t i);
  void *normalData(size_t i);
  void *indexData(size_t i);

  void getTriangles(std::vector<Triangle> &triangles, const Matrix4 &transform);

protected:
  /** \brief Calculate normals for each vertex.
   */
  void calculate_normal(size_t idx);

protected:
  std::string m_filename;
  std::vector<tinyobj::shape_t> m_shapes;

};

#endif // __MODEL_HPP__
