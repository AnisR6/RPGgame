# Architecture

---

## Contents

---

1. [File Tree](#1-file-tree)

## 1. File Tree

---

```text
.
├── build/
├── cmake-deps/
├── CMakeLists.txt
├── CMakePresets.json
├── .clang-format
├── vcpkg.json
├── docs/
│   ├── specification/
│   │   ├── SPECIFICATION.md
│   │   └── images/
│   ├── goals/
│   │   ├── GOALS.md
│   │   └── images/
│   ├── architecture/
│   │   ├── ARCHITECTURE.md
│   │   └── images/
│   └── presentations/
│       └── presentation0.pdf
├── assets/
│   ├── sprites/
│   │   ├── buildings/
│   │   ├── bushes/
│   │   ├── chests/
│   │   ├── items/
│   │   ├── npcs/
│   │   ├── players/
│   │   ├── savepoints/
│   │   ├── walls/
│   │   └── trees/
│   ├── backgrounds/
│   └── audio/
└── src/
    ├── Main.cpp
    ├── presentation/
    │   ├── rendering/
    │   ├── screens/
    │   │   ├── core/
    │   │   │   └── Screen.hpp
    │   │   ├── boot_screen/
    │   │   │   ├── MainMenuScreen.hpp
    │   │   │   ├── LoadMenuScreen.hpp
    │   │   │   └── ControlsMenuScreen.hpp
    │   │   ├── overworld_screen/
    │   │   │   ├── OverworldScreen.hpp
    │   │   │   ├── DialogScreen.hpp
    │   │   │   └── player_screen/
    │   │   │       ├── PlayerScreen.hpp
    │   │   │       ├── EquipmentScreen.hpp
    │   │   │       └── InventoryScreen.hpp
    │   │   └── battle_screen/
    │   │       └── BattleScreen.hpp
    │   └── widgets/
    │       ├── core/
    │       │   ├── Widget.hpp
    │       │   ├── DataWidget.hpp
    │       │   └── DataWidget.cpp
    │       └── menu/
    │           ├── MenuScreen.hpp
    │           ├── Menu.hpp
    │           ├── Menu.cpp
    │           ├── MenuLayout.hpp
    │           ├── MenuLayout.cpp
    │           ├── MenuColumn.hpp
    │           ├── MenuColumn.cpp
    │           ├── MenuRow.hpp
    │           ├── MenuRow.cpp
    │           ├── MenuButton.hpp
    │           ├── MenuButton.cpp
    │           ├── MenuContainer.hpp
    │           ├── MenuContainer.cpp
    │           ├── MenuText.hpp
    │           └── MenuText.cpp
    ├── application/
    │   ├── events/
    │   │   └── core/
    │   │       └── Event.hpp
    │   ├── controllers/
    │   │   └── core/
    │   │       └── Controller.hpp
    │   └── views/
    │       └── core/
    │           └── View.hpp
    ├── game_logic/
    │   ├── core/
    │   │   ├── Clock.hpp
    │   │   ├── Clock.cpp
    │   │   ├── Camera.hpp
    │   │   └── Camera.cpp
    │   ├── modes/
    │   │   ├── core/
    │   │   │   ├── Mode.hpp
    │   │   │   ├── ScreenManager.hpp
    │   │   │   └── ScreenManager.cpp
    │   │   ├── boot_mode/
    │   │   │   ├── BootMode.hpp
    │   │   │   └── BootMode.cpp
    │   │   └── game_mode/
    │   │       ├── GameMode.hpp
    │   │       ├── overworld_mode/
    │   │       │   ├── OverworldMode.hpp
    │   │       │   └── OverworldMode.cpp
    │   │       └── battle_mode/
    │   │           ├── BattleMode.hpp
    │   │           └── BattleMode.cpp
    │   └── systems/
    │       ├── core/
    │       │   ├── animation_system/
    │       │   │   ├── AnimationSystem.hpp
    │       │   │   └── AnimationSystem.cpp
    │       │   └── controls_system/
    │       │       ├── ControlsSystem.hpp
    │       │       └── ControlsSystem.cpp
    │       └── game/
    │           ├── equipment_system/
    │           │   ├── EquipmentSystem.hpp
    │           │   └── EquipmentSystem.cpp
    │           ├── overworld/
    │           │   ├── collision_system/
    │           │   │   ├── CollisionSystem.hpp
    │           │   │   └── CollisionSystem.cpp
    │           │   └── movement_system/
    │           │       ├── MovementSystem.hpp
    │           │       └── MovementSystem.cpp
    │           └── battle/
    │               └── battle_system/
    │                   ├── BattleSystem.hpp
    │                   └── BattleSystem.cpp
    ├── game_state/
    │   ├── core/
    │   │   ├── GameState.hpp
    │   │   ├── ModeManager/
    │   │   │   ├── ModeManager.hpp
    │   │   │   └── ModeManager.cpp
    │   │   └── GameSession/
    │   │       ├── GameSession.hpp
    │   │       └── GameSession.cpp
    │   ├── controls/
    │   │   ├── Controls.hpp
    │   │   └── Controls.cpp
    │   └── ecs/
    │       ├── registry/
    │       │   └── core/
    │       │       ├── Entity.hpp
    │       │       ├── Component.hpp
    │       │       ├── Registry.hpp
    │       │       ├── System.hpp
    │       │       └── Query.hpp
    │       ├── components/
    │       │   ├── core/
    │       │   │   └── Component.hpp
    │       │   ├── HealthComponent.hpp
    │       │   ├── PositionComponent.hpp
    │       │   ├── TransformComponent.hpp
    │       │   ├── MovementComponent.hpp
    │       │   ├── CollisionComponent.hpp
    │       │   ├── InventoryComponent.hpp
    │       │   ├── EquipmentComponent.hpp
    │       │   └── CombatStatsComponent.hpp
    │       ├── capabilities/
    │       │   ├── core/
    │       │   │   └── Capability.hpp
    │       │   ├── Animatable.hpp
    │       │   ├── Animatable.cpp
    │       │   ├── Livable.hpp
    │       │   ├── Livable.cpp
    │       │   ├── Moveable.hpp
    │       │   ├── Moveable.cpp
    │       │   ├── Collidable.hpp
    │       │   ├── Collidable.cpp
    │       │   ├── Dialogable.hpp
    │       │   ├── Dialogable.cpp
    │       │   ├── Battleable.hpp
    │       │   ├── Battleable.cpp
    │       │   ├── Partyable.hpp
    │       │   ├── Partyable.cpp
    │       │   ├── Enemyable.hpp
    │       │   ├── Enemyable.cpp
    │       │   ├── Itemable.hpp
    │       │   ├── Itemable.cpp
    │       │   ├── Inventoryable.hpp
    │       │   ├── Inventoryable.cpp
    │       │   ├── Equipable.hpp
    │       │   ├── Equipable.cpp
    │       │   ├── Saveable.hpp
    │       │   ├── Saveable.cpp
    │       │   ├── Audible.hpp
    │       │   └── Audible.cpp
    │       ├── archetypes/
    │       │   ├── core/
    │       │   │   └── Archetype.hpp
    │       │   ├── environment/
    │       │   │   ├── BuildingArchetype.hpp
    │       │   │   ├── TreeArchetype.hpp
    │       │   │   ├── BushArchetype.hpp
    │       │   │   └── WallArchetype.hpp
    │       │   ├── characters/
    │       │   │   ├── PlayerArchetype.hpp
    │       │   │   ├── EnemyArchetype.hpp
    │       │   │   └── NPCArchetype.hpp
    │       │   └── interactables/
    │       │       ├── SavePointArchetype.hpp
    │       │       ├── ChestArchetype.hpp
    │       │       └── ItemArchetype.hpp
    │       └── prefabs/
    │           ├── core/
    │           │   ├── Prefab.hpp
    │           │   └── PrefabFactory.hpp
    │           ├── environment/
    │           │   ├── building/
    │           │   │   ├── Building.hpp
    │           │   │   └── Building.cpp
    │           │   ├── tree/
    │           │   │   ├── Tree.hpp
    │           │   │   ├── Tree.cpp
    │           │   │   ├── OakTree.hpp
    │           │   │   ├── OakTree.cpp
    │           │   │   ├── PineTree.hpp
    │           │   │   └── PineTree.cpp
    │           │   ├── bush/
    │           │   │   ├── Bush.hpp
    │           │   │   └── Bush.cpp
    │           │   └── wall/
    │           │       ├── Wall.hpp
    │           │       ├── Wall.cpp
    │           │       ├── StoneWall.hpp
    │           │       ├── StoneWall.cpp
    │           │       ├── WoodenWall.hpp
    │           │       └── WoodenWall.cpp
    │           ├── characters/
    │           │   ├── player/
    │           │   │   ├── Player.hpp
    │           │   │   └── Player.cpp
    │           │   ├── enemy/
    │           │   │   ├── Enemy.hpp
    │           │   │   ├── Enemy.cpp
    │           │   │   ├── Slime.hpp
    │           │   │   ├── Slime.cpp
    │           │   │   ├── Goblin.hpp
    │           │   │   ├── Goblin.cpp
    │           │   │   ├── Boss.hpp
    │           │   │   └── Boss.cpp
    │           │   └── npc/
    │           │       ├── NPC.hpp
    │           │       ├── NPC.cpp
    │           │       ├── Shopkeeper.hpp
    │           │       ├── Shopkeeper.cpp
    │           │       ├── Villager.hpp
    │           │       └── Villager.cpp
    │           └── interactables/
    │               ├── savepoint/
    │               │   ├── SavePoint.hpp
    │               │   └── SavePoint.cpp
    │               ├── chest/
    │               │   ├── Chest.hpp
    │               │   ├── Chest.cpp
    │               │   ├── LockedChest.hpp
    │               │   ├── LockedChest.cpp
    │               │   ├── TreasureChest.hpp
    │               │   └── TreasureChest.cpp
    │               └── item/
    │                   ├── Item.hpp
    │                   ├── Item.cpp
    │                   ├── Potion.hpp
    │                   ├── Potion.cpp
    │                   ├── Weapon.hpp
    │                   ├── Weapon.cpp
    │                   ├── Key.hpp
    │                   └── Key.cpp
    ├── persistence/
    │   ├── GameSave.hpp
    │   ├── GameSave.cpp
    │   └── sqlite/
    │       ├── SQLiteDatabase.hpp
    │       ├── SQLiteDatabase.cpp
    │       ├── DatabaseSchema.hpp
    │       ├── DatabaseSchema.cpp
    │       ├── SaveRepository.hpp
    │       └── SaveRepository.cpp
    ├── game_loader/
    │   ├── GameLoader.hpp
    │   └── GameLoader.cpp
    └── tests/
        ├── unit/
        └── integration/
```
