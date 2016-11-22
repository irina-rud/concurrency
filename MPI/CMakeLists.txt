cmake_minimum_required(VERSION 2.6.4)
project(MPI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c98")

set(SOURCE_FILES main.cpp  CGameBoard.cpp CTorus.cpp )

add_executable(MPI ${SOURCE_FILES})
find_package ( MPI )
if ( MPI_FOUND )
        include_directories( ${MPI_INCLUDE_PATH} )
endif( MPI_FOUND )