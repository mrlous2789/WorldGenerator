#include "Program.h"
#include "LoadedMapState.h"
#include "GeneratedMapState.h"
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

		_data->window = glfwCreateWindow(1366, 768, "Project", NULL, NULL);
		glfwMakeContextCurrent(_data->window);
		glewInit();

		return true;
	}


	void Program::CleanUp()
	{
		glfwDestroyWindow(_data->window);

		glfwTerminate();
	}
}