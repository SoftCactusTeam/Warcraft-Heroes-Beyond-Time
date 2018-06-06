# Warcraft: Heroes Beyond Time

_Warcraft: HBT_ is a roguelike video game based on Warcraft 2 where you will take the role of the orc Thrall.

This project has been done by seven students of CITM - UPC (Terrasa, Spain).  

* [Github repository](https://github.com/SoftCactusTeam/Warcraft_Adventures)  
* [Website](https://softcactusteam.github.io/Warcraft-Heroes-Beyond-Time/)  
* [Twitter](https://twitter.com/SoftCactus_Team)
* [Facebook](https://www.facebook.com/SoftCactus-Team-486732635057713/)
* [Youtube](https://www.youtube.com/channel/UCTguladN9y3OM1WT4KbRM5A)
* [Instagram](https://www.instagram.com/softcactus_team/)

## About the game  

You are Thrall and your goal is to beat the boss Guld'an without dying. If you beat the boss you win the game. But first you must defeat all the archers in the 5 random generated rooms.

Archers have different colors (3 tiers in total) according to its difficulty (that will be increased as you approach to the boss room). When a room is cleared a portal appears near you and will take you to the next room. 

You can pick a item from 3 options in a chest that will appear in a small room at the beginning of each map. 

The player can move around, dash, use his melee attack, use his special skill and pick items. The skill is represented as a yellow bar on top left corner and it's charged when hitting enemies; it consists of an area attack that will affect enemies around you.

Items are barely explained so you'll need to figure out what they do with trial and error.

That's it! Good luck :)

## Changelog

v0.1
- FileSystem (Save/Load)
- GUIModule + Labels / InputBoxes
- EntitySystem
- Console
- Thrall Sprites
- Terrain Sprites
- Appveyor Integration

v0.2
- config.xml + some console commands
- Logical resolution fixed
- Player can move with controller too
- Sliders and Buttons
- Scene's Switching

v0.2.1
- Controller rumble implemented.
- Map procedural generation (now with walls)
- SDL libraries updated
- Render resolution changed to 360p 16:9.
- New player movement: Dash. (Press SPACEBAR / A (XBOX) ).

v0.2.2
- New ground and wall tiles art
- Camera following
- Player animation normalized
- Player animations match movement speed
- Dash and movement have now free angle (with gamepad)
- Minor bugs fixed

v0.2.3
- Functional Pause Menu (without really pausing)
- Random map generating with definitive tiles (Press G)
- Pathfinding and first enemy
- Colliders implemented
- Controller Input Fixed
- Thrall atack with controller implemented
- Camera Culling fixed
- Bugs fixing

v0.2.5
- Archer Enemy Implemented
- New Printing Module: Now sprites are reordered for being printed in order on screen.
- Real Pausing Implemented
- Labels and fonts error fixed
- Colliders Fixing
- Pathfinding Fixing
- Bugs Fixing
- Memory Leaks Solving

v0.2.6
- Brofiler implemented
- Rotating Sprites with ModulePrinter
- Some BSO's and sound effects added
- New console command: Print the player freezone.
- Bugs Fixing

v0.2.7
- Using pivots to print sprites
- Solving color lines in some resolutions
- Colliders optimized and reworked
- New health and energy bar
- Collision with the environment implemented
- Damage receiving, die animation and go back to the main menu implemented
- Bugs fixing

v0.2.7.1
- Fixed framerate calculations
- Attack, movement and dash pivots implemented
- Added dash CD
- Player Attack in all directions with attack colliders
- Bugs Fixing

v0.2.8
- First item implemented
- Gul'dan Boss implemented
- All thrall pivots fixed + Skill and attacks working
- Pausing game when console opened
- Adding new feature in ModulePrinter: Color tint
- Enemies and colliders optimized
- Bug Fixing

v0.2.8.1
- Win condition implemented
- Scene flow corrected
- Boss hp bar working
- Bug Fixing

v0.2.8.2
- Archer path fixed
- Added new sounds
- Item fixed
- Win condition
- Gamepad input
- Minor bugs fixed

v0.2.8.3
- Guldan Balls new sound implemented
- Labels with own multilabel
- Win BSO Implemented
- New final message + version in main menu
- More FX
- Item damage rebalanced
- Portal fixed
- Item fixed
- Bug Fixing

v0.3
- More audios
- Dash FX halted when you stop dashing
- Controller support in menues
- Some archer fixes
- Visual bugs solved
- Bug Fixing

v0.3.1
- Some Guldan's audios fixed
- Archers turning red when damaged
- Different menues depending of if there is a controller connected to the device

v0.3.1.1
- Detecting controllers and audio devices during the game execution
- Audio crashes solved
- Version in xml
- Bug Fixing

v0.3.2
- Map art reworked
- Offset and pivots are different concepts in ModulePrinter
- New collision system implemented
- Basic navigation through menues with gamepad
- New module for projectiles, not used for now

v0.4
- Map config depending of lvlIndex
- New Guldan attacks, testing and implementing them for now
- Items randomized and picked. Only 2 in the pool, gui has to improve more. Open chest attacking him
- Damage Ball item implemented, new item structure
- Module Effects implemented
- Time Based animation implemented
- New gui, still in progress

v0.5
- UI rework
- Three archers tiers ready to use in xml
- Solved several bugs on archers
- Changed module colliders
- Added projectile module
- New art for boss room
- Boss attacks implemented with two phases
- Minor bugs fixed
- Bug Fixing and Memory Leaks solved

v0.6
- Managing lvls generation + enemies spawning
- Thrall inmortal when doing the skill
- Thrall doesn't regenerate each lvl
- Item selection with gamepad
- GUI reworked
- Some enemies fixing
- Guldan has more attacks
- Bug fixing

v0.6.5
- Sliders adjusted to the new graphics
- Icons
- Blood on screen when Thrall gets hit
- Sliders feedback when focused
- 4th archer
- Some item effects implemented in archers
- Arrow doesn't destroy when thrall is dashing

v0.7
- Picking items with mouse, cant focus another one when one of them is being picked
- Thrall's skill artwork changed
- Map tiles fixed
- Guldan receives damage from the player skill
- Enemies balancing
- New items
- Thrall doesn't move when the scene is fading
- New initial room each lvl with a chest
- Bug Fixing

v0.9
- More Items with their descriptions
- Archers, Gul'dan and Thrall rebalancing
- Bug Fixing

v0.9.5
- New animated menu background with particles
- Gamepad rebinding available
- New archer effects sprites
- Rework art of items
- New VS boss fight cinematic
- Particles in player dash
- New intro video at the beginning
- Now the level number is shown in-game
- Save and Load implemented
- New sound FX
- Bug fixing and minor improvements in GUI

v0.9.9
- Dash sliding with walls.
- Intro video implemented.
- Dash particles implemented.
- Save y Load finished.
- New settings.
- New main menu.
- New transition before Guldan's fight
- FX Revision.
- Multiple fixes.
- Bug Fixing

v1.0
- Dash with keyboard
- Bug fixing

## Installation instructions  

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy!  

_**IMPORTANT:** do not modify, change or add any folder or file as you might not be able to execute the game._  

## Controls

### Gamepad
- LEFT STICK: move
- A BUTTON: dash and select
- X BUTTON: attack and open chest
- Y BUTTON: skill
- B BUTTON: go back in menus
- START: pause game

### Mouse and keyboard
- LEFT CLICK: select in menus
- WASD: move
- J: attack and open chest
- K: skill
- SPACE: dash
- ESC: pause
- F1: Go to Guldan's room.

## Debug features

Almost all debug features are accesed through the game console. To activate/deactivate the console just press the tilde key (next to 1).

- Show colliders: colliders_print_1
- Hide colliders: colliders_print_0
- Spawn archer: spawn_archer_(tier number, up to 3)
- Spawn another player: spawn_thrall
- Show walkable tiles: map_printwalkables_1
- Hide walkable tiles: map_printwalkables_0
- Charge player energy: player_energy_(energy value, up to 100)
- Damage the player: player_damage_(damage value, up to 100)
- Activate god mode: player_godmode_1
- Deactivate god mode: player_godmode_0

- Go to boss room: F1 key
- Go to next level: Q key

## Team members  

_Masó Garcia, Víctor's responsibility and Github account_  

* Lead  
* [Github account](https://github.com/nintervik)  

_Costa Miquel, Guillem's responsibility and Github account_  

* Code  
* [Github account](https://github.com/DatBeQuiet)  

_Calvis Gil, Isaac's responsibility and Github account_  

* Management  
* [Github account](https://github.com/isaaccalvis)  

_de Dios Miranda, Oriol's responsibility and Github account_  
 
* Design  
* [Github account](https://github.com/orioldedios)  

_Lozano Sánchez, David's responsibility and Github account_  

* Art and audio  
* [Github account](https://github.com/DavidTheMaaster)  

_Molina-Prados Ciudad, Jonathan's responsibility and Github account_  

* UI  
* [Github account](https://github.com/Jony635)  

_Morgado Expósito, Arnau's responsibility and Github account_  

* QA  
* [Github account](https://github.com/morgadoCV)  


## Tools used  

* **IDE:** Microsoft Visual Studio 2017  
* **Language:** C++  
* **Graphics and audio:** SDL 2.0  
* **Data:** pugixml 1.8  
* **Filesystem:** PhysicsFS
* **Profiler:** Brofiler  
* **Code repository:** GitHub  
* **Art:** Adobe Photoshop CS6, Aseprite, Paint.net and MS Paint 

## Credits for artwork, music/sound fx and fonts

Sound fx:
- https://freesound.org/people/jacobalcook/sounds/195568/
- https://freesound.org/people/taren1965/sounds/132502/
- https://freesound.org/people/alpharo/sounds/186696/
- https://freesound.org/people/Robinhood76/sounds/221591/
- https://freesound.org/people/D%20W/sounds/136696/
- https://freesound.org/people/samararaine/sounds/186374/
- https://freesound.org/people/spookymodem/sounds/202092/
