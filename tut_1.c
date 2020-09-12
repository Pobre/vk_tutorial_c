#include "vk_tutorial.h"

int main(int argc, char *argv[]) {
	struct base_info info = {};
	init_global_layer_properties(&info);

	/* VULKAN_KEY_START */

	// initialize the VkApplicationInfo structure
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "Not Engine name";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Not App Name";
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure
	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = NULL;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = NULL;

	VkResult res;

	res = vkCreateInstance(&inst_info, NULL, &info.vk_instance);
	if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("Nao ha Vulkan ICD compativel\n");
		exit(-1);
	} else if (res) {
		printf("Erro desconhecido: %d\n", res);
		exit(-1);
	}

	clean_up(&info);

	return 0;
}
