# SkyShield: Drone Defense

For the final project of the C++ Object-Oriented Programming 2 course, we designed and built SkyShield, an original 2D action game. You pilot a drone over a battlefield and drop bombs on waves of enemies closing in on your Command Vehicle from both sides. This repository showcases the implementation built using C++20 and the SFML 3 framework, with a strong focus on clean object-oriented architecture.

## 🌟 Key Features

* **Drone Combat Gameplay:** Fly left and right, drop bombs, and stop enemies before their shells wreck the Command Vehicle. Watch your battery and bomb supply, because running out of either can cost you the level.
* **Enemy AI:** Enemies advance, stop to fire, and panic and flee when the drone hovers above them. Fast enemies die in one hit, while tanks take two hits and telegraph their shots with a wind-up animation.
* **Shop & Progression:** Earn coins for every enemy destroyed and spend them on new vehicles, drones, enemy skins, backgrounds, music, and fonts. Some vehicles and drones also grant real bonuses, such as extra armor or a larger battery.
* **Persistent Save Data:** Coins, owned items, selections, and audio settings are saved to JSON and restored on the next launch.
* **Full UI:** Start, game, shop, help, and settings screens, plus pause and win/loss overlays with music ducking.
* **Data-Driven Levels:** Levels and the shop catalog live in `gameData.json`, so new levels or items can be added without recompiling.

## 🏗️ Implementation Details

This project was designed with a heavy focus on clean architecture and object-oriented design:

* **Graphics & User Interface:** Built entirely with SFML 3. Reusable components (buttons, overlays, scrollable text, shop previews) are shared across all screens.
* **Game Loop & Screen States:** A central `Game` class drives the loop (events → commands → update → render) and swaps between `Screen` objects (`StartScreen`, `GameScreen`, `ShopScreen`, `HelpScreen`, `SettingsScreen`) using the State pattern.
* **Object Hierarchy:** Every entity derives from an abstract `GameObject`. `World` owns them all as `unique_ptr`s and finds the player or the target through role predicates, without knowing any concrete type.
* **Collision & Interactions:** A multimethod lookup table maps each pair of dynamic types to a handler. Adding a new collision is a single new entry, and unregistered pairs simply ignore each other.
* **Object Creation:** A Singleton `Factory` builds game objects and enemies from enums.
* **Resource Management:** Singleton `ResourceManager`, `SoundManager`, and `DataManager` handle lazy asset loading, audio, and JSON data, including swapping skins at runtime based on shop selections.

A full breakdown of every file, data structure, and algorithm is available in [PROJECT_DETAILS.txt](PROJECT_DETAILS.txt).

## 👥 Authors

* **Benji Mandel:** User interface, screens, shop, application flow, and resource management.
* **Yitzhak Rose:** Game concept and story, enemy AI, gameplay objects, and the game world logic.

## 🛠️ Build Instructions

This project uses **CMake** for its build system.

### Prerequisites

* C++20 compiler
* CMake (3.26+)
* SFML 3

### Building

Clone the repository and build using CMake:

```bash
git clone https://github.com/yitzhak-rose/skyshield-game.git
cd skyshield-game

mkdir build
cd build

# Generate build files and compile
cmake ..
cmake --build .

```

### Running

Once built, you can run the executable directly from the build directory:

```bash
./oop2_project

```

## 🎮 Controls

* **Left / Right Arrows:** Move the drone
* **Space:** Drop a bomb
* **Esc:** Pause / Go back
* **Mouse:** Navigate menus and the shop

## 🏆 Credits

* **Sounds:** YouTube (Gaming Sound FX, Alec Koff Label), Pixabay (HitsLab sounds, Tuomas_Data, floraphonic)
* **Libraries:** [nlohmann/json](https://github.com/nlohmann/json), JSON for Modern C++ by Niels Lohmann (MIT License)

## ⚖️ License

**All Rights Reserved**

The source code in this repository is provided for viewing and portfolio purposes only. You are welcome to compile and play the game for personal entertainment. However, you may not copy, modify, distribute, or use this code for any other purpose.
