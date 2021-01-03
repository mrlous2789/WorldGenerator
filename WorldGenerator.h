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


namespace Mer
{
	class WorldGenerator
	{
	public:
		WorldGenerator();

		void Generate(int numSites);

		std::vector<Cell> cells;

	private:
		std::vector<mygal::Vector2<double>> GenerateSites(int numSites);

		void GenerateHeight();
		void GenerateNations();

		template<typename T>
		mygal::Diagram<T> GenerateDiagram(const std::vector<mygal::Vector2<T>>& points);



		std::vector<mygal::Vector2<double>> relaxedPoints;
		std::vector <glm::vec2> convertedRelaxedPoints;

	};
}


