#ifndef _SCE_RUN_H_
#define _SCE_RUN_H_
#include "sce_ir.h"
#include "sce_run_vm.h"
/**
* Memory Management Design
* 
* Store all reference information in a dedicated memory region within the VM.
* When an error occurs, inspect the VM's registers, stack, and variable memory to determine which resources should be released.
* If an address is referenced only by a register, release the register and deallocate the associated memory, since no other references exist.
* If an address is referenced by both a variable and a register, release only the variable. Keep the register and the associated memory allocated because the register still holds a valid reference.
*
*/
#ifdef GAME_VECTOR_LIB
/**
* NOT IMPLEMENT
*/
#include "game_vector_lib.h"
#endif
#define auto_struct(Name, Implemenet) typedef struct Name{Implemenet}Name
#define GET_VM_CONTEXT_INST_NAME(inst_) (inst_).scope2

sce_si_strap Sce_Virutal_Register * get_svr(Sce_Run_VM_Context* sce_vm_context, uint32_t idx) {
	if (idx >= SCE_VIRTUAL_REGISTER_MAX_) {
		idx -= SCE_VIRTUAL_REGISTER_MAX_ + sce_vm_context->now_vm_symbol_->sce_stack_.stack_point_;
		return 
		sce_vm_context->now_vm_symbol_->sce_stack_.stack_mem_ + idx;
	}
	return &sce_vm_context->svrs_.svr[idx];
}
sce_si_strap void delete_sce_run_obj_context(Sce_VM_Object_Table* obj_table) {
	while (obj_table) {

		Sce_VM_Object_Table* obj_ = obj_table;

		//s_free(obj_->obj_name_);
		delete_sce_run_virtual_memory(&obj_->svm);
		obj_table = obj_table->next_;
		s_free(obj_);
	}
}
sce_si_strap void delete_sce_run_label_table(Sce_VM_Label_Table* ctx) {
	while (ctx) {
		Sce_VM_Label_Table* ctx_ = ctx;
		ctx = ctx->next_;
		s_free(ctx_);

	}
}
sce_si_strap void delete_sce_run_label_context(Sce_VM_Label_Table_Context* ctx) {
	while (ctx) {

		ctx->lebel_adress_ = NULL;
		Sce_VM_Label_Table_Context* ctx_ = ctx;
		delete_sce_run_obj_context(ctx_->obj_table_);
		ctx = ctx->next_;
		s_free(ctx_);

	}
}

sce_si_strap void delete_sce_run_global_filed_label_context(Sce_VM_Label_Table_Context* ctx) {
	while (ctx) {
		ctx->lebel_adress_ = NULL;
		Sce_VM_Label_Table_Context* ctx_ = ctx;

		delete_sce_run_obj_context(ctx_->obj_table_);

		ctx = ctx->next_;

	}
}
sce_si_strap void delete_sce_run_symbol_table(Sce_VM_Symbol_Table* ctx) {
	while (ctx) {
		s_free(ctx->module_name_);
		delete_sce_run_global_filed_label_context(&ctx->global_filed);
		delete_sce_run_label_context(ctx->label_context);
		delete_sce_run_label_table(ctx->label_table_);

		Sce_VM_Symbol_Table* ctx_ = ctx;
		ctx = ctx->next_;
		s_free(ctx_);
	}
}
sce_si_strap void delete_sce_run_root_symbol_table(Sce_VM_Symbol_Table* ctx) {
	while (ctx) {
		s_free(ctx->module_name_);
		delete_sce_run_global_filed_label_context(&ctx->global_filed);
		delete_sce_run_label_context(ctx->label_context);
		delete_sce_run_label_table(ctx->label_table_);
		ctx = ctx->next_;
	}
}
sce_si_strap void delete_sce_run_vm_context(Sce_Run_VM_Context*ctx) {
	//delete_sce_run_root_symbol_table(&ctx->root_symbol);
	delete_sce_run_symbol_table(ctx->vm_symbol_);
	s_free(ctx->vm_error_status.error_str);
	s_free(ctx->vm_error_status.error_func);
	memset(ctx, 0, sizeof(Sce_Run_VM_Context));
}
//Sce_Binary_Machine_Instructions* set_start_vm_context(Sce_Run_VM_Context* sce_vm_context, const uint8_t* name_);
Sce_Run_VM_Context start_sce_vm_context(Sce_Binary_Machine_Instructions* sce_vm_inst);
void run_collect_label_sce_vm_context(Sce_Run_VM_Context* sce_vm_context);
void run_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, const uint8_t* name_);

void system_call_sce_vm(Sce_Run_VM_Context* vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void alloc_sce_vm_context_local(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void alloc_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void add_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void smov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void bmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void cmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void fmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void imov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void rmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void mmov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void mov_sce_vm(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* sce_inst);
void set_lable_name_sce_vm(Sce_Run_VM_Context* sce_vm_context);
void set_module_name_sce_vm(Sce_Run_VM_Context* sce_vm_context);

#endif // !_SCE_RUN_H_
