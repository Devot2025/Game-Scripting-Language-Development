#ifndef SCE_VM_ALLOC_H_
#define SCE_VM_ALLOC_H_
struct Sce_Virtual_Alloc_Map;
typedef struct Sce_Virtual_Alloc_Map Sce_Virtual_Alloc_Map;
#define SVA_ALLOC_MAP_SIZE 100

typedef unsigned char map_t;
typedef struct Sce_Virtual_Alloc_Map{
    void *  sv_memory_;
    map_t   alloc_map[SVA_ALLOC_MAP_SIZE];
    map_t   c_num;
    size_t  idx_;
}Sce_Virtual_Alloc_Map;

Sce_Virtual_Alloc_Map init_sc_virtual_alloc_map();
map_t init_check_sc_virtual_alloc_map(Sce_Virtual_Alloc_Map * sva_map);
void * sc_virtual_memory_alloc(Sce_Virtual_Alloc_Map * sva_map);

#endif