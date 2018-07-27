
struct Mesh
{

  public:

    glm::mat4 model;
    GLuint vbo[2], vao[1];

    Mesh(float x_pos)
    {
      model = glm::mat4(1.0f);

      model = glm::mat4 ();
      model = glm::translate (model, glm::vec3 (x_pos, 0.0f, -20.0f));
      model = glm::scale (model, glm::vec3 (4.0f, 4.0f, 0.0f));
    }

    void init ()
    {

      glGenBuffers(2, vbo);
      glGenVertexArrays(1, vao);
      glBindVertexArray(vao[0]);

      GLfloat vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
      };

      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
      glVertexAttribPointer (positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(positionAttributeIndex);

      GLfloat uv_buffer_data[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
      };

      glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
      glVertexAttribPointer (uvAttributeIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(uvAttributeIndex);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw (GLuint programID)
    {
      glUniformMatrix4fv (glGetUniformLocation (programID, "u_model"),
                1, GL_FALSE, glm::value_ptr (model));

      glBindVertexArray (vao[0]);
      glDrawArrays (GL_TRIANGLES, 0, 6);
    }

  private:

    static const uint32_t positionAttributeIndex = 0;
    static const uint32_t uvAttributeIndex = 1;

};
