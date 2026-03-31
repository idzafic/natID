#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <limits>

class HeightMap {
public:
    // Check if a point collides with any height layer at x,z
    bool collide(const glm::vec3& p, float epsilon = 0.1f) const {
        int x = static_cast<int>((p.x - minX) / cellSize);
        int z = static_cast<int>((p.z - minZ) / cellSize);
        if (!inside(x, z)) return false;

        const auto& heights = data[z * width + x];
        for (float h : heights) {
            if (std::abs(p.y - h) <= epsilon) return true;
            if (p.y < h - epsilon) return true; // If it's below a surface
        }
        return false;
    }

    // Set up the map from mesh vertices
    void postaviMapu(const std::vector<glm::vec3>& vertices, float cellSizePassed = 1.0f) {
        cellSize = cellSizePassed;

        if (vertices.empty()) return;

        // Compute bounds
        float minX_ = std::numeric_limits<float>::max();
        float maxX_ = std::numeric_limits<float>::lowest();
        float minZ_ = std::numeric_limits<float>::max();
        float maxZ_ = std::numeric_limits<float>::lowest();

        for (const auto& v : vertices) {
            minX_ = std::min(minX_, v.x);
            maxX_ = std::max(maxX_, v.x);
            minZ_ = std::min(minZ_, v.z);
            maxZ_ = std::max(maxZ_, v.z);
        }

        minX = minX_;
        minZ = minZ_;
        width = static_cast<int>((maxX_ - minX_) / cellSize) + 1;
        depth = static_cast<int>((maxZ_ - minZ_) / cellSize) + 1;
        data.resize(width * depth);

        // Fill heightmap
        for (const auto& v : vertices) {
            int x = static_cast<int>((v.x - minX) / cellSize);
            int z = static_cast<int>((v.z - minZ) / cellSize);
            if (inside(x, z)) {
                auto& heights = data[z * width + x];

                // Optional: avoid storing duplicate/very-close heights
                bool found = false;
                for (float h : heights) {
                    if (std::abs(h - v.y) < 0.05f) { // Small tolerance
                        found = true;
                        break;
                    }
                }
                if (!found)
                    heights.push_back(v.y);
            }
        }
    }

private:
    float minX, minZ, cellSize;
    int width, depth;
    std::vector<std::vector<float>> data;

    bool inside(int x, int z) const {
        return x >= 0 && x < width && z >= 0 && z < depth;
    }
};
