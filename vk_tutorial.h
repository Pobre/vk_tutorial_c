#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <wayland-client.h>

struct base_info {
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_surface *window;
	struct wl_shell *shell;
	struct wl_shell_surface *shell_surface;
#elif defined(_WIN32)
#else
	//xcb_connection_t *connection;
	//xcb_screen_t *screen;
	//xcb_window_t window;
	//xcb_intern_atom_reply_t *atom_wm_delete_window;
#endif

	int height;
	int width;
	VkInstance vk_instance;
	const char **instance_layer_names;
	uint32_t instance_layer_count;
	const char **instance_extension_names;
	uint32_t instance_extension_count;
	struct layer_properties *instance_layer_properties;
	VkExtensionProperties *instance_extension_properties;

	VkPhysicalDevice *gpus;
	uint32_t device_extension_count;
	const char **device_extension_names;
	VkExtensionProperties *device_extension_properties;
	VkDevice device;
	VkQueue graphics_queue;
	VkQueue presente_queue;
	uint32_t graphics_queue_family_index;
	uint32_t present_queue_family_index;
	VkPhysicalDeviceProperties gpu_properties;
	VkQueueFamilyProperties *queue_properties;
	VkPhysicalDeviceMemoryProperties memory_properties;

	uint32_t queue_family_count;
};

struct layer_properties {
	VkLayerProperties properties;
	VkExtensionProperties *instance_extensions;
	VkExtensionProperties *device_extensions;
};

void run(struct base_info *base);
void freep(void *p);
void freepp(void **p, uint32_t size);
void init_window(struct base_info *base);
VkResult init_global_layer_properties(struct base_info *base);
VkResult init_global_extension_properties(struct base_info *base, struct layer_properties *lp);
VkResult init_instance(struct base_info *base);
VkResult init_device(struct base_info *base);
VkResult init_enumerate_device(struct base_info *base, uint32_t gpu_count);
void destroy_window(struct base_info *base);
void clean_up(struct base_info *base);

