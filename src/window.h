

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        printf("SDL Error: %s\n", error);
        if (line != -1)
            printf(" + line: %i\n", line);
        SDL_ClearError();
    }
#endif
}

struct int8 {
	unsigned int data : 8;
};

struct Window {

  SDL_Window* _window;
  SDL_GLContext _context;
  SDL_Event e;

  int Init ()
  {
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
      return 1;
    }

#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    _window = SDL_CreateWindow ("earlyman",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (_window == NULL)
    {
      fprintf(stderr, "could not create window: %s\n", SDL_GetError());
      return 1;
    }
    checkSDLError (__LINE__);
    _context = SDL_GL_CreateContext (_window);
    if (!_context)
    {
      fprintf (stderr, "Couldn't create context: %s\n", SDL_GetError ());
      return 1;
    }
    checkSDLError (__LINE__);

    SDL_GL_SetSwapInterval (1);

#ifdef _WIN64
    glewExperimental = true;
    GLenum status = glewInit ();
    if (status != GLEW_OK) {
        std::cerr << "GLEW failed to initialize!" << std::endl;
    }
#endif

    return 0;
  }

  bool Refresh ()
  {
		bool quit = false;
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
					/*
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
					*/
      }
    }

    // Swap our buffers to make our changes visible
    SDL_GL_SwapWindow (_window);
		
		return quit;
  }

  void CleanUp ()
  {
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
  }
};
