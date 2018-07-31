
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const uint32_t positionAttributeIndex = 0;
const uint32_t uvAttributeIndex = 1;

struct Renderer {

  SDL_Window* _window;

  GLuint _textureID;
  GLuint _programID;

  glm::mat4 _view;
  glm::mat4 _projection;
  glm::quat _camera;

  int Init ()
  {

    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
      return 1;
    }
    _window = SDL_CreateWindow ("earlyman",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (_window == NULL)
    {
      fprintf(stderr, "could not create window: %s\n", SDL_GetError());
      return 1;
    }

    SetOpenGLAttributes ();
    SDL_GL_CreateContext (_window);

    SDL_GL_SetSwapInterval (1);

    glewExperimental = GL_TRUE;
    glewInit ();

    // Create and compile our GLSL program from the shaders
    _programID = LoadShaders ("shaders/simple.vert", "shaders/simple.frag");

    float aspect_ratio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    _projection = glm::perspective (glm::radians (90.0f), aspect_ratio, 0.0f, 100.0f);

    SDL_Surface* image_surface = SDL_LoadBMP ("assets/multi.bmp");

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // NOTE: this crazy GL_BGR thing for the format param, I guess windows bitmaps flip blue and red?
    // if I just use GL_RGB for format, red and blue channels are reversed
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_surface -> w, image_surface -> h,
        0, GL_BGR, GL_UNSIGNED_BYTE, image_surface -> pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable (GL_CULL_FACE);
    glEnable (GL_DEPTH_TEST);

    return 0;
  }

  bool SetOpenGLAttributes ()
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
  }

  void RotateCamera (glm::quat rot)
  {
    _camera *= rot;
  }

  void Draw (Mesh& mesh)
  {
    // apply camera rotation to view matrix
    _view *= glm::toMat4 (_camera);

    glClearColor (0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    glUseProgram (_programID);

    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_view"),
        1, GL_FALSE, glm::value_ptr (_view));
    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_projection"),
        1, GL_FALSE, glm::value_ptr (_projection));
    
    glDepthFunc (GL_LEQUAL);

    mesh . draw (_programID);

    // Swap our buffers to make our changes visible
    SDL_GL_SwapWindow (_window);
  }

  void CleanUp ()
  {
    SDL_DestroyWindow(_window);
    SDL_Quit();
  }
};
