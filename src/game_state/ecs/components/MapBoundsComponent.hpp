#pragma once

// One entity carries the map size so collision can treat cells outside the map as walls.
struct MapBoundsComponent {
	int widthInTiles = 0;
	int heightInTiles = 0;
	int tileSizeInPixels = 32;
};
