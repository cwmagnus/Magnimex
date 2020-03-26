#include "RenderAPI.h"

#include <vector>
#include <cstring>
#include <set>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "Optional.h"

// Disable unscoped enum warning in visual studio
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26812)
#endif // _MSC_VER

namespace mag
{
	namespace RenderAPI
	{
		struct QueueFamilyIndices 
		{
			Optional<uint32_t> GraphicsFamily;
			Optional<uint32_t> PresentFamily;

			bool IsComplete() 
			{
				return GraphicsFamily.HasValue() && PresentFamily.HasValue();
			}
		};

		const std::vector<const char*> VALIDATION_LAYERS = 
		{
			"VK_LAYER_KHRONOS_validation"
		};
#ifdef _DEBUG
		const bool ENABLE_VALIDATION_LAYERS = true;
#else
		const bool ENABLE_VALIDATION_LAYERS = false;
#endif // _DEBUG

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkSurfaceKHR surface;
		VkQueue presentQueue;

		void CreateInstance(const Window* window);
		void SetupDebugMessenger();
		void CreateSurface(const Window* window);
		void GetPhysicalDevice();
		void CreateLogicalDevice();

		void Init(const Window* window)
		{
			CreateInstance(window);
			SetupDebugMessenger();
			CreateSurface(window);
			GetPhysicalDevice();
			CreateLogicalDevice();
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		void Shutdown()
		{
			if (ENABLE_VALIDATION_LAYERS)
			{
				DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}
			vkDestroySurfaceKHR(instance, surface, nullptr);
			vkDestroyDevice(device, nullptr);
			vkDestroyInstance(instance, nullptr);
		}

		bool HasValidationLayerSupport()
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : VALIDATION_LAYERS)
			{
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					return false;
				}
			}

			return true;
		}

		std::vector<const char*> GetRequiredExtensions()
		{
			uint32_t extensionCount = 0;
			const char** extensionsRaw = glfwGetRequiredInstanceExtensions(&extensionCount);

			std::vector<const char*> extensions(extensionsRaw, extensionsRaw + extensionCount);

			if (ENABLE_VALIDATION_LAYERS)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		}

		bool HasRequiredExtensions(std::vector<const char*> requiredExtensions, const std::vector<VkExtensionProperties>& vulkanExtensions)
		{
			for (const char* extension : requiredExtensions)
			{
				bool foundExtension = false;

				for (const auto& vulkanExtension : vulkanExtensions)
				{
					if (strcmp(extension, vulkanExtension.extensionName) == 0)
					{
						foundExtension = true;
						break;
					}
				}

				if (!foundExtension)
				{
					return false;
				}
			}

			return true;
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) 
		{

			// TODO: Log

			return VK_FALSE;
		}

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) 
			{
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else 
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) 
			{
				func(instance, debugMessenger, pAllocator);
			}
		}

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
		{
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = DebugCallback;
		}

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice  device)
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			for (int i = 0; i < queueFamilies.size(); ++i)
			{
				if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.GraphicsFamily = i;
				}

				if (indices.IsComplete())
				{
					break;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
				if (presentSupport)
				{
					indices.PresentFamily = i;
				}
			}

			return indices;
		}

		bool IsDeviceSuitable(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices = FindQueueFamilies(device);

			return indices.IsComplete();
		}

		void CreateInstance(const Window* window)
		{
			if (ENABLE_VALIDATION_LAYERS && !HasValidationLayerSupport())
			{
				// TODO: Log
			}

			VkApplicationInfo appInfo = { };
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = window->GetTitle().c_str();
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Magnimex";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_2;

			auto requiredExtensions = GetRequiredExtensions();

			VkInstanceCreateInfo createInfo = { };
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
			createInfo.ppEnabledExtensionNames = requiredExtensions.data();
			if (ENABLE_VALIDATION_LAYERS)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
				createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

				PopulateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
			else
			{
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			{
				// TODO: Log
			}

			uint32_t vulkanExtensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);
			std::vector<VkExtensionProperties> vulkanExtensions(vulkanExtensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, vulkanExtensions.data());

			if (!HasRequiredExtensions(requiredExtensions, vulkanExtensions))
			{
				// TODO: Log
			}
		}

		void SetupDebugMessenger()
		{
			if (!ENABLE_VALIDATION_LAYERS)
			{
				return;
			}

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			PopulateDebugMessengerCreateInfo(createInfo);

			if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) 
			{
				// TODO: Log
			}
		}

		void CreateSurface(const Window* window)
		{
			if (glfwCreateWindowSurface(instance, window->GetGLFWwindow(), nullptr, &surface) != VK_SUCCESS) 
			{
				// TODO: Log
			}
		}

		void GetPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
			if (deviceCount == 0) 
			{
				// TODO: Log
			}
			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			for (const auto& device : devices) 
			{
				if (IsDeviceSuitable(device)) 
				{
					physicalDevice = device;
					break;
				}
			}

			if (physicalDevice == VK_NULL_HANDLE) 
			{
				// TODO: Log
			}
		}

		void CreateLogicalDevice()
		{
			QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.GetValue(), indices.PresentFamily.GetValue() };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) 
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = 0;

			if (ENABLE_VALIDATION_LAYERS) 
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
				createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
			}
			else 
			{
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
			{
				// TODO: Log
			}

			vkGetDeviceQueue(device, indices.GraphicsFamily.GetValue(), 0, &graphicsQueue);
			vkGetDeviceQueue(device, indices.PresentFamily.GetValue(), 0, &presentQueue);
		}
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _WIN32
