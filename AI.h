#pragma once
#include <iostream>
const int width = 10, height = 20, cubeSize = 24, layerSize = 100, numberOfLayers = 2;

class AI
{
private:
	double inputWeights[width * height][layerSize], layersWeights[numberOfLayers - 1][layerSize][layerSize],
		outputWeights[layerSize][5], layersValues[numberOfLayers][layerSize];
public:
	void InitRand();
	int InputData(double data[width * height]);

	//void print();
	void SaveWeights();
	void InitBestWeights(std::string path);

};