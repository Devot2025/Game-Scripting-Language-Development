#include "sce_dynamic_link.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include "sce_built_in_functions.h"
#define load_sce_dl_os(name) LoadLibrary((LPCWSTR)name)
#define load_sce_dl_func_os(id, name) (sce_built_in_func_api)GetProcAddress(id, name)
#define close_sce_dl_os(id) FreeLibrary(id)
typedef struct sce_dl_t {
	HMODULE d_acc;
}sce_dl_t;
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun)
#include <dlfcn.h>
#define load_sce_dl_os(name) dlopen(name, RTLD_LAZY)
#define load_sce_dl_func_os(id, name) (sce_built_in_func_api)dlsym(id, name)
#define close_sce_dl_os(id) dlclose(id) == 0
typedef struct sce_dl_t {
	void * d_acc;
}sce_dl_t;
#endif
sce_dl_t load_sce_dl(const char * name) {
	return (sce_dl_t) {
	.d_acc = load_sce_dl_os(name)
	};
}
bool is_open_sce_dl(sce_dl_t h_) { return (bool)h_.d_acc; }
sce_built_in_func_api load_sce_dl_func(sce_dl_t h_, const char* name) {
	return load_sce_dl_func_os(h_.d_acc, name);
}
bool close_sce_dl(sce_dl_t h_) {
	bool e_ = close_sce_dl_os(h_.d_acc);
	return e_;
}
