#pragma once
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For srand()
#include <random>
#include <3ds.h>

#include "cf_maths.hpp"

namespace randoms
{
    int randomInRange(int min, int max) {
        // Initialize number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> distribution(min, max);  // Define distribution interval
        int randomNumber = distribution(gen);   // generation of random number

        return randomNumber;
    }

    void generateScreenCoordinates(int screenWidth, int screenHeight, maths::vec2& leftCoord, maths::vec2& rightCoord) {
        // Using srand() for init the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));

        bool generateFixedY = rand() % 2 == 0;

        if (generateFixedY) {
            // Generating random coord in Y axis
            float yRandomStart = static_cast<float>(rand() % screenHeight);
            float yRandomEnd = static_cast<float>(rand() % screenHeight);

            // Left border coords
            leftCoord.x = 0;
            leftCoord.y = yRandomStart;

            // Right border coords
            rightCoord.x = static_cast<float>(screenWidth);
            rightCoord.y = yRandomEnd;
        }
        else {
            // Generating coord in the axis X
            float yRandomStart = static_cast<float>(rand() % screenWidth);
            float yRandomEnd = static_cast<float>(rand() % screenWidth);

            // Left border coords
            leftCoord.x = yRandomStart;
            leftCoord.y = 0;

            // Right border coords
            rightCoord.x = yRandomEnd;
            rightCoord.y = static_cast<float>(screenWidth) * 2;
        }
    }
} // namespace randoms
