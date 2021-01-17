#include "MapState.h"
namespace Mer
{
	bool MapState::isZoomIn = false;
	bool MapState::isZoomOut = false;
	MapState::MapState(ProgramDataReF data) : _data(data)
	{
	}
	void MapState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
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
	void MapState::char_callback(GLFWwindow* window, unsigned int key)
	{
		if (ImGui::GetIO().WantCaptureKeyboard)//send input to imgui
		{
			ImGui::GetIO().AddInputCharacter(key);
		}		
	}
	void MapState::Init()
	{

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(_data->window, false);
		ImGui_ImplOpenGL3_Init("#version 400");
		//ImGui::SetWindowFontScale(20.0f);
		ImGuiIO& io = ImGui::GetIO();

		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;

		io.Fonts->AddFontFromFileTTF("Fonts/PoiretOne-Regular.ttf", 18.0f, NULL, NULL);

		wm.Generate(cellCount,numOfHighIslands,numOfLowIslands,numOfNations, numOfCultures, numOfReligions);


		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumBuffers, Buffers);
		//glGenBuffers(NumBuffers, borderBuffers);

		glPointSize(1.0f);
		
		
		for (int i = 0; i < wm.cells.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
		glfwGetWindowSize(_data->window, &windowW, &windowH);
	}
	void MapState::HandleInput()
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
			if (glfwGetKey(_data->window, GLFW_KEY_BACKSPACE) == 1)
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]] = 1;
			else
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]] = 0;
			if (glfwGetKey(_data->window, GLFW_KEY_DELETE) == 1)
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Delete]] = 1;
			else
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Delete]] = 0;
			if (glfwGetKey(_data->window, GLFW_KEY_LEFT) == 1)
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow]] = 1;
			else
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow]] = 0;
			if (glfwGetKey(_data->window, GLFW_KEY_RIGHT) == 1)
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_RightArrow]] = 1;
			else
				ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_RightArrow]] = 0;
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

				Cell* temp = wm.getCellAtCoords(xpos, ypos);

				if (selectedCell != temp && temp!=nullptr)
				{
					selectedCell = temp;
					cellChanged = true;
				}
			}
		}

	}
	void MapState::Update()
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

		if (moved)//mvp is calculated after all offsets and zoomlevels so it only has to happen once rather than on every zoom or move
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

		if (cellChanged)//if a new cell has been selected
		{
			//edit cells if the user want it to be
			if (editStates)
			{
				if (selectedNation != -1 && selectedCell->type != "ocean")
				{
					selectedCell->state = wm.nations[selectedNation].id;
				}
			}
			else if (editCultures)
			{
				if (selectedCulture != -1 && selectedCell->type != "ocean")
				{
					selectedCell->culture = wm.cultures[selectedCulture].id;
				}
			}
			else if (editReligions)
			{
				if (selectedReligion != -1 && selectedCell->type != "ocean")
				{
					selectedCell->religion = wm.religions[selectedReligion].id;
				}
			}

			cellChanged = false;

		}


		if (generateNew)//generate new map and buffer data
		{
			wm.Generate(cellCount, numOfHighIslands, numOfLowIslands, numOfNations, numOfCultures, numOfReligions);
			for (int i = 0; i < wm.cells.size(); i++)
			{
				glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
				glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			}

			selectedCell = &wm.cells[0];
			generateNew = false;
		}

		if (loadNewMap)//load new map and buffer data
		{
			if (wm.LoadFromFile(cellFile, nationsFile, cultureFile, religionsFile))
			{
				for (int i = 0; i < wm.cells.size(); i++)
				{
					glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
					glBufferData(GL_ARRAY_BUFFER, wm.cells[i].coords.size() * sizeof(glm::vec3), &wm.cells[i].coords.front(), GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				}
				std::cout << "loaded file" << std::endl;
			}
			else
			{
				std::cout << "Error Loading file" << std::endl;
			}

			selectedCell = &wm.cells[0];
			loadNewMap = false;
		}
		if (savemap)//save map
		{
			wm.SaveMap(mapName);

			savemap = false;
		}
		if (deleteNation)
		{
			if (selectedNation != -1)
			{
				wm.deleteNation(selectedNation);
				if (selectedNation >= wm.nations.size())
				{
					selectedNation--;
				}
			}
			deleteNation = false;
		}
		if (deleteCulture)
		{
			if (selectedCulture != -1)
			{
				wm.deleteReligion(selectedCulture);
				if (selectedCulture >= wm.cultures.size())
				{
					selectedCulture--;
				}
			}
			deleteCulture = false;
		}
		if (deleteReligion)
		{
			if (selectedReligion != -1)
			{
				wm.deleteReligion(selectedReligion);
				if (selectedReligion >= wm.religions.size())
				{
					selectedReligion--;
				}
			}
			deleteReligion = false;
		}

		if (addNation)
		{
			wm.addNation();
			addNation = false;
		}
		if (addCulture)
		{
			wm.addCulture();
			addCulture = false;
		}
		if (addReligion)
		{
			wm.addReligion();
			addReligion = false;
		}

		//change resolution of screen
		if (changeto1080)
		{
			glfwSetWindowSize(_data->window, 1920, 1080);
			glfwGetWindowSize(_data->window, &windowW, &windowH);
			glViewport(0, 0, windowW, windowH);
			changeto1080 = false;
		}
		if (changeto900)
		{
			glfwSetWindowSize(_data->window, 1600, 900);
			glfwGetWindowSize(_data->window, &windowW, &windowH);
			glViewport(0, 0, windowW, windowH);
			changeto900 = false;
		}
		if (changeto768)
		{
			glfwSetWindowSize(_data->window, 1366, 768);
			glfwGetWindowSize(_data->window, &windowW, &windowH);
			glViewport(0, 0, windowW, windowH);
			changeto768 = false;
		}
		if (changeto720)
		{
			glfwSetWindowSize(_data->window, 1280, 720);
			glfwGetWindowSize(_data->window, &windowW, &windowH);
			glViewport(0, 0, windowW, windowH);
			changeto720 = false;
		}

		glfwPollEvents();
	}
	void MapState::Draw()
	{
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);


		for (int i = 0; i < wm.cells.size(); i++)
		{
			//decide what colour to give the cell
			if (mapmode == 1)
			{
				Nation nat = wm.getNationById(wm.cells[i].state);
				if (wm.cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else if (nat.id == -1)
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
				}
				else
				{
					color[0] = nat.colour[0];
					color[1] = nat.colour[1];
					color[2] = nat.colour[2];
				}

			}
			else if (mapmode == 2)
			{
				Culture cult = wm.getCultureById(wm.cells[i].culture);
				if (wm.cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else if (cult.id == -1)
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
				if (wm.cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
				}
				else if (rel.id == -1)
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
				if (wm.cells[i].id == selectedCell->id)
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

			//draw cells
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, wm.cells[i].coords.size());

			//set colour to black for cell borders
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;


			//draw cell borders on land
			if (showCellBorders && wm.cells[i].type == "land" )
			{
				myLoc = glGetUniformLocation(cellsShader, "color");
				glProgramUniform3fv(cellsShader, myLoc, 1, color);

				glDrawArrays(GL_LINE_LOOP, 0, wm.cells[i].coords.size());
			}



		}
		glDisableVertexAttribArray(0);
		glUseProgram(cellsShader);


		//ui code its messy i dont think ui code can never not be messy
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
		ImGui::Begin("Main menu");//main menu imgui window
		ImGui::SliderInt("Number of Cells", &cellCount, 3000, 12000, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of High Islands", &numOfHighIslands, 0, 3, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Low Islands", &numOfLowIslands, 0, 30, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Nations", &numOfNations, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Cultures", &numOfCultures, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Number of Religions", &numOfReligions, 3, 25, "%d", ImGuiSliderFlags_AlwaysClamp);
		if (ImGui::Button("New Map") && !generateNew)
			generateNew = true;
		if (ImGui::Button("Save map as") && !savemap)
			savemap = true;
		ImGui::SameLine(); ImGui::InputText("Map name", mapName, ARRAYSIZE(mapName));
		if (ImGui::Button("Load Map"))
			isLoadingMap = !isLoadingMap;
		if (ImGui::Button("Settings"))
			showSettings = !showSettings;
			
		ImGui::End();//end of main menu imgui window

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
			editStates = !editStates;
			editCultures = false;
			editReligions = false;
			mapmode = 1;
		}
		if (ImGui::Button("Edit cultures"))
		{
			editStates = false;
			editCultures = !editCultures;
			editReligions = false;
			mapmode = 2;
		}
		if (ImGui::Button("Edit religions"))
		{
			editStates = false;
			editCultures = false;
			editReligions = !editReligions;
			mapmode = 3;
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
				ImGui::ColorEdit3(name, wm.nations[j].colour,ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine();
				if (ImGui::Selectable(name,selectedNation == j))
				{					
					selectedNation = j;
				}
			}
			if (ImGui::Button("Delete nation"))
				deleteNation = true;
			ImGui::SameLine();
			if (ImGui::Button("Add nation"))
				addNation = true;
			ImGui::End();//state edit imgui window
		}
		if (editCultures)
		{
			ImGui::Begin("Cultures edit"); //cultures edit imgui window
			for (int j = 0; j < wm.cultures.size(); j++)
			{
				char name[32];
				sprintf_s(name, "Cutlure %d", j);
				ImGui::ColorEdit3(name, wm.cultures[j].colour, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine();
				if (ImGui::Selectable(name, selectedCulture == j))
				{
					selectedCulture = j;
				}
			}
			if (ImGui::Button("Delete culture"))
				deleteCulture = true;
			ImGui::SameLine();
			if (ImGui::Button("Add culture"))
				addCulture = true;
			ImGui::End();//cultures edit imgui window
		}
		if (editReligions)
		{
			ImGui::Begin("Religions edit"); //religions edit imgui window
			for (int j = 0; j < wm.religions.size(); j++)
			{
				char name[32];
				sprintf_s(name, "Religion %d", j);
				ImGui::ColorEdit3(name, wm.religions[j].colour, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				
				ImGui::SameLine();
				if (ImGui::Selectable(name, selectedReligion == j))
				{
					selectedReligion = j;
				}		
			}
			if (ImGui::Button("Delete religion"))
				deleteReligion = true;
			ImGui::SameLine();
			if (ImGui::Button("Add religion"))
				addReligion = true;
			ImGui::End();//religions edit imgui window
		}
		if (isLoadingMap)
		{
			ImGui::Begin("Load file");
			ImGui::InputText("Cell file: ", cellFile, ARRAYSIZE(cellFile));
			ImGui::InputText("Nations file: ", nationsFile, ARRAYSIZE(nationsFile));
			ImGui::InputText("Culture file: ", cultureFile, ARRAYSIZE(cultureFile));
			ImGui::InputText("Religion file: ", religionsFile, ARRAYSIZE(religionsFile));
			if (ImGui::Button("Load files") && !loadNewMap)
				loadNewMap = true;
			ImGui::End();
		}
		if (showSettings)
		{
			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Resolutions"))
			{
				if (ImGui::Button("1920x1080"))
					changeto1080 = true;
				if (ImGui::Button("1600x900"))
					changeto900 = true;
				if (ImGui::Button("1366x768"))
					changeto768 = true;
				if (ImGui::Button("1280x720"))
					changeto720 = true;
			}
			ImGui::End();
		}


		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		

		glfwSwapBuffers(_data->window);
	}
	void MapState::CleanUp()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteVertexArrays(NumVAOs, VAOs);
		
	}


	//map movements functions
	void MapState::ZoomIn()
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
	void MapState::ZoomOut()
	{
		if (zoomLevel - zoomRate > minZoom)
		{
			zoomLevel -= zoomRate;

			moved = true;

			isZoomOut = false;


			//if the new zoom level causes off map to be shown change offset
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
	
	void MapState::MoveUp()
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
	void MapState::MoveDown()
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
	void MapState::MoveLeft()
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
	void MapState::MoveRight()
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