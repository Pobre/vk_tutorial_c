#include "vk_tutorial.h"

void freep(void *p) {
	if(p != NULL) {
		free(p);
	}
}

void freepp(void **p, uint32_t size) {
	if(p != NULL) {
		for(int i = 0; i < size; i++) {
			free(p[i]);
		}
		free(p);
	}
}
