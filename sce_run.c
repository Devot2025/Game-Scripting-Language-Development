#include "sce_run.h"
#include "sce_system_call.h"
#include "sce_run_error_func.h"
#include "sce_built_in_functions.h"
#include "sce_run_vm_helper.h"
#include "sce_built_in_functions.h"
#include "sce_cmp_operator.h"
#include "sce_operator_process.h"
#include "sce_mov_process.h"
#define operator_sce_function_vm(ope, opestr) \
	if (mem1->sce_run_value.value_type == E_Sce_Run_Mem_Value) { \
		mem1 = mem1->sce_run_value.mvalue; \
		if (mem2->sce_run_value.value_type == E_Sce_Run_Mem_Value) { \
			mem2 = mem2->sce_run_value.mvalue; \
		} \
	} \
	else { \
		if (mem2->sce_run_value.value_type == E_Sce_Run_Mem_Value) { \
			mem2 = mem2->sce_run_value.mvalue; \
		} \
	} \
	if (mem1->sce_run_value.value_type == E_Sce_Run_Mem_Value || mem2->sce_run_value.value_type == E_Sce_Run_Mem_Value) { \
		GET_ERROR_STRING( \
			error_status->error_str, \
			"Reference objects cannot be " opestr " together." \
		); \
		if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE; \
		return; \
	} \
	bool d_ = mem1->sce_run_value.value_type == mem2->sce_run_value.value_type; \
	if (d_) { \
		switch (mem1->sce_run_value.value_type) { \
		case E_Sce_Run_String_Value: \
			GET_ERROR_STRING( \
				error_status->error_str, \
				opestr " operations between string and string are not supported." \
			); \
			if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE; \
			break; \
		case E_Sce_Run_Int_Value: \
			mem1->sce_run_value.ivalue ope##= mem2->sce_run_value.ivalue; \
			break; \
		case E_Sce_Run_Bool_Value: \
			mem1->sce_run_value.ivalue = (uint32_t)mem1->sce_run_value.bvalue ope (uint32_t)mem2->sce_run_value.bvalue; \
			mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
			break; \
		case E_Sce_Run_Float_Value: \
			mem1->sce_run_value.fvalue ope##= mem2->sce_run_value.fvalue; \
			break; \
		case E_Sce_Run_Double_Value: \
			mem1->sce_run_value.dvalue ope##= mem2->sce_run_value.dvalue; \
			break; \
		case E_Sce_Run_Null_Value: \
			break; \
		case E_Sce_Run_Virtual_Value: \
			break; \
		case E_Sce_Run_Obj_Value: \
			GET_ERROR_STRING( \
				error_status->error_str, \
				opestr " using objects is not possible" \
			); \
			break; \
		case E_Sce_Run_Char_Value: \
		{ \
			mem1->sce_run_value.ivalue = (uint32_t)mem1->sce_run_value.cvalue ope (uint32_t)mem2->sce_run_value.cvalue; \
			mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
		} \
			break; \
		} \
		return; \
	} \
	d_ = mem1->sce_run_value.value_type > mem2->sce_run_value.value_type; \
	if (d_) { \
		switch (mem1->sce_run_value.value_type) { \
		case E_Sce_Run_Obj_Value: \
			GET_ERROR_STRING( \
				error_status->error_str, \
				opestr " using objects is not possible" \
			); \
			break; \
		case E_Sce_Run_String_Value: \
			switch (mem2->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
				append_u8_byte_u8_string( \
					mem1->sce_run_value.ssvalue, \
					(uint8_t*)"null" \
				); \
				break; \
			case E_Sce_Run_Virtual_Value: \
				append_u8_byte_u8_string( \
					mem1->sce_run_value.ssvalue, \
					(uint8_t*)"virtual" \
				); \
				break; \
			default: \
				GET_ERROR_STRING( \
					error_status->error_str, \
					opestr " operations between %s and %s are not supported.", \
					run_value_type_str[mem1->sce_run_value.value_type], \
					run_value_type_str[mem2->sce_run_value.value_type] \
				); \
				if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE; \
				break; \
			} \
			break; \
		case E_Sce_Run_Int_Value: \
			switch (mem2->sce_run_value.value_type) { \
			case E_Sce_Run_Virtual_Value: break; \
			case E_Sce_Run_Null_Value: break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.ivalue ope##= (uint32_t)mem2->sce_run_value.bvalue; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.ivalue ope##= (uint32_t)mem2->sce_run_value.cvalue; \
				break; \
			} \
			break; \
		case E_Sce_Run_Float_Value: \
			switch (mem2->sce_run_value.value_type) { \
			case E_Sce_Run_Virtual_Value: break; \
			case E_Sce_Run_Null_Value: break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.fvalue ope##= (float)mem2->sce_run_value.bvalue; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.fvalue ope##= (float)mem2->sce_run_value.cvalue; \
				break; \
			case E_Sce_Run_Int_Value: \
				mem1->sce_run_value.fvalue ope##= (float)mem2->sce_run_value.ivalue; \
				break; \
			} \
			break; \
		case E_Sce_Run_Double_Value: \
			switch (mem2->sce_run_value.value_type) { \
			case E_Sce_Run_Virtual_Value: break; \
			case E_Sce_Run_Null_Value: break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.dvalue ope##= (double)mem2->sce_run_value.bvalue; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.dvalue ope##= (double)mem2->sce_run_value.cvalue; \
				break; \
			case E_Sce_Run_Int_Value: \
				mem1->sce_run_value.dvalue ope##= (double)mem2->sce_run_value.ivalue; \
				break; \
			case E_Sce_Run_Float_Value: \
				mem1->sce_run_value.dvalue ope##= (double)mem2->sce_run_value.fvalue; \
				break; \
			} \
			break; \
		} \
	} \
	else { \
		switch (mem2->sce_run_value.value_type) { \
		case E_Sce_Run_Obj_Value: \
			GET_ERROR_STRING( \
				error_status->error_str, \
				opestr " using objects is not possible" \
			); \
			if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE; \
			break; \
		case E_Sce_Run_String_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
				break; \
			case E_Sce_Run_Virtual_Value: \
				break; \
			default: \
				GET_ERROR_STRING( \
					error_status->error_str, \
					opestr " operations between %s and %s are not supported.", \
					run_value_type_str[mem1->sce_run_value.value_type], \
					run_value_type_str[mem2->sce_run_value.value_type] \
				); \
				if (!error_status->error_str) error_status->error_vm_fallback_code = SCE_VM_FALLBACK_CODE; \
				break; \
			} \
			break; \
		case E_Sce_Run_Double_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
			case E_Sce_Run_Virtual_Value: \
				mem1->sce_run_value.dvalue = mem2->sce_run_value.dvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Double_Value; \
				break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.dvalue = (double)mem1->sce_run_value.bvalue ope (double)mem2->sce_run_value.dvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Double_Value; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.dvalue = (double)mem1->sce_run_value.cvalue ope (double)mem2->sce_run_value.dvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Double_Value; \
				break; \
			case E_Sce_Run_Int_Value: \
				mem1->sce_run_value.dvalue = (double)mem1->sce_run_value.ivalue ope (double)mem2->sce_run_value.dvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Double_Value; \
				break; \
			case E_Sce_Run_Float_Value: \
				mem1->sce_run_value.dvalue = (double)mem1->sce_run_value.fvalue ope (double)mem2->sce_run_value.dvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Double_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Float_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
			case E_Sce_Run_Virtual_Value: \
				mem1->sce_run_value.fvalue = mem2->sce_run_value.fvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Float_Value; \
				break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.fvalue = (float)mem1->sce_run_value.bvalue ope (float)mem2->sce_run_value.fvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Float_Value; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.fvalue = (float)mem1->sce_run_value.cvalue ope (float)mem2->sce_run_value.fvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Float_Value; \
				break; \
			case E_Sce_Run_Int_Value: \
				mem1->sce_run_value.fvalue = (float)mem1->sce_run_value.ivalue ope (float)mem2->sce_run_value.fvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Float_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Int_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
			case E_Sce_Run_Virtual_Value: \
				mem1->sce_run_value.ivalue = mem2->sce_run_value.ivalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
				break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.ivalue = (uint32_t)mem1->sce_run_value.bvalue ope (uint32_t)mem2->sce_run_value.ivalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
				break; \
			case E_Sce_Run_Char_Value: \
				mem1->sce_run_value.ivalue = (uint32_t)mem1->sce_run_value.cvalue ope (uint32_t)mem2->sce_run_value.ivalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Char_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
			case E_Sce_Run_Virtual_Value: \
				mem1->sce_run_value.cvalue = mem2->sce_run_value.cvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Char_Value; \
				break; \
			case E_Sce_Run_Bool_Value: \
				mem1->sce_run_value.ivalue = (uint32_t)mem1->sce_run_value.bvalue ope (uint32_t)mem2->sce_run_value.cvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Int_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Bool_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
			case E_Sce_Run_Virtual_Value: \
				mem1->sce_run_value.bvalue = mem2->sce_run_value.bvalue; \
				mem1->sce_run_value.value_type = E_Sce_Run_Bool_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Virtual_Value: \
			switch (mem1->sce_run_value.value_type) { \
			case E_Sce_Run_Null_Value: \
				mem1->sce_run_value.value_type = E_Sce_Run_Virtual_Value; \
				break; \
			} \
			break; \
		case E_Sce_Run_Null_Value: \
			break; \
		} \
	} 

