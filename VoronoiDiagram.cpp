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
		FastNoiseLite noise;

		noise.SetFrequency(0.1f);
		noise.SetFractalLacunarity(0.1f);

		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		
		std::srand(time(NULL));
		int offset = rand();

		auto diagram = GenerateDiagram(points);

		for (size_t i = 0; i < 4; i++)
		{
			points = diagram.computeLloydRelaxation();
			diagram = GenerateDiagram(points);
		}

		diagram.intersect(mygal::Box<double>{-1, -1, 1, 1});

		auto triangulation = diagram.computeTriangulation();

		for (const auto& site : diagram.getSites())
		{
			auto center = site.point;
			auto face = site.face;
			auto halfedge = face->outerComponent;
			if (halfedge == nullptr)
				continue;

			while (halfedge->prev != nullptr)
			{
				halfedge = halfedge->prev;
				if (halfedge == face->outerComponent)
				{
					break;
				}
			}

			auto start = halfedge;
			Cell temp = Cell();
			temp.height = noise.GetNoise((site.point.x * 20) + offset, (site.point.y * 20) + offset);
			//std::cout << noise.GetNoise(site.point.x, site.point.y) << std::endl;
			while (halfedge != nullptr)
			{
				if (halfedge->origin != nullptr && halfedge->destination != nullptr)
				{
					temp.AddCoords(halfedge->origin->point.x, halfedge->origin->point.y);
				}
				halfedge = halfedge->next;
				if (halfedge == start)
					break;
			}
			cells.push_back(temp);
			//std::cout << temp.height << std::endl;
		}
	}
	template<typename T>
	mygal::Diagram<T> VoronoiDiagram::GenerateDiagram(const std::vector<mygal::Vector2<T>>& points)
	{
		auto algorithm = mygal::FortuneAlgorithm<double>(points);
		algorithm.construct();
		algorithm.bound(mygal::Box<double>{-1.05, -1.05, 1.05, 1.05});
		auto diagram = algorithm.getDiagram();
		diagram.intersect(mygal::Box<double>{-1, -1, 1, 1});
		return diagram;
	}

}