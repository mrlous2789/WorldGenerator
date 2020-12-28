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
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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


		enum VAO_IDs { Cell1, Cell2, Colours, Tex, NumVAOs = 6000 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 6000 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];
		GLuint colorBuffers[NumBuffers];
		GLuint texture1;


		//std::vector<GLuint> VAOs;
		//std::vector<GLuint> Buffers;

		bool mapmode = true;

		GLuint program;
		GLuint program2;

		int activeCell = 0;
		int cellCount = 5000;

		GLfloat color[3] = { 0.0f,0.0f,1.0f };

#define BUFFER_OFFSET(a) ((void*)(a))

	};
}

