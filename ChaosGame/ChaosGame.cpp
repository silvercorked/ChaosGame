
#include <ctime>
#include <string>
#include <iostream>

#include "Chaos.hpp"

int main(int argc, char* argv[]) {
    std::random_device rd;

    std::mt19937 gen(rd());
    Chaos c = Chaos(500, gen);
    //c.generate(1000000); // randomized
    c.run(9); // deterministic
    c.save("../chaosGameOutput.bmp");
    return 0;
}