#ifndef SCE_OPERATOR_PROCESS_H_
#define SCE_OPERATOR_PROCESS_H_
#include "sce_run_vm_helper.h"
#include "sce_run_error_func.h"

static const uint8_t bool_false_string[] = "false";
static const uint8_t bool_true_string[] = "true";
static const uint8_t virtual_string[] = "virtual";

#if FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128

static const union {
	float gf_____;
	uint32_t gi_____;
} global_inf = {
		.gi_____ = 0x7F800000U
};
#else
#error "sce_operator_process.h : Unsupported float format"
#endif

#define SCE_OPERATOR_STRENGTH 0
#define SCE_OPERATOR_STRING 1
#define SCE_OPERATOR_ERROR 2
#define SCE_OPERATOR_REFERENCE_ERROR 3
#define sce_operator_value(a_, b_, ope_) a_ ope_ b_
#define is_sce_fvalue_inf(a_) ((a_ == global_inf.gf_____) || (a_ == -global_inf.gf_____))
#define sce_value_is_zero(value_) value_ == 0

static inline sce_vm_ope_min_t get_opeartor_process(Sce_Virtual_Memory * mem1) {
	if (is_svr_value_type(*mem1, E_Sce_Run_String_Value)) return SCE_OPERATOR_STRING;
	if (is_svr_value_type(*mem1, E_Sce_Run_Mem_Value)) return SCE_OPERATOR_REFERENCE_ERROR;
	if (is_svr_value_type(*mem1, E_Sce_Run_Null_Value)) return SCE_OPERATOR_ERROR;
	if (mem1->sce_run_value.value_type >= E_Sce_Run_Obj_Value) return SCE_OPERATOR_ERROR;
	return SCE_OPERATOR_STRENGTH;
}
static inline bool add_svr_left_string(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory * mem1, Sce_Virtual_Memory*mem2) {
	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		append_u8_byte_u8_string_basic(
			mem1->sce_run_value.ssvalue,
			mem2->sce_run_value.bvalue ? bool_true_string : bool_false_string,
			mem2->sce_run_value.bvalue ? sizeof(bool_true_string) : sizeof(bool_false_string)

		);
		break;
	case E_Sce_Run_Virtual_Value:
		append_u8_byte_u8_string_basic(
			mem1->sce_run_value.ssvalue, virtual_string, sizeof(virtual_string)
		);
		break;
	case E_Sce_Run_Char_Value:
		append_u8_byte_u8_string_buffer(
			mem1->sce_run_value.ssvalue,
			mem2->sce_run_value.cvalue
		);
		break;
	case E_Sce_Run_Int_Value:
		to_d64_u8strbuf(
			mem1->sce_run_value.ssvalue,
			mem2->sce_run_value.ivalue
		);
		break;
	case E_Sce_Run_Float_Value:
		to_float_u8strbuf(
			mem1->sce_run_value.ssvalue,
			mem2->sce_run_value.fvalue
		);
		break;
	case E_Sce_Run_Double_Value:
		to_double_u8strbuf(
			mem1->sce_run_value.ssvalue,
			mem2->sce_run_value.dvalue
		);
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be added together.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}
static bool add_svr_right_string(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {
	U8_String_Buffers* new_ = smart_malloc(U8_String_Buffers, 1);
	if (!new_) {

		GET_ERROR_STRING(
			error_status->error_str,
			"Failed to memory alloc."
		);
		return false;
	}
	init_u8_string_buffers(new_, 20);

	switch (mem1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:

		append_u8_byte_u8_string_basic(new_, access_svr_bvalue(*mem1) ? bool_true_string : bool_false_string, sizeof(access_svr_bvalue(*mem1) ? bool_true_string : bool_false_string));
		break;
	case E_Sce_Run_Virtual_Value:
		append_u8_byte_u8_string_basic(new_, virtual_string, sizeof(virtual_string));
		break;
	case E_Sce_Run_Char_Value:
		append_u8_byte_u8_string(new_, access_svr_cvalue(*mem1));

		break;
	case E_Sce_Run_Int_Value:
		to_d64_u8strbuf(
			new_,
			access_svr_ivalue(*mem1)
		);
		break;
	case E_Sce_Run_Float_Value:
		to_float_u8strbuf(
			new_,
			access_svr_fvalue(*mem1)
		);
		break;
	case E_Sce_Run_Double_Value:
		to_double_u8strbuf(
			new_,
			access_svr_dvalue(*mem1)

		);
		break;
	default:
		delete_u8_string_buffers(new_);
		s_free(new_);
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be added together.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	append_u8_string_buffer_u8_string_buffer(new_, access_svr_ssvalue(*mem2));
	set_register_string_value(*mem1, new_);
	return true;
}
static bool add_svr_strength_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* memr, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	if (is_svr_value_type(*mem2, E_Sce_Run_Virtual_Value)) {
		*memr = *mem1;
		return true;
	}
	switch (mem1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		switch (mem2->sce_run_value.value_type){
		default:
			assert(false);
			exit(1);
		}
	case E_Sce_Run_Char_Value: {
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(cast_register_ivalue(access_svr_cvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), +);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	}
	case E_Sce_Run_Int_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_bvalue(*mem2)), +);
			break;
		case E_Sce_Run_Char_Value: 
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_cvalue(*mem2)), +);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = 0.0f;
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_bvalue(*mem2)), +);
			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_bvalue(*mem2), +));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Char_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_cvalue(*mem2)), +);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_cvalue(*mem2), +));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Int_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_ivalue(*mem2)), +);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_ivalue(*mem2), +));
			else set_register_float_value(*memr, x_);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;

	}
	case E_Sce_Run_Double_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_bvalue(*mem2)), +);
			break;
		case E_Sce_Run_Char_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_cvalue(*mem2)), +);
			break;
		case E_Sce_Run_Int_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_ivalue(*mem2)), +);
			break;
		case E_Sce_Run_Float_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_fvalue(*mem2)), +);
			break;
		default:
			assert(false);
			exit(1);
		}
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be added together.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}


