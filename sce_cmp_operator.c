#include "sce_cmp_operator.h"
#include "sce_run_vm_helper.h"
#include "sce_run_error_func.h"

#define VALUE_IS_CMP_PROCESS 0
#define VALUE_IS_CMP_ALWAYS_FALSE 1 
#define VALUE_IS_CMP_ALWAYS_TRUE 2
#define VALUE_IS_CMP_STRING_PROCESS 3
#define VALUE_IS_LEFT_ERROR_PROCESS 4
#define VALUE_IS_RIGHT_ERROR_PROCESS 5

#define SCE_COMPARE_LESS -1
#define SCE_COMPARE_GREATER 1
#define SCE_COMPARE_EQUAL 0
sce_si_strap sce_vm_ope_min_t is_valid_cmp_value_type(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	if (svr1->sce_run_value.value_type == E_Sce_Run_Null_Value) return VALUE_IS_LEFT_ERROR_PROCESS;
	if (svr2->sce_run_value.value_type == E_Sce_Run_Null_Value) return VALUE_IS_RIGHT_ERROR_PROCESS;
	if (svr1->sce_run_value.value_type == E_Sce_Run_String_Value && svr2->sce_run_value.value_type == E_Sce_Run_String_Value) return VALUE_IS_CMP_STRING_PROCESS;
	if (svr1->sce_run_value.value_type > E_Sce_Run_String_Value || svr2->sce_run_value.value_type > E_Sce_Run_String_Value) return VALUE_IS_CMP_ALWAYS_FALSE;
	if (svr1->sce_run_value.value_type == E_Sce_Run_Virtual_Value) {
		if (svr2->sce_run_value.value_type == E_Sce_Run_Virtual_Value) return VALUE_IS_CMP_ALWAYS_TRUE;
		else return VALUE_IS_CMP_ALWAYS_FALSE;
	}
	if (svr2->sce_run_value.value_type == E_Sce_Run_Virtual_Value) {
		if (svr1->sce_run_value.value_type == E_Sce_Run_Virtual_Value) return VALUE_IS_CMP_ALWAYS_TRUE;
		else return VALUE_IS_CMP_ALWAYS_FALSE;
	}
	return VALUE_IS_CMP_PROCESS;
}

sce_si_strap bool eq_same_value(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svrr) {
	switch (svr1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		return svrs_calc(access_svr_bvalue(*svrr), access_svr_bvalue(*svr1), == );
	case E_Sce_Run_Char_Value:
		return svrs_calc(access_svr_cvalue(*svrr), access_svr_cvalue(*svr1), == );
	case E_Sce_Run_Int_Value:
		return svrs_calc(access_svr_ivalue(*svrr), access_svr_ivalue(*svr1), == );
	case E_Sce_Run_Float_Value:
		return svrs_calc(access_svr_fvalue(*svrr), access_svr_fvalue(*svr1), == );
	case E_Sce_Run_Double_Value:
		return svrs_calc(access_svr_dvalue(*svrr), access_svr_dvalue(*svr1), == );
	default:
		assert(false);
		break;
	}
}

