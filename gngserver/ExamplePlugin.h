#if !defined(EXAMPLEPLUGIN_H_)
#define EXAMPLEPLUGIN_H_

#include <gngserver/Plugin.h>

class ExamplePlugin : public Plugin
{
 public:
  ExamplePlugin(const char *name)
    : Plugin(name, "Example") { }

  virtual const char *WhoAmI() const = 0;

};

#endif
