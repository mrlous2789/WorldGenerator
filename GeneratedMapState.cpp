#include "GeneratedMapState.h"
namespace Mer
{
	GeneratedMapState::GeneratedMapState(ProgramDataReF data) : _data(data)
	{

	}
	void GeneratedMapState::Init()
	{
		wg.GenerateSites(NumBuffers);
		wg.Compute();



		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumBuffers, Buffers);

		glPointSize(1.0f);
		

		for (int i = 0; i < wg.cells.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glBufferData(GL_ARRAY_BUFFER, wg.cells[i].coords.size() * sizeof(glm::vec3), &wg.cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			if (wg.cells[i].height < 0)
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
			}
			else
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}

			GLint myLoc = glGetUniformLocation(program, "color");
			glProgramUniform3fv(program, myLoc, 1, color);
		}
		
		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "media/cells.vert" },
			{ GL_FRAGMENT_SHADER, "media/cells.frag" },
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

		for (int i = 0; i < wg.cells.size(); i++)
		{
			if (wg.cells[i].height < 0.1)
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
			}
			else
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}

			GLint myLoc = glGetUniformLocation(program, "color");
			glProgramUniform3fv(program, myLoc, 1, color);

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



			glDrawArrays(GL_TRIANGLE_FAN, 0, wg.cells[i].coords.size());
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