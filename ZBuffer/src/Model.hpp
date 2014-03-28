#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <string>
#include <vector>
#include "tiny_obj_loader.h"

class Model {
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

protected:
  /** \brief Calculate normals for each vertex.
   */
  void calculate_normal(size_t idx);

protected:
  std::string m_filename;
  std::vector<tinyobj::shape_t> m_shapes;

};

#endif // __MODEL_HPP__
