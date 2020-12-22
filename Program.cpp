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

			reader.cells[activeCell].NormaliseCoords();
			reader.PrintDataByID(activeCell);




			glGenVertexArrays(NumVAOs, VAOs);
			glBindVertexArray(VAOs[Cells]);


			glUseProgram(program);

			static const GLfloat g_vertex_buffer_data[] = {
			   0.2196f, -0.2232f, 0.0f,
			   0.198f, -0.1908f, 0.0f,
			   0.234f, -0.1656f, 0.0f,
			   0.2808f, -0.1836f, 0.0f,
			   0.2484f,-0.2338f, 0.0f,
			   0.2196,-0.2232f, 0.0f
			};

			glLineWidth(1.0f);

			glGenBuffers(NumBuffers, Buffers);

			

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cells]);
			glBufferData(GL_ARRAY_BUFFER, reader.cells[activeCell].coords.size() * sizeof(glm::vec3), &reader.cells[activeCell].coords.front(), GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data, GL_STATIC_DRAW);
			
			//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

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

		window = glfwCreateWindow(800, 600, "Project", NULL, NULL);
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

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[Cells]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, reader.cells[activeCell].coords.size() * 10);
		glDisableVertexAttribArray(0);
		//glBindVertexArray(VAOs[Cells]);
		//glDrawElements(GL_LINE_LOOP, reader.cells[0].coords.size(), GL_UNSIGNED_INT, 0);
	}
}