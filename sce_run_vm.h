#ifndef SCE_RUN_VM_H_
#define SCE_RUN_VM_H_
#include "sce_class_define.h"

typedef struct Sce_Virtual_Memory Sce_Virutal_Register;
typedef void sce_built_in_func_api_(uint32_t argc, Sce_Virutal_Register* svrs, struct Sce_Run_VM_Error_Status* dst_);
typedef sce_built_in_func_api_* sce_built_in_func_api;

typedef enum Sce_Run_Value_Type {
	E_Sce_Run_Null_Value,
	E_Sce_Run_Virtual_Value,
	E_Sce_Run_Bool_Value,
	E_Sce_Run_Char_Value,
	E_Sce_Run_Int_Value,
	E_Sce_Run_Float_Value,
	E_Sce_Run_Double_Value,
	E_Sce_Run_String_Value,
	E_Sce_Run_Obj_Value,
	E_Sce_Run_Mem_Value,
	E_Sce_Run_Array_Value,
	E_Sce_Run_Function_Value,
	E_Sce_Run_Built_In_Function_Value,
}Sce_Run_Value_Type;
typedef enum Sce_Run_Ext_Value_Type {
	E_Sce_Run_Ext_Null_Value,
	E_Sce_Run_Ext_FILE_Value,
}Sce_Run_Ext_Value_Type;
static const uint8_t* run_value_type_str[] =
{ "null", "virtual", "bool", "char", "int", "float", "double", "string", "obj", "mem", "array","func", "bulitin_func" };

//typedef void *(sce_free_func_t)(void*);
//typedef sce_free_func_t* sce_free_func_ptr_t;

/**
* class String{
*	obj String(){
* 
*	}
*	obj upper(){
*		
*	}
* }
* NAME A{
* 
* void func(){
* 
* 
* }
* 
* }
*/
typedef struct Sce_Ext_Values {
	void* ptr__;
	Sce_Run_Ext_Value_Type ext_type;
}Sce_Ext_Values;
typedef struct Sce_Run_Values {
	Sce_Run_Value_Type value_type;
	union {
		void* vvalue;
		int64_t ivalue;/*uint64_t*/
		U8_String_Buffers* ssvalue;
		struct Sce_List_Array* lavalue;

		Sce_Ext_Values* evalue;
		uint8_t cvalue;
		float fvalue;
		double dvalue;
		bool bvalue;
		struct Sce_Virtual_Memory* mvalue;
		sce_built_in_func_api blvalue;
		Sce_Binary_Machine_Instructions* lvalue;
	};
}Sce_Run_Values;

#define SCE_VM_FALLBACK_CODE 1
#define IS_CONST_MEMORY true
#define NOT_IS_CONST_MEMORY false
typedef struct Sce_Virtual_Memory {
	bool is_const_value;
	Sce_Run_Values sce_run_value;
}Sce_Virtual_Memory;

typedef struct Sce_Run_Objects {
	Sce_Virtual_Memory* sce_vmem;
}Sce_Run_Objects;


typedef struct Sce_Virutal_Registers {
	Sce_Virutal_Register svr[SCE_VIRTUAL_REGISTER_MAX_];
}Sce_Virutal_Registers;

typedef struct Sce_Virtual_Stack {
	struct Sce_Virtual_Stack* next;
	Sce_Virtual_Memory svm;
}Sce_Virtual_Stack;
typedef struct Sce_VM_Label_Table {
	struct Sce_VM_Label_Table* next_;
	Sce_Binary_Machine_Instructions* lebel_adress_;
}Sce_VM_Label_Table;
struct Sce_VM_Object_Table;
typedef struct Sce_Run_VM_Tables {
	Sce_VM_Label_Table* label_table_;
	struct Sce_VM_Object_Table* obj_table_;
	Sce_VM_Label_Table* init_label_;
}Sce_Run_VM_Tables;
typedef struct Sce_VM_Label_Table_Context {
	//Sce_Run_VM_Tables symbol_table;
	/*next label*/
	uint32_t stack_point;
	struct Sce_VM_Object_Table* obj_table_;
	struct Sce_VM_Label_Table_Context* next_;
	Sce_Binary_Machine_Instructions* lebel_adress_;
}Sce_VM_Label_Table_Context;
typedef struct Sce_VM_Object_Table {
	struct Sce_VM_Object_Table* next_;
	uint8_t* obj_name_;
	Sce_Virtual_Memory svm;
}Sce_VM_Object_Table;

