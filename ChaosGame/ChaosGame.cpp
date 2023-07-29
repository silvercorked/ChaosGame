
#include <ctime>
#include <string>
#include <iostream>

#include "Chaos.hpp"
#include "Sierpinski.hpp"

int main(int argc, char* argv[]) {
    std::random_device rd;

    std::mt19937 gen(rd());
    /*
        unsigned int bmpSize,
	    std::mt19937& rGen,
	    WeightedTransformation* transforms,
	    size_t numTransforms
    */
    //Chaos c = Chaos(500, gen, (WeightedTransformation*) SierpinskyTransformations, 3);
    Chaos c = Chaos(500, gen, (WeightedTransformation*) BarnsleyTransformations, 3);
    //Chaos c = Chaos(1000, gen, (WeightedTransformation*) DragonCurveTransformations, 3);
    c.generate(10000000);
    c.save("C:/Users/Alex/Desktop/ChaosGameOutputBarnsley.bmp");
    //Sierpinski c = Sierpinski(500, gen);
    //c.generate(1000000); // randomized
    //c.run(9); // deterministic
    //c.save("../SierpinskiGameOutput.bmp");
    return 0;
}