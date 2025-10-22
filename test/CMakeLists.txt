set(target ComplexTest)

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../gtest")

add_executable(${target} ${srcs} ${hdrs})
target_link_libraries(${target} gtest ${strlibrary})
target_link_libraries(${target} gtest ${vectorlibrary})
target_link_libraries(${target} gtest ${matrixlibrary})
target_link_libraries(${target} gtest ${errorlibrary})