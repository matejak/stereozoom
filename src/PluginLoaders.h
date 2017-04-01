#pragma once

#include <string>
using std::string;
#include <ltdl.h>


class Loader;
class GenericUI;

class PluginLoader
{
public:
	PluginLoader(const char * path):
		handle(nullptr), path(path), init(nullptr), exit(nullptr){}
	virtual ~PluginLoader()
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
		handle = lt_dlopenext(path.c_str());
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


class LoaderLoader: public PluginLoader
{
public:
	LoaderLoader(const char * path):
		PluginLoader(path), loader(nullptr), destroyer(nullptr) {}
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


class UILoader: public PluginLoader
{
public:
	UILoader(const char * path):
		PluginLoader(path), loader(nullptr), destroyer(nullptr) {}
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
