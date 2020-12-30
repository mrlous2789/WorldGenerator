#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <random>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "FortuneAlgorithm.h"
#include "Cell.h"
#include "FastNoiseLite.h"

namespace Mer
{
	class VoronoiDiagram
	{
	public:
		VoronoiDiagram();

		void GenerateSites(int numSites);

		std::vector<glm::vec2> getSites();
		void outputSites();
		void Compute();

		std::vector<Cell> cells;

	private:

		
		std::vector<glm::vec2> sites;

		template<typename T>
		mygal::Diagram<T> GenerateDiagram(const std::vector<mygal::Vector2<T>>& points);

		std::vector<mygal::Vector2<double>> points;

		std::vector<mygal::Vector2<double>> relaxedPoints;
		std::vector <glm::vec2> convertedRelaxedPoints;

	};
}


