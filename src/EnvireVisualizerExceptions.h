#pragma once
#include <stdexcept>
#include <string>

namespace envire { namespace visualizer 
{
  
class PluginAlreadyExistsForType : public std::exception
{
public:
    explicit PluginAlreadyExistsForType(const std::string& newPluginName,
                                        const std::string& existingPluginName,
                                        const std::string& typeName) :
      msg("Unabled to load Plugin '" + newPluginName + "' because the type '" +
          typeName + "' is already covered by Plugin '" + existingPluginName + "'.") {}
    virtual char const * what() const throw() { return msg.c_str(); }
    const std::string msg;
};

class PluginLoadError : public std::exception
{
public:
    explicit PluginLoadError(const std::string& msg) :
      msg(msg) {}
    virtual char const * what() const throw() { return msg.c_str(); }
    const std::string msg;
};

}}