/**
* Register Reuse VM Memory Management
Goal

Reuse a fixed set of registers across the entire VM (even across function calls) while safely managing heap-allocated objects when errors occur.

Basic Idea

Registers own temporary values.

When a register is overwritten, the previous value owned by that register is released automatically.

Instructions should not manually free memory whenever possible.

Register Assignment

Instead of:

r0 = value;

always use:

SET_REGISTER(r0, value);

Implementation:

void SET_REGISTER(Register *reg, Value value)
{
    if (reg->owns)
        destroy(reg->value);

    reg->value = value;
    reg->owns = value.is_heap_object;
}
Example
mov r0, OBJECT3()
mov r1, OBJECT2()
add r0, r1

mov r1, OBJECT1()
add r0, r1

add r0, 1    // Error

Behavior:

SET_REGISTER() automatically releases the previous value in r0.
r1 is cleared after being consumed.
No instruction performs manual cleanup except when consuming operands.
Error Handling

When an error occurs:

Restore the VM stack.
Scan all registers.
Destroy every register that still owns a heap object.
for (int i = 0; i < REGISTER_COUNT; i++)
{
    if (registers[i].owns)
        destroy(registers[i].value);
}
Advantages
Fixed number of registers.
No temporary object list.
No extra garbage collector.
Register reuse works across function calls.
Memory management is centralized.
Error recovery is simple and deterministic.
Key Principle

Registers own values.

Writing to a register transfers ownership.

Overwriting a register automatically releases the previous owned value.

On VM errors, remaining owned register values are released by a final register scan.
*/
#define SCE_SYSTEM_STDOUT 0
#define SCE_SYSTEM_STDIN  1
#define SCE_SYSTEM_STDERR 2

static void run_vm_context_symbol(Sce_Run_VM_Context* vm_context) {
	Sce_VM_Label_Table_Context* label_table_ctx_ = vm_context->now_vm_symbol_->label_context;
	if (!label_table_ctx_) return;
	if (label_table_ctx_ == &vm_context->now_vm_symbol_->global_filed) return;
	Sce_VM_Label_Table_Context* next_ = label_table_ctx_->next_;
	label_table_ctx_->next_ = NULL;
	delete_sce_run_label_context(label_table_ctx_);
	/**
	* TODO:
	* ADD GC PROCESS TO LABEL CONTEXT.
	*/
	label_table_ctx_ = next_;
	vm_context->now_vm_symbol_->label_context = label_table_ctx_;
}
static void runtime_error_gc_general_register(Sce_Run_VM_Context* sce_vm_context) {
	Sce_Virutal_Register* svr = &sce_vm_context->svrs_.svr[0];
	printf("gc runtime register...\n");
	for (size_t i = 1; i < SCE_VIRTUAL_REGISTER_MAX_; ++i) {
		delete_sce_run_virtual_memory(svr + i);
	}
	printf("gc runtime stack...\n");
	Sce_VM_Label_Table_Context* label_ = sce_vm_context->now_vm_symbol_->label_context;
	Sce_Virtual_Stacks* sm = &sce_vm_context->now_vm_symbol_->sce_stack_;
	for (size_t i = 0; i < sm->stack_size_; ++i) {
		delete_sce_run_virtual_memory(sm->stack_mem_ + i);
	}
	printf("gc runtime local memory...\n");
	while (sce_vm_context->now_vm_symbol_->label_context) {
		run_vm_context_symbol(sce_vm_context);
	}
}
static void runtime_serious_error_process(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	printf("sce language runtime serious error\n");
	runtime_error_gc_general_register(sce_vm_context);
	printf("%s\n", sce_vm_context->vm_error_status.error_str);
	if(sce_inst) printf("bytecode instruction is %s\n", GET_SCE_IR_STRING(sce_inst));
	printf("Terminate the program.");
}

static void set_global_built_in_function(Sce_Run_VM_Context* sce_vm_context) {
	sce_vm_context->root_symbol.built_in_table.label_ = built_in_adress;
	sce_vm_context->root_symbol.built_in_table.size_ = 1;
}
void sub_svr_value(Sce_Run_VM_Error_Status* error_status, Sce_Virtual_Memory* mem1, Sce_Virutal_Register* mem2) {
	Sce_Virtual_Memory* tmp_mem2 = mem2;

	operator_sce_function_vm(
		-,
		"Subtraction"
	)
		delete_sce_run_virtual_memory(tmp_mem2);

}
static void sub_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(sce_vm_context, sce_inst->ope2);
	sub_svr_value(
		&sce_vm_context->vm_error_status,
		svr1, svr2
	);
}
static void sce_type(uint32_t argc, Sce_Virutal_Register* svrs, Sce_Run_VM_Error_Status* dst_) {
#define ARGC_CALL_TYPE 1
	if (argc != ARGC_CALL_TYPE) {
		GET_ERROR_STRING(
			dst_->error_str, 
			"System call '%s' expected %lu arguments, but got %lu.",
			"type",
			ARGC_CALL_TYPE,
			argc
		);
		return;
	}
	Sce_Virtual_Memory* svr1 = &svrs[1];
	Sce_Virtual_Memory* tmp_svr1 = &svrs[1];
	Sce_Virtual_Memory* svrr = &svrs[SCE_VIRTUAL_REGISTER_RETURN];
	if (svr1->sce_run_value.value_type) svr1 = svr1->sce_run_value.mvalue;
	const uint8_t* buf_ = run_value_type_str[svr1->sce_run_value.value_type];
	svrr->sce_run_value.ssvalue = smart_malloc(U8_String_Buffers, 1);
	if (!svrr->sce_run_value.ssvalue) {
		GET_ERROR_STRING(
			dst_->error_str,
			"Failed to Memory Allocation."
		);
		return;
	}
	uint32_t len_ = u8strlen_u32(buf_);
	init_u8_string_buffers(svrr->sce_run_value.ssvalue, len_);
	append_u8_byte_u8_string_basic(svrr->sce_run_value.ssvalue, buf_, len_);
	delete_sce_run_virtual_memory(svr1);
}

