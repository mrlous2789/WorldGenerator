#pragma once
#include "State.h"
#include "Program.h"
#include "WorldManager.h"
#include <string>



namespace Mer
{
	class GeneratedMapState : public State
	{
	public:
		GeneratedMapState(ProgramDataReF data);
		void Init();
		void HandleInput();
		void Update();
		void Draw();
		void CleanUp();


	private:
		ProgramDataReF _data;
		WorldManager wm;

		enum VAO_IDs { Cells, Borders, Colours, Tex, NumVAOs = 2 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 12000 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];		
		GLuint borderBuffers[NumBuffers];

		GLuint cellsShader;
		GLuint borderShader;

		std::vector<double> coords;

		int mapmode = 0;
		int cellCount = 8000;
		int numOfNations = 12;
		int numOfHighIslands = 1;
		int numOfLowIslands = 4;
		int numOfCultures = 12;
		int numOfReligions = 12;

		double xpos = 0, ypos = 0;
		int windowH = 0, windowW = 0;
		
		float height = 0;

		bool showCellBorders = true;
		bool generateNew = false;

		Cell* selectedCell;

		const char idchar[10] = "Id: ";
		const char statechar[10] = "State: ";

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 mvp;

		float zoomLevel = 1.0f;
		float zoomRate = 0.05f;
		float minZoom = 1.0f;
		float maxZoom = 2.0f;

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void char_callback(GLFWwindow* window, unsigned int key);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static bool isZoomOut;
		static bool isZoomIn;



		char filename[128] = "testFile.geojson";

		void ZoomOut();
		void ZoomIn();
		
		GLfloat color[3] = { 0.0f,0.0f,1.0f };
	};
}


