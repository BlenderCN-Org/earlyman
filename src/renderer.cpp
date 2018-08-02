
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const uint32_t positionAttributeIndex = 0;
const uint32_t uvAttributeIndex = 1;


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

struct Renderer {

  SDL_Window* _window;
  SDL_GLContext _context;

  GLuint _textureID;
  GLuint _programID;

  glm::mat4 _view;
  glm::mat4 _projection;
  glm::quat _camera;

  int Init (std::string image_path)
  {
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
      return 1;
    }

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);

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
    checkSDLError(__LINE__);
    _context = SDL_GL_CreateContext (_window);
    if (!_context)
    {
      fprintf (stderr, "Couldn't create context: %s\n", SDL_GetError ());
      return 1;
    }
    checkSDLError(__LINE__);
 
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    SDL_GL_SetSwapInterval (1);

    std::cout << "HHHHEEEEYYY!!!!" << std::endl;
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cerr << "GLEW failed to initialize!" << std::endl;
    }
 
    // Create and compile our GLSL program from the shaders
    _programID = LoadShaders ("shaders/simple.vert", "shaders/simple.frag");

    float aspect_ratio = static_cast<float> (SCREEN_WIDTH) / static_cast<float> (SCREEN_HEIGHT);
    // near _can't_ be 0.0, else depth doesn't work
    _projection = glm::perspective (glm::radians (90.0f), aspect_ratio, 0.1f, 100.0f);

    SDL_Surface* image_surface = SDL_LoadBMP (image_path . c_str ());

    glGenTextures (1, &_textureID);
    glBindTexture (GL_TEXTURE_2D, _textureID);

    // NOTE: this crazy GL_BGR thing for the format param, I guess windows bitmaps flip blue and red?
    // if I just use GL_RGB for format, red and blue channels are reversed
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, image_surface -> w, image_surface -> h,
        0, GL_BGR, GL_UNSIGNED_BYTE, image_surface -> pixels);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    int redbits; 
    SDL_GL_GetAttribute (SDL_GL_RED_SIZE, &redbits);
    std::cout << "red bits: " << redbits << std::endl;
    int depthbits; 
    SDL_GL_GetAttribute (SDL_GL_DEPTH_SIZE, &depthbits);
    std::cout << "depth bits: " << depthbits << std::endl;
    
    return 0;
  }

  void RotateCamera (glm::quat rot)
  {
    _camera *= rot;
  }

  void Draw (Mesh& mesh)
  {
    // apply camera rotation to view matrix
    _view *= glm::toMat4 (_camera);
    
    glClear (GL_COLOR_BUFFER_BIT);

    glUseProgram (_programID);

    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_view"),
        1, GL_FALSE, glm::value_ptr (_view));
    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_projection"),
        1, GL_FALSE, glm::value_ptr (_projection));
    
    mesh . draw (_programID);
    
    // Swap our buffers to make our changes visible
    SDL_GL_SwapWindow (_window);
  }

  void CleanUp ()
  {
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
  }
};