static uint8_t* get_svr_value_to_str_(bool* is_ref, uint32_t* size_, Sce_Virutal_Register* svr) {
	if (is_svr_value_type(*svr, E_Sce_Run_String_Value)) {
		uint8_t* dst_ = svr->sce_run_value.ssvalue->str__;
		*size_ = (uint32_t)strlen(dst_);
		*is_ref = true;
		return dst_;
	}
	else if (is_svr_value_type(*svr, E_Sce_Run_Obj_Value)) return NULL;
	else if (is_svr_value_type(*svr, E_Sce_Run_Mem_Value)) {  return NULL; }
	else if (is_svr_value_type(*svr, E_Sce_Run_Int_Value)) {

		return to_32_str(svr->sce_run_value.ivalue, size_);
	}
	else if (is_svr_value_type(*svr, E_Sce_Run_Float_Value)) {
		return to_float_str(svr->sce_run_value.fvalue, size_);
	}

	else if (is_svr_value_type(*svr, E_Sce_Run_Double_Value)) {
		return to_double_str(svr->sce_run_value.dvalue, size_);
	}
	return NULL;
}
static uint8_t* get_svr_value_to_str(bool* is_ref, uint32_t* size_, Sce_Virutal_Register* svr) {
	if (is_svr_value_type(*svr, E_Sce_Run_String_Value)) {
		uint8_t* dst_ = svr->sce_run_value.ssvalue->str__;
		*size_ = u8strlen_u32(dst_);
		*is_ref = true;

		return dst_;
	}
	else if (is_svr_value_type(*svr, E_Sce_Run_Obj_Value)) return NULL;
	else if (is_svr_value_type(*svr, E_Sce_Run_Mem_Value)) return get_svr_value_to_str_(is_ref, size_, svr->sce_run_value.mvalue);
	else if (is_svr_value_type(*svr, E_Sce_Run_Int_Value)) {
		return to_32_str(svr->sce_run_value.ivalue, size_);
	}
	else if (is_svr_value_type(*svr, E_Sce_Run_Float_Value)) {
		return to_float_str(svr->sce_run_value.fvalue, size_);
	}

	else if (is_svr_value_type(*svr, E_Sce_Run_Double_Value)) {
		return to_double_str(svr->sce_run_value.dvalue, size_);
	}
	return NULL;
}
static FILE* sce_open_system_call(Sce_Virutal_Register* src_svr, int* open_check, bool is_mem_value) {
	if (is_svr_value_type(*src_svr, E_Sce_Run_String_Value)) {

	}
	else if (E_Sce_Run_Int_Value) {
		switch (src_svr->sce_run_value.ivalue) {
		case SCE_SYSTEM_STDOUT:
			*open_check = 1;

			return stdout;
		case SCE_SYSTEM_STDIN:
			*open_check = 1;

			return stdin;
		case SCE_SYSTEM_STDERR:
			*open_check = 1;

			return stderr;
		default:
			*open_check = -1;
			return NULL;

		}
	}
	else if (E_Sce_Run_Mem_Value) {
		if (!is_mem_value) sce_open_system_call(src_svr, open_check, true);
	}
	*open_check = -2;
	return NULL;
}

static FILE* get_svr_file_object_basic(Sce_Virutal_Register* svr, int* error_check) {
	if (svr->sce_run_value.value_type == E_Sce_Run_Obj_Value) {
		if (svr->sce_run_value.evalue->ext_type == E_Sce_Run_Ext_FILE_Value) {
			return (FILE*)svr->sce_run_value.evalue->ptr__;
		}
	}
	*error_check = -1;
	return NULL;
}
static FILE* get_svr_file_object(Sce_Virutal_Register* svr, int* error_check) {
	if (svr->sce_run_value.value_type == E_Sce_Run_Obj_Value) {
		if (svr->sce_run_value.evalue->ext_type == E_Sce_Run_Ext_FILE_Value) {
			return (FILE*)svr->sce_run_value.evalue->ptr__;
		}
	}
	else if (svr->sce_run_value.value_type == E_Sce_Run_Mem_Value) {
		return get_svr_file_object_basic(svr->sce_run_value.mvalue, error_check);
	}
	*error_check = -1;
	return NULL;
}
static void set_vm_context_svr_file(Sce_Run_VM_Context* vm_context, bool is_const, FILE* fp) {

	access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).is_const_value = is_const;
	access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.value_type = E_Sce_Run_Obj_Value;
	access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.evalue = smart_malloc(Sce_Ext_Values, 1);
	if (!access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.evalue) return;
	access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.evalue->ext_type = E_Sce_Run_Ext_FILE_Value;
	access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.evalue->ptr__ = (void*)fp;

}
static Sce_VM_Symbol_Table* search_module_name(Sce_VM_Symbol_Table * vm_modules, const uint8_t * name_) {
	while (vm_modules) {

		if (simple_strcmp(vm_modules->module_name_, name_)) return vm_modules;
		vm_modules = vm_modules->next_;
	}
	return NULL;
}

static Sce_Built_In_Label* search_built_in_label_name(Sce_Built_In_Label_Table * built_in, const uint8_t* name_) {
	for (uint32_t i = 0; i < built_in->size_; ++i) {
		if (simple_strcmp((*(built_in->label_ + i)).label_name, name_)) return (built_in->label_ + i);
	}
	return NULL;
}
static Sce_VM_Label_Table* search_label_name(Sce_VM_Label_Table * vm_label, const uint8_t * name_) {
	while (vm_label) {


		if (simple_strcmp(vm_label->lebel_adress_->scope2, name_)) return vm_label;
		vm_label = vm_label->next_;
	}
	return NULL;
}

static void set_global_lable_name_sce_vm(Sce_Run_VM_Context* sce_vm_context) {
	Sce_VM_Label_Table* vm_label = search_label_name(
		sce_vm_context->root_symbol.label_table_, sce_vm_context->inst_->scope2
	);
	if (!vm_label) {
		Sce_VM_Label_Table* vm_label = smart_malloc(Sce_VM_Label_Table, 1);
		if (!vm_label) return;
		vm_label->lebel_adress_ = sce_vm_context->inst_;
		vm_label->next_ = sce_vm_context->root_symbol.label_table_;
		sce_vm_context->root_symbol.label_table_ = vm_label;
	}
	else {
		get_vm_error_status(
			*sce_vm_context,
			"Label %s redefined.",
			sce_vm_context->inst_->scope2
		);
	}
}
static Sce_VM_Symbol_Table* get_module_symbol_table(Sce_Run_VM_Context* sce_vm_context, const uint8_t* name_) {

	Sce_VM_Symbol_Table* module_;
	if (
		sce_vm_context->now_vm_symbol_ &&
		sce_vm_context->now_vm_symbol_->module_name_ &&
		simple_strcmp(sce_vm_context->now_vm_symbol_->module_name_, name_)
		) {
		module_ = sce_vm_context->now_vm_symbol_;
	}
	else {

		module_ = search_module_name(
			sce_vm_context->vm_symbol_, name_
		);
	}
	return module_;
}

static Sce_VM_Label_Table* set_new_label_adress_vm_context(Sce_Run_VM_Context* vm_context, Sce_VM_Label_Table* label_) {
	/*
	Sce_VM_Label_Table* label_adress = smart_malloc(Sce_VM_Label_Table, 1);
	if (!label_adress) return NULL;
	run_vm_context_symbol(vm_context);
	label_adress->lebel_adress_ = label_->lebel_adress_;
	label_adress->next_ = NULL;
	vm_context->now_vm_symbol_->label_context->lebel_adress_ = label_adress;
	return label_;
	*/
	run_vm_context_symbol(vm_context);
	Sce_VM_Label_Table_Context * label_adress_ctx = smart_malloc(Sce_VM_Label_Table_Context, 1);
	if (!label_adress_ctx) return NULL;
	label_adress_ctx->lebel_adress_ = label_->lebel_adress_;
	label_adress_ctx->obj_table_ = NULL;
	label_adress_ctx->next_ = vm_context->now_vm_symbol_->label_context;
	vm_context->now_vm_symbol_->label_context = label_adress_ctx;
	return label_;
}
static void sce_save_register_to_tail_register(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	memcpy(&vm_context->svrs_.svr[sce_inst->ope2 + 1], &vm_context->svrs_.svr[1], sizeof(Sce_Virutal_Register) * sce_inst->ope1);

}

