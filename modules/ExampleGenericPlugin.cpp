#include <gngserver/ExamplePlugin.h>

class ExampleGenericPlugin : public ExamplePlugin
{
public:
  ExampleGenericPlugin()
    : ExamplePlugin("Example Generic") { }

  const char *WhoAmI() const 
  { return "Example of a Generic plugin"; }
};

extern "C" {
  Plugin* pluginInit()
  {
    return new ExampleGenericPlugin;
  }
}
