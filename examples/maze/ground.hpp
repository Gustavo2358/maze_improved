#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"
#include "model.hpp"

class Ground {
 public:
  void initializeGL(GLuint program, std::string assetsPath);
  void drawGround(glm::mat4 viewMatrix, glm::mat4 projMatrix);
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLuint m_program{};

  Model m_model{};
  int m_trianglesToDraw{};

  

  // Light and material properties
  glm::vec4 m_lightDir{1.0f, -1.0f, 1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};
};

#endif