static void sce_load_register_to_tail_register(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {

	memcpy(&vm_context->svrs_.svr[1], &vm_context->svrs_.svr[sce_inst->ope1 + 1], sizeof(Sce_Virutal_Register) * sce_inst->ope2);
}
static Sce_Binary_Machine_Instructions* get_name_init_label_vm_context(Sce_Run_VM_Context* sce_vm_context) {
	return sce_vm_context->now_vm_symbol_->global_filed.lebel_adress_;
}
static void set_init_lable_name_isce_vm(Sce_Run_VM_Context* sce_vm_context) {
	sce_vm_context->vm_symbol_->global_filed.lebel_adress_ = sce_vm_context->inst_->next;
}

static void set_global_init_lable_name_isce_vm(Sce_Run_VM_Context* sce_vm_context) {
	sce_vm_context->root_symbol.global_filed.lebel_adress_ = sce_vm_context->inst_->next;
}
static Sce_Run_VM_Context gen_sce_vm_context() {
	return (Sce_Run_VM_Context) { 0 };
}

static int set_new_label_vm_context(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions** sce_inst) {
	Sce_Built_In_Label* built_in_label = search_built_in_label_name(&vm_context->now_vm_symbol_->built_in_table, (*sce_inst)->scope2);
	if (built_in_label) {
		get_vm_error_status(
			*vm_context,
			"%s is invalid label.",
			built_in_label->label_name
		);
		return -3;
	}
	Sce_VM_Label_Table* label_;
	label_ = search_label_name(vm_context->now_vm_symbol_->label_table_, (*sce_inst)->scope2);
	if (!label_) return -2;
	if (simple_strcmp((*sce_inst)->scope2, main_label)) {
		if (!set_new_label_adress_vm_context(vm_context, label_)) return -1;

		return 2;
	}
	else if (simple_strcmp((*sce_inst)->scope2, end_label)) {
		if (!set_new_label_adress_vm_context(vm_context, label_)) return -1;
		return 2;
	}
	else if (simple_strcmp((*sce_inst)->scope2, exit_label)) {
		run_vm_context_symbol(vm_context);
		return 2;
	}

	else {
		/*
		* CMP REQ
		*/

		Sce_Virutal_Register* svr128 = get_svr(vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
		if (svr128->sce_run_value.value_type == E_Sce_Run_Bool_Value) {
			if (svr128->sce_run_value.bvalue == true) return 3;
		}
		else return -3;

		(*sce_inst) = label_->lebel_adress_->next;
		return 1;
	}
}
static void test_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_) {
	Sce_Virutal_Register* svr1 = get_svr(sce_vm_context, inst_->ope1);
	Sce_Virutal_Register* svrr = get_svr(sce_vm_context, SCE_VIRTUAL_REGISTER_CMP_RETURN);
	Sce_Virutal_Register* t_svr1 = svr1;
	if (is_svr_type(svr1, E_Sce_Run_Mem_Value)) svr1 = access_svr_mvalue(*svr1);
	switch (access_svr_value(*svr1).value_type){
	case E_Sce_Run_Null_Value:
		get_vm_error_status(
			*sce_vm_context,
			"left hand is null value."
		);
		break;
	case E_Sce_Run_Virtual_Value:
		set_register_bool_value(*svrr, false);
		break;
	case E_Sce_Run_Bool_Value:
		set_register_bool_value(*svrr, access_svr_bvalue(*svr1));
		break;
	case E_Sce_Run_Char_Value:
		set_register_bool_value(*svrr, (bool)access_svr_cvalue(*svr1));
		break;
	case E_Sce_Run_Int_Value:
		set_register_bool_value(*svrr, (bool)access_svr_ivalue(*svr1));
		break;

	case E_Sce_Run_Float_Value:
		set_register_bool_value(*svrr, (bool)access_svr_fvalue(*svr1));
		break;
	case E_Sce_Run_Double_Value:
		set_register_bool_value(*svrr, (bool)access_svr_dvalue(*svr1));
		break;
	case E_Sce_Run_String_Value:
		set_register_bool_value(*svrr, (bool)access_svr_ssvalue(*svr1)->index__);
		break;
	case E_Sce_Run_Obj_Value:
		set_register_bool_value(*svrr, true);

		break;
	case E_Sce_Run_Mem_Value:
		set_register_bool_value(*svrr, true);
		break;
	default:
		assert(false);
	}
	delete_sce_run_virtual_memory(t_svr1);

}

static void run_collect_label_sce_vm_context(Sce_Run_VM_Context* sce_vm_context) {
	uint8_t is_label_mode = 0x0;
	while (sce_vm_context->inst_) {

		switch (sce_vm_context->inst_->sce_bmr_code) {
		case E_SCE_BINARY_INST_NAME__:
			if (check_global_module_name(sce_vm_context->inst_->scope2)) {
				is_label_mode = GLOBAL_LABEL;
				break;
			}
			is_label_mode = LOCAL_LABEL;
			set_module_name_sce_vm(
				sce_vm_context
			);

			break;
		case E_SCE_BINARY_INST_LABEL__:
			if (is_label_mode & GLOBAL_LABEL) {

				set_global_lable_name_sce_vm(
					sce_vm_context
				);
			}
			else if (is_label_mode & LOCAL_LABEL) {
				set_lable_name_sce_vm(
					sce_vm_context
				);
			}
			is_label_mode |= INIT_FINISHED;

			break;
		default:
			if (is_label_mode == GLOBAL_LABEL) {
				set_global_init_lable_name_isce_vm(sce_vm_context);

				is_label_mode |= INIT_FINISHED;
			}
			else if (is_label_mode == LOCAL_LABEL) {
				set_init_lable_name_isce_vm(sce_vm_context);

				is_label_mode |= INIT_FINISHED;
			}
		}
		sce_vm_context->inst_ = sce_vm_context->inst_->next;
	}

}
static void sce_save_register_to_stack(Sce_Virutal_Register* svrs, Sce_Virtual_Stacks* stacks, Sce_Binary_Machine_Instructions* sce_inst, Sce_Run_VM_Error_Status* error_status) {
	Sce_Virtual_Memory* stacks_ = smart_realloc(Sce_Virtual_Memory, stacks->stack_mem_, stacks->stack_size_ + sce_inst->ope1);
	if (!stacks_) {

		GET_ERROR_STRING(
			error_status->error_str,
			"failed to stack alloc memory."
		);
		return;
	}

	stacks->stack_mem_ = stacks_;
	smart_memcpy_p(stacks->stack_mem_ + stacks->stack_size_, svrs_access_begin(svrs), Sce_Virtual_Memory, sce_inst->ope1);
	stacks->stack_size_ = stacks->stack_size_ + sce_inst->ope1;
}

static void sce_load_register_to_stack(Sce_Virutal_Register* svrs, Sce_Virtual_Stacks* stacks, Sce_Binary_Machine_Instructions* sce_inst, Sce_Run_VM_Error_Status* error_status) {
	if (stacks->stack_size_ < sce_inst->ope1) {
		GET_ERROR_STRING(
			error_status->error_str,
			"stack underflow."
		);
		return;
	}
	smart_memcpy_p(svrs_access_begin(svrs), stacks->stack_mem_ + stacks->stack_size_ - sce_inst->ope1, Sce_Virtual_Memory, sce_inst->ope1);
	if (stacks->stack_size_ - sce_inst->ope1) {
		Sce_Virtual_Memory* stacks_ = smart_realloc(Sce_Virtual_Memory, stacks->stack_mem_, stacks->stack_size_ - sce_inst->ope1);
		if (!stacks_) {
			GET_ERROR_STRING(
				error_status->error_str,
				"failed to stack alloc memory."
			);
			return;

		}
		stacks->stack_mem_ = stacks_;
	}
	else s_free(stacks->stack_mem_);
	stacks->stack_size_  -= sce_inst->ope1;
}
static void access_sce_label_basic(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst, Sce_Virutal_Register * svr) {

	switch (svr->sce_run_value.value_type) {
	case E_Sce_Run_Obj_Value:
		assert(false);
		break;
	case E_Sce_Run_Array_Value:

	{
		Sce_Built_In_Label* built_label = search_built_in_label_name(&array_class_built_in_table, sce_inst->scope2);
		if (!built_label) {

			GET_ERROR_STRING(
				vm_context->vm_error_status.error_str,
				"string instance obj not has %s module.",
				sce_inst->scope2
			);
		}
		else {
			vm_context->svrs_.svr[sce_inst->ope1].is_const_value = IS_CONST_MEMORY;
			vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.blvalue = built_label->label_adress;
			vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type = E_Sce_Run_Built_In_Function_Value;
		}
	}
	break;
	case E_Sce_Run_String_Value:
	{
		Sce_Built_In_Label* built_label = search_built_in_label_name(&str_class_built_in_table, sce_inst->scope2);
		if (!built_label) {

			GET_ERROR_STRING(
				vm_context->vm_error_status.error_str,
				"string instance obj not has %s module.",
				sce_inst->scope2
			);
		}
		else {
			vm_context->svrs_.svr[sce_inst->ope1].is_const_value = IS_CONST_MEMORY;
			vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.blvalue = built_label->label_adress;
			vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type = E_Sce_Run_Built_In_Function_Value;
		}
	}
	break;
	default:
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"%s type is not obj.",
			run_value_type_str[svr->sce_run_value.value_type]
		);
		return;
		break;
	}
}
static void access_sce_label(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	/**
	* 
	@:
	R128 <- FUNCTION
	R128.upper()
	R128 NOT IS CONST
	tmp_ = R128.CLASS;
	DELETE R128
	MODULE R128
	:@
	*/
	switch (vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type){
	case E_Sce_Run_Obj_Value:
		/**
		* class A().u?
		* valid or invalid
		*/
		assert(false);
		break;
	case E_Sce_Run_Mem_Value:
		access_sce_label_basic(vm_context, sce_inst, vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.mvalue);
		break;
	default:
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"%s type is not obj.",
			run_value_type_str[vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type]
		);
		return;
		break;
	}
}
static void indirect_sce_label(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr = vm_context->svrs_.svr + sce_inst->ope1;
	if (is_svr_value_type(*svr, E_Sce_Run_Built_In_Function_Value)) {
		//printf("%lu\n", vm_context->svrs_.svr[1].sce_run_value.mvalue->sce_run_value.lavalue->size_);

		svr->sce_run_value.blvalue(sce_inst->ope2, vm_context->svrs_.svr, &vm_context->vm_error_status);
	}
	else if (is_svr_value_type(*svr, E_Sce_Run_Function_Value)) {
		assert(false);
	}
	else {

		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"%s type is not callable.",
			run_value_type_str[vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type]
		);
	}
}
static Sce_Binary_Machine_Instructions *  set_new_label_vm_context_table(Sce_VM_Symbol_Table*module_, Sce_VM_Label_Table*label_) {
	Sce_VM_Label_Table_Context* table_ctx = smart_malloc(Sce_VM_Label_Table_Context, 1);
	if (!table_ctx) return NULL;
	table_ctx->next_ = module_->label_context;
	table_ctx->obj_table_ = NULL;
	table_ctx->lebel_adress_ = label_->lebel_adress_;
	module_->label_context = table_ctx;
	return label_->lebel_adress_;
}
static void return_sce_label(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions** sce_inst) {
	run_vm_context_symbol(vm_context);
	*sce_inst = vm_context->now_vm_symbol_->label_context->lebel_adress_;
	vm_context->now_vm_symbol_->sce_stack_.stack_point_ = vm_context->now_vm_symbol_->label_context->stack_point;
}
static void omov_sce_vm_basic(Sce_Virutal_Register* svr1, Sce_Virutal_Register* svr2) {
	if (is_svr_value_type(*svr2, E_Sce_Run_Mem_Value)) {
		Sce_Virtual_Memory* mem = access_svr_mvalue(*svr2);
		*svr1 = *mem;
		mem->is_const_value = IS_CONST_MEMORY;
		mem->sce_run_value.vvalue = 0;
		mem->sce_run_value.value_type = E_Sce_Run_Virtual_Value;
	}
	else *svr1 = *svr2;
	svr2->is_const_value = IS_CONST_MEMORY;
	svr2->sce_run_value.vvalue = 0;
	svr2->sce_run_value.value_type = E_Sce_Run_Virtual_Value;

}
#define new_array_virtual_memory(array_, n__) (array_->mem_ = smart_malloc(Sce_Virtual_Memory, n__), array_->size_ = n__, (bool)array_->mem_)
#define new_zero_size_array_virtual_memory(array_) new_array_virtual_memory(array_, 1)
static void sce_access_array(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svrr = 
		get_svr(vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr1 =
		get_svr(vm_context, sce_inst->ope1);
	Sce_Virutal_Register* svr2 =
		get_svr(vm_context, sce_inst->ope2);
	svr1 = sce_resolve_register_type(svr1);

	svr2 = sce_resolve_register_type(svr2);

	if (is_svr_value_type(*svr1, E_Sce_Run_Array_Value)) {
		if (is_svr_value_type(*svr2, E_Sce_Run_Int_Value)) {
			svrr->is_const_value = IS_CONST_MEMORY;
			svrr->sce_run_value.value_type = E_Sce_Run_Mem_Value;
			if (check_range_idx(*svr1->sce_run_value.lavalue, svr2->sce_run_value.ivalue)) {
				svrr->sce_run_value.mvalue = &access_list_array_memory(*svr1->sce_run_value.lavalue, svr2->sce_run_value.ivalue);
			}
			else {
				GET_ERROR_STRING(
					vm_context->vm_error_status.error_str,
					"array %lu index out of %lld range.",
					svr2->sce_run_value.ivalue,
					svr1->sce_run_value.lavalue->size_
				);
			}
			return;
		}
		else {
			GET_ERROR_STRING(
				vm_context->vm_error_status.error_str,
				"%s is not int.",
				run_value_type_str[vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type]
			);
			return;
		}
	}
	GET_ERROR_STRING(
		vm_context->vm_error_status.error_str,
		"%s is not array.",
		run_value_type_str[vm_context->svrs_.svr[sce_inst->ope1].sce_run_value.value_type]
	);
}
static void amov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr1 =
		get_svr(sce_vm_context, sce_inst->ope1);
	svr1->is_const_value = NOT_IS_CONST_MEMORY;
	svr1->sce_run_value.value_type = E_Sce_Run_Array_Value;

	svr1->sce_run_value.lavalue = smart_malloc(Sce_List_Array, 1);
	if (!svr1->sce_run_value.lavalue) goto MEMORY_ERROR_PROCESS;
	if (is_memory_idx_register(sce_inst->ope2)) {
		uint32_t copy_range = sce_inst->ope2 - sce_inst->ope1;
		if (copy_range) {
			/*1 + 1*/
			svr1->sce_run_value.lavalue->mem_ = smart_malloc(Sce_Virtual_Memory, copy_range);
			if (!svr1->sce_run_value.lavalue->mem_)goto MEMORY_ERROR_PROCESS;
			svr1->sce_run_value.lavalue->size_= copy_range;
			smart_memcpy_p(svr1->sce_run_value.lavalue->mem_, svr1 + 1, Sce_Virtual_Memory, copy_range);
		}
		else {
			if (new_zero_size_array_virtual_memory(access_svr_lavalue(*svr1))) goto MEMORY_ERROR_PROCESS;
		}
	}
	else {


		if (is_memory_idx_general_register(sce_inst->ope1 + 1)) {
			Sce_Virutal_Register* svro = svr1 + 1;
			Sce_Virutal_Register* svrs = now_stack_mem(now_stack_idx(SCE_VIRTUAL_STACK_MIN, *sce_vm_context), *sce_vm_context);
			/**
			* reg 1~ 127, ret 128, stack 129~
			* ope1, 127
			* 127 = (129, ope2);
			* 127 + 1 < 128
			* 126 + 1 < 128
			* 129 - 128 = 1;
			* (129 - 129) + 0 = 
			* 129 - 12
			* 127 + 1 < 128
* 
			*/
			/*
			 * Memory layout
			 * 1-127 : General registers
			 * 128   : Return register
			 * 129-  : Stack
			 *
			 * When ope1+1 is still a register and ope2 is on the stack,
			 * copy the register part first and then the stack part.
			 */
			uint32_t res_size_ = 0;
			uint32_t i = SCE_VIRTUAL_REGISTER_MAX - sce_inst->ope1;
			uint32_t j = sce_inst->ope2 - SCE_VIRTUAL_GENERAL_REGISTER_MAX;
			res_size_ = i + j;
			svr1->sce_run_value.lavalue->mem_ = smart_malloc(Sce_Virtual_Memory, res_size_);
			if (!svr1->sce_run_value.lavalue->mem_) goto MEMORY_ERROR_PROCESS; 
			svr1->sce_run_value.lavalue->size_ = res_size_;
			smart_memcpy_p(svr1->sce_run_value.lavalue->mem_, svro, Sce_Virtual_Memory, i);
			smart_memcpy_p(svr1->sce_run_value.lavalue->mem_ + i, svrs, Sce_Virtual_Memory, j);

		}
		else {
			/*
			* 129 - 129
			* (129 - 129) + sp + stack_mem = adress 0x00010
			* (130 - 129) + sp + stack_mem = adress 0x0001
			* 130 - 129 = 1
			*/
			uint32_t res_size_ = sce_inst->ope2 - sce_inst->ope1;
			if (res_size_) {
				Sce_Virutal_Register* svrs = now_stack_mem(now_stack_idx(sce_inst->ope1 + 1, *sce_vm_context), *sce_vm_context);

				svr1->sce_run_value.lavalue->mem_ = smart_malloc(Sce_Virtual_Memory, res_size_);
				if (!svr1->sce_run_value.lavalue->mem_) goto MEMORY_ERROR_PROCESS; 
				svr1->sce_run_value.lavalue->size_ = res_size_;
				smart_memcpy_p(svr1->sce_run_value.lavalue->mem_, svrs, Sce_Virtual_Memory, res_size_);

			}
			else {
				if (new_zero_size_array_virtual_memory(access_svr_lavalue(*svr1))) goto MEMORY_ERROR_PROCESS;

			}

		}
	}
	return;
MEMORY_ERROR_PROCESS:
	GET_ERROR_STRING(
		sce_vm_context->vm_error_status.error_str,
		"Failed to memory alloc."
	);
}
static void omov_sce_vm(Sce_Run_VM_Context * sce_vm_context, Sce_Binary_Machine_Instructions * sce_inst) {
		Sce_Virutal_Register* svr1 =
			get_svr(sce_vm_context, sce_inst->ope1);
		Sce_Virutal_Register* svr2 =
			get_svr(sce_vm_context, sce_inst->ope2);
		if (svr2 && svr1) {
			//printf("%lu %lu\n", sce_inst->ope1, sce_inst->ope2);
			omov_sce_vm_basic(svr1, svr2);
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
static void call_sce_label(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions** sce_inst) {
	Sce_Binary_Machine_Instructions* sce_inst_ = *sce_inst;
	Sce_Built_In_Label* built_in_label = search_built_in_label_name(&vm_context->root_symbol.built_in_table, sce_inst_->scope2);

	if (built_in_label) {
		built_in_label->label_adress(sce_inst_->ope1, vm_context->svrs_.svr, &vm_context->vm_error_status);

		return;
	}
	if (&vm_context->root_symbol == vm_context->now_vm_symbol_) goto GLOBAL_SKIP_BUILT;
	built_in_label = search_built_in_label_name(&vm_context->now_vm_symbol_->built_in_table, sce_inst_->scope2);
GLOBAL_SKIP_BUILT:
	if (built_in_label) {
		built_in_label->label_adress(sce_inst_->ope1, vm_context->svrs_.svr, &vm_context->vm_error_status);
		return;
	}
	Sce_VM_Label_Table* label_;
	label_ = search_label_name(vm_context->root_symbol.label_table_, sce_inst_->scope2);
	if (!label_) {
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"%s function is not defined.",
			sce_inst_->scope2
		);
		return;
	}
	else {
		/**
		* SP 0
		* SS +3
		* 
		* SP=3
		* SS=+6
		* 
		* SS=-6
		*/
		uint32_t func_args_num_ = sce_inst_->ope1;

		vm_context->now_vm_symbol_->label_context->lebel_adress_ = sce_inst_;
		Sce_Binary_Machine_Instructions* vm_ = set_new_label_vm_context_table(vm_context->now_vm_symbol_, label_);
		*sce_inst = vm_;
		uint32_t require_args = vm_->ope1;
		if (func_args_num_ != require_args) {
			GET_ERROR_STRING(
				vm_context->vm_error_status.error_str,
				SCE_FUNCTION_ARGC_ERROR,
				vm_->scope2,
				require_args,
				func_args_num_
			);
			return;
		}
		vm_context->now_vm_symbol_->sce_stack_.stack_point_ = vm_context->now_vm_symbol_->sce_stack_.stack_size_;
		vm_context->now_vm_symbol_->label_context->stack_point = vm_context->now_vm_symbol_->sce_stack_.stack_point_;
		return;
	}
	label_ = search_label_name(vm_context->now_vm_symbol_->label_table_, sce_inst_->scope2);
	if (!label_) {
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"%s function is not defined.",
			sce_inst_->scope2
		);
		return;
	}
	uint32_t func_args_num_ = sce_inst_->ope1;

	vm_context->now_vm_symbol_->label_context->lebel_adress_ = sce_inst_->next;
	Sce_Binary_Machine_Instructions* vm_ = set_new_label_vm_context_table(vm_context->now_vm_symbol_, label_);

	*sce_inst = vm_;
	uint32_t require_args = vm_->ope1;
	if (func_args_num_ != require_args) {
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			SCE_FUNCTION_ARGC_ERROR,
			vm_->scope2,
			require_args,
			func_args_num_
		);
		return;

	}
	vm_context->now_vm_symbol_->sce_stack_.stack_point_ = vm_context->now_vm_symbol_->sce_stack_.stack_size_;
	vm_context->now_vm_symbol_->label_context->stack_point = vm_context->now_vm_symbol_->sce_stack_.stack_point_;

}
static void add_stack_point_sce_vm_context(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virtual_Memory* stacks_ = smart_realloc(
		Sce_Virtual_Memory, vm_context->now_vm_symbol_->sce_stack_.stack_mem_, vm_context->now_vm_symbol_->sce_stack_.stack_size_ + sce_inst->ope2);
	if (!stacks_) {

		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"failed to stack alloc memory."
		);
		return;
	}

	vm_context->now_vm_symbol_->sce_stack_.stack_mem_ = stacks_;
	vm_context->now_vm_symbol_->sce_stack_.stack_size_ += sce_inst->ope2;
}

