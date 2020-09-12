#include "vk_tutorial.h"

int main(int argc, char *argv[]) {
	struct base_info info = {};
	VkResult vk_res;

	//info.instance_layer_names = NULL;
	//info.instance_extension_names = NULL;
	//info.instance_layer_properties = NULL;
	//info.instance_extension_properties = NULL;

	init_global_layer_properties(&info);
	vk_res = init_instance(&info);

	if (vk_res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("Nao ha Vulkan ICD compativel\n");
		exit(-1);
	} else if (vk_res) {
		printf("Erro desconhecido: %d\n", vk_res);
		exit(-1);
	}

	// checar GPUs
	uint32_t gpu_count = 1;
	vk_res = vkEnumeratePhysicalDevices(info.vk_instance, &gpu_count, NULL);
	assert(gpu_count);
	info.gpus = realloc(info.gpus, gpu_count * sizeof(VkPhysicalDevice*));
	vk_res = vkEnumeratePhysicalDevices(info.vk_instance, &gpu_count, info.gpus);
	assert(!vk_res && gpu_count >= 1);

	clean_up(&info);

	return 0;
}
