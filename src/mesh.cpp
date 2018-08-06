
struct Mesh
{

  public:

  glm::mat4 model;
  GLuint vbo[2];
  GLuint vao[1];
  uint _size;

  Mesh(float x_pos, const rapidjson::Value& verts, const rapidjson::Value& uvs, GLuint programID)
  {
    model = glm::mat4 (1.0f);
    // usually scale, rotate, then translate
    /*
    model = glm::translate (model, glm::vec3 (x_pos, 0.0f, -20.0f));
    model = glm::scale (model, glm::vec3 (4.0f, 4.0f, 0.0f));
    model = glm::rotate (model, 0.4f, glm::vec3 (1.0f, 1.0f, 0.0f));
    */
    glGenBuffers(2, vbo);
#ifndef EMSCRIPTEN
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
#endif

    assert(verts . IsArray ());
    
    GLfloat vertex_buffer_data[verts . Size ()];
    for (rapidjson::SizeType i = 0; i < verts . Size (); i++)
    { vertex_buffer_data[i] = verts[i] . GetFloat (); 
    }

    GLfloat uv_buffer_data[uvs . Size ()];
    for (rapidjson::SizeType j = 0; j < uvs . Size (); j++)
    { uv_buffer_data[j] = uvs[j] . GetFloat ();
    }

    _size = verts . Size () / 3;

    printf ("size of verts: %d, size of uvs: %d\n", verts . Size (), uvs . Size ());

    glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer (glGetAttribLocation (programID, "pos"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray (glGetAttribLocation (programID, "pos"));

    glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof (uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer (glGetAttribLocation (programID, "uv"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray (glGetAttribLocation (programID, "uv"));

#ifndef EMSCRIPTEN
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  }

  void draw (GLuint programID)
  {
    model = glm::rotate (model, 0.02f, glm::vec3 (0, 0, 1));
    glUniformMatrix4fv (glGetUniformLocation (programID, "u_model"), 1,
        GL_FALSE, glm::value_ptr (model));

#ifndef EMSCRIPTEN
    glBindVertexArray (vao[0]);
#endif
    glDrawArrays (GL_TRIANGLES, 0, _size);
  }

};
