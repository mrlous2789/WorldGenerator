#include "Program.h"
#include "LoadedMapState.h"
#include "GeneratedMapState.h"


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
namespace Mer
{
	Program::Program()
	{
		if (Init())
		{
			this->_data->machine.AddState(StateRef(new GeneratedMapState(_data)));
			Run();
		}
	}
	void Program::Run()
	{
		//UINT32 newTime, frameTime, interpolation;



		while (!glfwWindowShouldClose(_data->window))
		{
			this->_data->machine.ProcessStateChanges();


			this->_data->machine.GetActiveState()->HandleInput();
			this->_data->machine.GetActiveState()->Update();
			this->_data->machine.GetActiveState()->Draw();
		}
		CleanUp();
	}
	bool Program::Init()
	{
		glfwInit();

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		_data->window = glfwCreateWindow(1366, 768, "Project", NULL, NULL);
		glfwMakeContextCurrent(_data->window);
		glewInit();

		icons[0].pixels = stbi_load(icon16x16Filename, &icons[0].width, &icons[0].height, 0, 4);
		icons[1].pixels = stbi_load(icon32x32Filename, &icons[1].width, &icons[1].height, 0, 4);

		glfwSetWindowIcon(_data->window, 2, icons);

		return true;
	}


	void Program::CleanUp()
	{
		glfwDestroyWindow(_data->window);

		glfwTerminate();
	}
}