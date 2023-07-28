
#include <ctime>
#include <string>
#include <iostream>

#include "Chaos.hpp"

int main(int argc, char* argv[]) {
    std::random_device rd;

    std::mt19937 gen(rd());
    Chaos c = Chaos(500, gen);
    //c.generate(100 * 100);
    c.run(9);
    c.save("C:/Users/Alex/Desktop/chaosGameOutput.bmp");
    return 0;
}