static void sub_stack_point_sce_vm_context(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	if (vm_context->now_vm_symbol_->sce_stack_.stack_size_ < sce_inst->ope1) {
		GET_ERROR_STRING(
			vm_context->vm_error_status.error_str,
			"stack undeflow."
		);
		return;
	}
	if (vm_context->now_vm_symbol_->sce_stack_.stack_size_ - sce_inst->ope1) {
		Sce_Virtual_Memory* stacks_ = smart_realloc(
			Sce_Virtual_Memory, vm_context->now_vm_symbol_->sce_stack_.stack_mem_, vm_context->now_vm_symbol_->sce_stack_.stack_size_ - sce_inst->ope1);
		if (!stacks_) {
			GET_ERROR_STRING(
				vm_context->vm_error_status.error_str,
				"failed to stack alloc memory."
			);
			return;
		}
		vm_context->now_vm_symbol_->sce_stack_.stack_mem_ = stacks_;
	}
	else s_free(vm_context->now_vm_symbol_->sce_stack_.stack_mem_);
	vm_context->now_vm_symbol_->sce_stack_.stack_size_ -= sce_inst->ope1;
}
static void run_sce_vm_context_basic(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	while (sce_inst) {

		switch (
			sce_inst->sce_bmr_code
			) {
		case E_SCE_BINARY_INST_ADD__:
			add_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_SUB__:
			sub_sce_vm_context(sce_vm_context, sce_inst);
			break;

		case E_SCE_BINARY_INST_MUL__:
			mul_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_DIV__:
			div_sce_vm_context(sce_vm_context, sce_inst);
			break;

		case E_SCE_BINARY_INST_MOD__:
			mod_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_INDIRECT_CALL__:
			indirect_sce_label(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_ACCESS_LABEL_MEMBER__:
			access_sce_label(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_ACCESS_MEM_MEMBER__:
			break;

		case E_SCE_BINARY_INST_ACCESS_ARRAY_MEMORY__:
			sce_access_array(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_CALL__:

			call_sce_label(sce_vm_context, &sce_inst);

			break;
		case E_SCE_BINARY_INST_RET__:
			return_sce_label(sce_vm_context, &sce_inst);
			break;

		case E_SCE_BINARY_INST_SAVE_REGISTER_STACK__:
			sce_save_register_to_stack(sce_vm_context->svrs_.svr, &sce_vm_context->now_vm_symbol_->sce_stack_, sce_inst, &sce_vm_context->vm_error_status);
			break;
		case E_SCE_BINARY_INST_LOAD_REGISTER_STACK__:
			sce_load_register_to_stack(sce_vm_context->svrs_.svr, &sce_vm_context->now_vm_symbol_->sce_stack_, sce_inst, &sce_vm_context->vm_error_status);
			break;
		case E_SCE_BINARY_INST_ADD_STACK__:
			add_stack_point_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_SUB_STACK__:
			sub_stack_point_sce_vm_context(sce_vm_context, sce_inst);

			break;

		case E_SCE_BINARY_INST_TEST__:
			test_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_EQ__:
			eq_sce_vm_context(sce_vm_context, sce_inst);
			break;

		case E_SCE_BINARY_INST_LSEQ__:
			leq_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_LS__:
			ls_sce_vm_context(sce_vm_context, sce_inst);
			break;

		case E_SCE_BINARY_INST_GTEQ__:
			geq_sce_vm_context(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_GT__:
			gt_sce_vm_context(sce_vm_context, sce_inst);
			break;

		case E_SCE_BINARY_INST_JMP__: {
			int i;
			i = set_new_label_vm_context(sce_vm_context, &sce_inst);
			if (i == -2) {
				GET_ERROR_STRING(
					sce_vm_context->vm_error_status.error_str,
					"%s Lable not defined.",
					sce_inst->scope2
				);
			}
			else if (i == -3) {
				GET_ERROR_STRING(
					sce_vm_context->vm_error_status.error_str,
					"An invalid value is being used."
				);
			}
			else if (i == -1) {
				GET_ERROR_STRING(
					sce_vm_context->vm_error_status.error_str,
					"Memory alloc failed."
				);
			}
			else if (i == 1) {
				continue;
			}
			else if (i == 2) {
				return;
			}
			else if (i == 3) break;
			else assert(false);
		}
									break;
		case E_SCE_BINARY_INST_DELETE_REGISTER__:
			delete_sce_run_virtual_memory(&sce_vm_context->svrs_.svr[sce_inst->ope1]);
			break;
		case E_SCE_BINARY_INST_SYSTEM_CALL__:

			system_call_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_SAVE_REGISTER__:
			//printf("%lu\n", sce_vm_context->svrs_.svr[1].sce_run_value.value_type);

			sce_save_register_to_tail_register(sce_vm_context, sce_inst);

			break;
		case E_SCE_BINARY_INST_LOAD_REGISTER__:

			sce_load_register_to_tail_register(sce_vm_context, sce_inst);

			break;
		case E_SCE_BINARY_INST_RALLOC__:

			ralloc_sce_vm_context_local(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_ALLOC__:
			alloc_sce_vm_context_local(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_MOV__:
			mov_sce_vm(sce_vm_context, sce_inst);

			break;
		case E_SCE_BINARY_INST_MMOV__:
			mmov_sce_vm(sce_vm_context, sce_inst);

			break;

		case E_SCE_BINARY_INST_RRMOV__:
			rrmov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_RMOV__:
			rmov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_BMOV__:
			bmov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_CMOV__:
			cmov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_IMOV__:
			imov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_FMOV__:
			fmov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_SMOV__:
			smov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_OMOV__:
			omov_sce_vm(sce_vm_context, sce_inst);
			break;
		case E_SCE_BINARY_INST_AMOV__:

			amov_sce_vm(sce_vm_context, sce_inst);
		default:
			break;
		}
		if (sce_vm_context->vm_error_status.error_str) {
			runtime_serious_error_process(sce_vm_context, sce_inst);
			break;
		}
		sce_inst = sce_inst->next;
	}
}
static Sce_Binary_Machine_Instructions* set_start_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_VM_Symbol_Table* module_) {


	if (!module_->label_table_) return NULL;
	if (module_->label_context) return module_->label_context->lebel_adress_;

	Sce_VM_Label_Table* label_ = search_label_name(module_->label_table_, start_label);
	if (!label_) return NULL;
	Sce_VM_Label_Table_Context* table_ctx = smart_malloc(Sce_VM_Label_Table_Context, 1);
	if (!table_ctx) return NULL;
	table_ctx->next_ = module_->label_context;
	table_ctx->obj_table_ = NULL;
	table_ctx->lebel_adress_ = label_->lebel_adress_;
	table_ctx->stack_point = 0;
	module_->label_context = table_ctx;
	sce_vm_context->now_vm_symbol_ = module_;
	return label_->lebel_adress_->next;
}

static void global_start_sce_vm_context(Sce_Run_VM_Context* sce_vm_context) {
	Sce_VM_Symbol_Table* global_module_ = get_global_module_symbol_table(*sce_vm_context);
	sce_vm_context->now_vm_symbol_ = global_module_;
	sce_vm_context->now_vm_symbol_->label_context = &sce_vm_context->now_vm_symbol_->global_filed;
	run_sce_vm_context_basic(sce_vm_context, global_module_->global_filed.lebel_adress_);
	sce_vm_context->now_vm_symbol_->global_filed.lebel_adress_ = NULL;
	sce_vm_context->now_vm_symbol_->label_context = NULL;
}
Sce_Run_VM_Context start_sce_vm_context(Sce_Binary_Machine_Instructions* sce_vm_inst) {
	Sce_Run_VM_Context sce_vm_context = gen_sce_vm_context();
	sce_vm_context.inst_ = sce_vm_inst;
	run_collect_label_sce_vm_context(&sce_vm_context);
	global_start_sce_vm_context(&sce_vm_context);
	set_global_built_in_function(&sce_vm_context);
	return sce_vm_context;
}
void run_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, const uint8_t* name_) {
	reflesh_ctx_general_memory(*sce_vm_context);
	Sce_VM_Symbol_Table* module_ = get_module_symbol_table(sce_vm_context, name_);
	if (!module_) return;
	Sce_Binary_Machine_Instructions* sce_name_init_inst = module_->global_filed.lebel_adress_;

	if (sce_name_init_inst) {
		sce_vm_context->now_vm_symbol_ = module_;
		sce_vm_context->now_vm_symbol_->label_context = &module_->global_filed;

		run_sce_vm_context_basic(sce_vm_context, sce_name_init_inst);
		sce_vm_context->now_vm_symbol_->global_filed.lebel_adress_ = NULL;
		sce_vm_context->now_vm_symbol_->label_context = NULL;
	}

	Sce_Binary_Machine_Instructions* sce_inst = set_start_vm_context(sce_vm_context, module_);

	run_sce_vm_context_basic(sce_vm_context, sce_inst);

}
void run_sce_vm_global_context(Sce_Run_VM_Context* sce_vm_context) {
	sce_vm_context->root_symbol.module_name_;
}

void update_sce_vm_context(Sce_Run_VM_Context* sce_vm_inst, const uint8_t* name_) {

	run_sce_vm_context(sce_vm_inst, name_);

}
/*
* CREATE VM CONETEXT
* -->
* NAME ENTRY
* 
* 
* --->
* UPDATE
* START
* 
* UPDATE
* MAIN
* UPDATE
* END
*
*/
static int sce_putchar_call(FILE * file) {
	int i = fgetc(file);
	if (i == '\n') return 0;
	if (i == EOF) return 0;
	return i;
}
static void sce_read_system_call(Sce_Run_VM_Context* vm_context, Sce_Virutal_Register * svr1, FILE * in_fp) {
	svr1->is_const_value = NOT_IS_CONST_MEMORY;
	svr1->sce_run_value.value_type = E_Sce_Run_String_Value;
	svr1->sce_run_value.ssvalue = smart_malloc(U8_String_Buffers, 1);
	if (!svr1->sce_run_value.ssvalue) return;
	init_u8_string_buffers(svr1->sce_run_value.ssvalue, 20);
	int c_;
	while (c_ = sce_putchar_call(in_fp)) {
		append_u8_byte_u8_string_buffer(svr1->sce_run_value.ssvalue, c_);
	}
}
/*
void delete_sce_registers(Sce_Run_VM_Context* vm_context, uint32_t size_) {
	for (uint32_t i = 0; i < size_; ++i) {
		Sce_Virutal_Register* c_svr = &vm_context->svrs_.svr[i];
		delete_sce_run_virtual_memory(c_svr);
	}
}
*/
void system_call_sce_vm(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* system_call_idx =
		get_svr(vm_context, 1);
	if (is_svr_value_type(*system_call_idx, E_Sce_Run_Int_Value)) {
		if (get_svr_ivalue(*system_call_idx) == SYSTEM_CALL_WRITE) {
			if (sce_inst->ope1 != system_call_write_args__) {
				get_vm_error_status(
					*vm_context,
					"System call 'write' expected %lu arguments, but got %lu.",
					system_call_write_args__,
					sce_inst->ope1
				);
				delete_sce_vm_registers(vm_context, sce_inst->ope1);
				return;
			}
			Sce_Virutal_Register* args1 =
				get_svr(vm_context, 2);
			Sce_Virutal_Register* args2 =
				get_svr(vm_context, 3);

			uint32_t size_ = 0;
			bool is_ref = false;
			int check_ = 0;
			FILE * arg_file_obj = get_svr_file_object(args1, &check_);
			if (check_ < 0) {
				get_vm_error_status(
					*vm_context,
					"File Ptr %s is invalid object type.",
					run_value_type_str[args2->sce_run_value.value_type]
				);
				return;
			}
			else {
				if (!arg_file_obj) {
					get_vm_error_status(
						*vm_context,
						"FILE Object is NULL Ptr Object"
					);
					return;

				}
			}
			uint8_t* arg_buf = get_svr_value_to_str(&is_ref, &size_, args2);
			uint64_t res = 0;
			if (!arg_buf) {
				get_vm_error_status(
					*vm_context,
					"Out value %s is invalid object type.",
					run_value_type_str[args2->sce_run_value.value_type]
				);

			}
			else res = SYSTEM_CALL_WRITE__(arg_file_obj, arg_buf, size_);
			if (!is_ref) {
				s_free(arg_buf);
			}
			delete_sce_vm_registers(vm_context, system_call_write_args__);
			access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).is_const_value = IS_CONST_MEMORY;
			access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.value_type = E_Sce_Run_Int_Value;
			access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN).sce_run_value.ivalue = res;
		}
		if (get_svr_ivalue(*system_call_idx) == SYSTEM_CALL_READ) {
			if (sce_inst->ope1 != system_call_read_args__) {
				get_vm_error_status(
					*vm_context,
					"System call 'read' expected %lu arguments, but got %lu.",
					system_call_read_args__,
					sce_inst->ope1
				);
				delete_sce_vm_registers(vm_context, sce_inst->ope1);
				return;
			}
			Sce_Virutal_Register* args1 =
				get_svr(vm_context, 2);

			int check_ = 0;
			FILE* arg_file_obj = get_svr_file_object(args1, &check_);
			if (check_ < 0) {
				get_vm_error_status(
					*vm_context,
					"File Ptr %s is invalid object type.",
					run_value_type_str[args1->sce_run_value.value_type]
				);
				return;
			}
			else {
				if (!arg_file_obj) {
					get_vm_error_status(
						*vm_context,
						"FILE Object is NULL Ptr Object"
					);
					return;

				}
			}

			sce_read_system_call(vm_context, &access_vm_ctx_svr(*vm_context, SCE_VIRTUAL_REGISTER_RETURN), arg_file_obj);
			delete_sce_vm_registers(vm_context, system_call_read_args__);

		}
		if (get_svr_ivalue(*system_call_idx) == SYSTEM_CALL_OPEN) {

			if (sce_inst->ope1 != system_call_open_args__) {
				get_vm_error_status(
					*vm_context,
					"System call 'open' expected %lu arguments, but got %lu.",
					system_call_open_args__,
					sce_inst->ope1
				);
				delete_sce_vm_registers(vm_context, sce_inst->ope1);
				return;
			}

			Sce_Virutal_Register* args1 =
				get_svr(vm_context, 2);
			uint32_t size_ = 0;
			int open_check = 0;
			FILE* fp = sce_open_system_call(args1, &open_check, false);
			switch (open_check){
			case -1:

				get_vm_error_status(
					*vm_context,
					"No such %"PRIu64" file Value or directory",
					args1->sce_run_value.ivalue
				);
				break;
			case -2:

				get_vm_error_status(
					*vm_context,
					"Invalid argument type."
				);
				break;
			case 1:
				set_vm_context_svr_file(vm_context, IS_CONST_MEMORY, fp);
				break;
			case 2:
				set_vm_context_svr_file(vm_context, NOT_IS_CONST_MEMORY, fp);
				break;
			default:
				break;
			}
			delete_sce_vm_registers(vm_context, system_call_open_args__);

		}
	}
	else {
		get_vm_error_status(
			*vm_context,
			"Invalid object type."
		);
	}
}

static Sce_VM_Object_Table * alloc_sce_vm_object_symbol(uint8_t* name_, Sce_VM_Object_Table* obj_table, Sce_Virtual_Memory *virtual_mem) {
	Sce_VM_Object_Table* new_ = smart_malloc(Sce_VM_Object_Table, 1);
	if (!new_) return obj_table;
	/*REFERENCE COPY*/
	new_->obj_name_ = name_;
	new_->svm = *virtual_mem;
	//new_->svm.is_const_value = NOT_IS_CONST_MEMORY;
	//new_->svm.sce_run_value.value_type = E_Sce_Run_Null_Value;
	new_->next_ = NULL;
	new_->next_ = obj_table;
	return new_;
}

static void alloc_sce_vm_object_symbol_main(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	//sce_vm_context->vm_symbol_->global_filed.obj_table_ = 
	//	alloc_sce_vm_object_symbol(sce_inst->scope2, sce_vm_context->now_vm_symbol_->global_filed.obj_table_, );
}

static void alloc_sce_vm_object_symbol_local(Sce_VM_Label_Table_Context * sce_context, Sce_Binary_Machine_Instructions* sce_inst, Sce_Virtual_Memory* virtual_mem) {
	sce_context->obj_table_ = alloc_sce_vm_object_symbol(sce_inst->scope2, sce_context->obj_table_, virtual_mem);
}

void ralloc_sce_vm_context_local(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	Sce_Virutal_Register* svr =
		get_svr(sce_vm_context, sce_inst->ope1);
	Sce_Virtual_Memory mem_;

	if (is_svr_value_type(*svr, E_Sce_Run_Mem_Value)) {
		mem_.is_const_value = IS_CONST_MEMORY;
		mem_.sce_run_value = svr->sce_run_value.mvalue->sce_run_value;
	}
	else {
		mem_ = *svr;
	}
	alloc_sce_vm_object_symbol_local(sce_vm_context->now_vm_symbol_->label_context, sce_inst, &mem_);
}
void alloc_sce_vm_context_local(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	
	Sce_Virtual_Memory mem_ = { .is_const_value = NOT_IS_CONST_MEMORY, .sce_run_value.vvalue = 0, .sce_run_value.value_type = E_Sce_Run_Virtual_Value };
	alloc_sce_vm_object_symbol_local(sce_vm_context->now_vm_symbol_->label_context, sce_inst, &mem_);
		/*
		return;
	}

	get_vm_error_status(
		*sce_vm_context,
		"error : Variable %s has been redefined.",
		obj_table_local_->obj_name_
	);
	*/
}
void alloc_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst) {
	uint8_t* symbol_ = GET_VM_CONTEXT_INST_NAME(*sce_inst);
	Sce_VM_Object_Table* obj_table_root_ = search_object_symbol(
		sce_vm_context->root_symbol.global_filed.obj_table_, sce_inst->scope2);

	Sce_VM_Object_Table* obj_table_ = search_object_symbol(
		sce_vm_context->now_vm_symbol_->global_filed.obj_table_, sce_inst->scope2);
	if (obj_table_) {

		get_vm_error_status(
			*sce_vm_context,
			"error : Variable %s has been redefined.",
			obj_table_->obj_name_
		);
		return;
	}
	alloc_sce_vm_object_symbol_main(sce_vm_context, sce_inst);
}


void set_lable_name_sce_vm(Sce_Run_VM_Context* sce_vm_context) {
	Sce_Built_In_Label* built_in = search_built_in_label_name(
		&sce_vm_context->vm_symbol_->built_in_table, 
		sce_vm_context->inst_->scope2
	);
	if (built_in) goto Sce_Error_Label;
	Sce_VM_Label_Table* vm_label = search_label_name(
		sce_vm_context->vm_symbol_->label_table_, sce_vm_context->inst_->scope2
	);

	if (!vm_label) {
		Sce_VM_Label_Table* vm_label = smart_malloc(Sce_VM_Label_Table, 1);
		if (!vm_label) return;
		vm_label->lebel_adress_ = sce_vm_context->inst_;
		vm_label->next_ = sce_vm_context->vm_symbol_->label_table_;
		sce_vm_context->vm_symbol_->label_table_ = vm_label;

	}
	else {
		Sce_Error_Label:
		get_vm_error_status(
			*sce_vm_context,
			"Label %s redefined.",
			sce_vm_context->inst_->scope2
		);
	}
}
void set_module_name_sce_vm(Sce_Run_VM_Context* sce_vm_context) {

	Sce_VM_Symbol_Table* vm_module = search_module_name(
		sce_vm_context->vm_symbol_, sce_vm_context->inst_->scope2
		);
	if (!vm_module) {
		Sce_VM_Symbol_Table* new_vm_module = smart_calloc(
			Sce_VM_Symbol_Table,
			1
		);

		if (!new_vm_module)return;
		new_vm_module->module_name_ = sce_vm_context->inst_->scope2;

		new_vm_module->next_ = sce_vm_context->vm_symbol_;
		sce_vm_context->vm_symbol_ = new_vm_module;
	}
	else {
		get_vm_error_status(
			*sce_vm_context,
			"Module %s redefined.",
			sce_vm_context->inst_->scope2
		);

	}
}
