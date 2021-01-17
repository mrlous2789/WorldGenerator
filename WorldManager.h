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
		bool LoadFromFile(std::string cellFile, std::string nationFile, std::string cultureFile, std::string religionFile);

		Cell* getCellAtCoords(double xpos, double ypos);

		//map data
		std::vector<Cell> cells;
		std::vector<Nation> nations;
		std::vector<Culture> cultures;
		std::vector<Religion> religions;
		
		Nation getNationById(int id);
		Culture getCultureById(int id);
		Religion getReligionById(int id);

		void SaveMap(std::string mapname);
		
		void deleteNation(int index);
		void deleteCulture(int index);
		void deleteReligion(int index);
		
		void addNation();
		void addCulture();
		void addReligion();
	private:
		
		WorldGenerator wg;
		Reader reader;

		void saveWorld(std::string mapname);
		void saveNations(std::string mapname);
		void saveCultures(std::string mapname);
		void saveReligions(std::string mapname);

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);

	};
}


