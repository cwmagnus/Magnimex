#include "RenderAPI.h"

#include <vector>
#include <cstring>
#include <vulkan/vulkan.h>

#include <iostream>

namespace mag
{
	namespace RenderAPI
	{
		const std::vector<const char*> validationLayers = 
		{
			"VK_LAYER_KHRONOS_validation"
		};
#ifdef _DEBUG
		const bool enableValidationLayers = true;
#else
		const bool enableValidationLayers = false;
#endif // _DEBUG

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;

		void CreateInstance(const Window* window);
		void SetupDebugMessenger();

		void Init(const Window* window)
		{
			CreateInstance(window);
			SetupDebugMessenger();
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		void Shutdown()
		{
			if (enableValidationLayers)
			{
				DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}

			vkDestroyInstance(instance, nullptr);
		}

		bool HasValidationLayerSupport()
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : validationLayers)
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

		std::vector<const char*> GetRequiredExtensions(const Window* window)
		{
			uint32_t extensionCount = 0;
			const char** extensionsRaw = window->GetRequiredInstanceExtensions(extensionCount);

			std::vector<const char*> extensions(extensionsRaw, extensionsRaw + extensionCount);

			if (enableValidationLayers)
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
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

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

		void CreateInstance(const Window* window)
		{
			if (enableValidationLayers && !HasValidationLayerSupport())
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

			auto requiredExtensions = GetRequiredExtensions(window);

			VkInstanceCreateInfo createInfo = { };
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
			createInfo.ppEnabledExtensionNames = requiredExtensions.data();
			if (enableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();

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
			if (!enableValidationLayers)
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
	}
}
