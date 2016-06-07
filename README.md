# TROLL Engine

A small toy 3D engine based on OpenGL 3.3, programmed in C++14 that I write and use
to learn about stuff.

You probably won't use this in anything serious, but if you're interested in learning
OpenGL and/or graphics programming, you might find some interesting stuff here.

Mainly developed on Linux. Somewhat tested on windows, but you'll need [MSYS2](https://msys2.github.io/)
to build.
You wanna build it another way, that's your problem. Mac users, you're on your own.

## Dependencies

The build system is based on CMake.

* GLFW3 or Qt5
* GLM
* FreeImage
* Assimp
* Mercurial (used by the build system to fetch other dependencies not usually bundled
  in distributions)
* Lua 5.1

## Hardware requirements

Most OpenGL 3.3 compliant hardware should work. The only requirement is the
`GL_KHR_DEBUG` extension, which most hardware supports.

The OpenGL target spec might change in the future if I ever need OpenGL 4 features.

## Features

At the time, not so many worth mentioning. I'm still building a lot of basics.

* Can be used with GLFW3 or Qt5
* Should be easily extendable to use with other windowing toolkits.

## License

Not decided yet, probably BSD or MIT or... something.

## FAQ

* Does the name stand for anything?

Not yet, but I'm sure I'll come up with something clever one day, like a better name.
