

struct Renderer {

  GLuint _textureID;
  GLuint _programID;

  glm::mat4 _view;
  glm::mat4 _projection;
  glm::quat _camera;

  int OnGLContextCreated (GLsizei width, GLsizei height, const GLvoid * pixels)
  {
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // Create and compile our GLSL program from the shaders
#ifndef EMSCRIPTEN
    _programID = LoadShaders (
        "assets/shaders/simple.core330.vert", "assets/shaders/simple.core330.frag");
#else
    _programID = LoadShaders (
        "assets/shaders/simple.es300.vert", "assets/shaders/simple.es300.frag");
#endif

    float aspect_ratio = static_cast<float> (SCREEN_WIDTH) / static_cast<float> (SCREEN_HEIGHT);
    // near _can't_ be 0.0, else depth doesn't work
    _projection = glm::perspective (glm::radians (50.0f), aspect_ratio, 0.1f, 100.0f);
    _view = glm::lookAt (glm::vec3 (3, 3, 3), glm::vec3 (0, 0, 0), glm::vec3 (0, 0, 1));

    glGenTextures (1, &_textureID);
    glBindTexture (GL_TEXTURE_2D, _textureID);

    // NOTE: this crazy GL_BGR thing for the format param, I guess windows bitmaps flip blue and red?
    // if I just use GL_RGB for format, red and blue channels are reversed
    std::cout << "pixels: " << pixels << std::endl; 
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    printf ("width: %d, height: %d\n", width, height);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap (GL_TEXTURE_2D);

    return 0;
  }

  void RotateCamera (glm::quat rot)
  {
    _camera *= rot;
  }

  GLuint getProgramID ()
  {
    return _programID;
  }

  void Draw (Mesh& mesh)
  {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // apply camera rotation to view matrix
    // _view *= glm::toMat4 (_camera);

    glUseProgram (_programID);

    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_view"),
        1, GL_FALSE, glm::value_ptr (_view));
    glUniformMatrix4fv (glGetUniformLocation (_programID, "u_projection"),
        1, GL_FALSE, glm::value_ptr (_projection));
    
    mesh . draw (_programID);
  }

  void CleanUp ()
  {
    glDeleteProgram(_programID);
  }

};
