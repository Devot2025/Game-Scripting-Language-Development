#ifndef SCE_BUILT_IN_FUNCTION_H_
#define SCE_BUILT_IN_FUNCTION_H_
#include "sce_run_vm_helper.h"
#include "sce_run_error_func.h"
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
#define SCE_VALUE_TYPE_ERROR "Invalid memory type : %s"
#define SCE_FUNCTION_ARGC_ERROR "call '%s' expected %lu arguments, but got %lu."
static inline sce_built_in_basic_define(len){

	if (sce_len_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			SCE_FUNCTION_ARGC_ERROR
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
#define sce_string_upper_argc_num 1
sce_si_strap sce_built_in_basic_define(upper) {
	if (sce_len_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			SCE_FUNCTION_ARGC_ERROR
			"upper",
			argc,
			sce_len_argc_num
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	Sce_Virutal_Register* svr1 = svrs_access_begin(svrs);
	if (svr1->sce_run_value.value_type != E_Sce_Run_Mem_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"the iden is not instance object."
		);
		delete_sce_registers(svrs, argc);

		return;
	}
	if (!svr1->sce_run_value.mvalue) {
		GET_ERROR_STRING(
			dst_->error_str,
			"instance object is null."
		);
		return;
	}

	if (svr1->sce_run_value.mvalue->sce_run_value.value_type != E_Sce_Run_String_Value) {

		GET_ERROR_STRING(
			dst_->error_str,
			"%s function not support the %s type.",
			"upper",
			run_value_type_str[svr1->sce_run_value.mvalue->sce_run_value.value_type]
		);
		return;

	}
	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;

	upper_u8_string_bufferr(svr1->sce_run_value.mvalue->sce_run_value.ssvalue);

	/*reference copy*/
	*svrr = *svr1;
}

#define sce_string_lower_argc_num 1
sce_si_strap sce_built_in_basic_define(lower) {
	if (sce_len_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			SCE_FUNCTION_ARGC_ERROR
			"lower",
			argc,
			sce_len_argc_num
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	Sce_Virutal_Register* svr1 = svrs_access_begin(svrs);
	if (svr1->sce_run_value.value_type != E_Sce_Run_Mem_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"the iden is not instance object."
		);
		delete_sce_registers(svrs, argc);

		return;
	}
	if (!svr1->sce_run_value.mvalue) {
		GET_ERROR_STRING(
			dst_->error_str,
			"instance object is null."
		);
		return;
	}

	if (svr1->sce_run_value.mvalue->sce_run_value.value_type != E_Sce_Run_String_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"%s function not support the %s type.",
			"lower",
			run_value_type_str[svr1->sce_run_value.mvalue->sce_run_value.value_type]
		);
		return;
	}
	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;
	lower_u8_string_bufferr(svr1->sce_run_value.mvalue->sce_run_value.ssvalue);
	/*reference copy*/
	*svrr = *svr1;
}


#define sce_array_size_argc_num 1
sce_si_strap sce_built_in_basic_define(size) {
	if (sce_array_size_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			SCE_FUNCTION_ARGC_ERROR
			"size",
			argc,
			sce_array_size_argc_num
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	Sce_Virutal_Register* svr1 = svrs_access_begin(svrs);
	if (svr1->sce_run_value.value_type != E_Sce_Run_Mem_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"the iden is not instance object."
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	if (!svr1->sce_run_value.mvalue) {
		GET_ERROR_STRING(
			dst_->error_str,
			"instance object is null."
		);
		return;
	}

	if (svr1->sce_run_value.mvalue->sce_run_value.value_type != E_Sce_Run_Array_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"%s function not support the %s type.",
			"array",
			run_value_type_str[svr1->sce_run_value.mvalue->sce_run_value.value_type]
		);
		return;
	}
	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;
	/*reference copy*/
	set_register_int_value(*svrr, access_svr_lavalue(*access_svr_mvalue(*svr1))->size_);
}
#define sce_array_sum_argc_num 1
sce_si_strap sce_built_in_basic_define(sum) {
	if (sce_array_sum_argc_num != argc) {
		GET_ERROR_STRING(
			dst_->error_str,
			SCE_FUNCTION_ARGC_ERROR
			"sum",
			argc,
			sce_array_sum_argc_num
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	Sce_Virutal_Register* svr1 = svrs_access_begin(svrs);
	if (svr1->sce_run_value.value_type != E_Sce_Run_Mem_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"the iden is not instance object."
		);
		delete_sce_registers(svrs, argc);
		return;
	}
	if (!svr1->sce_run_value.mvalue) {
		GET_ERROR_STRING(
			dst_->error_str,
			"instance object is null."
		);
		return;
	}

	if (svr1->sce_run_value.mvalue->sce_run_value.value_type != E_Sce_Run_Array_Value) {
		GET_ERROR_STRING(
			dst_->error_str,
			"%s function not support the %s type.",
			"array",
			run_value_type_str[svr1->sce_run_value.mvalue->sce_run_value.value_type]
		);
		return;
	}

	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;
	svrr->sce_run_value.ivalue = 0;
	svrr->sce_run_value.value_type = E_Sce_Run_Int_Value;
	/*reference copy*/
	Sce_Virtual_Memory* amem = svr1->sce_run_value.mvalue;
	for (uint32_t i = 0; i < amem->sce_run_value.lavalue->size_; i++){
		Sce_Virtual_Memory * mem_ = &amem->sce_run_value.lavalue->mem_[i];
		sce_vm_ope_min_t t_ = cmp_value_type(svrr, mem_);
		if (t_ == 1) {
			switch (mem_->sce_run_value.value_type){
			case E_Sce_Run_Virtual_Value:
				break;
			case E_Sce_Run_Bool_Value:
				switch (svrr->sce_run_value.value_type){
				case E_Sce_Run_Int_Value:
					access_svr_ivalue(*svrr) += (int64_t)access_svr_bvalue(*mem_);
					break;
				case E_Sce_Run_Float_Value:
					access_svr_fvalue(*svrr) += (float)access_svr_bvalue(*mem_);
					break;

				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) += (double)access_svr_bvalue(*mem_);
					break;

				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;
			case E_Sce_Run_Char_Value:

				switch (svrr->sce_run_value.value_type) {
				case E_Sce_Run_Int_Value:
					access_svr_ivalue(*svrr) += (int64_t)access_svr_cvalue(*mem_);
					break;
				case E_Sce_Run_Float_Value:
					access_svr_fvalue(*svrr) += (float)access_svr_cvalue(*mem_);
					break;

				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) += (double)access_svr_cvalue(*mem_);
					break;

				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;

			case E_Sce_Run_Int_Value:
				switch (svrr->sce_run_value.value_type) {
				case E_Sce_Run_Float_Value:
					access_svr_fvalue(*svrr) += (float)access_svr_ivalue(*mem_);
					break;
				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) += (double)access_svr_ivalue(*mem_);
					break;

				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;

			case E_Sce_Run_Float_Value:
				switch (svrr->sce_run_value.value_type) {
				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) += (double)access_svr_fvalue(*mem_);
					break;

				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;
			default:
				GET_ERROR_STRING(
					dst_->error_str,
					"%s type can'not able to sum.",
					run_value_type_str[mem_->sce_run_value.value_type]
				);
				break;
			}
		}
		else if (t_ == 2) {
			switch (svrr->sce_run_value.value_type) {
			case E_Sce_Run_Int_Value:
				switch (mem_->sce_run_value.value_type) {
				case E_Sce_Run_Float_Value:
					access_svr_fvalue(*svrr) = (float)access_svr_ivalue(*svrr) + access_svr_fvalue(*mem_);

					access_svr_value(*svrr).value_type = E_Sce_Run_Float_Value;
					break;
				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) = (double)access_svr_ivalue(*svrr) + access_svr_dvalue(*mem_);
					access_svr_value(*svrr).value_type = E_Sce_Run_Double_Value;

					break;

				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;

			case E_Sce_Run_Float_Value:
				switch (mem_->sce_run_value.value_type) {
				case E_Sce_Run_Double_Value:
					access_svr_dvalue(*svrr) = (double)access_svr_fvalue(*svrr) + access_svr_dvalue(*mem_);
					access_svr_value(*svrr).value_type = E_Sce_Run_Double_Value;
					break;
				default:
					assert(false);//仕様上起こりえないエラー
					exit(1);
				}
				break;
			default:
				GET_ERROR_STRING(
					dst_->error_str,
					"%s type can'not able to sum.",
					run_value_type_str[mem_->sce_run_value.value_type]
				);
				break;
			}
		}
		else {

			switch (svrr->sce_run_value.value_type) {
			case E_Sce_Run_Int_Value:
				access_svr_ivalue(*svrr) += access_svr_ivalue(*mem_);
				break;
			case E_Sce_Run_Float_Value:
				access_svr_fvalue(*svrr) += access_svr_fvalue(*mem_);
				break;

			case E_Sce_Run_Double_Value:
				access_svr_dvalue(*svrr) += access_svr_dvalue(*mem_);
				break;

			default:
				assert(false);//仕様上起こりえないエラー
				exit(1);
			}
		}
	}
	delete_sce_registers(svrs, argc);

}

typedef struct Sce_Built_In_Label {
	const char *label_name;
	sce_built_in_func_api label_adress;
}Sce_Built_In_Label;
static const Sce_Built_In_Label array_class_built_in_adress[] = {
	{.label_adress = sce_size, .label_name = "size"},
	{.label_adress = sce_sum, .label_name = "sum"},
};
static const Sce_Built_In_Label str_class_built_in_adress[] = {
	{.label_adress = sce_upper, .label_name = "upper"},
	{.label_adress = sce_lower, .label_name = "lower"},
};
static const Sce_Built_In_Label_Table str_class_built_in_table = {
	.label_ = str_class_built_in_adress,
	.size_ = sizeof(str_class_built_in_adress) / sizeof(Sce_Built_In_Label)
};

static const Sce_Built_In_Label_Table array_class_built_in_table = {
	.label_ = array_class_built_in_adress,
	.size_ = sizeof(array_class_built_in_adress) / sizeof(Sce_Built_In_Label)
};
static const Sce_Built_In_Label built_in_adress[] = {
	{.label_adress = sce_len, .label_name = "len"},
};
#endif