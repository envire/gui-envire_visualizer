#include "EnvireVisualizer.h"
#include <envire_core/graph/TransformGraph.hpp>
#include <envire_core/items/Item.hpp>
#include <memory>
#include <unistd.h>//sleep
#include <string>

using namespace envire::visualizer;
using namespace std;
using StrItem = envire::core::Item<std::string>;
int main()
{
  shared_ptr<envire::core::TransformGraph> g(new envire::core::TransformGraph());
  g->addFrame("root");
  StrItem::Ptr item(new StrItem("teeest"));
  g->addItemToFrame("root", item);
  
  EnvireVisualizer visualizer(g, "root");
  visualizer.loadPlugin("envire_visualizer_urdf_plugin");
  visualizer.start();
  
  while(true)
  {
    sleep(1);
  }
}
