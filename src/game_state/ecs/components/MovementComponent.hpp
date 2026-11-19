#pragma once

// How fast an entity walks. Which way it wants to walk lives on InputComponent.
struct MovementComponent {
	float speedInPixelsPerSecond = 120.0f;
};