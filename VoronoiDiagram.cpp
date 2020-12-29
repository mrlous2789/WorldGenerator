#include "VoronoiDiagram.h"
namespace Mer
{
	VoronoiDiagram::VoronoiDiagram()
	{

	}

	void VoronoiDiagram::GenerateSites(int numSites)
	{
		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine generator(seed);
		std::uniform_real_distribution<double> distribution(-1.0f, 1.0f);



		for (int i = 0; i < numSites; i++)
		{
			//sites.push_back(glm::vec2{ distribution(generator), distribution(generator) });
			points.push_back({ distribution(generator), distribution(generator) });
		}
	}

	std::vector<glm::vec2> VoronoiDiagram::getSites()
	{
		return sites;
	}

	void VoronoiDiagram::outputSites()
	{
		for (int i = 0; i < sites.size(); i++)
		{
			std::cout << "X: " << sites[i].x << " Y: " << sites[i].y << std::endl;
		}
	}

	void VoronoiDiagram::Compute()
	{
		auto algorithm = mygal::FortuneAlgorithm<double>(points);
		algorithm.construct();
		algorithm.bound(mygal::Box<double>{-1.05, -1.05, 1.05, 1.05});
		auto diagram = algorithm.getDiagram();
		diagram.intersect(mygal::Box<double>{-1, -1, 1, 1});
		auto triangulation = diagram.computeTriangulation();
		relaxedPoints = diagram.computeLloydRelaxation();
	}
	void VoronoiDiagram::ConvertData()
	{
		for (int i = 0; i < relaxedPoints.size(); i++)
		{
			convertedRelaxedPoints.push_back({ relaxedPoints[i].x, relaxedPoints[i].y });
		}
	}
	std::vector <glm::vec2> VoronoiDiagram::getRelaxedPoints()
	{
		return convertedRelaxedPoints;
	}

}