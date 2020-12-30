#pragma once
#include "State.h"
#include "Program.h"
#include "VoronoiDiagram.h"

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
		VoronoiDiagram vd;

		enum VAO_IDs { Cell1, Cell2, Colours, Tex, NumVAOs = 8000 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 8000 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];
		GLuint colorBuffers[NumBuffers];

		GLuint program;

		std::vector<double> coords;

		GLfloat color[3] = { 0.0f,0.0f,1.0f };
	};
}


