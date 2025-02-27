cmake_minimum_required (VERSION 3.25)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (SimpleFS C CXX)

add_compile_definitions(__DEBUG__)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_library(SimpleFS OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/ConsoleMain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/include/log.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp
)
target_link_libraries(SimpleFS PUBLIC DbgHelp)

enable_testing()
function(add_test_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} SimpleFS)
  add_test(NAME ${target} COMMAND ${target})
endfunction()

add_test_executable(test.log tests/log.cpp)
