
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

#include "bmpreader.h"
#include "mesh.h"
#include "shader.h"
#include "renderer.h"
#include "window.h"

int on_fail (std::string message)
{
  std::cout << "ON_FAIL: " << message << std::endl;
  return 1;
}

std::function<void()> loop;
void main_loop() { loop (); }

#ifdef EMSCRIPTEN
extern "C" void EMSCRIPTEN_KEEPALIVE handle_input(const char * selection)
{
  std::cout << "selection: " << selection << std::endl;

  char input[100];
  strcpy (input, "onReceiveData(");
  strcat (input, "{");
  if (strcmp ("open_door", selection) == 0)
  {
    strcat (input, "prompt: 'The door reveals a huge monster!',");
    strcat (input, "buttons: [");
    strcat (input, "{txt: 'Run away!', key: 'run_away'},");
    strcat (input, "{txt: 'Fight!', key: 'fight'}");
    strcat (input, "]");
  }
  else if (strcmp ("light", selection) == 0)
  {
    strcat (input, "prompt: 'The light reveals a dusty painting.',");
    strcat (input, "buttons: [");
    strcat (input, "{txt: 'Inspect it.', key: 'inspect'}");
    strcat (input, "]");
  }
  else
  {
    strcat (input, "prompt: 'There is dark hallway leading to a closed door.',");
    strcat (input, "buttons: [");
    strcat (input, "{txt: 'Open the door', key: 'open_door'},");
    strcat (input, "{txt: 'Turn on light', key: 'light'}");
    strcat (input, "]");
  }
  strcat (input, "}");
  strcat (input, ")");
  emscripten_run_script (input);
}

extern "C" {
  void my_js(const char *);
}
#endif

int main(int argc, char* args[])
{
  SDL_Event e;
  Window window;
  if (window . Init () > 0)
    return on_fail ("window init failed");

  GLsizei width, height;
  // hard coding 32x32 image with 3 color channels, each 8 bit
  auto * bmp_pixels = new unsigned char[32 * 32 * 3];
  ReadBMP ("assets/images/multi.bmp", width, height, bmp_pixels);
  printf ("width, height, pointer: %d, %d, %p\n", width, height, bmp_pixels);

  Renderer renderer;
  if (renderer . OnGLContextCreated (width, height, bmp_pixels) > 0)
    return on_fail ("renderer init failed");
 
  // load our model from file
  rapidjson::Document d;
  std::ifstream t("assets/models/cube_m.json");
  std::stringstream buffer;
  buffer << t.rdbuf();
  if (d . Parse (buffer . str () . c_str ()) . HasParseError ())
    return on_fail ("parsing json failed");

  Mesh my_cube (0.0f, d["verts"], d["uvs"], renderer . getProgramID ());

  // tell the ui layer the engine is ready
#ifdef EMSCRIPTEN
  emscripten_run_script ("onEngineReady()");
#endif

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
