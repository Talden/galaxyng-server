#include <gngserver/Plugin.h>

#include <string>

using std::string;

PluginManager *PluginManager::m_instance = 0;

PluginManager&
PluginManager::instance()
{
  if (! m_instance)
    m_instance = new PluginManager();

  return *m_instance;
}


Plugin&
PluginManager::FindPlugin(const char *type)
  throw (SharedLibraryException)
{
  if (m_pluginMap.count(type) > 0)
    return *(m_pluginMap[type])->m_plugin;

  PluginInfo *pi = new PluginInfo;
  
  pi->m_library = SharedLibrary::OpenSharedLibrary(libname);

  pluginInitFn pif = FindSymbol<pluginInitFn>(*pi->m_library, "plugin_init");
  pi->m_plugin =  (*pif)();
  if (!pi->m_plugin) {
    delete pi;
    throw SharedLibraryException("plugin init error");
  }

  m_pluginMap[name] = pi;
  return *(pi->m_plugin);
}


void
PluginManager::SearchPath(const char *path)
{
  char *tmp = strdup(path);
  char *ptr;

  ptr = strtok(tmp, ":\n");

  while (ptr) {
    m_searchList.push_back(ptr);
  } while ((ptr = strtok(NULL, ":\n")) != NULL);
}


void
PluginManager::FindAllPlugins(const char *gameType)
{
  vector<string>::iterator cp; // current path
  string searchDir = "";

  for (cp = m_searchList.begin(); cp != m_searchList.end(); cp++) {
    if (!searchDir.empty())
      searchDir.erase();

    searchDir = cp + "/" + gameType;

    cout << searchDir;
  }

  return;
}



