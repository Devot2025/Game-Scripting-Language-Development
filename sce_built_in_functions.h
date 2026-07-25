#ifndef SCE_BUILT_IN_FUNCTION_H_
#define SCE_BUILT_IN_FUNCTION_H_
#include "sce_run_vm_helper.h"
#include "sce_run_error_func.h"

typedef void sce_built_in_func_api_(uint32_t argc, Sce_Virutal_Register* svrs, Sce_Run_VM_Error_Status* dst_);
typedef sce_built_in_func_api_* sce_built_in_func_api;
#define sce_built_in_prefix(func_name) sce_##func_name
#if defined(_WIN32) || defined(_WIN64) 
#define sce_dl_strap __declspec(dllexport)
#else
#define sce_dl_strap
#endif

#define sce_built_in_basic_define(func_name) void sce_built_in_prefix(func_name)(uint32_t argc, Sce_Virutal_Register* svrs, Sce_Run_VM_Error_Status* dst_)
#define sce_len_argc_num 1

static const char sce_function_argc_error[] = "call '%s' expected %lu arguments, but got %lu.";
static const char sce_value_type_error[] = "Invalid memory type : %s";

static inline sce_built_in_basic_define(len){

	if (sce_len_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			"%s",
			sce_function_argc_error,
			"len",
			argc,
			sce_len_argc_num
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	Sce_Virutal_Register* tmp_svr1 = svrs + 1;

	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;
	Sce_Virutal_Register* svr1 = sce_resolve_register_type(tmp_svr1);

	switch (svr1->sce_run_value.value_type){
	case E_Sce_Run_Null_Value:
	case E_Sce_Run_Virtual_Value:
	case E_Sce_Run_Bool_Value:
	case E_Sce_Run_Char_Value:
	case E_Sce_Run_Int_Value:
	case E_Sce_Run_Float_Value:
	case E_Sce_Run_Double_Value:
	case E_Sce_Run_Obj_Value:
	case E_Sce_Run_Mem_Value:
		GET_ERROR_STRING(
			dst_->error_str,
			"%s type is has not len",
			run_value_type_str[svr1->sce_run_value.value_type]
			);
		break;
	case E_Sce_Run_String_Value:
		set_register_int_value(
			*svrr,
			svr1->sce_run_value.ssvalue->index__
		);
		break;
	default:
		assert(false);
	}

	delete_sce_run_virtual_memory(tmp_svr1);
}

typedef struct Sce_Built_In_Label {
	const char *label_name;
	sce_built_in_func_api label_adress;
}Sce_Built_In_Label;

static const Sce_Built_In_Label built_in_adress[] = {
	{.label_adress = sce_len, .label_name = "len"},
};
#endif