file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_library(${strlibrary} STATIC ${srcs} ${hdrs})

target_link_libraries(${strlibrary} ${ritlibrary})
target_link_libraries(${strlibrary} ${alloclibrary})
target_link_libraries(${strlibrary} ${arrlibrary})
target_link_libraries(${strlibrary} ${vectorlibrary})



