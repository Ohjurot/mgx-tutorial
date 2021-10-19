#define NOMINMAX
#include <Windows.h>

// Is defined in project setup
// #ifdef _DEBUG
// #define MGX_DEBUG
// #endif

#include <ModernGX.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GFXWindow.h>
#include <ModernGX/Core/GPU/GPUDevice.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>
#include <ModernGX/Core/GPU/GPUDescriptorHeap.h>

using namespace MGX;

INT wWinMain_safe(HINSTANCE hInstance, HINSTANCE hPrevHinstance, PWSTR cmdArgs, INT cmdShow)
{
    MGX_INIT();

    // Create core objects
    Core::GPU::Device device;
    Core::GPU::CommandQueue commandQueue(device);
    Core::GPU::CommandList commandList(device);

    // Descriptor heaps
    Core::GPU::DescriptorHeap rtvDescriptorHeap(device, Core::GPU::DescriptorHeapUsage::RenderTargetView, 4);
    Core::GPU::DescriptorHeap srvDescriptorHeap(device, Core::GPU::DescriptorHeapUsage::ShaderResource, 4);

    // Create window
    Core::Window window(L"Tutorial Window", device, commandQueue, rtvDescriptorHeap.Allocate(3));

    // Window loop
    while (window.ProcessWindowEvents())
    {
        // Window resizing
        if (window.NeedsResizing())
        {
            commandQueue.Flush(window.GetBufferCount());
            window.ResizeNow(device);
        }



        // Present
        window.Present(true);
    }

    // Flush and return
    commandQueue.Flush(window.GetBufferCount());
    return 0;
}

INT WINAPI wWinMain(HINSTANCE _In_ hInstance, HINSTANCE _In_opt_ hPrevHinstance, PWSTR _In_ cmdArgs, INT _In_ cmdShow)
{
    INT returnCode = -1;

    try 
    {
        returnCode = wWinMain_safe(hInstance, hPrevHinstance, cmdArgs, cmdShow);
    }
    catch (Foundation::HRException& ex)
    {
        ex.show();
    }
    catch (std::exception& ex)
    {
        MessageBoxA(NULL, ex.what(), "STD::Exception", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        MessageBoxA(NULL, "No information availible", "Unknown Exception", MB_OK | MB_ICONERROR);
    }

    return returnCode;
}
