#pragma once

// Draw order. Smaller zIndex is further back. The presentation forest groups entities by this.
struct LayerComponent {
	int zIndex = 0;
};
