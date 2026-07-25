#ifndef SCE_RUN_VM_H_
#define SCE_RUN_VM_H_
#include "sce_ir_helper.h"
#include "transfer_test_functions.h"
typedef struct Sce_Built_In_Label_Table {
	struct Sce_Built_In_Label* label_;
	uint32_t size_;
}Sce_Built_In_Label_Table;
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
	E_Sce_Run_Function_Value,
	E_Sce_Run_Built_In_Function_Value,
}Sce_Run_Value_Type;
typedef enum Sce_Run_Ext_Value_Type {
	E_Sce_Run_Ext_Null_Value,
	E_Sce_Run_Ext_FILE_Value,
}Sce_Run_Ext_Value_Type;
static const uint8_t* run_value_type_str[] =
{ "null", "virtual", "bool", "char", "int", "float", "double", "string", "obj", "mem", "func", "bulitin_func" };

//typedef void *(sce_free_func_t)(void*);
//typedef sce_free_func_t* sce_free_func_ptr_t;
typedef struct Sce_Ext_Values {
	void* ptr__;
	Sce_Run_Ext_Value_Type ext_type;
	//sce_free_func_ptr_t sce_free;
}Sce_Ext_Values;
typedef struct Sce_Run_Values {
	Sce_Run_Value_Type value_type;
	union {
		void* vvalue;
		int64_t ivalue;/*uint64_t*/
		U8_String_Buffers* ssvalue;
		Sce_Ext_Values* evalue;
		uint8_t cvalue;
		float fvalue;
		double dvalue;
		bool bvalue;
		struct Sce_Virtual_Memory* mvalue;
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

typedef Sce_Virtual_Memory Sce_Virutal_Register;

typedef struct Sce_Virutal_Registers {
	Sce_Virutal_Register svr[SCE_VIRTUAL_REGISTER_MAX + 2];
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

sce_si_strap void delete_sce_run_extends_object(Sce_Ext_Values* ext_value) {
	if (!ext_value)return;
	switch (ext_value->ext_type) {
	case E_Sce_Run_Ext_FILE_Value:
		fclose((FILE*)ext_value->ptr__);
	case E_Sce_Run_Ext_Null_Value:
	default:
		break;
	}
}
sce_si_strap void delete_sce_run_virtual_memory(Sce_Virtual_Memory* mem) {
	if (mem->is_const_value) return;
	else {
		switch (mem->sce_run_value.value_type) {
		case E_Sce_Run_Mem_Value:
			//delete_sce_run_virtual_memory(mem->sce_run_value.mvalue);
			break;
		case E_Sce_Run_Obj_Value:
			delete_sce_run_extends_object(mem->sce_run_value.evalue);
			s_free(mem->sce_run_value.evalue);
			break;
		case E_Sce_Run_String_Value:
			delete_u8_string_buffers(mem->sce_run_value.ssvalue);
			s_free(mem->sce_run_value.ssvalue);
			break;

		default:
			break;
		}
		mem->is_const_value = IS_CONST_MEMORY;
		memset(&mem->sce_run_value, 0, sizeof(Sce_Run_Values));
	}
}
#endif // !SCE_RUN_VM_H_
