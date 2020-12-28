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
			

			glGenVertexArrays(cellCount, VAOs);
			glGenBuffers(cellCount, Buffers);
			glGenBuffers(cellCount, colorBuffers);


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

				if (reader.cells[i].type == "ocean")
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
				glUseProgram(program);
				
			}
			glEnableVertexAttribArray(0);

			

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


		glEnableVertexAttribArray(0);

		for (int i = 0; i < cellCount; i++)
		{
			switch (reader.cells[i].biome)//decide what the color cell should be
			{
			case 1:
				color[0] = 0.98f;
				color[1] = 0.91f;
				color[2] = 0.62f;
				break;
			case 2:
				color[0] = 0.71f;
				color[1] = 0.72f;
				color[2] = 0.53f;
				break;
			case 3:
				color[0] = 0.81f;
				color[1] = 0.82f;
				color[2] = 0.51f;
				break;
			case 4:
				color[0] = 0.78f;
				color[1] = 0.84f;
				color[2] = 0.56f;
				break;
			case 5:
				color[0] = 0.71f;
				color[1] = 0.85f;
				color[2] = 0.37f;
				break;
			case 6:
				color[0] = 0.16f;
				color[1] = 0.74f;
				color[2] = 0.34f;
				break;
			case 7:
				color[0] = 0.49f;
				color[1] = 0.80f;
				color[2] = 0.21f;
				break;
			case 8:
				color[0] = 0.25f;
				color[1] = 0.61f;
				color[2] = 0.26f;
				break;
			case 9:
				color[0] = 0.29f;
				color[1] = 0.42f;
				color[2] = 0.20f;
				break;
			case 10:
				color[0] = 0.59f;
				color[1] = 0.47f;
				color[2] = 0.29f;
				break;
			case 11:
				color[0] = 0.84f;
				color[1] = 0.91f;
				color[2] = 0.92f;
				break;
			case 12:
				color[0] = 0.04f;
				color[1] = 0.57f;
				color[2] = 0.19f;
				break;

			default:
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
				break;
			}

			//pass colors uniform variable to shader 
			GLint myLoc = glGetUniformLocation(program, "color");
			glProgramUniform3fv(program, myLoc, 1, color);
			
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, reader.cells[i].coords.size());



		}
		glDisableVertexAttribArray(0);
		glUseProgram(program);
	}
}