# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
set(ROCK_USE_CXX11 TRUE)
find_package(Rock)
set(ROCK_TEST_ENABLED ON)

rock_init(envire_visualizer 0.1)
add_definitions(-DBASE_LOG_NAMESPACE=$PROJECT_NAME)
rock_standard_layout()


