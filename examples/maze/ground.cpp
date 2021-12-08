#include "ground.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>


void Ground::initializeGL(GLuint program, std::string assetsPath) {
  
  m_program = program;

  // Unit quad on the xz plane
  // clang-format off
  m_model.loadDiffuseTexture(assetsPath + "maps/pattern.png");
  m_model.loadNormalTexture(assetsPath + "maps/pattern_normal.png");
  // std::array vertices{glm::vec3( 0.0f, 0.0f,  1.0f), 
  //                     glm::vec3( 0.0f, 0.0f,  0.0f),
  //                     glm::vec3( 1.0f, 0.0f,  1.0f),
  //                     glm::vec3( 1.0f, 0.0f,  0.0f)};
  
  m_model.loadObj(assetsPath + "chamferbox.obj", false);
  
  m_model.setupVAO(m_program);

  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
  
  
  // clang-format on                      

  // Generate VBO
  // abcg::glGenBuffers(1, &m_VBO);
  // abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  // abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
  //                    GL_STATIC_DRAW);
  // abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // // Create VAO and bind vertex attributes
  // abcg::glGenVertexArrays(1, &m_VAO);
  // abcg::glBindVertexArray(m_VAO);
  // abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  // const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  // abcg::glEnableVertexAttribArray(posAttrib);
  // abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  // abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  // abcg::glBindVertexArray(0);

}

void Ground::drawGround(glm::mat4 viewMatrix, glm::mat4 projMatrix) {
  
// Get location of uniform variables (could be precomputed)
  const GLint modelMatrixLoc{abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint normalMatrixLoc{abcg::glGetUniformLocation(m_program, "normalMatrix")};
  const GLint lightDirLoc{abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  
  
  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};

  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  const GLint mappingModeLoc{abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables that will be used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, 0);

  const auto lightDirRotated{projMatrix * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  abcg::glBindVertexArray(m_VAO);
  for (const auto z : iter::range(0, 11)) {
    for (const auto x : iter::range(0, 11)) {
      // Set model matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x + 0.5f, -0.5f, z + 0.5f));
      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      auto modelViewMatrix{glm::mat3(viewMatrix * model)};
      glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
      glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

      
      //abcg::glFrontFace(GL_CW);
      m_model.render(m_trianglesToDraw);
      //abcg::glFrontFace(GL_CCW);
    }
  }
  abcg::glBindVertexArray(0);
}

void Ground::terminateGL() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}