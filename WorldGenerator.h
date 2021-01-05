#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <random>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <queue>
#include "FortuneAlgorithm.h"
#include "Cell.h"
#include "Nation.h"
#include "Religion.h"
#include "Culture.h"


namespace Mer
{
	

	class WorldGenerator
	{
	public:
		WorldGenerator();

		std::vector<Cell> Generate(int numSites);

		//std::vector<Cell> cells;
		void GenerateHeight(int numOfHighIslands, int numOfLowIslands, std::vector<Cell>* cells);

		std::vector<Nation> GenerateNations(int numOfNations, std::vector<Cell>* cells);
		std::vector<Culture> GenerateCultures(int numOfCultures, std::vector<Cell>* cells);
		std::vector<Religion> GenerateReligions(int numOfReligions, std::vector<Cell>* cells);
		
	private:
		std::vector<mygal::Vector2<double>> GenerateSites(int numSites);


		template<typename T>
		mygal::Diagram<T> GenerateDiagram(const std::vector<mygal::Vector2<T>>& points);




	};
}


