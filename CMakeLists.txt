cmake_minimum_required(VERSION 3.15)
project(Sortierverfahren)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# === Build-Typ auf Debug setzen, falls nicht gesetzt ===
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# === Includes ===

# === Quellcode-Dateien ===
add_executable(Sortierverfahren
     src/main.cpp
     src/Manager.cpp
     src/Dateimanager.cpp
     src/Listenersteler.cpp
     src/Sortierverfaren.cpp
     src/Mergesort.cpp
     src/Quiksort.cpp
)

