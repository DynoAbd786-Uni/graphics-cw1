#include <catch2/catch_amalgamated.hpp>

//TODO
#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("line adjacency", "[adjacency]")
{
    Surface surface(100, 100);
    surface.clear();

    ColorU8_sRGB standard_color = {245, 245, 245};
    Vec2f centre_point = {49, 49};

    SECTION("generic intersection, all gradient types")
    {
        std::vector<Vec2f> generic_points = {
            {49, 0}, {49, 99},  // Vertical
            {0, 49}, {99, 49},  // Horizontal
            {99, 0}, {0, 99},   // Perfect diagonals
            {0, 0}, {99, 99},
            {0, 24}, {99, 74},  // Low gradient 
            {0, 74}, {99, 24},
            {24, 0}, {74, 99},  // High gradient
            {74, 0}, {24, 99},
        };

        for (const auto& vec1 : generic_points)
        {
            for (const auto& vec2 : generic_points)
            {
                // Skip if vec1 == vec2
                if (vec1.x == vec2.x && vec1.y == vec2.y)
                    continue;

                // Draw lines
                draw_line_solid(surface, vec1, centre_point, standard_color);
                draw_line_solid(surface, centre_point, vec2, standard_color);

                // Count neighbours
                auto neighbour_list = count_pixel_neighbours(surface);

                // Ensure there are not 4 or more ends (= 2 or more lines)
                // Sharp corners may exist in drawing that lead to 3 points with 1 adjacent pixel 
                REQUIRE(neighbour_list[1] < 4);
                surface.clear();
            }
        }
    }

    SECTION("short lines")
    {
        // Define relative offsets for short lines in all directions
        std::vector<Vec2f> offsets = {
            {0, 1},  {1, 0},  {0, -1}, {-1, 0}, // Cardinal directions
            {1, 1},  {1, -1}, {-1, 1}, {-1, -1}, // Diagonal directions
            {0, 2},  {2, 0},  {0, -2}, {-2, 0}, // Slightly longer cardinal directions
            {2, 2},  {2, -2}, {-2, 2}, {-2, -2} // Diagonal, slightly longer
        };

        for (const auto& offset1 : offsets)
        {
            Vec2f p0 = { centre_point.x + offset1.x, centre_point.y + offset1.y };

            for (const auto& offset2 : offsets)
            {
                Vec2f p1 = { p0.x + offset2.x, p0.y + offset2.y };

                // Clear the surface before drawing
                surface.clear();

                // Draw two short connected lines
                draw_line_solid(surface, centre_point, p0, standard_color);
                draw_line_solid(surface, p0, p1, standard_color);

                // Count neighbours to check adjacency
                auto neighbour_list = count_pixel_neighbours(surface);

                // Ensure no gaps in connections
                // centre_point and p1 are endpoints; p0 is a shared connection
                REQUIRE(neighbour_list[1] < 4);  // Allow for 3 neighbours max (p0 counts once)
            }
        }
    }

    SECTION("multiple connected lines")
    {
        std::vector<Vec2f> points = {
            {10, 10}, {49, 49}, {90, 10}, {49, 90}, {10, 90}
        };

        // Draw connected lines sequentially
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            draw_line_solid(surface, points[i], points[i + 1], standard_color);
        }

        // Count neighbours
        auto neighbour_list = count_pixel_neighbours(surface);

        // Ensure no disconnections in the chain
        REQUIRE(neighbour_list[1] < 4);  // Max 2 endpoints, 2 for shared points

        surface.clear();
    }

    SECTION("polygon connections")
    {
        // Looking for no endpoints in a closed shape
        std::vector<Vec2f> polygon_points = {
            {10, 10}, {49, 10}, {49, 49}, {10, 49}, {10, 10}
        };

        // Draw the polygon edges
        for (size_t i = 0; i < polygon_points.size() - 1; ++i)
        {
            draw_line_solid(surface, polygon_points[i], polygon_points[i + 1], standard_color);
        }

        // Count neighbours
        auto neighbour_list = count_pixel_neighbours(surface);

        // Ensure only 2 endpoints for the closed polygon
        REQUIRE(neighbour_list[1] == 0);  // Closed polygon has no open endpoints

        surface.clear();
    }
}
