
cmake_minimum_required(VERSION 3.10)

# enables C and C++
project(openGL_study C CXX)

# Requests C++ 17
set(CMAKE_CXX_STANDARD 17)

#forces the compiler to actually use C++ 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#need to let vs code read cmake compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# "There is another CMAKE project here. Build it as part of mine"
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)

# Creates a static library target called glad
add_library(glad third_party/glad/src/glad.c)

# Tells CMake where glad.h lives
#PUBLIC means that glad itself can include its headers, anything that links against glad can include them too
#If this was PRIVATE my app could not see glad/glad.h
target_include_directories(glad PUBLIC third_party/glad/include)

add_executable(openGL_study src/main.cpp src/Camera.cpp
		src/Engine.cpp
		src/Engine.h
		src/Application.cpp
		src/Application.h
		src/Renderer.cpp
		src/Renderer.h
		src/Mesh.cpp
		src/Mesh.h
		src/Shader.cpp
		src/Shader.h
		src/Logger.h
		src/GLDebug.cpp
		src/GLDebug.h
		src/Voxel.h
		src/Chunk.cpp
		src/Chunk.h
		src/ChunkPos.h
		src/World.cpp
		src/World.h
		src/Frustum.cpp
		src/Frustum.h
		src/stb_image.h
		src/std_implementation.cpp
		src/std_implementation.h
		src/Texture.cpp
		src/Texture.h
)

#declares dependencies. 1 - links compiled code, 2 - propagates include paths, 3 - propagates compile definitions
target_link_libraries(openGL_study glfw glad glm)

#Linux-only requirement, dl provides: dlopen, dlsym. GLFW uses this to load OpenGL function pointers.
if(UNIX AND NOT APPLE)
	target_link_libraries(openGL_study dl)
endif()
