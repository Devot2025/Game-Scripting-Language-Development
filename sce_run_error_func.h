#ifndef _SCE_RUN_ERROR_FUNC_H_
#define _SCE_RUN_ERROR_FUNC_H_


#define GET_ERROR_STRING(dst, format, ...)\
	do{\
		uint32_t req = snprintf(NULL, 0, "error : "format, ##__VA_ARGS__);\
		req += 1;\
		dst =\
		smart_malloc(uint8_t, req);\
		if (!dst) break;\
		(void)snprintf(dst, req, "error : "format, ##__VA_ARGS__); \
	} while (0)

#define get_vm_error_status(dst, format, ...)\
	do{\
		GET_ERROR_STRING((dst).vm_error_status.error_str, format, ##__VA_ARGS__);\
		if(!(dst).vm_error_status.error_str) (dst).vm_error_status.error_vm_fallback_code = SCE_VM_FALLBACK_CODE;\
	}while(0)


#endif // !_SCE_RUN_ERROR_FUNC_H_
