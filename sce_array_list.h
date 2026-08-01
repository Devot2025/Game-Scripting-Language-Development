#ifndef SCE_ARRAY_LIST_H_
#define SCE_ARRAY_LIST_H_
#include "sce_run_vm.h"
#define check_range_idx(array_, idx__) (array_).size_ > idx__
#define access_list_array_memory(array_, idx__) (array_).mem_[idx__]
typedef struct Sce_List_Array {
	struct Sce_Virtual_Memory* mem_;
	uint32_t size_;
}Sce_List_Array;
sce_si_strap void delete_sce_run_array(Sce_List_Array* value) {
	for (uint32_t i = 0; i < value->size_; ++i) {
		delete_sce_run_virtual_memory(value->mem_ + i);
	}
}
#endif // !1
