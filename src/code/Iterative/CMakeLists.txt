CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Iterative)

INCLUDE_DIRECTORIES(${Halftone_SOURCE_DIR}/Library)

ADD_LIBRARY(${PROJECT_NAME}
ClassWeight.cpp
ClassWeight.hpp
ContinuousVoidCluster.cpp
ContinuousVoidCluster.hpp
DiscreteVoidCluster.cpp
DiscreteVoidCluster.hpp
FairClassWeight.cpp
FairClassWeight.hpp
GridContinuousVoidCluster.cpp
GridContinuousVoidCluster.hpp
GridJitterVoidCluster.cpp
GridJitterVoidCluster.hpp
GridRegularVoidCluster.cpp
GridRegularVoidCluster.hpp
RainbowClassWeight.cpp
RainbowClassWeight.hpp
VoidCluster.cpp
VoidCluster.hpp
WhiteRandomVoidCluster.cpp
WhiteRandomVoidCluster.hpp
)
