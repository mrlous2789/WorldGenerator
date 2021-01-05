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

		wm.Generate(cellCount,numOfHighIslands,numOfLowIslands,numOfNations, numOfCultures, numOfReligions);



		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumBuffers, Buffers);
		glGenBuffers(NumBuffers, borderBuffers);

		glPointSize(1.0f);
		

		for (int i = 0; i < wm.cells.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
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

		selectedCell = &wm.cells[0];

		glUseProgram(cellsShader);

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the view matrix
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the projection matrix
		projection = glm::perspective(90.0f, 1.0f, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		mvp = model * view * projection;


		//adding the Uniform to the shader
		int mvpLoc = glGetUniformLocation(cellsShader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));


		glfwGetWindowSize(_data->window, &windowW, &windowH);
	}
	void GeneratedMapState::HandleInput()
	{

	}
	void GeneratedMapState::Update()
	{
		if (generateNew)
		{
			wm.Generate(cellCount, numOfHighIslands, numOfLowIslands, numOfNations, numOfCultures, numOfReligions);
			for (int i = 0; i < wm.cells.size(); i++)
			{
				glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
				glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
				glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
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

				selectedCell = wm.getCellAtCoords(xpos, ypos);

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


		for (int i = 0; i < wm.cells.size(); i++)
		{
			if (mapmode == 1)
			{
				Nation nat = wm.getNationById(wm.cells[i].state);
				if (nat.id == -1)
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
				else
				{
					color[0] = wm.getNationById(wm.cells[i].state).colour[0];
					color[1] = wm.getNationById(wm.cells[i].state).colour[1];
					color[2] = wm.getNationById(wm.cells[i].state).colour[2];
				}

			}
			else if (mapmode == 2)
			{
				Culture cult = wm.getCultureById(wm.cells[i].culture);
				if (cult.id == -1)
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
				else
				{
					color[0] = cult.colour[0];
					color[1] = cult.colour[1];
					color[2] = cult.colour[2];
				}

			}
			else if (mapmode == 3)
			{
				Religion rel = wm.getReligionById(wm.cells[i].religion);
				if (rel.id == -1)
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
				else
				{
					color[0] = rel.colour[0];
					color[1] = rel.colour[1];
					color[2] = rel.colour[2];
				}
			}
			else
			{
				if (wm.cells[i].id == selectedCell->id && selectedCell != nullptr)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else if (wm.cells[i].height > 8000)
				{
					color[0] = 1.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
				}
				else if (wm.cells[i].height > 6000)
				{
					color[0] = 1.0f;
					color[1] = 0.18f;
					color[2] = 0.18f;
				}
				else if (wm.cells[i].height > 3000)
				{
					color[0] = 1.0f;
					color[1] = 0.4f;
					color[2] = 0.4f;
				}
				else if (wm.cells[i].height > 1000)
				{
					color[0] = 1.0f;
					color[1] = 0.7f;
					color[2] = 0.7f;
				}
				else if (wm.cells[i].height > 0)
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

			int mvpLoc = glGetUniformLocation(cellsShader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, wm.cells[i].coords.size());


			if (showCellBorders && wm.cells[i].height > 0)
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 0.0f;

				myLoc = glGetUniformLocation(cellsShader, "color");
				glProgramUniform3fv(cellsShader, myLoc, 1, color);

				glBindBuffer(GL_ARRAY_BUFFER, borderBuffers[i]);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glDrawArrays(GL_LINE_LOOP, 0, wm.cells[i].coords.size());
			}



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