#include "LoadedMapState.h"
namespace Mer
{
	LoadedMapState::LoadedMapState(ProgramDataReF data) : _data(data)
	{

	}

	void LoadedMapState::Init()
	{
		reader.ReadFile("testFile.geojson");

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(_data->window, false);
		ImGui_ImplOpenGL3_Init("#version 400");
		//ImGui::SetWindowFontScale(20.0f);
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF("Fonts/Atteron.ttf", 18.0f, NULL, NULL);
		

		cellCount = reader.cells.size();

		glGenVertexArrays(cellCount, VAOs);
		glGenBuffers(cellCount, Buffers);

		glUseProgram(program);


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


		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "media/cells.vert" },
			{ GL_FRAGMENT_SHADER, "media/cells.frag" },
			{ GL_NONE, NULL }
		};

		program = LoadShaders(shaders);

	}
	void LoadedMapState::HandleInput()
	{

	}
	void LoadedMapState::Update()
	{
		glfwPollEvents();
	}
	void LoadedMapState::Draw()
	{
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);



		glEnableVertexAttribArray(0);



		for (int i = 0; i < cellCount; i++)
		{
			if (mapmode == 1)
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
			}
			else if(mapmode == 0)
			{
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



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (ImGui::Button("Biomes"))
		{
			std::cout << "Button Pressed" << std::endl;
			mapmode = 1;
		}
		if (ImGui::Button("Land"))
		{
			std::cout << "Button Pressed" << std::endl;
			mapmode = 0;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(_data->window);
	}
	void LoadedMapState::CleanUp()
	{
		glDeleteBuffers(cellCount, Buffers);
		glDeleteVertexArrays(cellCount, VAOs);
	}
}