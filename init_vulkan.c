#include "vk_tutorial.h"

VkResult init_global_extension_properties(struct base_info *base, struct layer_properties *lp) {
	uint32_t instance_extension_count = 0;
	VkResult vk_res;
	char *layer_name = NULL;

	layer_name = lp->properties.layerName;

	do {
		vk_res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, NULL);

		if(vk_res) {
			return vk_res;
		}

		if(instance_extension_count == 0) {
			return VK_SUCCESS;
		}

		lp->instance_extensions = realloc(lp->instance_extensions, instance_extension_count * sizeof(VkExtensionProperties));
		base->instance_extension_properties = lp->instance_extensions;
		vk_res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, base->instance_extension_properties);
	} while(vk_res == VK_INCOMPLETE);

	base->instance_extension_count = instance_extension_count;

	//free(layer_name);

	return vk_res;
}

VkResult init_global_layer_properties(struct base_info *base) {
	uint32_t instance_layer_count = 0;
	VkLayerProperties *vk_layer_properties = NULL;
	VkResult vk_res;

	do {
		vk_res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
		if(vk_res) {
			return vk_res;
		}

		if(instance_layer_count == 0) {
			return VK_SUCCESS;
		}

		vk_layer_properties = realloc(vk_layer_properties, instance_layer_count * sizeof(VkLayerProperties));
		base->instance_layer_properties = realloc(base->instance_layer_properties, instance_layer_count * sizeof(struct layer_properties));
		
		vk_res = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_layer_properties);
	} while(vk_res == VK_INCOMPLETE);

	base->instance_layer_count = instance_layer_count;

	for(uint32_t i = 0; i < instance_layer_count; i++) {
		struct layer_properties lp = { vk_layer_properties[i] };
		vk_res = init_global_extension_properties(base, &lp);
		if(vk_res) {
			return vk_res;
		}
		base->instance_layer_properties[i] = lp;
	}

	free(vk_layer_properties);

	return vk_res;
}

VkResult init_instance(struct base_info *base) {
	VkApplicationInfo vk_app_info = {};
	vk_app_info.pNext = NULL;
	vk_app_info.pApplicationName = "Not Engine name";
	vk_app_info.applicationVersion = 1;
	vk_app_info.pEngineName = "Not App Name";
	vk_app_info.engineVersion = 1;
	vk_app_info.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo vk_inst_info = {};
	vk_inst_info.pNext = NULL;
	vk_inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vk_inst_info.flags = 0;
	vk_inst_info.pApplicationInfo = &vk_app_info;
	vk_inst_info.enabledExtensionCount = base->instance_extension_names != NULL ?
		base->instance_extension_count : 0;
	vk_inst_info.ppEnabledExtensionNames = base->instance_extension_names != NULL ?
		base->instance_extension_names : NULL;
	vk_inst_info.enabledLayerCount = base->instance_layer_names != NULL ?
		base->instance_layer_count : 0;
	vk_inst_info.ppEnabledLayerNames = base->instance_layer_names != NULL ?
		base->instance_layer_names : NULL;

	VkResult vk_res;

	vk_res = vkCreateInstance(&vk_inst_info, NULL, &base->vk_instance);
	if(vk_res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("Nao ha Vulkan ICD compativel\n");
		fflush(stdout);
		exit(1);
	} else if(vk_res) {
		printf("Erro desconhecido: %d\n", vk_res);
		fflush(stdout);
		exit(1);
	}

	return vk_res;
}

VkResult init_device(struct base_info *base) {
	VkResult vk_res;
	VkDeviceQueueCreateInfo vk_queue_info = {};

	float queue_priorities[1] = { 0.0 };
	vk_queue_info.pNext = NULL;
	vk_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk_queue_info.queueCount = 1;
	vk_queue_info.pQueuePriorities = queue_priorities;
	vk_queue_info.queueFamilyIndex = base->graphics_queue_family_index;

	VkDeviceCreateInfo vk_device_info = {};
	vk_device_info.pNext = NULL;
	vk_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vk_device_info.queueCreateInfoCount = 1;
	vk_device_info.pQueueCreateInfos = &vk_queue_info;
	vk_device_info.enabledExtensionCount = base->device_extension_names != NULL ?
		base->device_extension_count : 0;
	vk_device_info.ppEnabledExtensionNames = base->device_extension_names != NULL ?
		base->device_extension_names : NULL;
	vk_device_info.pEnabledFeatures = NULL;

	vk_res = vkCreateDevice(base->gpus[0], &vk_device_info, NULL, &base->device);
	assert(vk_res == VK_SUCCESS);

	return vk_res;
}

VkResult init_enumerate_device(struct base_info *base, uint32_t gpu_count) {
	const uint32_t req_count = gpu_count;
	VkResult vk_res = vkEnumeratePhysicalDevices(base->vk_instance, &gpu_count, NULL);
	
	assert(gpu_count);
	
	base->gpus = realloc(base->gpus, gpu_count * sizeof(VkPhysicalDevice));

	vk_res = vkEnumeratePhysicalDevices(base->vk_instance, &gpu_count, base->gpus);
	assert(!vk_res && gpu_count >= req_count);

	vkGetPhysicalDeviceQueueFamilyProperties(base->gpus[0], &base->queue_family_count, NULL);
	assert(base->queue_family_count >= 1);

	base->queue_properties = realloc(base->queue_properties, base->queue_family_count * sizeof(VkQueueFamilyProperties));

	return vk_res;
}

void clean_up(struct base_info *base) {
	vkDestroyInstance(base->vk_instance, NULL);

	freepp((void *)base->instance_layer_names, base->instance_layer_count);
	freepp((void *)base->instance_extension_names, base->instance_extension_count);
	freep(base->instance_layer_properties);
	freep(base->instance_extension_properties);
	freep(base->gpus);
	freep(base->device_extension_properties);
	freep(base->queue_properties);

	//if(base->instance_layer_names != NULL) {
	//	for(int i = 0; i < base->instance_layer_count; i++) {
	//		free(base->instance_layer_names[i]);
	//	}
	//	free(base->instance_layer_names);
	//}
	//
	//if(base->instance_extension_names != NULL) {
	//	for(int i = 0; i < base->instance_extension_count; i++) {
	//		free(base->instance_extension_names[i]);
	//	}
	//	free(base->instance_extension_names);
	//}

	//if(base->instance_layer_properties != NULL) {
	//	free(base->instance_layer_properties);
	//}

	//if(base->instance_extension_properties != NULL) {
	//	free(base->instance_extension_properties);
	//}

	//if(

	//if(base->validation_layers != NULL) {
	//	size_t tamanho = sizeof(base->validation_layers) / sizeof(base->validation_layers[0]);
	//	for(int i = 0; i < tamanho; i++) {
	//		free((char *)base->validation_layers[i]);
	//	}
	//	free(base->validation_layers);
	//}
}
