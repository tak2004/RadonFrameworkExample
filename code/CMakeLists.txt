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

# This functions are located at cmake/util/Macros.cmake . 
AddSourceDirectoryRecursive(srclist "client" "Source")
AddHeaderDirectoryRecursive(hdrlist "client" "Include")
# Generate looks for ${ProjectId}_FILES and add the containing files to the project.
set(CONSOLE_FILES ${srclist} ${hdrlist})

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