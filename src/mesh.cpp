
struct Mesh
{

  public:

    glm::mat4 model;
    GLuint vbo[2];
    GLuint vao[1];
    uint _size;

    Mesh(float x_pos, const rapidjson::Value& verts, const rapidjson::Value& uvs)
    {
      model = glm::mat4 ();
      // usually scale, rotate, then translate
      model = glm::translate (model, glm::vec3 (x_pos, 0.0f, -20.0f));
      model = glm::scale (model, glm::vec3 (4.0f, 4.0f, 0.0f));
      model = glm::rotate (model, 0.2f, glm::vec3 (1.0f, 1.0f, 0.0f));

      std::cout << glGenBuffers << std::endl;
      glGenBuffers(2, vbo);
      glGenVertexArrays(1, vao);
      glBindVertexArray(vao[0]);

      assert(verts . IsArray ());
      
      GLfloat vertex_buffer_data[verts . Size ()];
      for (rapidjson::SizeType i = 0; i < verts . Size (); i++)
      {
        vertex_buffer_data[i] = verts[i] . GetFloat (); 
      }

      GLfloat uv_buffer_data[uvs . Size ()];
      for (rapidjson::SizeType i = 0; i < uvs . Size (); i++)
      {
          uv_buffer_data[i] = uvs[i] . GetFloat ();
      }

      _size = verts . Size ();

      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
      glVertexAttribPointer (positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(positionAttributeIndex);

      glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
      glVertexAttribPointer (uvAttributeIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(uvAttributeIndex);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw (GLuint programID)
    {

      model = glm::rotate (model, 0.01f, glm::vec3 (0.0f, 1.0f, 0.0f));
      glUniformMatrix4fv (glGetUniformLocation (programID, "u_model"),
                1, GL_FALSE, glm::value_ptr (model));

      glBindVertexArray (vao[0]);
      glDrawArrays (GL_TRIANGLES, 0, _size);
    }

  private:

    static const uint32_t positionAttributeIndex = 0;
    static const uint32_t uvAttributeIndex = 1;

};
