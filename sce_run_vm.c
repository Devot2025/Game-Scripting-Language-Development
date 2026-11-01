#include "sce_run_vm.h"
#include "sce_array_list.h"
#include "sce_stack_object.h"
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