static bool sub_svr_strength_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* memr, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	if (is_svr_value_type(*mem2, E_Sce_Run_Virtual_Value)) {
		*memr = *mem1;
		return true;
	}
	switch (mem1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		switch (mem2->sce_run_value.value_type) {
		default:
			assert(false);
			exit(1);
		}
	case E_Sce_Run_Char_Value: {
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(cast_register_ivalue(access_svr_cvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), -);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	}
	case E_Sce_Run_Int_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_bvalue(*mem2)), -);
			break;
		case E_Sce_Run_Char_Value:
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_cvalue(*mem2)), -);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = 0.0f;
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_bvalue(*mem2)), -);
			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_bvalue(*mem2), -));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Char_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_cvalue(*mem2)), -);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_cvalue(*mem2), -));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Int_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_ivalue(*mem2)), -);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_ivalue(*mem2), -));
			else set_register_float_value(*memr, x_);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;

	}
	case E_Sce_Run_Double_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_bvalue(*mem2)), -);
			break;
		case E_Sce_Run_Char_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_cvalue(*mem2)), -);
			break;
		case E_Sce_Run_Int_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_ivalue(*mem2)), -);
			break;
		case E_Sce_Run_Float_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_fvalue(*mem2)), -);
			break;
		default:
			assert(false);
			exit(1);
		}
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be subtracted.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}


