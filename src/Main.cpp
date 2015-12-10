#include "EnvireVizPlugin.h"
#include <osgViz/OsgViz.hpp>

using namespace envire::visualizer;
int main()
{
  osgviz::OsgViz *osgViz = osgviz::OsgViz::getInstance();
  
  EnvireVizPlugin* plugin = osgViz->getVisualizerPlugin<EnvireVizPlugin>("envire_visualizer_urdf_plugin");  
  
}
