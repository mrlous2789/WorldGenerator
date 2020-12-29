#include "GeneratedMapState.h"
namespace Mer
{
	GeneratedMapState::GeneratedMapState(ProgramDataReF data) : _data(data)
	{

	}
	void GeneratedMapState::Init()
	{
		vd.GenerateSites(NumBuffers);
		vd.Compute();
		vd.ConvertData();
		
		//vd.outputSites();



		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumBuffers, Buffers);

		glPointSize(1.0f);
		

		for (int i = 0; i < vd.getRelaxedPoints().size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), &vd.getRelaxedPoints()[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		
		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "media/points.vert" },
			{ GL_FRAGMENT_SHADER, "media/points.frag" },
			{ GL_NONE, NULL }
		};

		program = LoadShaders(shaders);
	}
	void GeneratedMapState::HandleInput()
	{

	}
	void GeneratedMapState::Update()
	{
		glfwPollEvents();
	}
	void GeneratedMapState::Draw()
	{
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		for (int i = 0; i < vd.getRelaxedPoints().size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_POINTS, 0, 2);
		}



		glDisableVertexAttribArray(0);
		glUseProgram(program);

		glfwSwapBuffers(_data->window);
	}
	void GeneratedMapState::CleanUp()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteVertexArrays(NumBuffers, VAOs);
	}
}