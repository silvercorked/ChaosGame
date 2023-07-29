#pragma once

#include <utility> // pair
#include <stdexcept> // invalid_arugment
#include <random>
#include <string>
#include "Bitmap.hpp"

struct Pair_F {
	float x;
	float y;
	Pair_F(float x1, float y1) : x(x1), y(y1) {}
};

using Transformation = Pair_F(*)(const Pair_F&);

using WeightedTransformation = std::pair<Transformation, unsigned int>;

// used to generate Sierpinsky's triangle
const WeightedTransformation SierpinskyTransformations[3] = {
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				(0 + p.x) / 2,
				(0 + p.y) / 2
			);
		},
		1
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				(1 + p.x) / 2,
				(0 + p.y) / 2
			);
		},
		1
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				(0.5 + p.x) / 2,
				(1 + p.y) / 2
			);
		},
		1
	)
};

// used to generate Barnsley's fern
const WeightedTransformation BarnsleyTransformations[4] = {
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				0,
				0.16 * p.y
			);
		},
		1
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				0.85 * p.x + (0.04 * p.y),
				-0.04 * p.x + (0.85 * p.y) + 1.6
			);
		},
		85
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				0.2 * p.x - (0.26 * p.y),
				0.23 * p.x + (0.22 * p.y) + 1.6
			);
		},
		7
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				-0.15 * p.x + 0.28 * p.y,
				0.26 * p.x + (0.24 * p.y) + 0.44
			);
		},
		7
	)
};

// used to generate the Dragon curve
const WeightedTransformation DragonCurveTransformations[2] = {
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				0.5 * p.x - 0.5 * p.y,
				0.5 * p.x + 0.5 * p.y
			);
		},
		1
	),
	std::make_pair(
		[](const Pair_F& p) -> Pair_F {
			return Pair_F(
				-0.5 * p.x + 0.5 * p.y + 1,
				-0.5 * p.x - 0.5 * p.y
			);
		},
		1
	)
};
class Chaos {
	Bitmap bmp;
	size_t bmpSize;
	std::mt19937& gen;
	WeightedTransformation* transformations;
	size_t numTransformations;
	Pair_F movingVar;
	
	public:
		Chaos(unsigned int, std::mt19937&, WeightedTransformation*, size_t);
		auto generate(unsigned int) -> void;
		auto save(std::string) -> void;
	private:
		auto setPoint(Pair_F&, Transformation) -> void;
		auto selectRandomlyWeighted(
			const WeightedTransformation* w,
			size_t n,
			unsigned int tWeight = 0
		) -> const Transformation&;
};

Chaos::Chaos(
	unsigned int bmpSize,
	std::mt19937& rGen,
	WeightedTransformation* transforms,
	size_t numTransforms
) :
	bmpSize(bmpSize)
	, transformations(transforms)
	, numTransformations(numTransforms)
	, gen(rGen)
	, movingVar(Pair_F(0.5, 0.5))
{
	this->bmp.create(bmpSize, bmpSize);
	this->bmp.clear(255);
}

auto Chaos::generate(unsigned int runs) -> void {
	unsigned int cnt = 0, tWeight = 0, i;
	for (auto i = 0; i < numTransformations; i++)
		tWeight += transformations[i].second;
	while (cnt++ < runs) {
		setPoint(
			this->movingVar,
			this->selectRandomlyWeighted(
				transformations,
				numTransformations,
				tWeight
			)
		);
	}
}
auto Chaos::save(std::string s) -> void {
	this->bmp.saveBitmap(s);
}

auto Chaos::setPoint(Pair_F& o, Transformation f) -> void {
	o = f(o);
	// sierpinski offset (none)
	//float d1 = o.x;
	//float d2 = o.y;
	// barnsley offset
	float d1 = 200 + (o.x * ((float)bmpSize) / 8);
	float d2 = ((float)bmpSize) - (o.y * ((float)bmpSize) / 8);
	// dragon offset
	//float d1 = 200 + (o.x * ((float)bmpSize) / 3);
	//float d2 = 300 + (o.y * ((float)bmpSize) / 3);
	SetPixel(
		this->bmp.getDC(),
		d1,
		d2,
		RGB(255, 0, 0)
	);
}
auto Chaos::selectRandomlyWeighted(
	const WeightedTransformation* w,
	size_t n,
	unsigned int tWeight
) -> const Transformation& {
	unsigned int totalWeight = tWeight;
	if (tWeight == 0) // zero is an invalid total weight
		for (auto i = 0; i < n; i++)
			totalWeight += w[i].second;

	std::uniform_int_distribution<> dist(0, totalWeight - 1);
	unsigned int randVal = dist(gen);

	unsigned int runningWeight = 0;
	for (auto i = 0; i < n; i++) {
		runningWeight += w[i].second;
		if (randVal < runningWeight)
			return w[i].first;
	}
	// if we get here, something went crazy bad, like overflow (or all weights are zero)
	throw std::invalid_argument("Something went bad selecting a random weighted transformation");
}

