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
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;
//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool modelling_mode = false;
bool inspection_mode = true;

int mywindow_width = 512;
int mywindow_height = 512;

glm::vec4 polygon[1000];
glm::vec4 polygon_color[1000];
int num_points = 0;

glm::vec4 single_polygon[50];
glm::vec4 single_polygon_color[50];
int single_num_points = 0;

double zcor = 0.0;
glm::vec4 polygon_centroid = glm::vec4(xpos, ypos, zcor, 1.0);

double color_R=0.5, color_G=0.5, color_B=0.5;

double xtrans=0.0, ytrans=0.0, ztrans=0.0;

GLuint model_vbo, model_vao;

GLuint vPosition;
GLuint vColor;
//-------------------------------------------------------------------------

#endif
