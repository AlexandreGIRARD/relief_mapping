# RELIEF MAPPING

This project is a simple implementation of the *Relief Mapping* algorithm presented by Fabio Policarpo, Manuel M. Oliveira and Joao L. D. Comba in the paper [**Real-Time Relief Mapping on Arbitrary Polygonal Surfaces**](https://www.cs.purdue.edu/cgvlab/courses/434/434_Spring_2013/lectures/References/DepthImagesForRenderingSurfaceDetail.pdf) in 2005.

This project is implemented using OpenGL 4.3 and C++.

It used several external libraries:
- [GLFW](https://www.glfw.org/) --> OpenGL context and window API
- [Assimp](https://www.assimp.org/) --> 3D objects parser
- [Glad](https://github.com/Dav1dde/glad) --> OpenGL loader
- [glm](https://glm.g-truc.net/0.9.2/api/index.html) --> Vectors, matrices ... operations header
- [stbi_image](https://github.com/nothings/stb) --> Image loader

In order to use this project you must install GLFW and assimp on your computer.

:exclamation::exclamation: For now all textures should be at **jpeg** format.
