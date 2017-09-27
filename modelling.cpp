/*
  CS475 Assignment 1

  A program which allows you to draw model using mouse click.
  M for modelling mode
  I for inspection mode

  Use the arrow keys and PgUp,PgDn,
  keys to make the model move.

  Modified from Tutorial 4

  Written by Tejesh Raut and Ashna Gaur, 2017
*/


#include "modelling.hpp"

GLuint shaderProgram;
GLuint vbo[2], vao[2];

glm::mat4 rotation_matrix;
glm::mat4 translation_matrix;
glm::mat4 shift_to_origin_matrix;
glm::mat4 shift_to_centroid_matrix;
glm::mat4 projection_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;


//-----------------------------------------------------------------

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("my_vshader.glsl");
  std::string fragment_shader_file("my_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  
  glGenVertexArrays (1, &model_vao);
  glGenBuffers (1, &model_vbo);

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xtrans, ytrans, ztrans));

  shift_to_origin_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(polygon_centroid[0]+xtrans), -(polygon_centroid[1]+ytrans), -(polygon_centroid[2]+ztrans)));
  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot), glm::vec3(0.0f,0.0f,1.0f));
  shift_to_centroid_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((polygon_centroid[0]+xtrans), (polygon_centroid[1]+ytrans), (polygon_centroid[2]+ztrans)));
  model_matrix = shift_to_centroid_matrix*rotation_matrix*shift_to_origin_matrix*translation_matrix;

  //creating the projection matrix
  projection_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);

  view_matrix = projection_matrix;

  // Draw model points
  modelview_matrix = view_matrix*model_matrix;
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  glBindVertexArray (model_vao);
  glDrawArrays(GL_TRIANGLES, 0, num_points);

}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(mywindow_width, mywindow_height, "CS475/CS675 Assignment 1: Modelling", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //mouse button Callback
  glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);
  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

