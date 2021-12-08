project(maze)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp openglwindow.cpp 
                                model.cpp bunny.cpp maze.cpp skybox.cpp)
enable_abcg(${PROJECT_NAME})