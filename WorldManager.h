#pragma once
#include "Reader.h"
#include "WorldGenerator.h"
namespace Mer
{
	class WorldManager
	{
	public:
		WorldManager();
		bool Generate(int numOfSites, int numOfHighIslands, int numOfLowIslands, int numOfNations, int numOfCultures, int numOfReligions);
		bool LoadFromFile(std::string cellFile);

		Cell* getCellAtCoords(double xpos, double ypos);

		std::vector<Cell> cells;
		std::vector<Nation> nations;
		std::vector<Culture> cultures;
		std::vector<Religion> religions;
		
		Nation getNationById(int id);
		Culture getCultureById(int id);
		Religion getReligionById(int id);

	private:
		WorldGenerator wg;
		Reader reader;

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);

	};
}


