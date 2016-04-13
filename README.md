# TROLL Engine

A small toy 3D engine based on OpenGL 3.3, programmed in C++11 that I write and use
to learn about stuff. It's mostly focused on simplicity and ease of use.

You probably won't use this in anything serious, but if you're interested in learning
OpenGL and/or graphics programming, you might find some interesting stuff here.

Mainly developed on Linux. Somewhat tested on windows, but you'll need MSYS2 to build.
You wanna build it another way, that's your problem. Mac users, you're on your own.

## Dependencies

The build system is based on CMake.

* GLFW3
* GLM
* FreeImage
* Flex and Bison (used for OBJ parser, I'll get rid of this and use a proper lib
  soon)
* Mercurial (used by the build system to fetch other dependencies not usually bundled
  in distributions)

## Hardware requirements

Most OpenGL 3.3 compliant hardware should work. The only requirement is the
`GL_KHR_DEBUG` extension, which most hardware supports.

The OpenGL target spec might change in the future if I ever need OpenGL 4 features.

## License

Not decided yet, probably BSD.

## FAQ

* Does the name stand for anything?

Not yet, but I'm sure I'll come up with something clever one day, like a better name.

* Vulkan support?

I'm interested, but that's gonna have to wait.
