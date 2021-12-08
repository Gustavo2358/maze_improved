#ifndef BUNNY_HPP_
#define BUNNY_HPP_

#include "abcg.hpp"
#include "model.hpp"

#include <string>

class Bunny : public abcg::OpenGLWindow {
 public:
  void initializeBunny(GLuint program,  std::string assetsPath);
  void drawBunny(glm::mat4 viewMatrix, glm::mat4 projMatrix);
  void terminateBunny();

 private:
  GLuint m_program{};

  Model m_model;
  int m_trianglesToDraw{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};

};

#endif