sce_si_strap bool gt_same_value(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svrr) {
	switch (svr1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		return svrs_calc(access_svr_bvalue(*svrr), access_svr_bvalue(*svr1), > );
	case E_Sce_Run_Char_Value:
		return svrs_calc(access_svr_cvalue(*svrr), access_svr_cvalue(*svr1), > );
	case E_Sce_Run_Int_Value:
		return svrs_calc(access_svr_ivalue(*svrr), access_svr_ivalue(*svr1), > );
	case E_Sce_Run_Float_Value:
		return svrs_calc(access_svr_fvalue(*svrr), access_svr_fvalue(*svr1), > );
	case E_Sce_Run_Double_Value:
		return svrs_calc(access_svr_dvalue(*svrr), access_svr_dvalue(*svr1), > );
	default:
		assert(false);
		break;
	}
}
sce_si_strap bool ls_same_value(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svrr) {
	switch (svr1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		return svrs_calc(access_svr_bvalue(*svr1), access_svr_bvalue(*svrr), < );
	case E_Sce_Run_Char_Value:
		return svrs_calc(access_svr_cvalue(*svr1), access_svr_cvalue(*svrr), < );
	case E_Sce_Run_Int_Value:
		return svrs_calc(access_svr_ivalue(*svr1), access_svr_ivalue(*svrr), < );
	case E_Sce_Run_Float_Value:
		return svrs_calc(access_svr_fvalue(*svr1), access_svr_fvalue(*svrr), < );
	case E_Sce_Run_Double_Value:
		return svrs_calc(access_svr_dvalue(*svr1), access_svr_dvalue(*svrr), < );
	default:
		assert(false);
		break;
	}
}
sce_si_strap bool leq_same_value(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svrr) {
	switch (svr1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		return svrs_calc(access_svr_bvalue(*svr1), access_svr_bvalue(*svrr), <= );
	case E_Sce_Run_Char_Value:
		return svrs_calc(access_svr_cvalue(*svr1), access_svr_cvalue(*svrr), <= );
	case E_Sce_Run_Int_Value:
		return svrs_calc(access_svr_ivalue(*svr1), access_svr_ivalue(*svrr), <= );
	case E_Sce_Run_Float_Value:
		return svrs_calc(access_svr_fvalue(*svr1), access_svr_fvalue(*svrr), <= );
	case E_Sce_Run_Double_Value:
		return svrs_calc(access_svr_dvalue(*svr1), access_svr_dvalue(*svrr), <= );
	default:
		assert(false);
		break;
	}
}

