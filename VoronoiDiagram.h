#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <random>
#include <chrono>
#include <iostream>
#include "FortuneAlgorithm.h"

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

		void ConvertData();

		std::vector <glm::vec2> getRelaxedPoints();
	private:

		
		std::vector<glm::vec2> sites;


		std::vector<mygal::Vector2<double>> points;

		std::vector<mygal::Vector2<double>> relaxedPoints;
		std::vector <glm::vec2> convertedRelaxedPoints;


	};
}


