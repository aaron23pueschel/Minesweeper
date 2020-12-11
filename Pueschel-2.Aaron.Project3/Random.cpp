#include "Random.h"
#include <ctime>
std::mt19937 Random::random(time(0));

int Random::Int(int x_range, int y_range) {
	std::uniform_int_distribution<int> dist(x_range, y_range);
	return dist(random);

}
