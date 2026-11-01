#ifndef SCE_CTX_HEAP_H_
#define SCE_CTX_HEAP_H_
#include "sce_stack_object.h"
#include "sce_array_list.h"
#include "sce_run_vm_helper.h"

void delete_sce_object_memory(Sce_Label_Context_Heap_Table * memory_, Sce_Context_Heap_Table ** next_);
Sce_Context_Heap_Table * new_sce_context_heap_table(Sce_Context_Heap_Table * heap_table, size_t size_);
void is_object_memory(Sce_Virtual_Memory * v_memory);
void check_face_member_object_memory(Sce_Run_Value_Type type_, void * memory_);
void delete_sce_object_memory(Sce_Label_Context_Heap_Table * memory_, Sce_Context_Heap_Table ** next_);
void delete_sce_context_heap_memory(Sce_Run_VM_Context* vm_context);
void delete_approximate_sce_context_heap_memory(Sce_Run_VM_Context* vm_context);
void check_memory_face_member_object_memory(Sce_Virtual_Memory * v_memory);
void delete_sce_context_heap_memory_basic(Sce_Context_Heap_Table * heap_, Sce_Context_Heap_Table ** next_);
bool set_new_heap_memory_table(Sce_Label_Context_Heap_Table ** now_, size_t *size_, void * memory_, Sce_Run_Value_Type type_);
ctx_error_t new_sce_string_heap(Sce_Virtual_Memory * mem, Sce_Context_Heap_Table * heap_, U8_String_Buffers * u8_str_);

#endif
