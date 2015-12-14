#include "EnvireVisualizer.h"
#include "EnvireVizPlugin.h"
#include "EnvireVisualizerExceptions.h"
#include <envire_core/graph/TransformGraph.hpp>
#include <envire_core/events/ItemAddedEvent.hpp>
#include <envire_core/events/ItemRemovedEvent.hpp>
#include <envire_core/events/TransformModifiedEvent.hpp>
#include <envire_core/items/Item.hpp>
#include <iostream>
#define BASE_LOG_DEBUG
#include <base/Logging.hpp>
#include <osgViz/tools/TypeNameDemangling.h>


using namespace osgviz;
using namespace envire::core;

namespace envire { namespace visualizer 
{
  
EnvireVisualizer::EnvireVisualizer(std::shared_ptr< core::TransformGraph > graph,
                                   const FrameId rootFrame) :
  graph(graph), osgViz(OsgViz::getInstance()), rootFrame(rootFrame)
{
  assert(nullptr != graph.get());
  graph->getTree(rootFrame, true, &tree);//the view will be keept up to date automatically
  loadTree();
}

void EnvireVisualizer::loadPlugin(const std::string& name)
{
  EnvireVizPlugin* pPlugin = osgViz->getVisualizerPlugin<EnvireVizPlugin>(name);
  if(nullptr != pPlugin)
  {
    addPlugin(pPlugin);
    //FIXME only for testing
    loadTree();
  }
  else
  {
    throw PluginLoadError("unable to load plugin '" + name + "'. Reason: Pointer is null.");
  }
}

void EnvireVisualizer::start()
{
  osgViz->createWindow();
  osgViz->startThread();  
}
  
void EnvireVisualizer::addPlugin(EnvireVizPlugin* pPlugin)
{
  assert(nullptr != pPlugin);
  LOG_INFO_S << " Adding plugin " << pPlugin->getLibName() << " for types:";

  for(const std::type_index& type : pPlugin->getSupportedTypes())
  {
    if(plugins.find(type) != plugins.end())
    {
      throw PluginAlreadyExistsForType(pPlugin->getLibName(), plugins[type]->getLibName(), type.name());
    }
    plugins[type] = pPlugin;
    LOG_INFO_S << " " << demangledTypeName(type) << " Hash: " << pPlugin->getSupportedTypes().back().hash_code();
  }
}

void EnvireVisualizer::loadTree()
{
  osg::ref_ptr<osgviz::Object> root(new osgviz::Object());
  root->setName("root");
  vertex_descriptor rootVertex = graph->getVertex(rootFrame);
  addItems(rootVertex, root); //special case for items in the root vertex
  loadChildren(rootVertex, root);
  osgViz->addChild(root);
}


void EnvireVisualizer::loadChildren(const vertex_descriptor parentFrame,
                                    osg::ref_ptr<osgviz::Object> parent)
{
  const VertexRelation& relation = tree.tree[parentFrame];
  for(const vertex_descriptor child : relation.children)
  {
    const edge_descriptor edge = graph->getEdge(parentFrame, child);
    const Transform tf = graph->getTransform(edge);
    const base::Vector3d& translation = tf.transform.translation;
    const base::Orientation& orientation = tf.transform.orientation;
    
    osg::ref_ptr<osgviz::Object> transformNode(new osgviz::Object());
    transformNode->setPosition(osg::Vec3d(translation.x(), translation.y(), translation.z()));
    transformNode->setAttitude(osg::Quat(orientation.x(), orientation.y(), orientation.z(),
                                     orientation.w()));
    transformNode->setName(graph->getFrameId(parentFrame) + " --> " + graph->getFrameId(child));
    parent->addChild(transformNode);
    addItems(child, transformNode);
    loadChildren(child, transformNode);
  }
}

void EnvireVisualizer::addItems(const vertex_descriptor frame, osg::ref_ptr<osgviz::Object> node)
{
  for(auto pluginPair : plugins)
  {
    const std::type_index& type = pluginPair.first;
    if(graph->containsItems(frame, type))
    {
      using Items = Frame::ItemList;
      const Items& items = graph->getItems(frame, type);
      EnvireVizPlugin* plugin = pluginPair.second;
      for(const ItemBase::Ptr item : items)
      {
          osg::ref_ptr<osgviz::Object> visual = plugin->createItemVisualization(type, item);
          node->addChild(visual);
      }
    }
  }
}


void EnvireVisualizer::itemAdded(const core::ItemAddedEvent& e)
{

}

void EnvireVisualizer::itemRemoved(const core::ItemRemovedEvent& e)
{

}

void EnvireVisualizer::transformModified(const core::TransformModifiedEvent& e)
{

}



}}