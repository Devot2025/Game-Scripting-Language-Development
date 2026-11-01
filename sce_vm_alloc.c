#include "sce_vm_alloc.h"
#define sce_start_address NULL
#include <sce_run_vm.h>
#if defined(WIN32_) || defined(WIN64_)
#include <Windows.h>
#define sce_alloc_type MEM_RESERVE | MEM_COMMIT
#define sce_os_alloc(size__) VirtualAlloc(sce_start_address, size__, sce_alloc_type, PAGE_READWRITE)
#else 
#include <sys/mman.h>
#define sce_alloc_type MAP_PRIVATE | MAP_ANONYMOUS
#define sce_os_alloc(size__) mmap(sce_start_address, size__, PROT_READ | PROT_WRITE, sce_alloc_type, -1, 0)
#define is_os_alloc_memory_valid(ptr__) ptr__ == MAP_FAILED
#endif
#if CHAR_BIT != 8
#warning "This allocator requires 8-bit bytes (CHAR_BIT == 8)."
#endif

#define SCE_VIRTUAL_MEMORY_SIZE sizeof(Sce_Virtual_Memory)
#define SVA_ALLOC_MEMORY_SIZE SVA_ALLOC_MAP_SIZE * CHAR_BIT * SCE_VIRTUAL_MEMORY_SIZE

#define C_LEVEL 5


#define sva_alloc_map_idx(sva__idx__) (sva__idx__ / CHAR_BIT)
#define sva_alloc_map_bit_idx(sva__idx__) (sva__idx__ % CHAR_BIT)
#define get_sva_memory_alloc_map(sva_map__) ((sva_map__).alloc_map[sva_alloc_map_idx((sva_map__).idx_)] >> sva_alloc_map_bit_idx((sva_map__).idx_)) & 0x1
#define set_sva_memory_alloc_map(sva_map__) (sva_map__).alloc_map[sva_alloc_map_idx((sva_map__).idx_)] |= (0x1 << sva_alloc_map_bit_idx((sva_map__).idx_))
Sce_Virtual_Alloc_Map init_sc_virtual_alloc_map(){
    return (Sce_Virtual_Alloc_Map){
        .sv_memory_ = sce_os_alloc(SVA_ALLOC_MEMORY_SIZE),
        0
    };
}
map_t init_check_sc_virtual_alloc_map(Sce_Virtual_Alloc_Map * sva_map){
    return is_os_alloc_memory_valid(sva_map->sv_memory_);
}
void * sc_virtual_memory_alloc(Sce_Virtual_Alloc_Map * sva_map){
    map_t a_map_ = get_sva_memory_alloc_map(*sva_map);
    if(a_map_) {
        size_t i_ = 0;
        size_t d_ = 0;
        if(sva_map->c_num <= C_LEVEL){
            sva_map->c_num++;
            while(i_ < SVA_ALLOC_MAP_SIZE){
                map_t pack_ = *(sva_map->alloc_map + i_);
                if(~pack_){
                    size_t j_ = 0;
                    while (j_ < CHAR_BIT){
                        if(pack_ & (0x1 << j_)) j_++;
                        else {
                            d_ = i_ * CHAR_BIT + j_;
                            goto Found_Label;
                        }
                    }
                }
                i_++;
            }
            return NULL;
            Found_Label:
        }
        else{
            sva_map->c_num = 0;

            size_t d__ = 0;
            size_t stride_ = 0;
            size_t stride__ = 0;
            while (i_ < SVA_ALLOC_MAP_SIZE){
                map_t pack_ = *(sva_map->alloc_map + i_);
                if(!~pack_){
                    if(stride__ > stride_){
                        stride_ = stride__;
                        d_ = d__;
                    }
                    stride__ = 0;
                    d__ = i_ * CHAR_BIT + CHAR_BIT;
                }
                else{
                    size_t j_ = 0;
                    while (j_ < CHAR_BIT){
                        if(pack_ & (0x1 << j_)){
                            if(stride__ > stride_){
                                stride_ = stride__;
                                d_ = d__;
                            }
                            stride__ = 0;
                            d__ = i_ * CHAR_BIT + j_ + 1;
                        }
                        else stride__++;
                        ++j_;
                    }
                }
                i_++;
            }
            if(stride__ > stride_){
                stride_ = stride__;
                d_ = d__;
            }
            if(!stride_) return NULL;
        }

        sva_map->idx_ = d_;
    }
    size_t i_ = sva_map->idx_ * SCE_VIRTUAL_MEMORY_SIZE;
    set_sva_memory_alloc_map(*sva_map);

    if(sva_map->idx_ + 1 < SVA_ALLOC_MAP_SIZE * CHAR_BIT) sva_map->idx_++;
    return (void*)((char *) sva_map->sv_memory_ + i_);
}
