#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot;

extern bool modelling_mode;
extern bool inspection_mode;

extern int mywindow_width;
extern int mywindow_height;

extern glm::vec4 polygon[1000];
extern glm::vec4 single_polygon[50];
extern glm::vec4 polygon_centroid;
extern int num_points;
extern int single_num_points;
extern glm::vec4 polygon_color[1000];
extern glm::vec4 single_polygon_color[50];
extern double zcor;

extern double color_R, color_G, color_B;

extern double xtrans, ytrans, ztrans;

extern GLuint model_vbo, model_vao;
extern GLuint vPosition;
extern GLuint vColor;

namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    if(modelling_mode){
      if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if(mods == GLFW_MOD_SHIFT){
          single_num_points--;
          if(single_num_points<0){
            single_num_points = 0;
            num_points--;
            num_points--;
            num_points--;
            if(num_points<0){
              num_points = 0;
            }
          }
          std::cout<<"Total points in current polygon: "<<single_num_points<<std::endl;
          std::cout<<"Total points upto now: "<<num_points<<std::endl;
        }
        else{
          double xpos, ypos;
          glfwGetCursorPos(window, &xpos, &ypos);
          xpos = (xpos/(mywindow_width/2))-1;
          ypos = (ypos/(mywindow_height/2))-1;
          xpos *=2;
          ypos *=2;
          ypos = -ypos;

          if(single_num_points>=50){
            std::cout<<"Atmost 50 points can be used for a single polygon"<<std::endl;
            single_num_points = 19;
          }
          single_polygon[single_num_points] = glm::vec4(xpos, ypos, zcor, 1.0);
          single_polygon_color[single_num_points] = glm::vec4(color_R, color_G, color_B, 1.0);
          single_num_points++;

          std::cout << "Point set at (" << xpos << ", " << ypos <<", "<<zcor<<")"<< std::endl;
          std::cout<<"Total points in current polygon: "<<single_num_points<<std::endl;
          std::cout<<"Total points: "<<num_points<<std::endl;
        }
      }
    }
  }

  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_M && action == GLFW_PRESS){
      inspection_mode = false;
      modelling_mode = true;
      xrot=0.0;
      yrot=0.0;
      zrot=0.0;
      std::cout<<"MODELLING MODE"<<std::endl;
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS){
      modelling_mode = false;
      inspection_mode = true;
      std::cout<<"INSPECTION MODE"<<std::endl;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
      if(inspection_mode){
        yrot -= 1.0;
      }
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
      if(inspection_mode){
        yrot += 1.0;
      }
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
      if(inspection_mode){
        xrot -= 1.0;
      }
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
      if(inspection_mode){
        xrot += 1.0;
      }
    }
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS){
      if(inspection_mode){
        zrot -= 1.0;
      }
      else{
        zcor -=0.1;
        if((zcor<0.01)&&(zcor>-0.01)){
          zcor =0;
        }
        std::cout<<"z coordinate: "<<zcor<<std::endl;
      }
    }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS){
      if(inspection_mode){
        zrot += 1.0;
      }
      else{
        zcor +=0.1;
        if((zcor<0.01)&&(zcor>-0.01)){
          zcor =0;
        }
        std::cout<<"z coordinate: "<<zcor<<std::endl;
      }
    }
    else if (key == GLFW_KEY_K && action == GLFW_PRESS){
      if(inspection_mode){
        // save current coordinates to file
        std::ofstream savefile;
        std::string s;
        std::cout<<"Enter file name(without extension) to save: "<<std::endl;
        std::cin>>s;
        s += ".raw";
        std::cin.clear();
        savefile.open(s.c_str());
        for(int i=0; i<num_points; i++){
          savefile<<polygon[i][0]<<" "<<polygon[i][1]<<" "<<polygon[i][2]<<" ";
          savefile<<polygon_color[i][0]<<" "<<polygon_color[i][1]<<" "<<polygon_color[i][2]<<std::endl;
        }
        savefile.close();
        num_points = 0;
      }
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS){
      if(inspection_mode){
        std::string s;
        std::cout<<"Enter file name(with extension) to laod: "<<std::endl;
        std::cin>>s;
        std::cin.clear();
        std::ifstream loadfile;
        loadfile.open(s.c_str(), std::ifstream::in);
        if(loadfile.is_open()){
          double val1, val2, val3, val4, val5, val6;
          num_points = 0;
          while(loadfile>>val1){
            loadfile>>val2;
            loadfile>>val3;
            loadfile>>val4;
            loadfile>>val5;
            loadfile>>val6;
            polygon[num_points] = glm::vec4(val1, val2, val3, 1.0);
            polygon_color[num_points] = glm::vec4(val4, val5, val6, 1.0);
            num_points++;
          }
          glBindVertexArray(model_vao);
          glBindBuffer (GL_ARRAY_BUFFER, model_vbo);
          glBufferData (GL_ARRAY_BUFFER, num_points*sizeof (polygon[0]) + num_points*sizeof(polygon_color[0]), NULL, GL_STATIC_DRAW);
          glBufferSubData( GL_ARRAY_BUFFER, 0, num_points*sizeof(polygon[0]), polygon );
          glBufferSubData( GL_ARRAY_BUFFER, num_points*sizeof(polygon[0]), num_points*sizeof(polygon_color[0]), polygon_color );
          glEnableVertexAttribArray( vPosition );
          glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
          glEnableVertexAttribArray( vColor );

          glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(num_points*sizeof(polygon[0])) );
        }
      }
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
      if(modelling_mode){
        color_R -= 0.1;
        if(color_R<=0.01){
          color_R = 0.0;
        }
        if(color_R>=0.99){
          color_R = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS){
      if(modelling_mode){
        color_R += 0.1;
        if(color_R<=0.01){
          color_R = 0.0;
        }
        if(color_R>=0.99){
          color_R = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS){
      if(modelling_mode){
        color_G -= 0.1;
        if(color_G<=0.01){
          color_G = 0.0;
        }
        if(color_G>=0.99){
          color_G = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS){
      if(modelling_mode){
        color_G += 0.1;
        if(color_G<=0.01){
          color_G = 0.0;
        }
        if(color_G>=0.99){
          color_G = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS){
      if(modelling_mode){
        color_B -= 0.1;
        if(color_B<=0.01){
          color_B = 0.0;
        }
        if(color_B>=0.99){
          color_B = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS){
      if(modelling_mode){
        color_B += 0.1;
        if(color_B<0.01){
          color_B = 0.0;
        }
        if(color_B>0.99){
          color_B = 1.0;
        }
        std::cout<<"R: "<<color_R<<" G: "<<color_G<<" B: "<<color_B<<std::endl;
      }
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS){
      if(inspection_mode){
        ytrans +=0.1;
        if((ytrans<0.01)&&(ytrans>-0.01)){
          ytrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS){
      if(inspection_mode){
        ytrans -=0.1;
        if((ytrans<0.01)&&(ytrans>-0.01)){
          ytrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS){
      if(inspection_mode){
        xtrans -=0.1;
        if((xtrans<0.01)&&(xtrans>-0.01)){
          xtrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS){
      if(inspection_mode){
        xtrans +=0.1;
        if((xtrans<0.01)&&(xtrans>-0.01)){
          xtrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS){
      if(inspection_mode){
        ztrans -=0.1;
        if((ztrans<0.01)&&(ztrans>-0.01)){
          ztrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS){
      if(inspection_mode){
        ztrans +=0.1;
        if((ztrans<0.01)&&(ztrans>-0.01)){
          ztrans =0;
        }
      }
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS){
      if(modelling_mode){
        if(single_num_points<3){
          std::cout<<"Can't draw a polygon with "<<single_num_points<<" points"<<std::endl;
        }
        else{
          glm::vec4 first = single_polygon[0];
          glm::vec4 first_color = single_polygon_color[0];
          glm::vec4 last = single_polygon[1];
          glm::vec4 last_color = single_polygon_color[1];
          for(int i=2; i<single_num_points; i++){
            if(num_points>997){
              std::cout<<"Model can contain atmost 1000 points"<<std::endl;
            }
            polygon[num_points] = first;
            polygon_color[num_points] = first_color;
            num_points++;
            polygon[num_points] = last;
            polygon_color[num_points] = last_color;
            num_points++;
            polygon[num_points] = single_polygon[i];
            polygon_color[num_points] = single_polygon_color[i];
            num_points++;
            last = single_polygon[i];
            last_color = single_polygon_color[i];
          }
          glBindVertexArray(model_vao);
          glBindBuffer (GL_ARRAY_BUFFER, model_vbo);
          glBufferData (GL_ARRAY_BUFFER, num_points*sizeof (polygon[0]) + num_points*sizeof(polygon_color[0]), NULL, GL_STATIC_DRAW);
          glBufferSubData( GL_ARRAY_BUFFER, 0, num_points*sizeof(polygon[0]), polygon );
          glBufferSubData( GL_ARRAY_BUFFER, num_points*sizeof(polygon[0]), num_points*sizeof(polygon_color[0]), polygon_color );
          glEnableVertexAttribArray( vPosition );
          glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
          glEnableVertexAttribArray( vColor );

          glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(num_points*sizeof(polygon[0])) );
          single_num_points = 0;
        }
      }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS){
      if(inspection_mode){
        xtrans = -(polygon_centroid[0]);
        ytrans = -(polygon_centroid[1]);
        ztrans = -(polygon_centroid[2]);
      }
    }
  }
};  
  


