#if !defined(SHAREDLIBRARY_H_)
#define SHAREDLIBRARY_H_

#include <string>

class SharedLibraryException
{
public:
  SharedLibraryException(const char *error)
    : m_error(error) { }

  const char *GetError() const { return m_error.c_str(); }

private:
  std::string m_error;
};


class SharedLibrary
{
public:
  static SharedLibrary *OpenSharedLibrary(const char *name)
    throw (SharedLibraryException);

  virtual ~SharedLibrary() { }

  virtual void *FindSymbol(const char *name) = 0;

protected:
  SharedLibrary() { }

private:  // for disallowing
  SharedLibrary(const SharedLibrary& right);
};


template<class T>
T FindSymbol(SharedLibrary& sl, const char *name)
{
  return (T)sl.FindSymbol(name);
}

#endif

  
  
