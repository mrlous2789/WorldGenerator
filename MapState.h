#pragma once
#include "State.h"
#include "Program.h"
#include "WorldManager.h"
#include <string>


namespace Mer
{
	class MapState : public State
	{
	public:
		MapState(ProgramDataReF data);
		void Init();
		void HandleInput();
		void Update();
		void Draw();
		void CleanUp();


	private:
		ProgramDataReF _data;
		WorldManager wm;

		enum VAO_IDs { Cells, Borders, Colours, Tex, NumVAOs = 2 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 12000, NumRivers = 200 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];		
		GLuint riverBuffers[NumRivers];

		GLuint cellsShader;
		GLuint riverShader;

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


		bool showRivers = false;
		bool showCellBorders = true;
		bool generateNew = false;
		bool savemap = false;
		bool loadNewMap = false;

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
		float maxZoom = 3.0f;

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void char_callback(GLFWwindow* window, unsigned int key);

		static bool isZoomOut;
		static bool isZoomIn;

		float xoffset = 0.0f;
		float yoffset = 0.0f;
		float moveSpeed = 0.01f;

		bool isMoveUp = false;
		bool isMoveDown = false;
		bool isMoveRight = false;
		bool isMoveLeft = false;

		bool showSettings = false;

		bool editStates = false;
		bool editCultures = false;
		bool editReligions = false;

		bool deleteNation = false;
		bool deleteCulture = false;
		bool deleteReligion = false;	

		bool addNation = false;
		bool addCulture = false;
		bool addReligion = false;

		bool isLoadingMap = false;

		bool changeto900 = false;
		bool changeto1080 = false;
		bool changeto768 = false;
		bool changeto720 = false;

		std::vector<bool>selectedNations;

		int selectedNation = -1;
		int selectedCulture = -1;
		int selectedReligion = -1;

		Reader reader;

		bool moved = false;
		bool cellChanged = false;

		char filename[128] = "testFile.geojson";
		char cellFile[128] = ".\\OutputFiles\\mapname.geojson";
		char riversFile[128] = ".\\OutputFiles\\mapname_rivers.geojson";
		char nationsFile[128] = ".\\OutputFiles\\mapname_nations.mer";
		char cultureFile[128] = ".\\OutputFiles\\mapname_cultures.mer";
		char religionsFile[128] = ".\\OutputFiles\\mapname_religions.mer";
		char mapName[128] = "mapname";

		void ZoomOut();
		void ZoomIn();

		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		
		GLfloat color[3] = { 0.0f,0.0f,1.0f };
	};
}