sce_si_strap bool geq_same_value(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svrr) {
	switch (svr1->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		return svrs_calc(access_svr_bvalue(*svr1), access_svr_bvalue(*svrr), >= );
	case E_Sce_Run_Char_Value:
		return svrs_calc(access_svr_cvalue(*svr1), access_svr_cvalue(*svrr), >= );
	case E_Sce_Run_Int_Value:
		return svrs_calc(access_svr_ivalue(*svr1), access_svr_ivalue(*svrr), >= );
	case E_Sce_Run_Float_Value:
		return svrs_calc(access_svr_fvalue(*svr1), access_svr_fvalue(*svrr), >= );
	case E_Sce_Run_Double_Value:
		return svrs_calc(access_svr_dvalue(*svr1), access_svr_dvalue(*svrr), >= );
	default:
		assert(false);
		break;
	}
}
static sce_vm_ope_min_t compare_promoted_value(Sce_Virtual_Memory* svrl, Sce_Virtual_Memory* svrr) {
	/*svr1 <= svrr
	* svrr <= svr1
	* 
	*/
	switch (svrr->sce_run_value.value_type) {
	case E_Sce_Run_Bool_Value:
		switch (svrl->sce_run_value.value_type) {
		default:
			assert(false);
		}
		break;

	case E_Sce_Run_Char_Value:
		switch (svrl->sce_run_value.value_type) {

		case E_Sce_Run_Bool_Value: {
			uint8_t tmp1_ = (uint8_t)access_svr_bvalue(*svrl);
			uint8_t tmp_ = access_svr_cvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		default:
			assert(false);
		}
		break;
	case E_Sce_Run_Int_Value:
		switch (svrl->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
		{
			uint64_t tmp1_ = (uint64_t)access_svr_bvalue(*svrl);
			uint64_t tmp_ = access_svr_ivalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Char_Value: {
			uint64_t tmp1_ = (uint64_t)access_svr_cvalue(*svrl);
			uint64_t tmp_ = access_svr_ivalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		default:
			assert(false);
		}
		break;

	case E_Sce_Run_Float_Value:

		switch (svrl->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
		{
			float tmp1_ = (float)access_svr_bvalue(*svrl);
			float tmp_ = access_svr_fvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Char_Value: {
			float tmp1_ = (float)access_svr_cvalue(*svrl);
			float tmp_ = access_svr_fvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Int_Value: {
			float tmp1_ = (float)access_svr_ivalue(*svrl);
			float tmp_ = access_svr_fvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		default:
			assert(false);
		}
		break;

	case E_Sce_Run_Double_Value:

		switch (svrl->sce_run_value.value_type) {
		case E_Sce_Run_Bool_Value:
		{
			double tmp1_ = (double)access_svr_bvalue(*svrl);
			double tmp_ = access_svr_dvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Int_Value: {
			double tmp1_ = (double)access_svr_ivalue(*svrl);
			double tmp_ = access_svr_dvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Char_Value: {
			double tmp1_ = (double)access_svr_cvalue(*svrl);
			double tmp_ = access_svr_dvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		case E_Sce_Run_Float_Value: {
			double tmp1_ = (double)access_svr_fvalue(*svrl);
			double tmp_ = access_svr_dvalue(*svrr);
			if (tmp1_ > tmp_) return SCE_COMPARE_GREATER;
			if (tmp1_ < tmp_) return SCE_COMPARE_LESS;
			return SCE_COMPARE_EQUAL;
		}
		default:
			assert(false);

		}
		break;

	default:
		assert(false);
	}
	return false;
}

static bool to_leq_value_sce_memory(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	sce_vm_ope_min_t i = cmp_value_type(svr1, svr2);

	if (i == 0) {
		return leq_same_value(svr1, svr2);
	}
	else if (i == 1) {
		/**
		* 右レジスタ強い、左レジスタ弱い
		* しかし比較は<=
		* svr2 <= svr1
		* を渡すということに等しい
		* svr1 <= svr2で帰ってくるのは
		* 左のほうが大きい
		* 右のほうが大きい
		* 同じである
		* この中で
		* 左が来たら、FALSEをつくればいい
		* これを反対に
		* 左が大きいであるので
		* svr1 <= svr2にするには
		* svr2, svr1
		* を渡すとき
		* 右が大きい
		* 左が大きいの逆を取る
		* つまり、svr2が大きい右が大きいをfalseにする
		*/
		return compare_promoted_value(svr2, svr1) != SCE_COMPARE_LESS;
	}
	else {
		return compare_promoted_value(svr1, svr2) != SCE_COMPARE_GREATER;
	}

}

static bool to_ls_value_sce_memory(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	char i = cmp_value_type(svr1, svr2);
	if (i == 0) {
		return ls_same_value(svr1, svr2);
	}
	else if (i == 1) {
		return compare_promoted_value(svr2, svr1) == SCE_COMPARE_GREATER;
	}
	else {
		return compare_promoted_value(svr1, svr2) == SCE_COMPARE_LESS;
	}
}

static bool to_geq_value_sce_memory(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	char i = cmp_value_type(svr1, svr2);
	if (i == 0) {
		return geq_same_value(svr1, svr2);
	}
	else if (i == 1) {
		return compare_promoted_value(svr2, svr1) != SCE_COMPARE_GREATER;
	}
	else {
		return compare_promoted_value(svr1, svr2) != SCE_COMPARE_LESS;
	}
}
static bool to_gt_value_sce_memory(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	char i = cmp_value_type(svr1, svr2);
	if (i == 0) {
		return gt_same_value(svr1, svr2);
	}
	else if (i == 1) {
		return compare_promoted_value(svr2, svr1) == SCE_COMPARE_LESS;
	}
	else {
		return compare_promoted_value(svr1, svr2) == SCE_COMPARE_GREATER;
	}
}

static bool to_eq_value_sce_memory(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	char i = cmp_value_type(svr1, svr2);
	if (i == 0) {
		return eq_same_value(svr1, svr2);
	}
	else if (i == 1) {
		return compare_promoted_value(svr2, svr1) == SCE_COMPARE_EQUAL;
	}
	else {
		return compare_promoted_value(svr1, svr2) == SCE_COMPARE_EQUAL;
	}
}

void leq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svr2 = get_svr(sce_vm_context, inst_->ope2);
	Sce_Virutal_Register* tmp_svr1 = svr1;
	Sce_Virutal_Register* tmp_svr2 = svr2;

	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	if (is_svr_value_type(*svr1, E_Sce_Run_Mem_Value)) tmp_svr1 = access_svr_mvalue(*tmp_svr1);
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) tmp_svr2 = access_svr_mvalue(*tmp_svr2);
	sce_vm_ope_min_t i = is_valid_cmp_value_type(tmp_svr1, tmp_svr2);
	if (i == VALUE_IS_CMP_ALWAYS_FALSE) {
		set_register_bool_value(*svrr, false);
	}
	else if (i == VALUE_IS_CMP_ALWAYS_TRUE) {
		set_register_bool_value(*svrr, true);
	}
	if (i == VALUE_IS_CMP_PROCESS) {
		set_register_bool_value(*svrr, to_leq_value_sce_memory(tmp_svr1, tmp_svr2));

	}
	if (i == VALUE_IS_LEFT_ERROR_PROCESS) {
		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"left value is null"
		);
		return;

	}
	if (i == VALUE_IS_RIGHT_ERROR_PROCESS) {

		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"right value is null"
		);
		return;

	}
	if (i == VALUE_IS_CMP_STRING_PROCESS) {
		set_register_bool_value(*svrr, leq_u8_string_buffers(tmp_svr1, tmp_svr2));
	}
	delete_sce_run_virtual_memory(svr1);
	delete_sce_run_virtual_memory(svr2);
}

void eq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svr2 = get_svr(sce_vm_context, inst_->ope2);
	Sce_Virutal_Register* tmp_svr1 = svr1;
	Sce_Virutal_Register* tmp_svr2 = svr2;
	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	if (is_svr_value_type(*svr1, E_Sce_Run_Mem_Value)) tmp_svr1 = access_svr_mvalue(*tmp_svr1);
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) tmp_svr2 = access_svr_mvalue(*tmp_svr2);
	sce_vm_ope_min_t i = is_valid_cmp_value_type(tmp_svr1, tmp_svr2);
	if (i == VALUE_IS_CMP_ALWAYS_FALSE) {
		set_register_bool_value(*svrr, false);
	}
	else if(i == VALUE_IS_CMP_ALWAYS_TRUE) {
		set_register_bool_value(*svrr, true);
	}
	if (i == VALUE_IS_CMP_PROCESS) {
		set_register_bool_value(*svrr, to_eq_value_sce_memory(tmp_svr1, tmp_svr2));
	}
	if (i == VALUE_IS_LEFT_ERROR_PROCESS) {
		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"left value is null"
		);
		return;

	}
	if (i == VALUE_IS_RIGHT_ERROR_PROCESS) {

		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"right value is null"
		);
		return;

	}
	if (i == VALUE_IS_CMP_STRING_PROCESS) {
		set_register_bool_value(*svrr, is_same_u8_string_buffers(tmp_svr1, tmp_svr2));
	}
	delete_sce_run_virtual_memory(svr1);
	delete_sce_run_virtual_memory(svr2);
}

void ls_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svr2 = get_svr(sce_vm_context, inst_->ope2);
	Sce_Virutal_Register* tmp_svr1 = svr1;
	Sce_Virutal_Register* tmp_svr2 = svr2;
	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	if (is_svr_value_type(*svr1, E_Sce_Run_Mem_Value)) tmp_svr1 = access_svr_mvalue(*tmp_svr1);
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) tmp_svr2 = access_svr_mvalue(*tmp_svr2);
	sce_vm_ope_min_t i = is_valid_cmp_value_type(tmp_svr1, tmp_svr2);
	if (i == VALUE_IS_CMP_ALWAYS_FALSE) {
		set_register_bool_value(*svrr, false);
	}
	else if (i == VALUE_IS_CMP_ALWAYS_TRUE) {
		set_register_bool_value(*svrr, false);
	}
	if (i == VALUE_IS_CMP_PROCESS) {
		set_register_bool_value(*svrr, to_ls_value_sce_memory(tmp_svr1, tmp_svr2));

	}
	if (i == VALUE_IS_LEFT_ERROR_PROCESS) {
		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"left value is null"
		);
		return;

	}
	if (i == VALUE_IS_RIGHT_ERROR_PROCESS) {

		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"right value is null"
		);
		return;

	}
	if (i == VALUE_IS_CMP_STRING_PROCESS) {
		set_register_bool_value(*svrr, ls_u8_string_buffers(tmp_svr1, tmp_svr2));
	}
	delete_sce_run_virtual_memory(svr1);
	delete_sce_run_virtual_memory(svr2);
}


