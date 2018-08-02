

struct int8 {
  unsigned int data : 8;

  operator int() { return data; }
};

void ErrorCallback(int, const char* err_str)
{
	 std::cout << "GLFW Error: " << err_str << std::endl;
}

struct Window {
  
  GLFWwindow * _window;

  int Init (std::string image_path, GLsizei& width, GLsizei& height, int8 * bmp_pixels)
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



		///**** CUSTOM BMP READER *****////

		// Data read from the header of the BMP file
		unsigned char header[54]; // Each BMP file begins by a 54-bytes header
		unsigned int dataPos;     // Position in the file where the actual data begins
		unsigned int bmp_width, bmp_height;
		unsigned int imageSize;   // = width*height*3
		// Actual RGB data
		unsigned char * data;

		FILE * file = fopen (image_path . c_str (),"rb");
		if (!file){printf("Image could not be opened\n"); return 0;}

		if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
			printf("Not a correct BMP file\n");
			return 1;
		}
		if ( header[0]!='B' || header[1]!='M' ){
			printf("Not a correct BMP file\n");
			return 1;
		}
    
		// Read ints from the byte array
		dataPos    = *(int*)&(header[0x0A]);
		imageSize  = *(int*)&(header[0x22]);
		bmp_width      = *(int*)&(header[0x12]);
		bmp_height     = *(int*)&(header[0x16]);

    printf ("imageSize: %d\n", imageSize);
		// Some BMP files are misformatted, guess missing information
		if (imageSize==0)    imageSize=bmp_width*bmp_height*3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos==0)      dataPos=54; // The BMP header is done that way
		// Create a buffer
		data = new unsigned char [imageSize];

		// Read the actual data from the file into the buffer
		fread (data, 1, imageSize, file);

		//Everything is in memory now, the file can be closed
		fclose (file);

		///**** CUSTOM BMP READER *****////

		width = bmp_width;
		height = bmp_height;
		memcpy (bmp_pixels, data, 32 * 32 * 3);
		
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
