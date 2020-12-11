#pragma once
#include <iostream>
#include <random>
using namespace std;
class Random
{

	static std::mt19937 random;

public:

	static int Int(int x_range, int y_range);

};