static bool mul_svr_strength_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* memr, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {
	if (is_svr_value_type(*mem2, E_Sce_Run_Virtual_Value)) {
		*memr = *mem1;
		return true;
	}
	switch (mem1->sce_run_value.value_type) {

	case E_Sce_Run_Bool_Value:
		switch (mem2->sce_run_value.value_type) {
		default:
			assert(false);
			exit(1);
		}
	case E_Sce_Run_Char_Value: {
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(cast_register_ivalue(access_svr_cvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), *);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	}
	case E_Sce_Run_Int_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_bvalue(*mem2)), *);
			break;
		case E_Sce_Run_Char_Value:
			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_cvalue(*mem2)), *);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = 0.0f;
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_bvalue(*mem2)), *);
			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_bvalue(*mem2), *));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Char_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_cvalue(*mem2)), *);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_cvalue(*mem2), *));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Int_Value:
			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_ivalue(*mem2)), *);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_ivalue(*mem2), *));
			else set_register_float_value(*memr, x_);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;

	}
	case E_Sce_Run_Double_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_bvalue(*mem2)), *);
			break;
		case E_Sce_Run_Char_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_cvalue(*mem2)), *);
			break;
		case E_Sce_Run_Int_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_ivalue(*mem2)), *);
			break;
		case E_Sce_Run_Float_Value:
			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_fvalue(*mem2)), *);
			break;
		default:
			assert(false);
			exit(1);
		}
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be multiplied.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}

/**
* 1 * virtual = 1
* 1 / virtual = 1
* 1 + virtual = 1
* 1 - virtual = 1
* virtual + virtual =  virtual
*
**/

static bool div_svr_strength_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* memr, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {
	/**
	* left + right 
	* virtual + right 
	* VIRTUAL > INT 
	* 1 > 3 => 
	* left = right + left
	* INT > virtual
	* left = left + right
	* ?=>weak type is always back ?
	* 1 + virtual
	*/
	if (is_svr_value_type(*mem2, E_Sce_Run_Virtual_Value)) {
		*memr = *mem1;
		return true;
	}
	switch (mem1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		switch (mem2->sce_run_value.value_type) {
		default:
			assert(false);
			exit(1);
		}
	case E_Sce_Run_Char_Value: {
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(cast_register_ivalue(access_svr_cvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), /);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	}
	case E_Sce_Run_Int_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_bvalue(*mem2)), /);
			break;

		case E_Sce_Run_Char_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_cvalue(*mem2)), /);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = 0.0f;
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_fvalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_bvalue(*mem2)), /);
			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_bvalue(*mem2), /));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Char_Value:
			if (sce_value_is_zero(cast_register_fvalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_cvalue(*mem2)), /);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_cvalue(*mem2), /));
			else set_register_float_value(*memr, x_);
			break;
		case E_Sce_Run_Int_Value:
			if (sce_value_is_zero(cast_register_fvalue(access_svr_ivalue(*mem2)))) goto Zero_Error;

			x_ = svrs_calc(access_svr_fvalue(*mem1), cast_register_fvalue(access_svr_ivalue(*mem2)), /);

			if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*memr, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_ivalue(*mem2), /));
			else set_register_float_value(*memr, x_);
			break;
		default:

			assert(false);
			exit(1);
		}
		break;

	}
	case E_Sce_Run_Double_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_dvalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_bvalue(*mem2)), /);
			break;
		case E_Sce_Run_Char_Value:
			if (sce_value_is_zero(cast_register_dvalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_cvalue(*mem2)), /);
			break;
		case E_Sce_Run_Int_Value:
			if (sce_value_is_zero(cast_register_dvalue(access_svr_ivalue(*mem2)))) goto Zero_Error;

			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_ivalue(*mem2)), /);
			break;
		case E_Sce_Run_Float_Value:
			if (sce_value_is_zero(cast_register_dvalue(access_svr_fvalue(*mem2)))) goto Zero_Error;

			access_svr_dvalue(*memr) = svrs_calc(access_svr_dvalue(*mem1), cast_register_dvalue(access_svr_fvalue(*mem2)), /);
			break;
		default:
			assert(false);
			exit(1);
		}
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be divided.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
Zero_Error:

	GET_ERROR_STRING(
		error_status->error_str,
		"Division by zero."
	);
	if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
	return false;
}


