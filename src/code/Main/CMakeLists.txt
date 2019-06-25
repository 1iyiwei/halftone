CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Stub)

# List all source files
SET(SOURCE_FILES
    Stub.cpp)

# library path
INCLUDE_DIRECTORIES(${Halftone_SOURCE_DIR}/Library)
LINK_DIRECTORIES(${Halftone_BINARY_DIR}/Library)

# Create the executable target
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Library)

