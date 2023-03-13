# Feature Roadmap

A list of features that I find interesting to play around with.
The [Backlog](#Backlog) contains an unsorted list of random ideas.
Inside [Versions](#Versions) you'll find more "changelog"-style features, either implemented or WIP.


## Backlog
 - Spend Mana for Attacks / Skills
 - UI
   * Damage Numbers
   * Healthbar
   * Main Menu
   * Mana spending, Ability Picker
 - Controls / Abilities
   * Move with stick is boring. Better keep moving and change direction with swipe?
   * Double swipe to dash. Requires charge? Does damage etc based on skills.
 - Attacks
   * Sword Rain. Shoots swords from the sky, one by one, sticking them into the ground.
 - Attack Types
   * [ ] Magic Arrow
     - Spawn at player
     - Move to enemy
	 - Damage 1 target once
   * [ ] Fireball
     - Spawn at player
     - Move to enemy
	 - Damage 1 target once / Damage targets in area once
   * [ ] Lightning
     - Spawn at enemy
	 - Stay
	 - Damage 1 target once
   * [ ] Meteor
     - Spawn at random position
	 - Stay
	 - Damage all targets in area once
   * [ ] Cyclone
     - Spawn at random position
	 - Move random walk
	 - Damage targets in area over time
   * [ ] Electric Shock
     - Spawn at player
	 - Connect to enemy
	 - Damage 1 target once / Damage targets in area once
   * [ ] Energy Bolt
     - Spawn at player
	 - Move random direction
	 - Damage targets in area once / Damage over time
   * [ ] Incineration
     - Spawn at player
	 - Move with player position and rotation
	 - Damage targets in area over time
   * [ ] Blizzard
     - Spawn at random position
	 - Stay
	 - Damage targets in area over time
   * [ ] Tsunami Wave
     - Spawn at fixed position
	 - Move fixed direction
	 - Damage targets in area over time
   * [ ] Spirit
     - Spawn at player
	 - Move to enemy
	 - Damage 1 target once
   * [ ] Sattelite
     - Spawn at player offset
	 - Move rotate around player
	 - Damage targets once with reset cooldown
   * [ ] Arcane Ray
     - Spawn at player
	 - Move with player
	 - Damage targets in area over time
   * [ ] Electric Zone
     - Spawn at player
	 - Move with player
	 - Damage targets in area over time
   * [ ] Frost Nova
     - Spawn at player
	 - Move with player
	 - Damage targets in area once
   * [ ] Lava Zone
     - Spawn at random position
	 - Stay
	 - Damage targets in area over time
   * [ ] Shield
     - Spawn at player
	 - Move with player
	 - ?
   * [ ] Cloaking
     - ?
   * [ ] Magic Circle
     - Spawn at player
	 - Move with player
	 - ?
   * [ ] Armageddon
     - Spawn
	 - 
	 - Damage all enemies once
   * [ ] Flash Shock
     - Spawn at player
	 - Stay
	 - Damage targets in area once
 - Special/"Triggered" Attacks
   * Backstab (Triggers when switching direction by ~180°)
 - Player Stats
   * HP
   * Movement Speed
   * Attack Damage
   * Attack Size
   * Attack Speed/Cooldown
   * Attack Duration
   * Attack Critical Strike
   * Item Collect Distance
 - Synergies
   * Buildable Attacks? (think Noita wands)
   * Chainable Attacks? (Shoot bullet, Sword Swing attack attached to bullet will swing around it)
   * Elements? (think Genshin Impact)
 - Juice!
   * Walk Animation
   * Screen Shake
     - Zoom out a bit on mana collect? Adds together
   * On Enemy Hit
     - White Flash
	 - Bounce back
   * Animate Healthbar Loss
 - Enemies
   * Walk towards player
   * Fly in a fixed direction near player (maybe with swarm-like flocking?)
   * Stand in a fixed position, ranged attacks
   * Bosses
     - Minions swarming around them
	 - Special attack patterns
	 - Not just stronger enemies that follow you.
 - Levels
   * Infinite Mode with waves, Shop / Camp spawns in between waves.
   * "Story"-Mode, premade/generated Maps with Enemies
   * In both(?) modes: Challenge NPC (think Enter the Gungeon).
     - High Damage but Single Hit
	 - ...
 - Performance
   * Spatial Partitioning (Grid / Quadtree) System


## Versions

### [WIP]
 - [ ] Basic Game Loop (1+ attack, 1+ enemy)

### [v0.1] Initial Build
 - [x] Linux
 - [ ] Windows
 - [x] Web
 - [ ] Android


## Rendering Backend

I want to decide & settle on a rendering backend for this & future projects.
Generally, I want pretty good performance, a non-intrusive and clean API as well as cross platform support.
SDL2 is a great choice that will definitely stay (as an alternative GLFW feels a bit better to use, but the platform support of SDL2 is unmatched).

 1. [bgfx](https://github.com/bkaradzic/bgfx) is pretty nice, but documentation/resources are not that much to my liking.
 Additionally, its shading language & tool `shaderc` does feel somewhat awkward to use and documentation is rather sparse.
 Still, platform support & performance is great.

 2. [sokol](https://github.com/floooh/sokol) seems to be a bit nicer, but I haven't really looked into it.
 Resources seems to be a bit better than bgfx, however the stb-style library seems to be a bit too awkward for projects of this size.

 3. Another choice would be to write my own rendering abstraction layer, and use OpenGL 3/4 for Linux/Windows and OpenGL ES for Emscripten/Android.
 This seems really interesting and fun, but would potentially interfere more with my goal of actually writing a game rather than an engine.
 On the other hand, I feel really at home with OpenGL which could make development faster. OpenGL ES is similar enough to Core so it shouldn't be too much of a hassle to write 2 shaders for everything once the backends are implemented.
 Implementing Vulkan/DirectX/Metal backends could at some point be a great learning exercise.


As I am currently pretty much just rendering sprites I will continue working with bgfx until I have more motivation to compare alternatives.

