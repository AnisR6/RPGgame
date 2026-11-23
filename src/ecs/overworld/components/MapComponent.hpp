#pragma once

#include <cstdint>
#include <vector>

struct MapComponent {
        int widthInTiles = 0;
        int heightInTiles = 0;
        int tileSizeInPixels = 32;
        int layerCount = 0;
        int sortedLayerIndex = 0;
        std::vector<std::uint8_t> solidCells;
};