void gt_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svr2 = get_svr(sce_vm_context, inst_->ope2);
	Sce_Virutal_Register* tmp_svr1 = svr1;
	Sce_Virutal_Register* tmp_svr2 = svr2;
	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	if (is_svr_value_type(*svr1, E_Sce_Run_Mem_Value)) tmp_svr1 = access_svr_mvalue(*tmp_svr1);
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) tmp_svr2 = access_svr_mvalue(*tmp_svr2);
	sce_vm_ope_min_t i = is_valid_cmp_value_type(tmp_svr1, tmp_svr2);
	if (i == VALUE_IS_CMP_ALWAYS_FALSE) {
		set_register_bool_value(*svrr, false);
	}
	else if (i == VALUE_IS_CMP_ALWAYS_TRUE) {
		set_register_bool_value(*svrr, false);
	}
	if (i == VALUE_IS_CMP_PROCESS) {
		set_register_bool_value(*svrr, to_gt_value_sce_memory(tmp_svr1, tmp_svr2));

	}
	if (i == VALUE_IS_LEFT_ERROR_PROCESS) {
		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"left value is null"
		);
		return;

	}
	if (i == VALUE_IS_RIGHT_ERROR_PROCESS) {

		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"right value is null"
		);
		return;

	}
	if (i == VALUE_IS_CMP_STRING_PROCESS) {
		set_register_bool_value(*svrr, gt_u8_string_buffers(tmp_svr1, tmp_svr2));
	}
	delete_sce_run_virtual_memory(svr1);
	delete_sce_run_virtual_memory(svr2);
}


