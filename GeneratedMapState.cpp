#include "GeneratedMapState.h"
namespace Mer
{
	bool GeneratedMapState::isZoomIn = false;
	bool GeneratedMapState::isZoomOut = false;
	GeneratedMapState::GeneratedMapState(ProgramDataReF data) : _data(data)
	{
	}
	void GeneratedMapState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (yoffset < 0)
		{
			isZoomOut = true;
		}
		else if (yoffset > 0)
		{
			isZoomIn = true;
		}
	}
	void GeneratedMapState::char_callback(GLFWwindow* window, unsigned int key)
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			ImGui::GetIO().AddInputCharacter(key);
		}		
	}
	void GeneratedMapState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		std::cout << key << std::endl;
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
		model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
		model = glm::translate(model, glm::vec3(xoffset, yoffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));


		// creating the view matrix
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the projection matrix
		projection = glm::perspective(89.535f, 1.0f, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;


		//adding the Uniform to the shader
		int mvpLoc = glGetUniformLocation(cellsShader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glfwSetScrollCallback(_data->window, scroll_callback);
		glfwSetCharCallback(_data->window, char_callback);
		//glfwSetKeyCallback(_data->window, key_callback);
		glfwGetWindowSize(_data->window, &windowW, &windowH);
	}
	void GeneratedMapState::HandleInput()
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			if (glfwGetKey(_data->window, GLFW_KEY_LEFT_CONTROL) == 1)
			{
				ImGui::GetIO().KeyCtrl = true;
			}
			else
			{
				ImGui::GetIO().KeyCtrl = false;
			}
		}

		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			if (glfwGetKey(_data->window, GLFW_KEY_W) == 1)
				isMoveUp = true;
			if (glfwGetKey(_data->window, GLFW_KEY_S) == 1)
				isMoveDown = true;
			if (glfwGetKey(_data->window, GLFW_KEY_A) == 1)
				isMoveLeft = true;
			if (glfwGetKey(_data->window, GLFW_KEY_D) == 1)
				isMoveRight = true;
		}
		else
		{

		}

	}
	void GeneratedMapState::Update()
	{
		if (isZoomIn)
			ZoomIn();
		if (isZoomOut)
			ZoomOut();
		if (isMoveUp)
			MoveUp();
		if (isMoveDown)
			MoveDown();
		if (isMoveLeft)
			MoveLeft();
		if (isMoveRight)
			MoveRight();

		if (moved)
		{

			// creating the model matrix
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
			model = glm::translate(model, glm::vec3(xoffset, yoffset, 0.0f));
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));


			// Adding all matrices up to create combined matrix
			mvp = projection * view * model;

			moved = false;

		}


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

			selectedCell = &wm.cells[0];
			generateNew = false;
		}


		

		int state = glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			
			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (ImGui::GetIO().WantCaptureMouse)
			{

			}
			else if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				xpos -= (windowW / 2);
				xpos = xpos / (windowW / 2);
				ypos -= (windowH / 2);
				ypos = ypos / (windowH / 2);
				ypos *= -1;

				xpos /= zoomLevel;
				ypos /= zoomLevel;

				xpos -= xoffset;
				ypos -= yoffset;

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


			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;



			if (showCellBorders && wm.cells[i].height > 0)
			{
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
		ImGui::Begin("Map modes");//map modes imgui window
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
		if (ImGui::Button("Show Borders") && !generateNew)
		{
			showCellBorders = !showCellBorders;
		}
		ImGui::End();//end of map modes imgui window
		ImGui::Begin("Settings");//settings imgui window
		ImGui::SliderInt("Number of Cells", &cellCount, 3000, 12000, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of High Islands", &numOfHighIslands, 1, 3, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Low Islands", &numOfLowIslands, 1, 30, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Nations", &numOfNations, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Cultures", &numOfCultures, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Religions", &numOfReligions, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		if (ImGui::Button("New Map") && !generateNew)
			generateNew = true;

		ImGui::InputText("Filename: ", filename, ARRAYSIZE(filename));
		ImGui::End();//end of settings imgui window

		ImGui::Begin("Selected cell"); //Seleted cell imgui window

		ImGui::Text("Id: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->id).c_str());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->height).c_str());

		ImGui::Text("State: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->state).c_str());

		ImGui::Text("Culture: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->culture).c_str());

		ImGui::Text("Religion: "); ImGui::SameLine();
		ImGui::Text(std::to_string(selectedCell->religion).c_str());

		ImGui::Text("Type: "); ImGui::SameLine();
		ImGui::Text(selectedCell->type.c_str());

		ImGui::End();//end of selected cell imgui window

		ImGui::Begin("Edit menu");//edit menu imgui window
		if (ImGui::Button("Edit nations"))
		{
			editStates = true;
			editCultures = false;
			editReligions = false;
		}
		if (ImGui::Button("Edit cultures"))
		{
			editStates = false;
			editCultures = true;
			editReligions = false;
		}
		if (ImGui::Button("Edit religions"))
		{
			editStates = false;
			editCultures = false;
			editReligions = true;
		}
		ImGui::End();//end of edit menu imgui window

		if (editStates)
		{
			ImGui::Begin("States edit"); //state edit imgui window
			static int selected = -1;
			for (int j = 0; j < wm.nations.size(); j++)
			{
				char name[32];
				sprintf_s(name, "Nation %d", j);
				if (ImGui::Selectable(name,selected == j))
				{
					selected = j;
				}
			}
			ImGui::End();//state edit imgui window
		}
		if (editCultures)
		{
			ImGui::Begin("Cultures edit"); //cultures edit imgui window
			ImGui::End();//cultures edit imgui window
		}
		if (editReligions)
		{
			ImGui::Begin("Religions edit"); //religions edit imgui window
			ImGui::End();//religions edit imgui window
		}

		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		

		glfwSwapBuffers(_data->window);
	}
	void GeneratedMapState::CleanUp()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteVertexArrays(NumVAOs, VAOs);
	}

	void GeneratedMapState::ZoomIn()
	{
		if (zoomLevel + zoomRate < maxZoom)
		{
			zoomLevel += zoomRate;

			moved = true;

			isZoomIn = false;
		}
		else
		{
			isZoomIn = false;
		}

	}
	void GeneratedMapState::ZoomOut()
	{
		if (zoomLevel - zoomRate > minZoom)
		{
			zoomLevel -= zoomRate;

			moved = true;

			isZoomOut = false;

			if ((yoffset - (1.0f / zoomLevel)) < -0.99f)
			{
				yoffset = -0.99f + (1.0f / zoomLevel);
			}
			else if ((yoffset + (1.0f / zoomLevel)) > 0.99f)
			{
				yoffset = 0.99f - (1.0f / zoomLevel);
			}
			if ((xoffset + (1.0f / zoomLevel)) > 0.99f)
			{
				xoffset = 0.99f - (1.0f / zoomLevel);
			}
			else if ((xoffset - (1.0f / zoomLevel)) < -0.99f)
			{
				xoffset = -0.99f + (1.0f / zoomLevel);
			}
		}
		else
		{
			isZoomOut = false;
		}
	}
	
	void GeneratedMapState::MoveUp()
	{
		if ((yoffset - (1.0f / zoomLevel)) > -0.99f)
		{
			yoffset -= moveSpeed;
			
			moved = true;

			isMoveUp = false;
		}
		else
		{
			isMoveUp = false;
		}
	}
	void GeneratedMapState::MoveDown()
	{
		if ((yoffset + (1.0f/ zoomLevel)) < 0.99f)
		{
			yoffset += moveSpeed;

			moved = true;

			isMoveDown = false;
		}
		else
		{
			isMoveDown = false;
		}

	}
	void GeneratedMapState::MoveLeft()
	{
		if ((xoffset + (1.0f / zoomLevel)) < 0.99f)
		{
			xoffset += moveSpeed;

			moved = true;

			isMoveLeft = false;
		}
		else
		{
			isMoveLeft = false;
		}

	}
	void GeneratedMapState::MoveRight()
	{
		if ((xoffset - (1.0f / zoomLevel)) > -0.99f)
		{
			xoffset -= moveSpeed;

			moved = true;

			isMoveRight = false;
		}
		else
		{
			isMoveRight = false;
		}
	}
}