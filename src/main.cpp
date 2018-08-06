
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Include GLFW
//#define GLFW_INCLUDE_GLCOREARB
//#include <GLFW/glfw3.h>

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#include "emscripten.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <SDL2/SDL.h>

#include <functional>
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
#include "window.cpp"

int on_fail (std::string message)
{
  std::cout << "ON_FAIL: " << message << std::endl;
  return 1;
}

std::function<void()> loop;
void main_loop() { loop (); }

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
  loop = [&]
  {
#ifdef EMSCRIPTEN
	  if (quit) emscripten_cancel_main_loop();
#endif
    renderer . Draw (my_cube);
    quit = window . Refresh ();
  };
  
#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop, 0, true);
#else
  while (!quit)
  {
    loop ();
  }
#endif
  window . CleanUp ();
  renderer . CleanUp ();
  
  return EXIT_SUCCESS;
}
