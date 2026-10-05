# CMakeList.txt : CMake project for SimpleRaytracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SimpleRaytracer")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# all header files
set (HEADERS 
	src/camera.h
	src/color.h
	src/hittable.h
	src/hittableList.h
	src/interval.h
	src/material.h
	src/math.h
	src/ray.h
	src/sphere.h
	src/timer.h
	src/vec3.h
)

# all source files
set (SOURCES
	src/main.cpp
	src/material.cpp
	src/camera.cpp 
	src/interval.cpp
	src/sphere.cpp
	)

# Add necessary files to this project's executable.
add_executable (SimpleRaytracer ${SOURCES} ${HEADERS})

find_package(Threads REQUIRED)
target_link_libraries(SimpleRaytracer PRIVATE Threads::Threads)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SimpleRaytracer PROPERTY CXX_STANDARD 20)
endif()

