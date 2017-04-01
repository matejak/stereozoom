#pragma once

// required by https://www.gnu.org/software/libtool/manual/html_node/Modules-for-libltdl.html#Modules-for-libltdl
// solution ctaken from http://stackoverflow.com/questions/16591642/combining-two-defined-symbols-in-c-preprocessor
#define CONCAT2(a, b, c) a ## b ## c
#define CONCAT(a, b, c) CONCAT2(a, b, c)
#define PUBLIC(name) CONCAT(MODULE_NAME, _LTX_, name)
#define PRIVATE(name) CONCAT(_, MODULE_NAME, name)


extern "C"
{
#ifdef LOADER
	void * PUBLIC(get_loader)();
	void PUBLIC(delete_loader)(void * loader);
#endif
#ifdef UI
	void * PUBLIC(get_ui)();
	void PUBLIC(delete_ui)(void * loader);
#endif
	void PUBLIC(init)();
	void PUBLIC(exit)();
}
