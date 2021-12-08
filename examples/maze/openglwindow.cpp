#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <array>
#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w))
      m_dollySpeed = speed;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -speed;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -speed;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = speed;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -speed;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = speed;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {

  abcg::glClearColor(0, 0, 0, 1);

  // // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);
  
  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/normalmapping.vert",
                                    getAssetsPath() + "shaders/normalmapping.frag");
          
  m_skyProgram = createProgramFromFile(getAssetsPath() + "shaders/skybox.vert",
                                    getAssetsPath() + "shaders/skybox.frag");

  m_ground.initializeGL(m_program, getAssetsPath());
  m_bunny.initializeBunny(m_program, getAssetsPath());
  m_maze.initializeMaze(m_program, getAssetsPath());
  m_skybox.initializeSkybox(m_skyProgram, getAssetsPath());
}

void OpenGLWindow::paintGL() {
  update();

  // // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  
  //const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  // Draw maze objects
  m_maze.drawMaze(m_camera.m_viewMatrix, m_camera.m_projMatrix);
  m_ground.drawGround(m_camera.m_viewMatrix, m_camera.m_projMatrix);
  m_bunny.drawBunny(m_camera.m_viewMatrix, m_camera.m_projMatrix);
  
  abcg::glUseProgram(0);

  m_skybox.drawSkybox(m_camera.m_viewMatrix, m_camera.m_projMatrix);
  
  
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();
  m_bunny.terminateBunny();
  m_maze.terminateGL();
  m_skybox.terminateSkybox();

  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // avoid going through the walls
  m_maze.wallCollision(m_camera.m_eye, m_camera.m_at, m_camera.m_up,
  m_dollySpeed, m_truckSpeed);

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}
