# Animated wallpaper for Windows 10

### Run GLSL shaders directly on your desktop background

This project allows you to run your GLSL shaders directly on your desktop background (wallpaper) (currently only for Windows 10). Modify the file "shaders/fragment.shader" to your liking!
Feel free to use this project in any way you want.


## How to compile this project

[MSYS2](https://www.msys2.org/) has been used to compile this project. You will need:
- mingw64: `pacman -S mingw-w64-x86_64-toolchain`
- [make](https://packages.msys2.org/packages/make): `pacman -S make`
- [glm](https://packages.msys2.org/packages/mingw-w64-x86_64-glm): `pacman -S mingw-w64-x86_64-glm`

1. Run `git clone https://github.com/andre9432/animated-wallpaper.git`
2. Run `make` in the root directory.
3. Run the executable *run.exe*.

## Known issues
Does not work on Windows 11 (only 10) yet. Working on it..
