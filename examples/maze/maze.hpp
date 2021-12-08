#ifndef MAZE_HPP_
#define MAZE_HPP_

#include "abcg.hpp"
#include "model.hpp"

#include <string>

class Maze : public abcg::OpenGLWindow {
 public:
  void initializeMaze(GLuint program, std::string assetsPath);
  void drawMaze(glm::mat4 viewMatrix, glm::mat4 projMatrix);
  void terminateGL();
  void wallCollision(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float& dolly, float& truck);
 private:

  GLuint m_program{};

  Model m_model;
  int m_trianglesToDraw{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  
  std::array<std::array<int, 11>, 11> maze{{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                            {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
                                            {1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1},
                                            {1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
                                            {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
                                            {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1},
                                            {1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
                                            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                            {1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
                                            {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};
  bool isWall(int x, int z, std::array<std::array<int, 11>, 11> maze);
};




#endif