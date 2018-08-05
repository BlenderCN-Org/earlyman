
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Include GLFW
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <iostream>
#include <string>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <rapidjson/document.h>

#include "bmpreader.cpp"
#include "mesh.cpp"
#include "shader.cpp"
#include "renderer.cpp"
#include "glfw_window.cpp"

int on_fail (std::string message)
{
  std::cout << "ON_FAIL: " << message << std::endl;
  return 1;
}

int main(int argc, char* args[])
{
  Window window;
  if (window . Init () > 0)
    return on_fail ("window init failed");

  GLsizei width, height;
  // hard coding 32x32 image with 3 color channels, each 8 bit
  auto * bmp_pixels = new unsigned char[32 * 32 * 3];
  ReadBMP ("assets/multi.bmp", width, height, bmp_pixels);
  printf ("width, height, pointer: %d, %d, %p\n", width, height, bmp_pixels);

  Renderer renderer;
  if (renderer . OnGLContextCreated (width, height, bmp_pixels) > 0)
    return on_fail ("renderer init failed");
 
  // load our model from file
  rapidjson::Document d;
  std::ifstream t("models/cube_m.json");
  std::stringstream buffer;
  buffer << t.rdbuf();
  if (d . Parse (buffer . str () . c_str ()) . HasParseError ())
    return on_fail ("parsing json failed");

  Mesh my_cube (0.0f, d["verts"], d["uvs"], renderer . getProgramID ());

  bool quit = false;
  while (!quit)
  {
    renderer . Draw (my_cube);
    quit = window . Refresh ();
  }
  window . CleanUp ();
  renderer . CleanUp ();
  return 0;
}
