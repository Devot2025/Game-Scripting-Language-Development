#ifndef SCE_MOV_PROCESS_H_
#define SCE_MOV_PROCESS_H_
#include "sce_run_vm_helper.h"
#include "sce_run_error_func.h"

void smov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void bmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void cmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void fmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void imov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void rmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void mmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void mov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
static Sce_VM_Object_Table* search_object_symbol(Sce_VM_Object_Table* obj_table, const uint8_t* name_) {
	while (obj_table) {
		if (simple_strcmp(obj_table->obj_name_, name_)) {
			return obj_table;
		}
		obj_table = obj_table->next_;
	}
	return NULL;
}

static Sce_VM_Object_Table* search_local_object_symbol(Sce_VM_Label_Table_Context* local_context, const uint8_t* name_) {
	while (local_context) {
		search_object_symbol(
			local_context->obj_table_, name_
		);
		local_context = local_context->next_;
	}
	return NULL;
}
static void smov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	uint8_t* sbuf = sce_inst->scope2;
	if (svr1) {

		svr1->is_const_value = NOT_IS_CONST_MEMORY;
		svr1->sce_run_value.value_type = E_Sce_Run_String_Value;
		svr1->sce_run_value.ssvalue = smart_malloc(U8_String_Buffers, 1);
		if (!svr1->sce_run_value.ssvalue) return;

		uint32_t len_ = u8strlen_u32(sbuf);
		svr1->sce_run_value.ssvalue->index__ = len_;
		svr1->sce_run_value.ssvalue->size__ = len_ + 1;
		svr1->sce_run_value.ssvalue->str__ = sbuf;

		svr1->sce_run_value.ssvalue->str__ = smart_malloc(uint8_t, len_ + 1);
		if (!svr1->sce_run_value.ssvalue->str__) return;

		svr1->sce_run_value.ssvalue->size__ = len_ + 1;
		svr1->sce_run_value.ssvalue->index__ = len_;
		memcpy(
			svr1->sce_run_value.ssvalue->str__,
			sce_inst->scope2,
			svr1->sce_run_value.ssvalue->size__
		);
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope1,
		SCE_VIRTUAL_REGISTER_MAX
	);
}
static void bmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	bool bbuf = sce_inst->bcope2;
	if (svr1) {
		svr1->is_const_value = true;
		svr1->sce_run_value.value_type = E_Sce_Run_Bool_Value;
		svr1->sce_run_value.cvalue = bbuf;
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope1,
		SCE_VIRTUAL_REGISTER_MAX
	);
}
static void cmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	uint8_t cbuf = sce_inst->ccope2;
	if (svr1) {
		svr1->is_const_value = true;
		svr1->sce_run_value.value_type = E_Sce_Run_Char_Value;
		svr1->sce_run_value.cvalue = cbuf;
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_vm_context->inst_->ope1,
		SCE_VIRTUAL_REGISTER_MAX
	);
}
static void fmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	float fbuf = sce_inst->fcope2;
	if (svr1) {
		svr1->is_const_value = true;
		svr1->sce_run_value.value_type = E_Sce_Run_Float_Value;
		svr1->sce_run_value.fvalue = fbuf;
		return;

	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope1,
		SCE_VIRTUAL_REGISTER_MAX
	);
}
static void imov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	uint64_t ibuf = sce_inst->icope2;
	if (svr1) {
		svr1->is_const_value = true;
		svr1->sce_run_value.value_type = E_Sce_Run_Int_Value;
		svr1->sce_run_value.ivalue = ibuf;
		return;

	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope1,
		SCE_VIRTUAL_REGISTER_MAX
	);
}

