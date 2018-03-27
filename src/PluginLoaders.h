#pragma once

#include <sstream>
#include <string>
using std::string;
#include <stdexcept>
using std::runtime_error;
#include <ltdl.h>


class Loader;
class GenericUI;

class PluginBase
{
public:
	PluginBase(const char * path):
		handle(nullptr), path(path), init(nullptr), exit(nullptr){}
	virtual ~PluginBase()
	{
		if (handle != nullptr)
		{
			exit();
			lt_dlclose(handle);
			handle = nullptr;
		}
	}

	void Load()
	{
		lt_dlerror();
		handle = lt_dlopenext(path.c_str());
		if (handle == nullptr)
		{
			std::ostringstream stringStream;
			stringStream << "Loading of '" << path << "' failed: " << lt_dlerror();
			string msg = stringStream.str();

			throw runtime_error(msg.c_str());
		}
		init = (void (*)())(lt_dlsym(handle, "init"));
		exit = (void (*)())(lt_dlsym(handle, "exit"));
		LoadFunctions();
		init();
	}

protected:
	lt_dlhandle handle;
	string path;

	void (* init)();
	void (* exit)();

	virtual void LoadFunctions() = 0;
};


class LoaderLoader: public PluginBase
{
public:
	LoaderLoader(const char * path):
		PluginBase(path), loader(nullptr), destroyer(nullptr) {}
	Loader * getLoader()
	{
		return loader();
	}

	void deleteLoader(Loader * loader)
	{
		destroyer(loader);
	}
private:
	void LoadFunctions() override
	{
		loader = (Loader * (*)())(lt_dlsym(handle, "get_loader"));
		destroyer = (void (*)(Loader *))(lt_dlsym(handle, "delete_loader"));
	}

	Loader * (* loader)();
	void (* destroyer)(Loader *);
};


class UILoader: public PluginBase
{
public:
	UILoader(const char * path):
		PluginBase(path), loader(nullptr), destroyer(nullptr) {}
	GenericUI * getUI()
	{
		return loader();
	}

	void deleteUI(GenericUI * ui)
	{
		destroyer(ui);
	}
private:
	void LoadFunctions() override
	{
		loader = (GenericUI * (*)())(lt_dlsym(handle, "get_ui"));
		destroyer = (void (*)(GenericUI *))(lt_dlsym(handle, "delete_ui"));
	}

	GenericUI * (* loader)();
	void (* destroyer)(GenericUI *);
};
