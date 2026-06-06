cmake_minimum_required(VERSION 3.15)
project(Sortierverfahren)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(MSVC)
    add_compile_options(/std:c++latest)
endif()

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
     src/Quicksort.cpp
     src/MiniSort.cpp
     src/Messdaten.cpp
     src/string/ManagerString.cpp
     src/string/ListenerstelerString.cpp
     src/string/SortierverfarenString.cpp
     src/string/MergesortString.cpp
     src/string/QuicksortString.cpp
)

