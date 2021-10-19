#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GPU/GPUHeapAllocationCookie.h>

#define __MGX_DEVICE_FEATURE_HELPER(data, val) template<> static constexpr D3D12_FEATURE __FeatureHelper<data>() { return val; }

namespace MGX::Core::GPU 
{
    // Format support group
    struct FormatSupport
    {
        D3D12_FORMAT_SUPPORT1 fmt1 = (D3D12_FORMAT_SUPPORT1)0xFFFFFFFF;
        D3D12_FORMAT_SUPPORT2 fmt2 = (D3D12_FORMAT_SUPPORT2)0xFFFFFFFF;
    };

    // MSAA Support indication
    struct MSAASupport
    {
        union 
        {
            UINT values[5] = {0};
            struct
            {
                UINT qLevelMSAA1;
                UINT qLevelMSAA2;
                UINT qLevelMSAA4;
                UINT qLevelMSAA8;
                UINT qLevelMSAA16;
            };
        };
    };

    // GPU Device
    class Device : public Foundation::COMGetable<ID3D12Device> 
    {
        public:
            // Construct
            Device();
            Device(LUID luid);

            // Destruct
            ~Device() noexcept;

            // Get device description
            DXGI_ADAPTER_DESC3 GetDescription() noexcept;

            // Get allocation info for resource
            HeapAllocationCookie GetAllocationInfo(const D3D12_RESOURCE_DESC* ptrDesc) noexcept;

            // Check format support
            FormatSupport CheckFormatSupport(DXGI_FORMAT format) noexcept;

            // Check msaa support
            MSAASupport CheckMSAASupport(DXGI_FORMAT format, D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE) noexcept;

            // Check feature support
            template<typename T>
            HRESULT CheckFeatureSupport(T* ptrFeature) noexcept 
            {
                return m_ptrBase->CheckFeatureSupport(__FeatureHelper<T>(), ptrFeature, sizeof(T));
            }

        private:
            #ifdef _DEBUG
            ComPointer<ID3D12DebugDevice> m_ptrDebugDevice;
            void __DebugInit();
            #endif

        private:
            // Blueprint helper
            template<typename T>
            static constexpr D3D12_FEATURE __FeatureHelper() { return (D3D12_FEATURE)NULL; }

            // Implement
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS, D3D12_FEATURE_D3D12_OPTIONS)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_ARCHITECTURE, D3D12_FEATURE_ARCHITECTURE)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_FEATURE_LEVELS, D3D12_FEATURE_FEATURE_LEVELS)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_FORMAT_SUPPORT, D3D12_FEATURE_FORMAT_SUPPORT)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS, D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_FORMAT_INFO, D3D12_FEATURE_FORMAT_INFO)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT, D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_SHADER_MODEL, D3D12_FEATURE_SHADER_MODEL)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS1, D3D12_FEATURE_D3D12_OPTIONS1)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT, D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_ROOT_SIGNATURE, D3D12_FEATURE_ROOT_SIGNATURE)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_ARCHITECTURE1, D3D12_FEATURE_ARCHITECTURE1)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS2, D3D12_FEATURE_D3D12_OPTIONS2)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_SHADER_CACHE, D3D12_FEATURE_SHADER_CACHE)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY, D3D12_FEATURE_COMMAND_QUEUE_PRIORITY)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS3, D3D12_FEATURE_D3D12_OPTIONS3)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_EXISTING_HEAPS, D3D12_FEATURE_EXISTING_HEAPS)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS4, D3D12_FEATURE_D3D12_OPTIONS4)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_SERIALIZATION, D3D12_FEATURE_SERIALIZATION)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_CROSS_NODE, D3D12_FEATURE_CROSS_NODE)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS5, D3D12_FEATURE_D3D12_OPTIONS5)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_DISPLAYABLE, D3D12_FEATURE_DISPLAYABLE)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS6, D3D12_FEATURE_D3D12_OPTIONS6)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_QUERY_META_COMMAND, D3D12_FEATURE_QUERY_META_COMMAND)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS7, D3D12_FEATURE_D3D12_OPTIONS7)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPE_COUNT, D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPE_COUNT)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPES, D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPES)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS8, D3D12_FEATURE_D3D12_OPTIONS8)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS9, D3D12_FEATURE_D3D12_OPTIONS9)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS10, D3D12_FEATURE_D3D12_OPTIONS10)
            __MGX_DEVICE_FEATURE_HELPER(D3D12_FEATURE_DATA_D3D12_OPTIONS11, D3D12_FEATURE_D3D12_OPTIONS11)
    };
}