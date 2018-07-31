
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <rapidjson/document.h>

#include "mesh.cpp"
#include "shader.cpp"
#include "renderer.cpp"

int on_fail (std::string message)
{
  std::cout << "ON_FAIL: " << message << std::endl;
  return 1;
}

int main(int argc, char* args[])
{
  Renderer renderer;
  if (renderer . Init () > 0) on_fail ("init failed");
  
  // load our model from file
  rapidjson::Document d;
  std::ifstream t("models/cube_m.json");
  std::stringstream buffer;
  buffer << t.rdbuf();
  if (d . Parse (buffer . str () . c_str ()) . HasParseError ()) on_fail ("parsing json failed");

  Mesh my_cube (0.0f, d["verts"], d["uvs"]);

  bool quit = false;

  SDL_Event e;
  while (!quit)
  {
    while (SDL_PollEvent ( &e ) != 0)
    {
      switch( e.type )
      {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          if(e.key.keysym.sym == SDLK_ESCAPE)
          {
            quit = true;
          }
          else if(e.key.keysym.sym == SDLK_UP)
          {
            // this path defines using a vec3 as a constructor:
            // /usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/quaternion.hpp
            glm::quat inc = glm::quat (glm::vec3 (0.01f, 0.0f, 0.0f));
            renderer . RotateCamera (inc);
          }
          else if(e.key.keysym.sym == SDLK_DOWN)
          {
            glm::quat inc = glm::quat (glm::vec3 (-0.01f, 0.0f, 0.0f));
            renderer . RotateCamera (inc);
          }
          else if(e.key.keysym.sym == SDLK_LEFT)
          {
            glm::quat inc = glm::quat (glm::vec3 (0.0f, -0.01f, 0.0f));
            renderer . RotateCamera (inc);
          }
          else if(e.key.keysym.sym == SDLK_RIGHT)
          {
            glm::quat inc = glm::quat (glm::vec3 (0.0f, 0.01f, 0.0f));
            renderer . RotateCamera (inc);
          }
      }
    }
    renderer . Draw (my_cube);
  }
  renderer . CleanUp ();
  return 0;
}
