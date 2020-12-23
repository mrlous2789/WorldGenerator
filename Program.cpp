#include "Program.h"
namespace Mer
{
	Program::Program()
	{
		if (Init())
		{
			Run();
		}
	}
	void Program::Run()
	{
		while (!glfwWindowShouldClose(window))
		{
			Draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	bool Program::Init()
	{
		if (InitOpenGL())
		{
			if (!reader.ReadFile("testFile.geojson"))
			{
				return false;
			}

			cellCount = reader.cells.size();


			for (int i = 0; i < cellCount; i++)
			{
				reader.cells[i].NormaliseCoords();
			}

			//for (int i = 0; i < reader.cells.size() / 2; i++)
			//{
			//	reader.cells[i].NormaliseCoords();
			//}
			//reader.cells[1].NormaliseCoords();
			//reader.PrintDataByID(activeCell);
			

			glGenVertexArrays(cellCount, VAOs);
			glGenBuffers(cellCount, Buffers);
			


			glUseProgram(program);

			static const GLfloat g_vertex_buffer_data[] = {
			   0.2196f, -0.2232f, 0.0f,
			   0.198f, -0.1908f, 0.0f,
			   0.234f, -0.1656f, 0.0f,
			   0.2808f, -0.1836f, 0.0f,
			   0.2484f,-0.2338f, 0.0f,
			   0.2196,-0.2232f, 0.0f
			};
			static const GLfloat g_vertex_buffer_data_second[] = {
			   0.00036f, -0.2772f, 0.0f,
			   -0.018f, -0.2628f, 0.0f,
			   0.0f, -0.2376f, 0.0f,
			   0.0792f, -0.1872f, 0.0f,
			   0.1332f,-0.1944f, 0.0f,
			   0.0936f,-0.2916f, 0.0f,
			   0.0648f,-0.2988f,0.0f,
			   0.0036f,-0.2772f,0.0f
			};

			glLineWidth(1.0f);



			for (int i = 0; i < cellCount; i++)
			{
				glBindVertexArray(i);
				glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
				glBufferData(GL_ARRAY_BUFFER, reader.cells[i].coords.size() * sizeof(glm::vec3), &reader.cells[i].coords.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			}

			//glBindVertexArray(VAOs[Cell1]);

			//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell1]);
			//glBufferData(GL_ARRAY_BUFFER, reader.cells[activeCell].coords.size() * sizeof(glm::vec3), &reader.cells[activeCell].coords.front(), GL_STATIC_DRAW);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			//glBindVertexArray(VAOs[Cell2]);

			//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell2]);
			//glBufferData(GL_ARRAY_BUFFER, reader.cells[activeCell + 1].coords.size() * sizeof(glm::vec3), &reader.cells[activeCell + 1].coords.front(), GL_STATIC_DRAW);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			//glBindVertexArray(VAOs[Cell1]);

			//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell1]);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data[0], GL_STATIC_DRAW);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			//glBindVertexArray(VAOs[Cell2]);

			//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell2]);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_second), &g_vertex_buffer_data_second[0], GL_STATIC_DRAW);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			

			// creating the model matrix
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

			// creating the view matrix
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

			// creating the projection matrix
			glm::mat4 projection = glm::perspective(90.0f, 4.0f / 3, 0.1f, 20.0f);

			// Adding all matrices up to create combined matrix
			glm::mat4 mvp = projection * view * model;


			//adding the Uniform to the shader
			//int mvpLoc = glGetUniformLocation(program, "mvp");
			//glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
			ShaderInfo  shaders[] =
			{
				{ GL_VERTEX_SHADER, "media/triangles.vert" },
				{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
				{ GL_NONE, NULL }
			};

			program = LoadShaders(shaders);
			
			return true;
		}
		else
		{
			std::cout << "Opengl failed" << std::endl;
			return false;
		}

	}
	bool Program::InitOpenGL()
	{
		glfwInit();

		window = glfwCreateWindow(1920, 1080, "Project", NULL, NULL);
		glfwMakeContextCurrent(window);
		glewInit();

		return true;
	}

	void Program::Draw()
	{
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);



		//glBindVertexArray(VAOs[Cell1]);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		//glBindVertexArray(VAOs[Cell2]);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 8);



		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell1]);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, reader.cells[activeCell].coords.size());
		//glDisableVertexAttribArray(0);



		for (int i = 0; i < cellCount; i++)
		{
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glDrawArrays(GL_TRIANGLE_FAN, 0, reader.cells[i].coords.size());
			glDisableVertexAttribArray(0);
		}



		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cell2]);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, reader.cells[activeCell + 1].coords.size());
		//glDisableVertexAttribArray(0);
	}
}