typedef struct Sce_Virtual_Stacks {
	Sce_Virtual_Memory* stack_mem_;
	uint32_t stack_size_;
	uint32_t stack_point_;
}Sce_Virtual_Stacks;
/**
 * Use all registers.
 *
 * Registers:
 * - 127, 128: reserved for return values.
 * - 129, 130, 131: used as general-purpose local registers.
 *
 * Register mapping:
 * 129 - 129 = 0
 * 130 - 129 = 1
 * 131 - 129 = 2
 *
 * A stack is used here.
 *
 * If the current stack base is 129, then registers
 * 129, 130, and 131 can be saved there.
 *
 * When calling a function:
 * The stack base always starts at register 129 because of
 * the bytecode design.
 *
 * Therefore, when entering a function, the function must
 * determine its own stack frame position.
 *
 * Example:
 * If registers 129 and 130 are used,
 * the stack currently contains:
 *
 *     129, 130, 131
 *
 * Move the current frame position to 131, then:
 *
 *     129 -> local 0
 *     130 -> local 1
 *
 * The stack layout becomes:
 *
 *     129, 130, 131, 132, 133
 *
 * ----------------------------------------------------------
 * START
 *
 * SP = 0
 * FC = 0 + 3
 *
 * 129 - 129 = 0 + 0
 * 130 - 129 = 1 + 0
 * 131 - 129 = 2 + 0
 *
 * call 0x0003
 *
 * SP = 3
 * FC = 3 + 4
 *
 * 129 - 129 = 0 + 3
 * 130 - 129 = 1 + 3
 * 131 - 129 = 2 + 3
 * 132 - 129 = 3 + 3
 *
 * call 0x0004
 *
 * SP = 7
 * FC = 7 + 5
 *
 * FC = 12 - 5 = 7
 * SP = 3
 *
 * FC = 7 - 4
 * SP = 3 - 3
 *
 * ----------------------------------------------------------
 *
 * call 0x0003
 *
 * call start
 *
 * SP = 0
 * SP += 3;
 *
 * 129 - 129 = 0 -> SP_MEM[SP - 1 + 0]
 * 130 - 129 = 1 -> SP_MEM[SP - 1 + 1]
 * 131 - 129 = 2 -> SP_MEM[SP - 1 + 2]
 *
 * call test
 *
 * SP += 5
 *
 * call test1
 *
 * SP += 7
 *
 * 129 - 129 = 0 -> SP_MEM[SP - 1 + 0]
 *
 * SP -= 7
 * ret
 *
 * SP -= 5
 * ret
 *
 * SP -= 3
 *
 * ----------------------------------------------------------
 *
 * call start
 *
 * SP = 0
 * FC = 129
 *
 * SP += 3
 *
 * FC:
 *   129 -> +3 -> 132 -> +5 -> 137
 *       -> -5 -> 132 -> -3 -> 129
 *
 * SP:
 *   0 -> +3 -> 3 -> +5 -> 8
 *     -> -5 -> 3 -> -3 -> 0
 *
 * Register mapping:
 *
 * 129 - 129 = 0 => 129 - 129 + 0
 * 130 - 129 = 1 => 129 - 129 + 1
 * 131 - 129 = 2 => 129 - 129 + 2
 *
 * Nested frame:
 *
 * 129 - 129 = 0 => 132 - 129 + 0
 *
 * FC:
 *   3 -> +3 -> 6 -> +5 -> 11
 *     -> -5 -> 6 -> -3 -> 3
 *
 * SP:
 *   0 -> +3 -> 3 -> +5 -> 8
 *     -> -5 -> 3 -> -3 -> 0
 *
 * SP 0
 * FC 3
 * 
 * labe:
 * SP=3
 * FC+=5=8
 * 
 * 
 * FC-=5
 * SP = 3
 * 
 * ----------------------------------------------------------
 *
 * start
 *
 * SP = 0
 * FC += 3
 *
 * test
 *
 * SP = FC
 * FC += 3
 *
 * test1
 *
 * SP = FC
 * FC += 5
 *
 * SP -= 5
 * FC -= 5
 *
 * ret
 *
 * ----------------------------------------------------------
 *
 * Register access:
 *
 * 129 - 129 = 0
 * 130 - 129 = 1
 * 131 - 129 = 2
 *
 * Local variable access:
 *
 * SP_MEM[FC - 129 + 0]
 * SP_MEM[FC - 129 + 1]
 * SP_MEM[FC - 129 + 2]
 *
 * call test1
 *
 * FC = SP + 3
 * SP += 5
 * sp += 3
sp[2 - 0] = 0;
sp[2 - 1] = 1;
sp[2 - 2] = 2;
sp+=4
sp[6 - 0] = 1;
sp[6 - 1] = 2;
sp[6 - 2] = 3;
sp[6 - 3] = 4;
sp -= 4
ret
sp-=3


add sp, 3

100, 101, 102

0    , 1    ,  2

2 - (2 - 2) = 0

2 - (2 - 1) = 1

2 - (2 - 2) = 2
1010Ç≈ 1010, 0 1010, 1 1010, 2 1010, 3 1010, 4 1010, 5
1010-5=1005 1010-0=1010
 */
