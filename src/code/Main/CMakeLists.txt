CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Stub)

# List all source files
SET(SOURCE_FILES
    Stub.cpp)

# library path
INCLUDE_DIRECTORIES(${Halftone_SOURCE_DIR}/Library)
INCLUDE_DIRECTORIES(${Halftone_SOURCE_DIR}/Iterative)
INCLUDE_DIRECTORIES(${Halftone_SOURCE_DIR}/ErrorDiffusion)

LINK_DIRECTORIES(${Halftone_BINARY_DIR}/Library)
LINK_DIRECTORIES(${Halftone_BINARY_DIR}/Iterative)
LINK_DIRECTORIES(${Halftone_BINARY_DIR}/ErrorDiffusion)


SET(LIBRARIES Library Iterative ErrorDiffusion)

set(projects
ConstrainedErrorDiffuse
ErrorDiffuse
Histogram
Stub
Test
VoidCluster
VoidClusterVis
)

foreach(project ${projects})

project(${project})
add_executable(${project} ${project}.cpp)
target_link_libraries(${project} ${LIBRARIES})

endforeach(project)
