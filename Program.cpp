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
			glGenVertexArrays(NumVAOs, VAOs);
			glBindVertexArray(VAOs[Triangles]);

			ShaderInfo  shaders[] =
			{
				{ GL_VERTEX_SHADER, "media/triangles.vert" },
				{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
				{ GL_NONE, NULL }
			};

			GLuint program = LoadShaders(shaders);
			glUseProgram(program);

			glGenBuffers(NumBuffers, Buffers);

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(reader.cells[0].coords), &reader.cells[0].coords, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);
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

		glBindVertexArray(VAOs[Triangles]);

	}
}