typedef struct Sce_VM_Symbol_Table {
	Sce_VM_Label_Table_Context global_filed;
	Sce_VM_Label_Table* label_table_;
	Sce_VM_Label_Table_Context* label_context;
	Sce_Built_In_Label_Table built_in_table;
	uint8_t* module_name_;
	uint32_t module_counter_;
	uint32_t module_counter_range_;
	Sce_Virtual_Stacks sce_stack_;
	struct Sce_VM_Symbol_Table* next_;
}Sce_VM_Symbol_Table;

typedef struct Sce_Run_VM_Error_Status {
	uint8_t* error_str;
	uint8_t* error_func;
	uint8_t error_vm_fallback_code;
}Sce_Run_VM_Error_Status;
typedef int ctx_error_t;
typedef struct Sce_Run_VM_Context {
	//Sce_Run_VM_Tables root_symbols;
	Sce_VM_Symbol_Table root_symbol;
	Sce_Virutal_Registers svrs_;

	Sce_VM_Symbol_Table* vm_symbol_;

	Sce_VM_Symbol_Table* now_vm_symbol_;

	Sce_Binary_Machine_Instructions* inst_;
	Sce_Run_VM_Error_Status vm_error_status;
}Sce_Run_VM_Context;
void delete_sce_run_extends_object(Sce_Ext_Values* ext_value);
void delete_sce_run_virtual_memory(Sce_Virtual_Memory* mem);

sce_si_strap Sce_Virutal_Register* get_svr(Sce_Run_VM_Context* sce_vm_context, uint32_t idx) {
	if (idx >= SCE_VIRTUAL_REGISTER_MAX_) {
		/**
		* 129 - 129 = 0 + 3
		* 130 - 129 = 1 + 3
		*
		*/
		idx = (idx - SCE_VIRTUAL_REGISTER_MAX_) + sce_vm_context->now_vm_symbol_->sce_stack_.stack_point_;
		return
			sce_vm_context->now_vm_symbol_->sce_stack_.stack_mem_ + idx;
	}
	return &sce_vm_context->svrs_.svr[idx];
}

#endif // !SCE_RUN_VM_H_
/**
* 1. Reverse Offset Method

The Reverse Offset Method allocates a stack frame by increasing the stack pointer. Since the stack pointer points to the end of the allocated frame, local variables are accessed using a reverse index.

Formula:

index = (frame_size - 1) - local_index
address = sp[index]

Example (frame_size = 3):

local0 Å® sp[(2 - 0)] = sp[2]
local1 Å® sp[(2 - 1)] = sp[1]
local2 Å® sp[(2 - 2)] = sp[0]

The first declared local variable has the highest index, while the last declared local variable has index 0. This method requires neither a frame pointer nor a saved stack pointer.

2. SP Minus Method

The SP Minus Method allocates a stack frame by increasing the stack pointer. The beginning of the current stack frame is calculated by subtracting the frame size from the current stack pointer, and local variables are accessed relative to this base.

Formula:

base = stack_memory + stack_pointer - frame_size
address = base + local_index

Equivalent formula:

address = stack_memory + stack_pointer - frame_size + local_index

Example (frame_size = 3):

local0 Å® stack_memory + stack_pointer - 3
local1 Å® stack_memory + stack_pointer - 2
local2 Å® stack_memory + stack_pointer - 1

For local_index = 1:

address = stack_memory + stack_pointer - 3 + 1
        = stack_memory + stack_pointer - 2

This method keeps the local variables in their natural order and also requires neither a frame pointer nor a saved stack pointer.

3. Frame Save Method

The Frame Save Method saves the current frame state before entering a new function. Typically, the stack pointer (sp) and frame counter (fc) are stored, a new frame is allocated, and local variables are accessed relative to the saved frame counter. When the function returns, the previous frame state is restored.

Example:

push(sp)
push(fc)

fc = sp
sp += frame_size

local0 Å® sp[fc + 0]
local1 Å® sp[fc + 1]
local2 Å® sp[fc + 2]

sp = fc

pop(fc)
pop(sp)

This method naturally supports recursion and nested function calls because each invocation has its own saved frame state. However, it requires additional storage and save/restore operations for every function call.
*/