static void rmov_sce_vm_basic(Sce_Virutal_Register* rv1, Sce_Virutal_Register* rv2) {

	/*
	* i = 1;
	*
	* IMOV SVR0, 0
	* MOV  SVR1, &i
	* RMOV SVR1, SVR0
	* i = k;
	* MOV   SVR0, &i
	* MMOV SVR1, k
	* RMOV SVR0, SVR1
	* 0 = 1
	* IMOV SVR0, 0
	* IMOV SVR1, 1
	* RMOV SVR0, SVR1
	*
	*/
	if (is_svr_type(rv1, E_Sce_Run_Mem_Value)) {

		delete_sce_run_virtual_memory(rv1->sce_run_value.mvalue);
		if (is_svr_type(rv2, E_Sce_Run_Mem_Value)) {

			/*
			* Since the memory is saved as is,
			* we just need to copy the contents as they are.
			* Conceptually,
			* RV1 contains &i
			* so
			* RV2 contains &u
			* In other words,
			* by copying (*i) = (*u),
			* immediate values remain unchanged
			* classes are copied by reference as they are.
			*
			*/

			access_svr_mvalue(*rv1)->sce_run_value
				= access_svr_mvalue(*rv2)->sce_run_value;
			access_svr_mvalue(*rv1)->is_const_value = IS_CONST_MEMORY;
		}
		else {

			*access_svr_mvalue(*rv1) = *rv2;
			rv2->is_const_value = IS_CONST_MEMORY;
		}
	}
	else {
		*(rv1) = *(rv2);
		rv2->is_const_value = IS_CONST_MEMORY;
	}
}
static void rmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	/*
	MMOV (Memory Copy)

Creates a new independent value from the source operand.

Behavior:
- Primitive types (int, float, bool, etc.) are copied by value.
- String values are deep-copied (or copied according to the type's clone semantics).
- Object values are cloned, creating a new object instance.
- Array values are cloned, creating a new array instance.
- The destination register always stores a value, never a memory reference.

Notes:
- MMOV never preserves memory references.
- MMOV is intended for expression evaluation where the original value must remain unchanged.
- Any type-specific copy behavior is defined by the type's clone/copy implementation.

	*/
	if (svr2 && svr1) {

		rmov_sce_vm_basic(svr1, svr2);
		return;
	}
	if (!svr1) {
		get_vm_error_status(
			*sce_vm_context,
			"Sce Virutal Register %lu is %lu or greater.",
			sce_inst->ope1,
			SCE_VIRTUAL_REGISTER_MAX
		);
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope2,
		SCE_VIRTUAL_REGISTER_MAX
	);
}
static void mmov_mem_to_svr(Sce_Virutal_Register* svr, Sce_Virtual_Memory* mem) {
	/**
	* MEMORY
	*/
	switch (mem->sce_run_value.value_type) {
	case E_Sce_Run_Mem_Value:
		svr->is_const_value = NOT_IS_CONST_MEMORY;
		svr->sce_run_value.mvalue = mem;
		svr->sce_run_value.value_type = E_Sce_Run_Mem_Value;
		break;
	case E_Sce_Run_Array_Value:
		assert(false);
		break;
	case E_Sce_Run_String_Value:
		svr->is_const_value = NOT_IS_CONST_MEMORY;
		svr->sce_run_value.ssvalue = smart_malloc(U8_String_Buffers, 1);
		if (!svr->sce_run_value.ssvalue) return;
		init_u8_string_buffers(svr->sce_run_value.ssvalue, 20);
		svr->sce_run_value.value_type = mem->sce_run_value.value_type;
		append_u8_string_buffer_u8_string_buffer(svr->sce_run_value.ssvalue, mem->sce_run_value.ssvalue);
		break;
	default:
		svr->is_const_value = IS_CONST_MEMORY;
		memcpy(&svr->sce_run_value, &mem->sce_run_value, sizeof(Sce_Run_Values));
	}
}
static void mmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr =
		get_svr(sce_vm_context, sce_inst->ope1);
	if (!svr) {
		get_vm_error_status(
			*sce_vm_context,
			"Sce Virutal Register %lu is %lu or greater.",
			sce_inst->ope1,
			SCE_VIRTUAL_REGISTER_MAX
		);
		return;
	}
	if (&sce_vm_context->root_symbol != sce_vm_context->now_vm_symbol_) {
		Sce_VM_Object_Table* obj_table_root_ = search_object_symbol(sce_vm_context->root_symbol.global_filed.obj_table_, sce_inst->scope2);
		if (obj_table_root_) {
			mmov_mem_to_svr(svr, &obj_table_root_->svm);
			return;
		}
	}
	Sce_VM_Object_Table* obj_table_ = search_object_symbol(sce_vm_context->now_vm_symbol_->global_filed.obj_table_, sce_inst->scope2);
	if (obj_table_) {
		mmov_mem_to_svr(svr, &obj_table_->svm);
		return;
	}
	if (!(eqaul_now_ctx_global(*sce_vm_context))) {

		obj_table_ = search_object_symbol(sce_vm_context->now_vm_symbol_->label_context->obj_table_, sce_inst->scope2);
		if (obj_table_) {
			mmov_mem_to_svr(svr, &obj_table_->svm);
			return;
		}
	}
	get_vm_error_status(
		*sce_vm_context,
		"Variable %s not defined.",
		sce_inst->scope2
	);
}
static void mov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr =
		get_svr(sce_vm_context, sce_inst->ope1);
	if (!svr) {
		get_vm_error_status(
			*sce_vm_context,
			"Sce Virutal Register %lu is %lu or greater.",
			sce_inst->ope1,
			SCE_VIRTUAL_REGISTER_MAX
		);
		return;
	}
	Sce_VM_Object_Table* obj_table_ = NULL;

	if (&sce_vm_context->root_symbol != sce_vm_context->now_vm_symbol_) {
		obj_table_ =
			search_object_symbol(sce_vm_context->root_symbol.global_filed.obj_table_, sce_inst->scope2);
	}
	if (!obj_table_) {
		obj_table_ = search_object_symbol(
			sce_vm_context->now_vm_symbol_->global_filed.obj_table_,
			sce_inst->scope2);
	}
	if (!obj_table_) {
		if (!(eqaul_now_ctx_global(*sce_vm_context))) {
			obj_table_ = search_object_symbol(sce_vm_context->now_vm_symbol_->label_context->obj_table_, sce_inst->scope2);
		}
	}
	if (obj_table_) {
		svr->is_const_value = NOT_IS_CONST_MEMORY;
		svr->sce_run_value.mvalue = &obj_table_->svm;
		svr->sce_run_value.value_type = E_Sce_Run_Mem_Value;
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Variable %s not defined.",
		sce_inst->scope2
	);
}

static void rrmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	if (svr2 && svr1) {
		/**
		* &str = new allocated
		* 
		*/
		*svr1 = *svr2;
		svr2->is_const_value = IS_CONST_MEMORY;
		return;
	}
	if (!svr1) {
		get_vm_error_status(
			*sce_vm_context,
			"Sce Virutal Register %lu is %lu or greater.",
			sce_inst->ope1,
			SCE_VIRTUAL_REGISTER_MAX
		);
		return;
	}
	get_vm_error_status(
		*sce_vm_context,
		"Sce Virutal Register %lu is %lu or greater.",
		sce_inst->ope2,
		SCE_VIRTUAL_REGISTER_MAX
	);

}
#endif // !SCE_MOV_PROCESS_H_