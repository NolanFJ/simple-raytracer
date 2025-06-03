# CMakeList.txt : CMake project for SimpleRaytracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SimpleRaytracer")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
add_executable (SimpleRaytracer "src/main.cpp"  "src/vec3.h" "src/color.h" "src/ray.h" "src/hittable.h" "src/sphere.h" "src/hittableList.h" "src/math.h" "src/interval.h" "src/camera.h" "src/material.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SimpleRaytracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
