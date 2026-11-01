
#ifndef LOAD_FILE_H_
#define LOAD_FILE_H_
#include "ext_alloc.h"
static uint8_t* load_files(const uint8_t* paths) {

	if (!paths) return NULL;
	FILE* f__ = fopen((const char*)paths, "rb");
	if (!f__) return NULL;
	if (fseek(f__, 0, SEEK_END) != 0) {
		fclose(f__);
		return NULL;
	}
	long file_size = ftell(f__);
	if (file_size < 0 || fseek(f__, 0, SEEK_SET) != 0) {
		fclose(f__);
		return NULL;
	}
	size_t size_ = (size_t)file_size;
	uint8_t* buf = smart_malloc(uint8_t, size_ + 1);
	if (!buf) {
		fclose(f__);
		return NULL;
	}
	if (fread(buf, sizeof(uint8_t), size_, f__) != size_) {
		s_free(buf);
		fclose(f__);
		return NULL;
	}
	buf[size_] = '\0';
	fclose(f__);
	if (size_ >= 3 && buf[0] == 0xef && buf[1] == 0xbb && buf[2] == 0xbf)
		memmove(buf, buf + 3, size_ - 2);
	return buf;
}
#endif