#pragma once
#include "State.h"
#include "Program.h"
#include "WorldGenerator.h"

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
		WorldGenerator wg;

		enum VAO_IDs { Cells, Borders, Colours, Tex, NumVAOs = 2 };
		enum Buffer_IDs { ArrayBuffer, NumBuffers = 8000 };
		enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

		GLuint VAOs[NumVAOs];
		GLuint Buffers[NumBuffers];		
		GLuint borderBuffers[NumBuffers];

		GLuint cellsShader;
		GLuint borderShader;

		std::vector<double> coords;

		GLfloat color[3] = { 0.0f,0.0f,1.0f };
	};
}


