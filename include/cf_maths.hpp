#pragma once
#include <3ds.h>

#include "cf_colors.hpp"

#include <algorithm>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

namespace maths
{
    typedef struct vec2 {
        double x;
        double y;

        explicit operator char() {
            return static_cast<char>(x)+static_cast<char>(y);
        }

        vec2 operator +(vec2 a) {
            return {a.x+x,a.y+y};
        }

        bool operator ==(vec2 a) {
            if (a.x == x && a.y == y) {
                return true;
            } else {
                return false;
            }
        }
    };

    vec2 VECZERO = {0, 0};

    template <typename T>
    const T& clamp(const T &value, const T &low, const T &high) {
        return std::max(low, std::min(value, high));
    }

    u32 colorAverage(u32 color1, u32 color2) {
        return (color1 + color2) / 2;
    }

    // Calc distance between a point (Px, Px) and a line 
    double PointToLineDistance(double Px, double Py, vec2 P1, vec2 P2) {
        double A = P2.y - P1.y;
        double B = P1.x - P2.x;
        double C = P2.x * P1.y - P1.x * P2.y;

        return std::abs(A * Px + B * Py + C) / std::sqrt(A * A + B * B);
    }

    // Check collision between 2 squares and a line
    bool CheckCollision(double x, double y, double L, vec2 P1, vec2 P2) {
        // Calc coord of square center
        double Cx = x + L / 2;
        double Cy = y + L / 2;

        double distance = PointToLineDistance(Cx, Cy, P1, P2);

        // Compare distance and return
        return distance <= L / 1.5;
    }

    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    u32 lerpColor(u32 color1, u32 color2, float t) {
        // Decompose color1 RGBA
        u8 r1 = (color1 >> 24) & 0xFF;
        u8 g1 = (color1 >> 16) & 0xFF;
        u8 b1 = (color1 >> 8) & 0xFF;
        u8 a1 = color1 & 0xFF;

        // Decompose color2 RGBA
        u8 r2 = (color2 >> 24) & 0xFF;
        u8 g2 = (color2 >> 16) & 0xFF;
        u8 b2 = (color2 >> 8) & 0xFF;
        u8 a2 = color2 & 0xFF;
        
        // Make the interpolation for each decomposed colors
        u8 r = static_cast<u8>(r1 + t * (r2 - r1));
        u8 g = static_cast<u8>(g1 + t * (g2 - g1));
        u8 b = static_cast<u8>(b1 + t * (b2 - b1));
        u8 a = static_cast<u8>(a1 + t * (a2 - a1));

        return ((u32)a << 24) | ((u32)r << 16) | ((u32)g << 8) | (u32)b;
    }

    bool isPointInRectangle(vec2 position, float width, float height, vec2 touchPos) {
        // Calculate of the coordinates of the bottom right corner of the rectangle
        vec2 bottomRight = {position.x + width, position.y + height};
    
        // Utilisation de la fonction std::min et std::max pour déterminer les coordonnées du coin en haut à gauche
        vec2 topLeft = {std::min(position.x, bottomRight.x), std::min(position.y, bottomRight.y)};
    
        // Vérification si le point est à l'intérieur du rectangle
        bool isInsideRectangle = (touchPos.x >= position.x) && (touchPos.x <= bottomRight.x) &&
                                (touchPos.y >= position.y) && (touchPos.y <= bottomRight.y);
    
        return isInsideRectangle;
    }

} // namespace maths
