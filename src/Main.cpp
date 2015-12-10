#include "EnvireVizPlugin.h"
#include <osgViz/OsgViz.hpp>

using namespace envire::Visualizer;
int main()
{
  osgviz::OsgViz *osgViz = osgviz::OsgViz::getInstance();
  
  EnvireVizPlugin* plugin = osgViz->getVisualizerPlugin<EnvireVizPlugin>("EnvireUrdfVisualizer");
  
  
  
}
