#pragma once

// The invisible square that hits walls. Solid tiles block movement; the player's own box does not.
struct CollisionComponent {
	float boxSizeInPixels = 24.0f;
	bool solid = false;
};