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

		void Generate(int numSites, int numOfHighIslands, int numOfLowIslands, int numOfNations, int numOfCultures, int numOfReligions);

		std::vector<Cell> cells;

		int getNationCount();
		float getNationRed(int id);
		float getNationGreen(int id);
		float getNationBlue(int id);

		int getCultureCount();
		float getCultureRed(int id);
		float getCultureGreen(int id);
		float getCultureBlue(int id);

		int getReligionCount();
		float getReligionRed(int id);
		float getReligionGreen(int id);
		float getReligionBlue(int id);

		float getHeightofCellatCoords(double xpos, double ypos);
		
	private:
		std::vector<mygal::Vector2<double>> GenerateSites(int numSites);

		void GenerateHeight(int numOfHighIslands, int numOfLowIslands);
		void GenerateNations(int numOfNations);
		void GenerateCultures(int numOfCultures);
		void GenerateReligions(int numOfReligions);

		template<typename T>
		mygal::Diagram<T> GenerateDiagram(const std::vector<mygal::Vector2<T>>& points);

		struct Nation 
		{
			int id;
			int capitalID;
			float color[3];
		};
		struct Culture
		{
			int id;
			float color[3];
		};
		struct Religion
		{
			int id;
			float color[3];
		};

		std::vector<Nation> nations;
		std::vector<Culture> cultures;
		std::vector<Religion> religions;


	};
}


