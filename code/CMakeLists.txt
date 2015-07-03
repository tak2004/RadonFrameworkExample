# This example show how to use Radon Framework as external library.
cmake_minimum_required(VERSION 2.8)
project(Console)

# Integrate.cmake was copied from the Radon Framework repository.
# It can be find at ${RADONFRAMEWORK_DIR}/cmake/extern/Integrate.cmake
include("Integrate.cmake")
# Look at Integrate.cmake for detailed description of this function.
Integrate(RADONFRAMEWORK "Radon framework" "RADONFRAMEWORK_HOME")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)

# Generate looks for ${ProjectId}_FILES and add the containing files to the project.
set(CONSOLE_FILES "client/console.cpp")
# You find further information in cmake/extern/Generate.cmake .
Generate(EXECUTABLE CONSOLE console "Frontend")
# Propagate settings from current target to Radon Framework.
if (${CONSOLE_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
AddDependency(CONSOLE RadonFramework)
# This call will take care of compiler and linker settings.
Finalize(CONSOLE)

set(WINDOW_FILES "client/window.cpp")
Generate(EXECUTABLE WINDOW window "Frontend")
if (${WINDOW_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
AddDependency(WINDOW RadonFramework)
Finalize(WINDOW)

set(SYSTEMTRAY_FILES "client/systemtray.cpp")
Generate(EXECUTABLE SYSTEMTRAY systemtray "Frontend")
if (${SYSTEMTRAY_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
AddDependency(SYSTEMTRAY RadonFramework)
Finalize(SYSTEMTRAY)