void geq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svr2 = get_svr(sce_vm_context, inst_->ope2);
	Sce_Virutal_Register* tmp_svr1 = svr1;
	Sce_Virutal_Register* tmp_svr2 = svr2;
	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	if (is_svr_value_type(*svr1, E_Sce_Run_Mem_Value)) tmp_svr1 = access_svr_mvalue(*tmp_svr1);
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) tmp_svr2 = access_svr_mvalue(*tmp_svr2);
	sce_vm_ope_min_t i = is_valid_cmp_value_type(tmp_svr1, tmp_svr2);
	if (i == VALUE_IS_CMP_ALWAYS_FALSE) {
		set_register_bool_value(*svrr, false);
	}
	else if (i == VALUE_IS_CMP_ALWAYS_TRUE) {
		set_register_bool_value(*svrr, true);
	}
	if (i == VALUE_IS_CMP_PROCESS) {
		set_register_bool_value(*svrr, to_geq_value_sce_memory(tmp_svr1, tmp_svr2));

	}
	if (i == VALUE_IS_LEFT_ERROR_PROCESS) {
		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"left value is null"
		);
		return;

	}
	if (i == VALUE_IS_RIGHT_ERROR_PROCESS) {

		GET_ERROR_STRING(
			sce_vm_context->vm_error_status.error_str,
			"right value is null"
		);
		return;

	}
	if (i == VALUE_IS_CMP_STRING_PROCESS) {
		set_register_bool_value(*svrr, geq_u8_string_buffers(tmp_svr1, tmp_svr2));
	}
	delete_sce_run_virtual_memory(svr1);
	delete_sce_run_virtual_memory(svr2);
}
