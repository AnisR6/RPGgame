#include "game_logic/modes/game_mode/overworld_mode/OverworldMode.hpp"

#include "game_logic/systems/core/animation_system/AnimationSystem.hpp"
#include "game_logic/systems/game/overworld/movement_system/MovementSystem.hpp"
#include "presentation/rendering/SpriteRenderer.hpp"

namespace {
const char *mapFilePath = "assets/maps/overworld1.json";
const char *tilesetFolderPath = "assets/tilesets";
const char *playerSpriteSheetPath = "assets/tilesets/Player.png";
const sf::Vector2f playerStartPosition(640.0f, 576.0f);
} // namespace

OverworldMode::OverworldMode()
    : tileMap_(mapFilePath, tilesetFolderPath), player_(playerStartPosition, playerSpriteSheetPath)
{
}

void OverworldMode::update(float secondsSinceLastFrame)
{
	// 1. Which way does the player want to go
	player_.movement.walkDirection = MovementSystem::readArrowKeys();

	// 2. Look that way
	AnimationSystem::faceWalkDirection(player_.sprite, player_.movement);

	// 3. Move, unless a wall is in the way
	MovementSystem::move(player_.position, player_.movement, player_.collision, tileMap_, secondsSinceLastFrame);
}

void OverworldMode::render(sf::RenderTarget &target) const
{
	// 1. Start from the normal view (same size as the window)
	sf::View camera = target.getDefaultView();

	// 2. Put its centre on the player
	camera.setCenter(player_.position.position);


	// 3. Draw the world through the camera
	target.setView(camera);
	tileMap_.render(target); // first the map
	SpriteRenderer::draw(target, player_.sprite, player_.position); // then the player on top

	// 4. Back to the normal view
	target.setView(target.getDefaultView());
}