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

		wg.Generate(cellCount,numOfHighIslands,numOfLowIslands,numOfNations, numOfCultures, numOfReligions);



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

		selectedCell = &wg.cells[0];

		glfwGetWindowSize(_data->window, &windowW, &windowH);
	}
	void GeneratedMapState::HandleInput()
	{

	}
	void GeneratedMapState::Update()
	{
		if (generateNew)
		{
			wg.Generate(cellCount, numOfHighIslands, numOfLowIslands, numOfNations, numOfCultures, numOfReligions);
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


		

		int state = glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			
			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (ImGui::GetIO().WantCaptureMouse)
			{

			}
			else if (xpos >= 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				xpos -= (windowW / 2);
				xpos = xpos / (windowW / 2);
				ypos -= (windowH / 2);
				ypos = ypos / (windowH / 2);
				ypos *= -1;
				selectedCell = wg.getCellAtCoords(xpos, ypos);
			}
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
				if (wg.cells[i].state != -1)
				{
					color[0] = wg.getNationRed(wg.cells[i].state);
					color[1] = wg.getNationGreen(wg.cells[i].state);
					color[2] = wg.getNationBlue(wg.cells[i].state);
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}

			}
			else if (mapmode == 2)
			{
				if (wg.cells[i].culture != -1)
				{
					color[0] = wg.getCultureRed(wg.cells[i].culture);
					color[1] = wg.getCultureGreen(wg.cells[i].culture);
					color[2] = wg.getCultureBlue(wg.cells[i].culture);
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
			}
			else if (mapmode == 3)
			{
				if (wg.cells[i].religion != -1)
				{
					color[0] = wg.getReligionRed(wg.cells[i].religion);
					color[1] = wg.getReligionGreen(wg.cells[i].religion);
					color[2] = wg.getReligionBlue(wg.cells[i].religion);
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
			}
			else
			{
				if (wg.cells[i].id == selectedCell->id && selectedCell != nullptr)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else if (wg.cells[i].height > 8000)
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
		if (ImGui::Button("Land"))
		{
			mapmode = 0;
		}
		if (ImGui::Button("Nations"))
		{
			mapmode = 1;
		}
		if (ImGui::Button("Cultures"))
		{
			mapmode = 2;
		}
		if (ImGui::Button("Religions"))
		{
			mapmode = 3;
		}
		if (ImGui::Button("New Map") && !generateNew)
		{
			generateNew = true;
		}
		if (ImGui::Button("Show Borders") && !generateNew)
		{
			showCellBorders = !showCellBorders;
		}
		
		ImGui::SliderInt("Number of Cells", &cellCount, 3000, 12000, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of High Islands", &numOfHighIslands, 1, 3, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Low Islands", &numOfLowIslands, 1, 30, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Nations", &numOfNations, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Cultures", &numOfCultures, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Religions", &numOfReligions, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Id: ");ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->id).c_str());

		ImGui::Text("Height: ");ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->height).c_str());

		ImGui::Text("State: ");ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->state).c_str());

		ImGui::Text("Culture: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->culture).c_str());

		ImGui::Text("Religion: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->religion).c_str());

		ImGui::Text("Type: "); ImGui::SameLine();
		ImGui::Text(selectedCell->type.c_str());
		
		ImGui::ShowDemoWindow();

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