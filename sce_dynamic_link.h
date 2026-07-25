#ifndef SCE_DYNAMIC_LINK_H_
#define SCE_DYNAMIC_LINK_H_
#include "sce_built_in_functions.h"
typedef struct sce_dl_t sce_dl_t;
sce_dl_t load_sce_dl(const char* name);
bool is_open_sce_dl(sce_dl_t h_);
sce_built_in_func_api load_sce_dl_func(sce_dl_t h_, const char* name);
bool close_sce_dl(sce_dl_t h_);
#endif// SCE_DYNAMIC_LINK_H_