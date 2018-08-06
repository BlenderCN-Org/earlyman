
void ErrorCallback(int, const char* err_str)
{
	 std::cout << "GLFW Error: " << err_str << std::endl;
}

struct Window {
  
  GLFWwindow * _window;

  int Init ()
  {
		glfwSetErrorCallback (ErrorCallback);

		const int ret = glfwInit ();
    if (GL_FALSE == ret)
    { std::cout << "GLFW Error!n";
		}
       
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    _window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "earlyman", NULL, NULL);
    if (_window == NULL)
    {
      fprintf( stderr, "Failed to open GLFW window.\n" );
      getchar();
      glfwTerminate();
      return -1;
    }
    glfwMakeContextCurrent (_window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode (_window, GLFW_STICKY_KEYS, GL_TRUE);

    return 0;
  }

  bool Refresh ()
  {
    glfwSwapBuffers (_window);
    glfwPollEvents ();

  	return glfwGetKey (_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS &&
       glfwWindowShouldClose (_window) == 0;
  }

  void CleanUp ()
  {
		glfwTerminate ();
  }
};
