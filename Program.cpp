#include "Program.h"
#include "MapState.h"


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
namespace Mer
{
	Program::Program()
	{
		if (Init())//if init is succesful add the map state to the stack
		{
			this->_data->machine.AddState(StateRef(new MapState(_data)));
			Run();
		}
	}
	void Program::Run()
	{
		while (!glfwWindowShouldClose(_data->window))//main program loop
		{
			this->_data->machine.ProcessStateChanges();


			this->_data->machine.GetActiveState()->HandleInput();
			this->_data->machine.GetActiveState()->Update();
			this->_data->machine.GetActiveState()->Draw();
		}
		CleanUp();//clean up memory before exiting
	}
	bool Program::Init()//init glfw and glew and load icon
	{
		glfwInit();

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		_data->window = glfwCreateWindow(1920, 1080, "Project", NULL, NULL);
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