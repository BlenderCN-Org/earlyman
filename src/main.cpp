
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "shader.cpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct BMP_DICT
{
  SDL_Surface* player;
  SDL_Surface* enemy;
};

bool loadAssets (BMP_DICT* dict)
{
  SDL_Surface* red = SDL_LoadBMP("assets/red.bmp");
  SDL_Surface* blue = SDL_LoadBMP("assets/blue.bmp");
  dict -> player = blue;
  dict -> enemy = red;
  return true;
}

bool SetOpenGLAttributes ()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

GLuint programID;
GLuint vertexbuffer;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::quat camera;


void GLInit ()
{
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders( "shaders/simple.vert", "shaders/simple.frag" );

  static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // 90 degrees
	projection = glm::perspective(
		glm::radians(90.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		0.0f,
		100.0f
	);

 	model = glm::mat4(1.0f);
}

void Render (SDL_Window* window)
{
  // apply camera rotation to view matrix
  glm::mat4 rotation_matrix = glm::toMat4(camera);
  view = view * rotation_matrix;

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(programID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glUniformMatrix4fv( glGetUniformLocation( programID, "u_model" ),
      1, GL_FALSE, glm::value_ptr( model ) );
	glUniformMatrix4fv( glGetUniformLocation( programID, "u_view" ),
      1, GL_FALSE, glm::value_ptr( view ) );
	glUniformMatrix4fv( glGetUniformLocation( programID, "u_projection" ),
      1, GL_FALSE, glm::value_ptr( projection ) );
	
	model = glm::mat4();
	model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -20.0f ));
	model = glm::scale( model, glm::vec3( 10.0f, 10.0f, 0.0f ));
	
	glUniformMatrix4fv( glGetUniformLocation( programID, "u_model" ), 1, GL_FALSE, glm::value_ptr( model ) );
	
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(window);
}

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_GLContext mainContext = NULL;
  SDL_Surface* redBlock = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow("earlyman",
          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
          SCREEN_WIDTH, SCREEN_HEIGHT,
          SDL_WINDOW_OPENGL
          );
  if (window == NULL)
  {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }

	SetOpenGLAttributes (); 
  mainContext = SDL_GL_CreateContext (window);

  SDL_GL_SetSwapInterval (1);

  glewExperimental = GL_TRUE; 
  glewInit();

	GLInit ();

  /*
  BMP_DICT image_dict;
  loadAssets (&image_dict);

  int red_change = 1;
  SDL_Rect red_loc = {0,0,600,400};
  SDL_Rect blue_loc = {0,0,600,400};
  */

  //Main loop flag
  bool quit = false;
  bool red_dead = false;

  //Event handler
  SDL_Event e;

  //While application is running
  while( !quit )
  {
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
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
            camera *= inc;
          }
          else if(e.key.keysym.sym == SDLK_DOWN)
          {
            glm::quat inc = glm::quat (glm::vec3 (-0.01f, 0.0f, 0.0f));
            camera *= inc;
          }
          else if(e.key.keysym.sym == SDLK_LEFT)
          {
            glm::quat inc = glm::quat (glm::vec3 (0.0f, -0.01f, 0.0f));
            camera *= inc;
          }
          else if(e.key.keysym.sym == SDLK_RIGHT)
          {
            glm::quat inc = glm::quat (glm::vec3 (0.0f, 0.01f, 0.0f));
            camera *= inc;
          }
      }
    }

    Render (window);
  }
 
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
