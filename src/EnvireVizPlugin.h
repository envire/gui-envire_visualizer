#pragma once
#include <typeindex>
#include <stdexcept>
#include <string>
#include <osgViz/plugins/OsgVizVisualizerPlugin.h>
#include <envire_core/items/ItemBase.hpp>

namespace envire { namespace Visualizer {

/**The base class for all osgViz plugins that are able to visualize envire::Items */
class EnvireVizPlugin : public osgviz::OsgVizVisualizerPlugin
{
public:
  
  
  struct TypeNotSupportedException : public std::exception
  {
    explicit TypeNotSupportedException(const const std::string& type_name) :
      msg("Plugin does not support type: " + type_name) {}
    virtual char const * what() const throw() { return msg.c_str(); }
    const std::string msg;
  }; 
  
  /**Creates a 3d visualization of @p item.
   * @param item The item that should be visualized
   * @param type The real type of @p item. This information is needed for downcasting.
   * @throw TypeNotSupportedException if @p type is not supported. 
   * @return A memory managed pointer to the created visualization.
   */
  virtual osg::ref_ptr<Object> createItemVisualization(const std::type_index& type,
                                                       const envire::core::ItemBase::Ptr item) = 0;
                                                       
  /** @return a vector of all types that are supported by this Visualizer.
   *          I.e. The visualizer is able to create a visual for each of those types*/                                                       
  virtual const std::vector<std::type_index>& getSupportedTypes() = 0;
};

}}