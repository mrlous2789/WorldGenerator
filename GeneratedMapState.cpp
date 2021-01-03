#include "GeneratedMapState.h"
namespace Mer
{
	GeneratedMapState::GeneratedMapState(ProgramDataReF data) : _data(data)
	{

	}
	void GeneratedMapState::Init()
	{
		wg.Generate(NumBuffers);



		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumBuffers, Buffers);
		glGenBuffers(NumBuffers, borderBuffers);

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

			GLint myLoc = glGetUniformLocation(cellsShader, "color");
			glProgramUniform3fv(cellsShader, myLoc, 1, color);

			glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, wg.cells[i].coords.size() * sizeof(glm::vec3), &wg.cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		
		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "media/cells.vert" },
			{ GL_FRAGMENT_SHADER, "media/cells.frag" },
			{ GL_NONE, NULL }
		};
		ShaderInfo  borderShaders[] =
		{
			{ GL_VERTEX_SHADER, "media/points.vert" },
			{ GL_FRAGMENT_SHADER, "media/points.frag" },
			{ GL_NONE, NULL }
		};
		cellsShader = LoadShaders(shaders);
		borderShader = LoadShaders(borderShaders);
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
		glEnableVertexAttribArray(1);


		for (int i = 0; i < wg.cells.size(); i++)
		{
			if (wg.cells[i].height < 0)
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
			}
			else if (wg.cells[i].height == 9000)
			{
				color[0] = 1.0f;
				color[1] = 0.0f;
				color[2] = 0.0f;
			}
			else if (wg.cells[i].height == 4000)
			{
				color[0] = 1.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}
			else if (wg.cells[i].height > 6000)
			{
				color[0] = 1.0f;
				color[1] = 0.18f;
				color[2] = 0.18f;
			}
			else if (wg.cells[i].height > 3000)
			{
				color[0] = 1.0f;
				color[1] = 0.4f;
				color[2] = 0.4f;
			}
			else if (wg.cells[i].height > 1000)
			{
				color[0] = 1.0f;
				color[1] = 0.7f;
				color[2] = 0.7f;
			}
			else
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}
			//if (wg.cells[i].id == 0)
			//{
			//	color[0] = 0.0f;
			//	color[1] = 0.5f;
			//	color[2] = 0.5f;
			//}
			//for (int j = 0; j < wg.cells[0].neighbors.size(); j++)
			//{
			//	if (wg.cells[i].id == wg.cells[0].neighbors[j])
			//	{
			//		color[0] = 0.5f;
			//		color[1] = 0.5f;
			//		color[2] = 0.5f;
			//	}
			//}


			GLint myLoc = glGetUniformLocation(cellsShader, "color");
			glProgramUniform3fv(cellsShader, myLoc, 1, color);

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, wg.cells[i].coords.size());

			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;

			myLoc = glGetUniformLocation(cellsShader, "color");
			glProgramUniform3fv(cellsShader, myLoc, 1, color);

			glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINE_LOOP, 0, wg.cells[i].coords.size());
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(cellsShader);

		glfwSwapBuffers(_data->window);
	}
	void GeneratedMapState::CleanUp()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteVertexArrays(NumBuffers, VAOs);
	}
}