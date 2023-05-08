project "fude"
    kind "StaticLib"
    language "C"
    targetdir "%{wks.location}/build/bin"
    objdir "%{wks.location}/build/bin-int"
    location "%{wks.location}/build/scripts"

	files {
		"src/*.c",
		"src/external/src/glad.c",
		"src/external/include/GLFW/glfw3.h",
		"src/external/include/GLFW/glfw3native.h",
		"src/external/src/internal.h",
		"src/external/src/platform.h",
		"src/external/src/mappings.h",
		"src/external/src/context.c",
		"src/external/src/init.c",
		"src/external/src/input.c",
		"src/external/src/monitor.c",
		"src/external/src/platform.c",
		"src/external/src/vulkan.c",
		"src/external/src/window.c",
		"src/external/src/egl_context.c",
		"src/external/src/osmesa_context.c",
		"src/external/src/null_platform.h",
		"src/external/src/null_joystick.h",
		"src/external/src/null_joystick.c",
		"src/external/src/null_init.c",
		"src/external/src/null_monitor.c",
		"src/external/src/null_window.c",
    }

	includedirs {
		"include",
		"src",
		"src/external/include",
        "src/external/include/glad",
		"src/external/include/GLFW",
		"src/external/include/KHR",
	}

	filter "system:windows"
		files {
			"src/external/src/win32_init.c",
			"src/external/src/win32_joystick.c",
			"src/external/src/win32_module.c",
			"src/external/src/win32_monitor.c",
			"src/external/src/win32_time.c",
			"src/external/src/win32_thread.c",
			"src/external/src/win32_window.c",
			"src/external/src/wgl_context.c",
			"src/external/src/egl_context.c",
			"src/external/src/osmesa_context.c"
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"Dwmapi",
			"gdi32",
			"opengl32",
		}

	filter "system:linux"
		files {
			"src/external/src/x11_init.c",
			"src/external/src/x11_monitor.c",
			"src/external/src/x11_window.c",
			"src/external/src/xkb_unicode.c",
			"src/external/src/posix_time.c",
			"src/external/src/posix_thread.c",
			"src/external/src/posix_module.c",
			"src/external/src/posix_poll.c",
			"src/external/src/glx_context.c",
			"src/external/src/egl_context.c",
			"src/external/src/osmesa_context.c",
			"src/external/src/linux_joystick.c"
		}
		defines {
			"_GLFW_X11"
		}
		links {
			"X11",
			"m"
		}
    