#include "maze.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void Maze::initializeMaze(GLuint program, std::string assetsPath) {
  m_program = program;

  m_model.loadDiffuseTexture(assetsPath + "maps/brick_base.jpg");
  m_model.loadNormalTexture(assetsPath + "maps/brick_normal.jpg");
  m_model.loadObj(assetsPath + "chamferbox.obj", false);

  m_model.setupVAO(m_program);

  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void Maze::drawMaze(glm::mat4 viewMatrix, glm::mat4 projMatrix) {
  // Draw white boxes

  // Get location of uniform variables (could be precomputed)
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};

  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};

  const GLint diffuseTexLoc{
      abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(m_program, "mappingMode")};

  const auto lightDirRotated{projMatrix * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables that will be used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, 0);

  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  //abcg::glFrontFace(GL_CW);
  for (auto i : iter::range(0, 11)) {
    for (auto j : iter::range(0, 11)) {
      if (maze[abs(i)][j] == 1) {
        glm::mat4 model{1.0f};
        model = glm::translate(
            model, glm::vec3(float(i) + 0.5f, 0.5f, float(j) + 0.5f));
        abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

        auto modelViewMatrix{glm::mat3(viewMatrix * model)};
        glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

        m_model.render(m_trianglesToDraw);
      }
    }
  }
  //abcg::glFrontFace(GL_CCW);
}

void Maze::terminateGL() { m_model.terminateGL(); }

bool Maze::isWall(int x, int z, std::array<std::array<int, 11>, 11> maze) {
  if (maze[x][z] == 1) {
    return true;
  } else {
    return false;
  }
}

void Maze::wallCollision(glm::vec3 eye, glm::vec3 at, glm::vec3 up,
                         float& dolly, float& truck) {
  glm::vec3 forward = glm::normalize(at - eye) * 0.2f;
  glm::vec3 frontPoint = eye + forward;
  glm::vec3 backPoint = eye - forward;
  glm::vec3 left = glm::normalize(glm::cross(up, forward)) * 0.2f;
  glm::vec3 leftPoint = eye + left;
  glm::vec3 rightPoint = eye - left;

  if (dolly > 0 && isWall(frontPoint.x, frontPoint.z, maze)) {
    truck = 0.0;
    dolly = 0.0;
  }
  if (dolly < 0 && isWall(backPoint.x, backPoint.z, maze)) {
    truck = 0.0;
    dolly = 0.0;
  }
  if (truck < 0 && isWall(leftPoint.x, leftPoint.z, maze)) {
    truck = 0.0;
    dolly = 0.0;
  }
  if (truck > 0 && isWall(rightPoint.x, rightPoint.z, maze)) {
    truck = 0.0;
    dolly = 0.0;
  }
}