static bool mod_svr_strength_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* memr, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {
	/**
	* left + right
	* virtual + right
	* VIRTUAL > INT
	* 1 > 3 =>
	* left = right + left
	* INT > virtual
	* left = left + right
	* ?=>weak type is always back ?
	* 1 + virtual
	*/
	if (is_svr_value_type(*mem2, E_Sce_Run_Virtual_Value)) {
		*memr = *mem1;
		return true;
	}
	switch (mem1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		switch (mem2->sce_run_value.value_type) {
		default:
			assert(false);
			exit(1);
		}
	case E_Sce_Run_Char_Value: {
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(cast_register_ivalue(access_svr_cvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), % );
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	}
	case E_Sce_Run_Int_Value:
		switch (mem2->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_bvalue(*mem2)), % );
			break;

		case E_Sce_Run_Char_Value:
			if (sce_value_is_zero(cast_register_ivalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

			access_svr_ivalue(*memr) = svrs_calc(access_svr_ivalue(*mem1), cast_register_ivalue(access_svr_cvalue(*mem2)), % );
			break;
		default:

			assert(false);
			exit(1);
		}
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be used with modulo.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
Zero_Error:

	GET_ERROR_STRING(
		error_status->error_str,
		"Modulo by zero."
	);
	if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
	return false;
}
static bool add_svr_same_type_num_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		access_svr_ivalue(*mem1) = svrs_calc(cast_register_ivalue(access_svr_bvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), + );
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	case E_Sce_Run_Virtual_Value:
		break;
	case E_Sce_Run_Char_Value: {
		uint8_t c_ = mem1->sce_run_value.cvalue;
		mem1->sce_run_value.ssvalue = smart_malloc(U8_String_Buffers, 1);
		if (!mem1->sce_run_value.ssvalue) {
			GET_ERROR_STRING(
				error_status->error_str,
				"Failed to memory alloc."
			);
			error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
			return false;
		}
		append_u8_byte_u8_string_buffer(mem1->sce_run_value.ssvalue, c_);
		append_u8_byte_u8_string_buffer(mem1->sce_run_value.ssvalue, mem2->sce_run_value.cvalue);
		mem1->sce_run_value.value_type = E_Sce_Run_String_Value;
		break;
	}
	case E_Sce_Run_Int_Value:

		svrs_calc(access_svr_ivalue(*mem1), access_svr_ivalue(*mem2), +=);
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = svrs_calc(access_svr_fvalue(*mem1), access_svr_fvalue(*mem2), +);
		if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*mem1, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_fvalue(*mem2), +));
		else set_register_float_value(*mem1, x_);
		break;
	}
	case E_Sce_Run_Double_Value:

		svrs_calc(access_svr_dvalue(*mem1), access_svr_dvalue(*mem2), +=);
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be added together.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}

static bool sub_svr_same_type_num_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Virtual_Value:
		break;
	case E_Sce_Run_Bool_Value:
		access_svr_ivalue(*mem1) = svrs_calc(cast_register_ivalue(access_svr_bvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), -);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	case E_Sce_Run_Char_Value: {
		access_svr_ivalue(*mem1) = svrs_calc(
			cast_register_ivalue(access_svr_cvalue(*mem1)),
			cast_register_ivalue(access_svr_cvalue(*mem2)),
			-
			);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	}
	case E_Sce_Run_Int_Value:

		svrs_calc(access_svr_ivalue(*mem1), access_svr_ivalue(*mem2), -=);
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = svrs_calc(access_svr_fvalue(*mem1), access_svr_fvalue(*mem2), -);
		if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*mem1, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_fvalue(*mem2), -));
		else set_register_float_value(*mem1, x_);
		break;
	}
	case E_Sce_Run_Double_Value:

		svrs_calc(access_svr_dvalue(*mem1), access_svr_dvalue(*mem2), -=);
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be subtracted.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}

