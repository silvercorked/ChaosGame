#pragma once

#include <windows.h>
#include <random>

#include "Bitmap.hpp"

#include <iostream>

constexpr unsigned int NUM_BOUNDS = 3;

class Sierpinski {
    Bitmap bmp;
    const POINT bounds[NUM_BOUNDS];
    const COLORREF colors[NUM_BOUNDS];
    POINT movingVar;
    std::mt19937 randomGen;
    const long halfSize;

public:
    Sierpinski(unsigned int, std::mt19937&);
    Sierpinski(Sierpinski&) = delete;
    Sierpinski(Sierpinski&&) = default;
    auto generate(unsigned int) -> void;
    auto run(unsigned int) -> void;
    auto save(std::string) const -> void;
private:
    auto setPoint(POINT&, POINT, int) -> void;
    auto recursiveRun(unsigned int, unsigned int) -> void;
};

Sierpinski::Sierpinski(unsigned int bmpSize, std::mt19937& rGen)
    : colors{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) }
    , halfSize((long) bmpSize >> 1)
    , bounds{ // setup bounds for triangle (top middle, bottom left, bottom right)
        { (long)bmpSize >> 1, 0 },                              // top middle
        { 0, (long) bmpSize - 1 },                              // bottom left
        { (long) bmpSize - 1, (long) bmpSize - 1 }              // bottom right
    }
    , randomGen(rGen)
{
    this->movingVar = { this->halfSize, this->halfSize };
    this->bmp.create(bmpSize, bmpSize);
    this->bmp.clear(255);
}

auto Sierpinski::save(std::string path) const -> void {
    this->bmp.saveBitmap(path);
}
auto Sierpinski::setPoint(POINT& o, POINT v, int i) -> void {
    o.x = (o.x + v.x) >> 1; o.y = (o.y + v.y) >> 1; // avg o (moving var) with v (bounds point)
    SetPixel(this->bmp.getDC(), o.x, o.y, this->colors[i]); // set color on bmp of given point. color driven by i
}
/*
    So, this seems to randomly "eventually" generate all the pixel values
    and paint them. However, there's not necessarily any reason this needs to be random.
    With a Depth, the sequence of bounds selected can generate unique pixel values.
    Ex. Depth: 2 is all sequences of length 2 of the bounds (combinations of bounds)
    {
        (0, 0)
        (0, 1) // is this the same as (1, 0)? if so, permutations, else combinations
        (0, 2)
        (1, 0)
        (1, 1)
        (1, 2)
        (2, 0)
        (2, 1)
        (2, 2)
    }
    sequence: [0, 0]
    Suppose size = 100 and bounds[0] = 0,0 and movingVar = 50,50.
    setPoint(movingVar, bounds[0], 0); // movingVar = 25,25
    setPoint(movingVar, bounds[0], 0); // movingVar = 12,12
    sequence: [1, 0, 1]
    Suppose size = 100 and bounds[0] = 0,0 bounds[1] = 0,50 and movingVar = 50,50.
    setPoint(movingVar, bounds[1], 0); // movingVar = 25, 50
    setPoint(movingVar, bounds[0], 0); // movingVar = 12, 25
    setPoint(movingVar, bounds[1], 0); // movingVar = 6, 37
*/
auto Sierpinski::generate(unsigned int runs) -> void { // randomly move to generate Sierpinski Triangle
    unsigned int cnt = 0, i;
    std::uniform_int_distribution<> dist(0, 5); // inclusive
    while (cnt++ < runs) {
        switch (dist(this->randomGen)) {
            case 0: case 1: i = 0; break;
            case 2: case 3: i = 1; break;
            case 4: case 5: default: i = 2;
        }
        setPoint(this->movingVar, this->bounds[i], i);
    }
}
auto Sierpinski::run(unsigned int depth) -> void { // non randomly move to generate Sierpinski Triangle
    this->recursiveRun(0, depth);
}
auto Sierpinski::recursiveRun(unsigned int curr, unsigned int depth) -> void {
    if (curr >= depth) return;
    POINT temp;
    for (unsigned int i = 0; i < NUM_BOUNDS; i++) {
        temp = this->movingVar;
        this->setPoint(this->movingVar, this->bounds[i], i);
        this->recursiveRun(curr + 1, depth);
        this->movingVar = temp;
    }
}