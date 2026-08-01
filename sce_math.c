#include "sce_math.h"
#define sce_sqrt_argc_num 1
#define sce_argc_error(func_name, req_argc__)\
do{\
if (req_argc__ != argc) {\
GET_ERROR_STRING(\
	dst_->error_str,\
	"%s",\
	sce_function_argc_error,\
	"SS",\
	argc,\
	req_argc__\
);\
delete_sce_registers(svrs, argc);\
return;\
}\
} while (0)

sce_built_in_basic_define(sqrt) {
	sce_argc_error(
		sqrt, 1
	);
	Sce_Virutal_Register* tmp_svr1 = svrs;
	Sce_Virutal_Register* svrr = svrs + SCE_VIRTUAL_REGISTER_RETURN;
	Sce_Virutal_Register* svr1 = sce_resolve_register_type(tmp_svr1);
	svrr->sce_run_value.value_type = E_Sce_Run_Double_Value;
	switch (svr1->sce_run_value.value_type){
		sce_type_switch_filed_statement(
			GET_ERROR_STRING(
				dst_->error_str,
				"Null type is Invalid."
			); break;,
			access_svr_dvalue(*svrr) = 0; break; ,
			access_svr_dvalue(*svrr) = sqrt((double)access_svr_bvalue(*svr1)); break; ,
			access_svr_dvalue(*svrr) = sqrt((double)access_svr_cvalue(*svr1)); break; ,
			access_svr_dvalue(*svrr) = sqrt((double)access_svr_ivalue(*svr1)); break; ,
			access_svr_dvalue(*svrr) = sqrt((double)access_svr_fvalue(*svr1)); break; ,
			access_svr_dvalue(*svrr) = sqrt(access_svr_dvalue(*svr1)); break; ,
			,,,
			GET_ERROR_STRING(
				dst_->error_str,
				"%s type is Invalid.",
				run_value_type_str[svr1->sce_run_value.value_type]
			); break;
	)
	default:
		assert(false);
	}
	delete_sce_run_virtual_memory(tmp_svr1);

}