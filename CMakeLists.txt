# This example show how to use Radon Framework as external library.

if(NOT CMAKE_PROJECT_NAME OR "${CMAKE_PROJECT_NAME}" MATCHES "^RadonExamples$")
    cmake_minimum_required(VERSION 2.8)
    project(RadonExamples)
    
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
endif()

# Integrate.cmake was copied from the Radon Framework repository.
# It can be find at ${RADONFRAMEWORK_DIR}/cmake/extern/Integrate.cmake
include("Integrate.cmake")
# Look at Integrate.cmake for detailed description of this function.

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs")

# Generate looks for ${ProjectId}_FILES and add the containing files to the project.
set(CONSOLE_FILES "code/console/console.cpp")
# You find further information in cmake/extern/Generate.cmake .
Generate(EXECUTABLE CONSOLE console "Frontend")
# Propagate settings from current target to Radon Framework.
if (${CONSOLE_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
AddDependency(CONSOLE RadonFramework)
add_dependencies(console ${RADONFRAMEWORK_STATIC_SERVICE_TARGETS} ${RADONFRAMEWORK_DYNAMIC_SERVICE_TARGETS})
# This call will take care of compiler and linker settings.
Finalize(CONSOLE)

set(WINDOW_FILES "code/window/window.cpp" "code/window/window.hpp")
AddSourceDirectoryRecursive(filelist "code/shared" "Sources\\Shared")
list(APPEND WINDOW_FILES ${filelist})
AddHeaderDirectoryRecursive(filelist "code/shared" "Includes\\Shared")
list(APPEND WINDOW_FILES ${filelist})
Generate(EXECUTABLE WINDOW window "Frontend")
if (${WINDOW_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
target_link_libraries(window nvapi64)
AddDependency(WINDOW RadonFramework)
add_dependencies(window ${RADONFRAMEWORK_STATIC_SERVICE_TARGETS} ${RADONFRAMEWORK_DYNAMIC_SERVICE_TARGETS})
Finalize(WINDOW)

set(SYSTEMTRAY_FILES "code/systemtray/systemtray.cpp")
Generate(EXECUTABLE SYSTEMTRAY systemtray "Frontend")
if (${SYSTEMTRAY_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
AddDependency(SYSTEMTRAY RadonFramework)
add_dependencies(systemtray ${RADONFRAMEWORK_STATIC_SERVICE_TARGETS} ${RADONFRAMEWORK_DYNAMIC_SERVICE_TARGETS})
Finalize(SYSTEMTRAY)

set(UI_FILES "")
AddSourceDirectoryRecursive(filelist "code/ui" "Sources")
list(APPEND UI_FILES ${filelist})
AddHeaderDirectoryRecursive(filelist "code/ui" "Includes")
list(APPEND UI_FILES ${filelist})
AddSourceDirectoryRecursive(filelist "code/shared" "Sources\\Shared")
list(APPEND UI_FILES ${filelist})
AddHeaderDirectoryRecursive(filelist "code/shared" "Includes\\Shared")
list(APPEND UI_FILES ${filelist})
Generate(EXECUTABLE UI ui "Frontend")
if (${UI_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
target_link_libraries(ui nvapi64)
AddDependency(UI RadonFramework)
add_dependencies(ui ${RADONFRAMEWORK_STATIC_SERVICE_TARGETS} ${RADONFRAMEWORK_DYNAMIC_SERVICE_TARGETS})
Finalize(UI)

set(RENDERER_FILES "")
AddSourceDirectoryRecursive(filelist "code/renderer" "Sources")
list(APPEND RENDERER_FILES ${filelist})
AddHeaderDirectoryRecursive(filelist "code/renderer" "Includes")
list(APPEND RENDERER_FILES ${filelist})
AddSourceDirectoryRecursive(filelist "code/shared" "Sources\\Shared")
list(APPEND RENDERER_FILES ${filelist})
AddHeaderDirectoryRecursive(filelist "code/shared" "Includes\\Shared")
list(APPEND RENDERER_FILES ${filelist})
Generate(EXECUTABLE RENDERER Renderer "Frontend")
if (${RENDERER_LINKER_USE_DEFAULTLIB})
	set(RADONFRAMEWORK_USE_MICROCRT OFF CACHE BOOL "" FORCE)
else()
	set(RADONFRAMEWORK_USE_MICROCRT ON CACHE BOOL "" FORCE)
endif()
target_link_libraries(Renderer nvapi64)
AddDependency(RENDERER RadonFramework)
add_dependencies(Renderer ${RADONFRAMEWORK_STATIC_SERVICE_TARGETS} ${RADONFRAMEWORK_DYNAMIC_SERVICE_TARGETS})
Finalize(RENDERER)