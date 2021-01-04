#pragma once
#include "State.h"
#include "Program.h"
#include "Reader.h"

namespace Mer
{
	class LoadedMapState : public State
	{
	public:
		LoadedMapState(ProgramDataReF data);

		void Init();
		void HandleInput();
		void Update();
		void Draw();
		void CleanUp();
	private:
		ProgramDataReF _data;

		enum VAO_IDs { Cell1, Cell2, Colours, Tex, NumVAOs = 6000 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 6000 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];
		GLuint colorBuffers[NumBuffers];

		int mapmode = 1;

		GLuint program;
		GLuint program2;

		Reader reader;

		int activeCell = 0;
		int cellCount = 5000;

		char filename[128] = "testFile.geojson";

		bool loadfile = false;

		GLfloat color[3] = { 0.0f,0.0f,1.0f };

	};
}


