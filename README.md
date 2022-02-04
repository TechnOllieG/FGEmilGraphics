# FGEmilGraphics
Hello Emil! This is my graphics project, I've made several changes to the original project we made together. Here is a list of those changes:

1. Abstracted stuff away so that you now can just spawn objects with spawnObject and they will be automatically added to an object list and then rendered on screen. Basically a simple engine.
2. Made camera respond to mouse movement by using glfwGetCursorPos/glfwSetCursorPos etc. Multiplied two angle axis quaternions together to make this work (see Engine.h:315).
3. Added deltaTime which I multiplied camera movement stuff with.
