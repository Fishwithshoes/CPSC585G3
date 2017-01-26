#pragma once

//Standard libraries
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>

//For Windows (glew) or UNIX (direct to gl)?
#ifdef _WIN32
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

//Program specific middleware
#include <GLFW/glfw3.h>
#ifdef _WIN32 //Only For - Seulement Pour VS 2015. Get from middleware directory
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#else //UNIX. Get from local directory
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#endif

//Additional user-defined dependencies (Include absurdly common headers here)
#include "Structs.h"
#include "Mathf.h"
#include "Random.h"
