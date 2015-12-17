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
#include <osgViz/tools/GraphPrinter.h>


using namespace osgviz;
using namespace envire::core;

namespace envire { namespace visualizer 
{
  
EnvireVisualizer::EnvireVisualizer(std::shared_ptr< core::TransformGraph > graph,
                                   const FrameId rootFrame) :
  graph(graph), osgViz(OsgViz::getInstance()), rootFrame(rootFrame), root(new osgviz::Object()),
  transformations(10, EdgeHash<core::TransformGraph>(graph))
{
  assert(nullptr != graph.get());
  root->setName("envire fake root");
  osgViz->addChild(root);
  graph->getTree(rootFrame, true, &tree);//the view will be keept up to date automatically
}

void EnvireVisualizer::loadPlugin(const std::string& name)
{
  EnvireVizPlugin* pPlugin = osgViz->getVisualizerPlugin<EnvireVizPlugin>(name);
  if(nullptr != pPlugin)
  {
    addPlugin(pPlugin);
    loadTree(pPlugin->getSupportedTypes());
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

void EnvireVisualizer::loadTree(const std::vector<std::type_index>& types)
{
  vertex_descriptor rootVertex = graph->getVertex(rootFrame);
  addItems(rootVertex, root, types); //special case for items in the root vertex
  loadChildren(rootVertex, root, types);
}


void EnvireVisualizer::loadChildren(const vertex_descriptor parentFrame,
                                    osg::ref_ptr<osgviz::Object> parent,
                                    const std::vector<std::type_index>& types)
{
  const VertexRelation& relation = tree.tree[parentFrame];
  for(const vertex_descriptor child : relation.children)
  {
    osg::ref_ptr<osgviz::Object> transformNode(nullptr);
    const edge_descriptor edge = graph->getEdge(parentFrame, child);
    if(transformations.find(edge) == transformations.end())
    {
      //FIXME speration of concerns? should this method create the nodes?
      //this is a new transformation, need to add it.
      const Transform tf = graph->getTransform(edge);
      const base::Vector3d& translation = tf.transform.translation;
      const base::Orientation& orientation = tf.transform.orientation;
      transformNode = new osgviz::Object();
      transformNode->setPosition(osg::Vec3d(translation.x(), translation.y(), translation.z()));
      transformNode->setAttitude(osg::Quat(orientation.x(), orientation.y(), orientation.z(),
                                           orientation.w()));
      transformNode->setName(graph->getFrameId(parentFrame) + " --> " + graph->getFrameId(child));
      parent->addChild(transformNode);
      transformations[edge] = transformNode;
    }
    else
    {
      transformNode = transformations[edge];
    }
    
    addItems(child, transformNode, types);
    loadChildren(child, transformNode, types);
  }
}

void EnvireVisualizer::addItems(const vertex_descriptor frame, osg::ref_ptr<osgviz::Object> node,
                                const std::vector<std::type_index>& types)
{
  for(const std::type_index& type : types)
  {
    if(graph->containsItems(frame, type))
    {
      const Frame::ItemList& items = graph->getItems(frame, type);
      assert(plugins.find(type) != plugins.end()); //a plugin for this type has to exist, otherwise the type should not be in the list 
      EnvireVizPlugin* plugin = plugins[type];
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

void EnvireVisualizer::printTotGraph(const std::string& filename)
{
  if(filename.empty())
  {
    GraphPrinter::print(root.get(), true, nullptr);
  }
  else
  {
    GraphPrinter::print(root.get(), true, filename.c_str());
  }
}


}}