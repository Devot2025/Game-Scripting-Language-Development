#ifndef SCE_CLASS_DEFINE_H_
#define SCE_CLASS_DEFINE_H_

#include "sce_ir_helper.h"
#include "transfer_test_functions.h"

struct Sce_Virtual_Memory;

typedef struct Sce_Built_In_Label_Table {
	struct Sce_Built_In_Label* label_;
	uint32_t size_;
}Sce_Built_In_Label_Table;

typedef Sce_Built_In_Label_Table Sce_Class_Define;
	Sce_Built_In_Label_Table label_;
typedef struct Sce_User_Class_Define {
	struct Sce_VM_Object_Table* decl_object_mapped;
	struct Sce_VM_Label_Table*  decl_label_mapped;
	struct Sce_Class_Define* decl_class_mapped;
	Sce_Class_Define basic_class_define;
}Sce_User_Class_Define;

typedef Sce_Class_Define Sce_Built_In_Class_Define;

#endif SCE_CLASS_DEFINE_H_