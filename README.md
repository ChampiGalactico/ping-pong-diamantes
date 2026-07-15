# PingPongDiamantes

A local co-op Pong game with powers, built in Unreal Engine using C++.

## About this project

This is a C++ rewrite of an earlier version of the same game that I originally built entirely with Blueprints. I'm using this project as a hands-on way to learn C++ and Unreal's C++/Blueprint hybrid workflow — moving core gameplay logic (paddle movement, input handling, game modes, ball physics) from visual scripting into native code, while still using Blueprints for designer-facing configuration (meshes, materials, tuning values).

This is my first C++ project, so the codebase reflects an active learning process rather than a finished, polished product.

## Features implemented so far
- Pawn-based paddle with Enhanced Input (split-keyboard local co-op)
- Custom GameMode handling paddle spawning, player possession, and mode switching
- GameInstance persisting single-player vs. co-op selection across levels
- Physics-based ball with bounce, speed scaling, and sound
- Fixed top-down camera
