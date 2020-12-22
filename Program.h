#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr

#include "Reader.h"

namespace Mer
{
	class Program
	{
	public:
		Program();
	private:
		void Run();
		bool Init();
		bool InitOpenGL();

		void Draw();

		GLFWwindow* window;

		Reader reader;

		enum VAO_IDs { Cells, Indices, Colours, Tex, NumVAOs = 1 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];
		GLuint texture1;

		GLuint program;

		int activeCell = 0;



#define BUFFER_OFFSET(a) ((void*)(a))

	};
}

