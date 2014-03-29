#include "glew.h"
#include "GLWidget.hpp"
#include "Logger.hpp"

GLWidget::GLWidget(Model *model, QWidget * parent)
  : QGLViewer(parent),
    m_model(model),
    m_numShapes(0)
{
  memset(m_indexBuffer, 0, sizeof(GLuint)*MAX_SHAPES);
  memset(m_vertexBuffer, 0, sizeof(GLuint)*MAX_SHAPES);
}

GLWidget::~GLWidget()
{
  glDeleteBuffers(m_numShapes, m_indexBuffer);
  glDeleteBuffers(m_numShapes, m_vertexBuffer);
}

void GLWidget::init()
{
  ASSERT_MSG(GLEW_OK==glewInit(), "GLWidget: GLEW failed to initialize!");
  ASSERT_MSG(m_model, "GLWidget: model failed to load!");

  m_model->debug();

  m_numShapes = m_model->numShapes();
  if ( m_numShapes > MAX_SHAPES )
    m_numShapes = MAX_SHAPES;

  // Generate buffers
  glGenBuffers(m_numShapes, m_indexBuffer);
  glGenBuffers(m_numShapes, m_vertexBuffer);

  for ( size_t i=0; i < m_numShapes; i++ )
  {
    // Bind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer[i]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[i]);

    // Transfer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(m_model->indexSize(i)), m_model->indexData(i), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(m_model->vertexSize(i)+m_model->normalSize(i)), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*(m_model->vertexSize(i)), m_model->vertexData(i));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*(m_model->vertexSize(i)), sizeof(float)*(m_model->normalSize(i)), m_model->normalData(i));

    // Unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  float shininess = 15.0f;
  float diffuseColor[3] = {0.929524f, 0.796542f, 0.178823f};
  float specularColor[4] = {1.00000f, 0.980392f, 0.549020f, 1.0f};

  // set specular and shiniess using glMaterial (gold-yellow)
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

  // set ambient and diffuse color using glColorMaterial (gold-yellow)
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glColor3fv(diffuseColor);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void GLWidget::draw()
{
  for ( size_t i=0; i < m_numShapes; i++ )
  {
    // Bind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer[i]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[i]);

    // Draw
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (GLvoid*)(sizeof(float)*(m_model->vertexSize(i))));
    glDrawElements(GL_TRIANGLES, m_model->indexSize(i), GL_UNSIGNED_INT, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // Unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
