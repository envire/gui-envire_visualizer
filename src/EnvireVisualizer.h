#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <envire_core/events/GraphEventDispatcher.hpp>
#include <envire_core/graph/TransformGraphTypes.hpp>
#include <envire_core/graph/TreeView.hpp>
#include <osgViz/OsgViz.hpp>


namespace envire { namespace core 
{
  class TransformGraph;
}}

namespace envire { namespace visualizer 
{
  class EnvireVizPlugin;

/** Loads all available visualizer plugins and uses them to visualize a 
 *  TransformGraph. */ 
class EnvireVisualizer : envire::core::GraphEventDispatcher
{
public:
  
  /** @param pGraph the Graph that should be visualized */
  EnvireVisualizer(std::shared_ptr<core::TransformGraph> graph, const envire::core::FrameId rootFrame);
  
  /**Loads the specified plugin
   * @throw PluginAlreadyExistsForType If another plugin already supports at least one of the
   *                                   types supported by the specified plugin.
   * @throw PluginLoadError If the plugin could not be loaded for any other reason*/
  void loadPlugin(const std::string& name);
  
  
  /**Start internal update thread and show window */
  void start();
  
  virtual void transformModified(const envire::core::TransformModifiedEvent& e);
  virtual void itemAdded(const envire::core::ItemAddedEvent& e);
  virtual void itemRemoved(const envire::core::ItemRemovedEvent& e);
  
protected:
  
  /** Add the plugin to the plugins list.
   * @throw PluginAlreadyExistsForType If another plugin already supports at least one of the
   *                                   types supported by the specified plugni.*/
  void addPlugin(EnvireVizPlugin* pPlugin);
  
  /**recursivly loads all children of the @p frame and add them to @node */
  void loadTree();
  void loadChildren(const envire::core::vertex_descriptor parentFrame, osg::ref_ptr<osgviz::Object> parent);
  
  /**Add all items of @p frame to @p node */
  void addItems(const envire::core::vertex_descriptor frame, osg::ref_ptr<osgviz::Object> node);
  
private:
  std::shared_ptr<core::TransformGraph> graph;
  osgviz::OsgViz* osgViz; /**< Pointer to the OsgViz singleton */
  std::unordered_map<std::type_index, EnvireVizPlugin*> plugins;/**< Stores the responsible plugin for each supported type */
  const envire::core::FrameId rootFrame;/**< Id of the frame used as root for visualization */
  envire::core::TreeView tree; /**< A tree representation of the graph starting at rootFrame*/
  
};

}}