static bool mul_svr_same_type_num_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Virtual_Value:
		break;
	case E_Sce_Run_Bool_Value:
		access_svr_ivalue(*mem1) = svrs_calc(cast_register_ivalue(access_svr_bvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), *);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	case E_Sce_Run_Char_Value: {
		access_svr_ivalue(*mem1) = svrs_calc(
			cast_register_ivalue(access_svr_cvalue(*mem1)),
			cast_register_ivalue(access_svr_cvalue(*mem2)),
			*
		);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	}
	case E_Sce_Run_Int_Value:

		svrs_calc(access_svr_ivalue(*mem1), access_svr_ivalue(*mem2), *=);
		break;
	case E_Sce_Run_Float_Value: {
		float x_ = svrs_calc(access_svr_fvalue(*mem1), access_svr_fvalue(*mem2), *);
		if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*mem1, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_fvalue(*mem2), *));
		else set_register_float_value(*mem1, x_);
		break;
	}
	case E_Sce_Run_Double_Value:

		svrs_calc(access_svr_dvalue(*mem1), access_svr_dvalue(*mem2), *=);
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be multiplied.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
}
static bool div_svr_same_type_num_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Virtual_Value:
		break;
	case E_Sce_Run_Bool_Value:
		if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;
		access_svr_ivalue(*mem1) = svrs_calc(cast_register_ivalue(access_svr_bvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), /);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	case E_Sce_Run_Char_Value: {
		if (sce_value_is_zero(cast_register_ivalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

		access_svr_ivalue(*mem1) = svrs_calc(
			cast_register_ivalue(access_svr_cvalue(*mem1)),
			cast_register_ivalue(access_svr_cvalue(*mem2)),
			/
		);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	}
	case E_Sce_Run_Int_Value:
		if (sce_value_is_zero(access_svr_ivalue(*mem2))) goto Zero_Error;
		svrs_calc(access_svr_ivalue(*mem1), access_svr_ivalue(*mem2), /=);
		break;
	case E_Sce_Run_Float_Value: {
		if (sce_value_is_zero(access_svr_fvalue(*mem2))) goto Zero_Error;

		float x_ = svrs_calc(access_svr_fvalue(*mem1), access_svr_fvalue(*mem2), /);
		if (is_sce_fvalue_inf(x_)) 	set_register_double_value(*mem1, svrs_calc(cast_register_dvalue(access_svr_fvalue(*mem1)), access_svr_fvalue(*mem2), /));
		else set_register_float_value(*mem1, x_);
		break;
	}
	case E_Sce_Run_Double_Value:
		if (sce_value_is_zero(access_svr_dvalue(*mem2))) goto Zero_Error;

		svrs_calc(access_svr_dvalue(*mem1), access_svr_dvalue(*mem2), /=);
		break;
	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be divided.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
Zero_Error:
	GET_ERROR_STRING(
		error_status->error_str,
		"Division by zero."

	);
	if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
	return false;
}

static bool mod_svr_same_type_num_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virtual_Memory* mem2) {

	switch (mem2->sce_run_value.value_type) {
	case E_Sce_Run_Virtual_Value:
		break;
	case E_Sce_Run_Bool_Value:
		if (sce_value_is_zero(cast_register_ivalue(access_svr_bvalue(*mem2)))) goto Zero_Error;
		access_svr_ivalue(*mem1) = svrs_calc(cast_register_ivalue(access_svr_bvalue(*mem1)), cast_register_ivalue(access_svr_bvalue(*mem2)), % );
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	case E_Sce_Run_Char_Value: {
		if (sce_value_is_zero(cast_register_ivalue(access_svr_cvalue(*mem2)))) goto Zero_Error;

		access_svr_ivalue(*mem1) = svrs_calc(
			cast_register_ivalue(access_svr_cvalue(*mem1)),
			cast_register_ivalue(access_svr_cvalue(*mem2)),
			%
		);
		access_svr_value(*mem1).value_type = E_Sce_Run_Int_Value;
		break;
	}
	case E_Sce_Run_Int_Value:
		if (sce_value_is_zero(access_svr_ivalue(*mem2))) goto Zero_Error;
		svrs_calc(access_svr_ivalue(*mem1), access_svr_ivalue(*mem2), %=);
		break;

	default:
		GET_ERROR_STRING(
			error_status->error_str,
			"%s cannot be used with modulo.",
			run_value_type_str[mem2->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return false;
	}
	return true;
Zero_Error:
	GET_ERROR_STRING(
		error_status->error_str,
		"Modulo by zero."

	);
	if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
	return false;
}
void add_svr_values(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;
	mem1 = sce_resolve_register_type(mem1);
	mem2 = sce_resolve_register_type(mem2);
	sce_vm_ope_min_t l_ = get_opeartor_process(mem1);
	if (l_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Addition using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	sce_vm_ope_min_t r_ = get_opeartor_process(mem2);

	if (l_ == SCE_OPERATOR_REFERENCE_ERROR || r_ == SCE_OPERATOR_REFERENCE_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Reference objects cannot be added."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}

	if (r_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Addition using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	if (l_ == SCE_OPERATOR_STRING) {
		if (!add_svr_left_string(error_status, mem1, mem2)) return;
	}
	else if (r_ == SCE_OPERATOR_STRING) {
		if(!add_svr_right_string(error_status, mem1, mem2)) return;
	}
	else if (l_ == SCE_OPERATOR_STRENGTH && r_ == SCE_OPERATOR_STRENGTH) {
		l_ = cmp_value_type(mem1, mem2);
		switch (l_){
		case 0:
			if (!add_svr_same_type_num_value(error_status, mem1, mem2)) return;
			break;
		case 1:
			if (!add_svr_strength_value(error_status, mem1, mem1, mem2))return;
			break;
		case 2:
			if(!add_svr_strength_value(error_status, mem1, mem2, mem1))return;
			break;
		default:
			break;
		}

	}
	else {
		assert(false);//??
	}
	delete_sce_run_virtual_memory(tmp_mem2);
}

void sub_svr_values(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;
	mem1 = sce_resolve_register_type(mem1);
	mem2 = sce_resolve_register_type(mem2);
	sce_vm_ope_min_t l_ = get_opeartor_process(mem1);
	sce_vm_ope_min_t r_ = get_opeartor_process(mem2);
	if (l_ == SCE_OPERATOR_STRING || r_ == SCE_OPERATOR_STRING || l_ == SCE_OPERATOR_ERROR || r_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Subtraction using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	if (l_ == SCE_OPERATOR_REFERENCE_ERROR || r_ == SCE_OPERATOR_REFERENCE_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Reference objects cannot be subtracted."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}

	if (l_ == SCE_OPERATOR_STRENGTH && r_ == SCE_OPERATOR_STRENGTH) {
		l_ = cmp_value_type(mem1, mem2);
		switch (l_) {
		case 0:
			if (!sub_svr_same_type_num_value(error_status, mem1, mem2)) return;
			break;
		case 1:
			if (!sub_svr_strength_value(error_status, mem1, mem1, mem2))return;
			break;
		case 2:
			if (!sub_svr_strength_value(error_status, mem1, mem2, mem1))return;
			break;
		default:
			break;
		}

	}
	else {
		assert(false);//??
	}
	delete_sce_run_virtual_memory(tmp_mem2);
}

void mul_svr_values(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;
	mem1 = sce_resolve_register_type(mem1);
	mem2 = sce_resolve_register_type(mem2);
	sce_vm_ope_min_t l_ = get_opeartor_process(mem1);
	sce_vm_ope_min_t r_ = get_opeartor_process(mem2);
	if (l_ == SCE_OPERATOR_STRING || r_ == SCE_OPERATOR_STRING || l_ == SCE_OPERATOR_ERROR || r_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Multiplication using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	if (l_ == SCE_OPERATOR_REFERENCE_ERROR || r_ == SCE_OPERATOR_REFERENCE_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Reference objects cannot be multiplied."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}

	if (l_ == SCE_OPERATOR_STRENGTH && r_ == SCE_OPERATOR_STRENGTH) {
		l_ = cmp_value_type(mem1, mem2);
		switch (l_) {
		case 0:
			if (!mul_svr_same_type_num_value(error_status, mem1, mem2)) return;
			break;
		case 1:
			if (!mul_svr_strength_value(error_status, mem1, mem1, mem2))return;
			break;
		case 2:
			if (!mul_svr_strength_value(error_status, mem1, mem2, mem1))return;
			break;
		default:
			break;
		}

	}
	else {
		assert(false);//??
	}
	delete_sce_run_virtual_memory(tmp_mem2);
}

void div_svr_values(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;
	mem1 = sce_resolve_register_type(mem1);
	mem2 = sce_resolve_register_type(mem2);
	sce_vm_ope_min_t l_ = get_opeartor_process(mem1);
	sce_vm_ope_min_t r_ = get_opeartor_process(mem2);
	if (l_ == SCE_OPERATOR_STRING || r_ == SCE_OPERATOR_STRING || l_ == SCE_OPERATOR_ERROR || r_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Division using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	if (l_ == SCE_OPERATOR_REFERENCE_ERROR || r_ == SCE_OPERATOR_REFERENCE_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Reference objects cannot be divided."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}

	if (l_ == SCE_OPERATOR_STRENGTH && r_ == SCE_OPERATOR_STRENGTH) {
		l_ = cmp_value_type(mem1, mem2);
		switch (l_) {
		case 0:
			if (!div_svr_same_type_num_value(error_status, mem1, mem2)) return;
			break;
		case 1:
			if (!div_svr_strength_value(error_status, mem1, mem1, mem2))return;
			break;
		case 2:
			if (!div_svr_strength_value(error_status, mem1, mem2, mem1))return;
			break;
		default:
			break;
		}

	}
	else {
		assert(false);//??
	}
	delete_sce_run_virtual_memory(tmp_mem2);
}

void mod_svr_values(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;
	mem1 = sce_resolve_register_type(mem1);
	mem2 = sce_resolve_register_type(mem2);

	sce_vm_ope_min_t l_ = get_opeartor_process(mem1);
	sce_vm_ope_min_t r_ = get_opeartor_process(mem2);
	if (l_ == SCE_OPERATOR_STRING || r_ == SCE_OPERATOR_STRING || l_ == SCE_OPERATOR_ERROR || r_ == SCE_OPERATOR_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Modulo using %s is not possible",
			run_value_type_str[mem1->sce_run_value.value_type]
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}

	if (l_ == SCE_OPERATOR_REFERENCE_ERROR || r_ == SCE_OPERATOR_REFERENCE_ERROR) {
		GET_ERROR_STRING(
			error_status->error_str,
			"Reference objects cannot use modulo."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;
	}
	if (
		is_svr_value_type(*mem1, E_Sce_Run_Float_Value) || is_svr_value_type(*mem1, E_Sce_Run_Double_Value) ||
		is_svr_value_type(*mem2, E_Sce_Run_Double_Value) || is_svr_value_type(*mem2, E_Sce_Run_Float_Value)) {

		GET_ERROR_STRING(
			error_status->error_str,
			"Modulo operands must be integers."
		);
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE;
		return;

	}
	else{
		l_ = cmp_value_type(mem1, mem2);
		switch (l_) {
		case 0:
			if (!mod_svr_same_type_num_value(error_status, mem1, mem2)) return;
			break;
		case 1:
			if (!mod_svr_strength_value(error_status, mem1, mem1, mem2))return;
			break;
		case 2:
			if (!mod_svr_strength_value(error_status, mem1, mem2, mem1))return;
			break;
		default:
			assert(false);
			break;
		}
	}
	delete_sce_run_virtual_memory(tmp_mem2);
}
static void div_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst){
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	div_svr_values(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);

}

static void mul_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	mul_svr_values(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);

}
static void add_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	add_svr_values(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);

}

static void sub_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	sub_svr_values(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);

}

static void mod_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	mod_svr_values(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);

}

#endif// SCE_OPERATOR_PROCESS_H_