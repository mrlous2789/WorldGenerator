#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"

namespace Mer
{
	class Cell
	{
	public:
		Cell();
		Cell(std::vector<glm::vec3> inCoords, int inId, int inHeight, int inBiome, std::string inType, int inPopulation, int inState, int inProvince, int inCulture, int inReligion, std::vector<int> inNeighbors);
		void PrintProperties();

		std::vector<glm::vec3> coords;//coords of the cells vertices
		int id = -1;//cell id
		float height = 0;//hieght of cell
		int biome = -1;//biome id
		std::string type;//type of cells e.g. ocean or land
		int population = 0;//cells population
		int state = -1;//cell nation state id 
		int province = -1;//cell province
		int culture = -1;//cell culture
		int religion = -1;//cell religion
		std::vector<int> neighbors;//neighboring cell ids

		void AddCoords(float x, float y);
		
		bool edited = false;
		bool added = false;
		bool nation = false;
		bool capital = false;
		bool suitable = false;
		bool hasReligion = false;
		bool hasCulture = false;

		void NormaliseCoords(float xDiff, float yDiff, float xEdge, float yEdge);

	private:

	};
}
