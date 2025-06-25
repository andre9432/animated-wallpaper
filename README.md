# Animated wallpaper for Windows 10

### Run GLSL shaders directly on your desktop background

This project allows you to run your GLSL shaders directly on your desktop background (wallpaper) (currently only for Windows 10). Modify the file "shaders/fragment.shader" to your liking!
Feel free to use this project in any way you want.


## How to compile this project

[MSYS2](https://www.msys2.org/) has been used to compile this project. You will need:
- gcc: `pacman -S mingw-w64-ucrt-x86_64-gcc`
- make: `pacman -S make`
- git: `pacman -S git`
- glm: `pacman -S mingw-w64-ucrt-x86_64-glm`

**If you are compiling outside the msys2 environment, be sure to add `msys64\ucrt64\bin` and `msys64\usr\bin` to your environment variables' path**

**If you installed the above dependencies, you should have glm and GL folders in `msys64/ucrt64/include`, copy them into the project `include` directory.**

### Steps:

1. Run `git clone https://github.com/andre9432/animated-wallpaper.git`
2. Run `make` in the root directory.
3. Run the executable *run.exe*.

## Known issues
Does not work on Windows 11 (only 10) yet. Working on it..
