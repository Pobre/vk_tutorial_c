#include "vk_tutorial.h"

int main(int argc, char *argv[]) {
	struct base_info info = {};
	init_global_layer_properties(&info);
	init_instance(&info);

	init_enumerate_device(&info, 1);

	VkDeviceQueueCreateInfo vk_queue_info = {};

	vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_properties);
	assert(info.queue_family_count >= 1);

	bool found = false;
	for(uint32_t i = 0; i < info.queue_family_count; i++) {
		if(info.queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			vk_queue_info.queueFamilyIndex = i;
			found = true;
			break;
		}
	}
	assert(found);
	assert(info.queue_family_count >= 1);

	float queue_properties[1] = { 0.0 };
	vk_queue_info.pNext = NULL;
	vk_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk_queue_info.queueCount = 1;
	vk_queue_info.pQueuePriorities = queue_properties;

	VkDeviceCreateInfo vk_device_info = {};
	vk_device_info.pNext = NULL;
	vk_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vk_device_info.queueCreateInfoCount = 1;
	vk_device_info.pQueueCreateInfos = &vk_queue_info;
	vk_device_info.enabledExtensionCount = 0;
	vk_device_info.ppEnabledExtensionNames = NULL;
	vk_device_info.enabledLayerCount = 0;
	vk_device_info.ppEnabledLayerNames = NULL;
	vk_device_info.pEnabledFeatures = NULL;

	VkDevice vk_device;
	VkResult vk_res = vkCreateDevice(info.gpus[0], &vk_device_info, NULL, &vk_device);
	assert(vk_res == VK_SUCCESS);

	vkDestroyDevice(vk_device, NULL);

	clean_up(&info);

	return 0;
}
