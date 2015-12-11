  
rock_library(envire_visualizer
    SOURCES
    EnvireVisualizer.cpp
    HEADERS 
    EnvireVisualizer.h
    EnvireVisualizerExceptions.h    
    DEPS_PKGCONFIG osgVizPlugin
    DEPS_PKGCONFIG envire_core
    DEPS_PKGCONFIG osgViz
    DEPS_PKGCONFIG base-types)
    
rock_executable(envire_visualizer_bin
    SOURCES 
    Main.cpp
    DEPS envire_visualizer
    DEPS_PKGCONFIG envire_core)


install(FILES
    DESTINATION include/envire_visualizer)
