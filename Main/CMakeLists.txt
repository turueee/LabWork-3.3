file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")
add_executable(${application} ${srcs} ${hdrs})

target_link_libraries(${application} ${strlibrary})
target_link_libraries(${application} ${matrixlibrary})
target_link_libraries(${application} ${arrlibrary})

#add_library(${MyLib1} STATIC ${srcs} ${hdrs})


