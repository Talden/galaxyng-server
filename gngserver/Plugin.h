/* -*- mode: c++ -*- */
#if !defined(PLUGIN_H_)
#define PLUGIN_H_
/*
  Copyright 2005, Kenneth D. Weinert
    
  This file is part of GNGServer

  GNGServer is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  GNGServer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with GNGServer; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  Based on code that appeared in the July 2004 _C/C++ Users Journal_
  that was written by Arthur J. Musgrove. See ACKNOWLEDGEMENTS in the
  main directory for more information
*/

#include "gngserver/SharedLibrary.h"
#include <memory>
#include <string>
#include <map>

//////////////////////////////////////////
/// This is the base class for all plugins.

/// All plugins must have a name and a type. This is the base class
/// that is returned from all plugin operations.

class Plugin
{
public:
  /// Constructor, takes a name and a type

  /// \param name  full name of the plugin
  /// \param type  type of plugin - where type will be something like
  /// "map" or "report"
  Plugin(const char *name, const char *type)
    : m_name(name), m_type(type) { }

  virtual ~Plugin() { };

  /// Returns the name of the plugin.
  virtual const char *Name() const { return m_name.c_str(); }
  /// Returns the type of the plugin. 
  virtual const char *Type() const { return m_type.c_str(); }
  
private:
  std::string m_name;
  std::string m_type;
};

//////////////////////////////////////////
/// The initialization typedef for all plugins.

typedef Plugin* (*pluginInitFn)(void);

//////////////////////////////////////////
/// This is the class that manages all plugins.

/// The PluginManager does the initial load of plugins, can load a
/// specific plugin on request and will return an instance of a plugin
/// by type.
/// This is a Highlander plugin system - there can only be one of each
/// type. This is because plugins are used to adapt the game engine to
/// the type of game that is being run and it allows for rules
/// variations. This makes it easier to extend the game, etc - but you
/// don't want, for example, two different rule sets in play for how a
/// planet gets bombed

class PluginManager
{
public:
  static PluginManager& instance();

  /// Add a path to the list of paths to search in for plugins

  /// SearchPath() adds one or more paths to the list of directories
  /// to be searched for plugins if just a plugin name is given
  /// \param path can be a single directory or a ':' separated list of
  /// directories 
  void SearchPath(const char *path);

  /// FindPlugin() will look for a specific plugin on the filesystem
  /// by name
  /// \param name the name of the plugin to search for. It is assumed
  /// that you have provided a full name (readmap.so) but not
  /// necessarily a full path
  Plugin& FindPlugin(const char *name)
    throw (SharedLibraryException);

  /// FindAllPlugins() searches a tree (specified by configure,
  /// default is /usr/local/GNGServer/modules) for all available plugins,
  /// first by game type (/usr/local/GNGServer/modules/<gameType>) and
  /// then generic ones. Later occurring ones do *not* override ones
  /// already loaded
  /// \param gameType the type of game (examples: NG, Blind) - this
  /// translates directly into a directory name when searching for
  /// plugins 
  void FindAllPlugins(const char *gameType);

private:
  struct PluginInfo {
    SharedLibrary *m_library;
    std::string m_libraryName;
    Plugin *m_plugin;

    ~PluginInfo() { delete m_plugin; delete m_library; }
    PluginInfo() : m_library(0), m_plugin(0) { }
  };

  PluginManager() { }
  static PluginManager *m_instance;
  std::map<std::string, PluginInfo*> m_pluginMap;

  std::vector<std::string> m_searchList;
  
 private:  // in order to disallow
  ~PluginManager();
  PluginManager(const PluginManager& right);
};

class PluginNameMap
{
 public:
  static PluginNameMap& instance();
  const char *translate(const char *name) const;

 private:
  static PluginNameMap *m_instance;
  PluginNameMap();

 private:  // in order to disallow
  ~PluginNameMap();
  PluginNameMap(const PluginNameMap& right);
};

#endif
