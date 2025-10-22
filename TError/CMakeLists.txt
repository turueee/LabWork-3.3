
file(GLOB hdrs "*.h*")

file(GLOB srcs "*.cpp")

add_library(${errorlibrary} STATIC ${srcs} ${hdrs})

target_link_libraries(${errorlibrary} ${strlibrary})


