#include "vk_tutorial.h"


#if defined(VK_USE_PLATFORM_WAYLAND_KHR)

void handle_ping(void *data, wl_shell_surface *shell_surface, uint32_t serial) {
	wl_shell_surface_pong(shell_surface, serial);
}

void handle_configure(void *data, wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {}

void handle_popup_done(void *data, wl_shell_surface *shell_surface) {}

wl_shell_surface_listener shell_surface_listener = { handle_ping, handle_configure, handle_popup_done };

void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
	base_info *base = (base_info *)data;
	// pega objetos do wayland quando aparecerem
	if(strcmp(interface, "wl_compositor") == 0) {
		base->compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
	} else if(strcmp(interface, "wl_shell") == 0) {
		base->compositor = (wl_shell *)wl_registry_bind(registry, id &wl_shell_interface, 1);
	}
}

void registry_handle_global_remove(void *data, wl_registry *registry, uint32_t name) {}

wl_registry_listener registry_listener = { registry_handle_global, registry_handle_global_remove };

void init_window(struct base_info *base) {
	assert(base->width > 0);
	assert(base->height > 0);

	base->window = NULL;
	base->compositor = NULL;
	base->window = wl_compositor_create_surface(base->compositor);
	if(!base->window) {
		printf("Falha ao criar wayland_surface do compositor\n");
		fflush(stdout);
		exit(1);
	}

	base->shell_surface = NULL;
	base->shell_surface = wl_shell_get_shell_surface(base->shell, base->window);
	if(!base->shell_surface) {
		printf("Falha ao pegar shell_surface do wayland_surface\n");
		fflush(stdout);
		exit(1);
	}

	//wl_shell_surface_add_listener(base->shell_surface, &shell_surface_listener, base);
	//wl_shell_surface_set_toplevel(base->shell_surface);
}

void destroy_window(struct base_info *base) {
	wl_shell_surface_destroy(base->shell_surface);
	wl_surface_destroy(base->window);
	wl_shell_destroy(base->shell);
	wl_compositor_destroy(base->compositor);
	wl_registry_destroy(base->registry);
	wl_display_disconnect(base->display);
}
#endif

