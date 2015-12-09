set(headers EnvireVizPlugin.h)            
set(sources Main.cpp)    

rock_executable(envire_visualizer
    HEADERS ${headers} 
    SOURCES ${sources}
    Main.cpp
    DEPS_PKGCONFIG osgViz
    DEPS_PKGCONFIG base-types)


install(FILES
    all
    DESTINATION include/envire_core)
