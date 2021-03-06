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
#include "StateMachine.h"



namespace Mer
{
	struct ProgramData //data that the whole program will need state machine became irrelavant as i managed to fit everything i wanted into one state
	{
		StateMachine machine;
		GLFWwindow* window;
	};

	typedef std::shared_ptr<ProgramData> ProgramDataReF;

	class Program
	{
	public:
		Program();
	private:
		void Run();
		bool Init();
		void CleanUp();

		GLFWimage icons[2];

		char icon16x16Filename[128] = "media\\icon\\icon16x16.png";//icon file paths
		char icon32x32Filename[128] = "media\\icon\\icon32x32.png";

		ProgramDataReF _data = std::make_shared<ProgramData>();

#define BUFFER_OFFSET(a) ((void*)(a))//i dont know what this does it came from the original code i got but im not removing it as everthing works

	};
}

