set(headers EnvireVizPlugin.h)            
set(sources Main.cpp)    

rock_executable(envire_visualizer_bin
    HEADERS ${headers} 
    SOURCES ${sources}
    Main.cpp
    DEPS_PKGCONFIG osgViz
    DEPS_PKGCONFIG base-types)
    
rock_library(envire_visualizer
    SOURCES
    HEADERS
    ${headers} 
    DEPS_PKGCONFIG osgVizPlugin)


install(FILES
    DESTINATION include/envire_visualizer)
