#include "sce_run_vm.h"
#include "sce_array_list.h"
void delete_sce_run_extends_object(Sce_Ext_Values* ext_value) {
	if (!ext_value)return;
	switch (ext_value->ext_type) {
	case E_Sce_Run_Ext_FILE_Value:
		fclose((FILE*)ext_value->ptr__);
	case E_Sce_Run_Ext_Null_Value:
	default:
		break;
	}
}
void delete_sce_run_virtual_memory(Sce_Virtual_Memory* mem) {
	if (mem->is_const_value) return;
	else {
		switch (mem->sce_run_value.value_type) {
		case E_Sce_Run_Mem_Value:
			//delete_sce_run_virtual_memory(mem->sce_run_value.mvalue);
			break;
		case E_Sce_Run_Obj_Value:
			delete_sce_run_extends_object(mem->sce_run_value.evalue);
			s_free(mem->sce_run_value.evalue);
			break;
		case E_Sce_Run_Array_Value:
			delete_sce_run_array(mem->sce_run_value.lavalue);
			s_free(mem->sce_run_value.lavalue);
			break;
		case E_Sce_Run_String_Value:
			delete_u8_string_buffers(mem->sce_run_value.ssvalue);
			s_free(mem->sce_run_value.ssvalue);
			break;

		default:
			break;
		}
		mem->is_const_value = IS_CONST_MEMORY;
		memset(&mem->sce_run_value, 0, sizeof(Sce_Run_Values));
	}
}