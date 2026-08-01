#ifndef SCE_CMP_OPERATOR_H_
#define SCE_CMP_OPERATOR_H_
#include "sce_run_vm_helper.h"

void eq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_);
void leq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_);
void ls_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_);
void geq_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_);
void gt_sce_vm_context(Sce_Run_VM_Context* sce_vm_context, Sce_Binary_Machine_Instructions* inst_);
#endif // !SCE_CMP_OPERATOR_H_
