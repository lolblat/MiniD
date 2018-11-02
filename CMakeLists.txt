cmake_minimum_required (VERSION 2.6)
project (MiniD)

add_executable(MiniD program.cpp Debugger.cpp)
set_target_properties(MiniD PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
