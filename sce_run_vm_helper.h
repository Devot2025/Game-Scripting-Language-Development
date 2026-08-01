#ifndef SCE_RUN_VM_HELPER_H_
#define SCE_RUN_VM_HELPER_H_
#include "sce_run_vm.h"
#include "sce_array_list.h"

#define check_global_module_name(str1) simple_strcmp(str1, global_label)
#define get_global_module_symbol_table(vm_ctx) &((vm_ctx).root_symbol)
#define access_vm_ctx_svr(vm_ctx, svr_idx) (vm_ctx).svrs_.svr[svr_idx]
#define eqaul_now_ctx_global(ctx_) &(ctx_).now_vm_symbol_->global_filed == (ctx_).now_vm_symbol_->label_context

#define is_svr_type(srv_ptr, type) srv_ptr->sce_run_value.value_type == type
#define access_svr_value(svr) (svr).sce_run_value
#define access_svr_mvalue(svr) (access_svr_value(svr).mvalue)
#define access_svr_bvalue(svr) (access_svr_value(svr).bvalue)
#define access_svr_cvalue(svr) (access_svr_value(svr).cvalue)
#define access_svr_ivalue(svr) (access_svr_value(svr).ivalue)
#define access_svr_fvalue(svr) (access_svr_value(svr).fvalue)
#define access_svr_dvalue(svr) (access_svr_value(svr).dvalue)
#define access_svr_ssvalue(svr) (access_svr_value(svr).ssvalue)
#define access_svr_lavalue(svr) (access_svr_value(svr).lavalue)
#define reflesh_ctx_general_memory(ctx__) memset((ctx__).svrs_.svr, 0, sizeof((ctx__).svrs_.svr))
#define set_register_value_basic(reg__, acc_v__, v__, t__) ((reg__).sce_run_value.value_type = t__, (reg__).sce_run_value.acc_v__ = v__)
#define set_register_virtual_value(reg__) set_register_value_basic(reg__, vvalue, NULL, E_Sce_Run_Virtual_Value)
#define set_register_bool_value(reg__, vb__) set_register_value_basic(reg__, bvalue, vb__, E_Sce_Run_Bool_Value)
#define set_register_int_value(reg__, vb__) set_register_value_basic(reg__, ivalue, vb__, E_Sce_Run_Int_Value)
#define set_register_char_value(reg__, vb__) set_register_value_basic(reg__, cvalue, vb__, E_Sce_Run_Char_Value)
#define set_register_float_value(reg__, vb__) set_register_value_basic(reg__, fvalue, vb__, E_Sce_Run_Float_Value)
#define set_register_double_value(reg__, vb__) set_register_value_basic(reg__, dvalue, vb__, E_Sce_Run_Double_Value)
#define set_register_string_value(reg__, vb__) set_register_value_basic(reg__, ssvalue, vb__, E_Sce_Run_String_Value)
#define cast_register_cvalue(reg___) (uint8_t)reg__
#define cast_register_ivalue(reg___) (int64_t)reg___
#define cast_register_fvalue(reg___) (float)reg___
#define cast_register_dvalue(reg___) (double)reg___
#define is_svr_value_type(svr, type) (svr).sce_run_value.value_type == type
#define get_svr_ivalue(svr) (svr).sce_run_value.ivalue
#define sce_resolve_register_type(svrs__) svrs__->sce_run_value.value_type == E_Sce_Run_Mem_Value ? svrs__->sce_run_value.mvalue : svrs__
#define svrs_access_begin(svrs__) (svrs__ + 1)
#define svrs_calc(svr1_, svr2_, ope1_) svr1_ ope1_ svr2_
#define sce_type_switch_filed_statement(state1, state2, state3, state4, state5, state6, state7, state8, state9, state10)\
case E_Sce_Run_Null_Value: state1 \
case E_Sce_Run_Virtual_Value: state2 \
case E_Sce_Run_Bool_Value: state3 \
case E_Sce_Run_Char_Value: state4 \
case E_Sce_Run_Int_Value: state5 \
case E_Sce_Run_Float_Value: state6 \
case E_Sce_Run_Double_Value: state7 \
case E_Sce_Run_String_Value: state8 \
case E_Sce_Run_Obj_Value: state9 \
case E_Sce_Run_Mem_Value: state10 

#define GLOBAL_LABEL (0x1 << 0x0)
#define LOCAL_LABEL (0x1 << 0x1)
#define INIT_FINISHED   (0x1 << 0x2)

_EXTERN_C_BEGIN_
static inline void delete_sce_registers(Sce_Virutal_Register* svrs, uint32_t size_) {
	for (uint32_t i = 0; i < size_; ++i) {
		Sce_Virutal_Register* c_svr = svrs + i;
		delete_sce_run_virtual_memory(c_svr);
	}
}
static inline void delete_sce_vm_registers(Sce_Run_VM_Context* vm_context, uint32_t size_) {
	for (uint32_t i = 0; i < size_; ++i) {
		Sce_Virutal_Register* c_svr = vm_context->svrs_.svr + 1;
		delete_sce_run_virtual_memory(c_svr);
	}
}
#define IS_ERROR_CALC_VALUE 0
#define IS_CALC_VALUE 1
#define IS_VIRTUAL_CALC_VALUE 1
#define IS_STRING_CALC_VALUE 2
#define IS_OBJECT_CALC_VALUE 3
static inline int is_calc_value_sce_memory(Sce_Virtual_Memory* svr1) {
	switch (svr1->sce_run_value.value_type){
	case E_Sce_Run_Null_Value:
		return IS_ERROR_CALC_VALUE;
	case E_Sce_Run_Virtual_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_Bool_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_Char_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_Int_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_Float_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_Double_Value:
		return IS_CALC_VALUE;
	case E_Sce_Run_String_Value:
		return IS_STRING_CALC_VALUE;
	case E_Sce_Run_Obj_Value:
		return IS_OBJECT_CALC_VALUE;
	case E_Sce_Run_Mem_Value:
		return IS_ERROR_CALC_VALUE;
	default:
		assert(false);
	}
	return false;
}
typedef char sce_vm_ope_min_t;

sce_si_strap sce_vm_ope_min_t cmp_value_type(Sce_Virtual_Memory* svr1, Sce_Virtual_Memory* svr2) {
	if (svr1->sce_run_value.value_type == svr2->sce_run_value.value_type) return 0;
	if (svr1->sce_run_value.value_type > svr2->sce_run_value.value_type) return 1;
	return 2;
}
_EXTERN_C_END_
#endif // !SCE_RUN_VM_HELPER_H_
