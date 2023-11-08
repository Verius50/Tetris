#include "AI.h"

void AI::InitRand()
{
	srand(time(0));
	for (size_t i = 0; i < width * height * layerSize; i++)
		inputWeights[i / (width * height)][i % (width * height)] = (rand() % 100) / 100.0;
	//for (size_t i = 0; i < (numberOfLayers - 1) * layerSize * layerSize; i++)
		//layersWeights[][i / (width * height)][i % (width * height)] = (rand() % 100) / 100.0;
}

int AI::InputData(double data[width * height])
{
	return 0;
}

void AI::SaveWeights()
{
}

void AI::InitBestWeights(std::string path)
{
}
