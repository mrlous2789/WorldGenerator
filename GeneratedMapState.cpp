#include "GeneratedMapState.h"
namespace Mer
{
	GeneratedMapState::GeneratedMapState(ProgramDataReF data) : _data(data)
	{

	}
	void GeneratedMapState::Init()
	{

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(_data->window, false);
		ImGui_ImplOpenGL3_Init("#version 400");
		//ImGui::SetWindowFontScale(20.0f);
		ImGuiIO& io = ImGui::GetIO();

		//io.Fonts->AddFontFromFileTTF("Fonts/Atteron.ttf", 18.0f, NULL, NULL);

		wg.Generate(cellCount,numOfHighIslands,numOfLowIslands,numOfNations);



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
		if (generateNew)
		{
			wg.Generate(cellCount, numOfHighIslands, numOfLowIslands, numOfNations);
			for (int i = 0; i < wg.cells.size(); i++)
			{
				glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
				glBufferData(GL_ARRAY_BUFFER, wg.cells[i].coords.size() * sizeof(glm::vec3), &wg.cells[i].coords.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
				glBufferData(GL_ARRAY_BUFFER, wg.cells[i].coords.size() * sizeof(glm::vec3), &wg.cells[i].coords.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			}
			generateNew = false;
		}
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
			if (mapmode == 1)
			{
				if (wg.cells[i].capital)
				{
					color[0] = 1.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
				}
				else
				{
					switch (wg.cells[i].state)
					{
					case 1:
						color[0] = 0.61f;
						color[1] = 0.18f;
						color[2] = 0.18f;
						break;
					case 2:
						color[0] = 0.77f;
						color[1] = 0.86f;
						color[2] = 0.29f;
						break;
					case 3:
						color[0] = 0.31f;
						color[1] = 0.74f;
						color[2] = 0.93f;
						break;
					case 4:
						color[0] = 0.69f;
						color[1] = 0.22f;
						color[2] = 0.53f;
						break;
					case 5:
						color[0] = 0.91f;
						color[1] = 0.86f;
						color[2] = 0.51f;
						break;
					case 6:
						color[0] = 0.17f;
						color[1] = 0.64f;
						color[2] = 0.54f;
						break;
					case 7:
						color[0] = 0.34f;
						color[1] = 0.15f;
						color[2] = 0.11f;
						break;
					case 8:
						color[0] = 0.98f;
						color[1] = 0.35f;
						color[2] = 0.71f;
						break;
					case 9:
						color[0] = 0.37f;
						color[1] = 0.77f;
						color[2] = 0.47f;
						break;
					case 10:
						color[0] = 0.56f;
						color[1] = 1.0f;
						color[2] = 0.15f;
						break;
					case 11:
						color[0] = 0.86f;
						color[1] = 0.34f;
						color[2] = 0.34f;
						break;
					case 12:
						color[0] = 1.0f;
						color[1] = 1.0f;
						color[2] = 0.06f;
						break;
					default:
						color[0] = 0.0f;
						color[1] = 0.0f;
						color[2] = 1.0f;
						break;
					}
				}
			}
			else
			{

				if (wg.cells[i].height > 8000)
				{
					color[0] = 1.0f;
					color[1] = 0.0f;
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
				else if (wg.cells[i].height > 0)
				{
					color[0] = 0.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
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


			if (showCellBorders && wg.cells[i].height > 0)
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 0.0f;

				myLoc = glGetUniformLocation(cellsShader, "color");
				glProgramUniform3fv(cellsShader, myLoc, 1, color);

				glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			}


			glDrawArrays(GL_LINE_LOOP, 0, wg.cells[i].coords.size());
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(cellsShader);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (ImGui::Button("Nations"))
		{
			std::cout << "Button Pressed" << std::endl;
			mapmode = 1;
		}
		if (ImGui::Button("Land"))
		{
			std::cout << "Button Pressed" << std::endl;
			mapmode = 0;
		}
		if (ImGui::Button("New Map") && !generateNew)
		{
			generateNew = true;
		}
		if (ImGui::Button("Show Borders") && !generateNew)
		{
			showCellBorders = !showCellBorders;
		}
		
		ImGui::SliderInt("Number of Cells", &cellCount, 3000, 12000, "%d");
		ImGui::SliderInt("Number of High Islands", &numOfHighIslands, 1, 3, "%d");
		ImGui::SliderInt("Number of Low Islands", &numOfLowIslands, 1, 30, "%d");
		ImGui::SliderInt("Number of Nations", &numOfNations, 3, 25, "%d");


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(_data->window);
	}
	void GeneratedMapState::CleanUp()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteVertexArrays(NumBuffers, VAOs);
	}
}