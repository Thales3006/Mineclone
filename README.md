# Mineclone
A simple [minecraft](https://pt.wikipedia.org/wiki/Minecraft) clone in C++

This little project was made to understand in depth the inner mechanics of the pure openGL API in C++ and computer graphics in general (vectors, matrices and etc).

The game used C++ for logic and overall functioning and setting openGL parameters and initializations. 

The openGL function loader used was [GLAD](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library), 
this library is available [here](https://glad.dav1d.de).

As the shading language to comunicate with the GPU, the [GLSL](https://en.wikipedia.org/wiki/OpenGL_Shading_Language) language was used, as it is simple and resembles C/C++ language style.

Collisions were also added and simple physics to imitate minecraft's survival mode.

## Example

![Screenshot example](./imgs/example.png)

This image shows the program functioning as expected.

It is possible to see the textures and illumination working properly, even though it could be improved to resemble the original game, 
the main objective of understanding computer graphics and openGL was accomplished.

## Dependencies

The project only requires [nix](https://nixos.org/download/) packet manager using flakes.
It can be installed in any Linux distro and in WSL (Windows Subsystem for Linux).

## Run

You can run the project without installing it permanently using the following command: 

```sh
nix run github:Thales3006/Mineclone
```

It should take care of all dependencies automatically.

You can also clone the repository and run it from local sources:

```sh
git clone https://github.com/Thales3006/Mineclone
cd Mineclone

nix run
```

## Build

To build, the same logic as to installing is applied:

```sh
nix build github:Thales3006/Mineclone
```

Or, you could also:

```sh
git clone https://github.com/Thales3006/Mineclone
cd Mineclone

nix run
```

This will create a symlink `results/bin` to the build.

Then, to run the game:
```sh
./results/bin/mineclone
```

### Windows

Because nix can only be used with WSL, in windows. It needs to build the app instead of just running it.
There is a windows build that can be built using:

```sh
nix build github:Thales3006/Mineclone#windows

# or

git clone https://github.com/Thales3006/Mineclone
cd Mineclone

nix build .#windows
```

But it is still a work in progress with some texture rendering bugs.
```
```

## Considerations

I plan to upgrade to project in the future in my free time, but this cannot be assured as work and life becomes more agitated.
