#define NOMINMAX
#include <Windows.h>

// Is defined in project setup
// #ifdef _DEBUG
// #define MGX_DEBUG
// #endif

#include <ModernGX.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GFXWindow.h>
#include <ModernGX/Core/WindowEventListener.h>
#include <ModernGX/Core/GPU/GPUDevice.h>
#include <ModernGX/Core/GPU/GPUQueue.h>
#include <ModernGX/Core/GPU/GPUCommandList.h>
#include <ModernGX/Core/GPU/GPUDescriptorHeap.h>
#include <ModernGX/Core/GPU/GPUPipelineState.h>
#include <ModernGX/Core/GPU/GPURootConfiguration.h>
#include <ModernGX/Core/GPUTypes/GTypeBuffer.h>
#include <ModernGX/Helpers/GPUUploadStack.h>

using namespace MGX;

// Vertex buffer
struct Vertex
{
    float x, y;
};

// Vertices for a quad
const Vertex quadVertices[] =
{
    { -1.0f, -1.0f },
    { -1.0f,  1.0f },
    {  1.0f, -1.0f },
    {  1.0f,  1.0f },
};

// Color of quad being drawn
class ColorKey : public Core::WindowEventListener
{
    public:
        float quadColor[3] = { 0.0f, 0.5f, 0.5f };

        inline MGX::Core::WindowEventResult OnWindowEvent(MGX::Core::WindowEvent& e) override
        {
            if (e.msg == WM_KEYDOWN && e.wParam == VK_SPACE)
            {
                quadColor[0] = 0.7f;
            }
            else if (e.msg == WM_KEYUP && e.wParam == VK_SPACE)
            {
                quadColor[0] = 0.0f;
            }

            return Core::WindowEventResult::NotHandled;
        }
};

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

    // Load pipeleline state
    Core::GPU::PipelineState pipelineState(device, L"./TutorialPipeline.xml");

    // Upload buffer
    Helpers::GPUUploadStack uploadBuffer(device, 1024 * 1024 * 16);
    uploadBuffer.Reset(&commandList);

    // Vertex buffer
    Core::GType::Buffer vertexBuffer(device, Core::GPU::HeapUsage::Default, sizeof(Vertex) * 4);
    uploadBuffer.CopyBuffer(&vertexBuffer, quadVertices, sizeof(Vertex) * 4);

    // Vertex buffer view
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    vertexBuffer.CreateVBV<Vertex>(&vertexBufferView, 4);

    // Set resource states
    vertexBuffer.SetResourceState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, commandList.ResourceBarrierPeekAndPush());

    // Close upload buffer
    uploadBuffer.Close();

    // Window class color key
    ColorKey colorKey;

    // Rootsignature configruation
    Core::GPU::RootConfiguration rootSignatureConfiguration(pipelineState.GetType(), 1, 
        Core::GPU::RootConfigurationEntry::MakeRootConstant(3, colorKey.quadColor)
    );

    // Create window
    Core::Window window(L"Tutorial Window", device, commandQueue, rtvDescriptorHeap.Allocate(3));
    window.AddWindowEventListener(&colorKey);
    D3D12_RECT windowScissorRect = window.GetScissorRect();
    D3D12_VIEWPORT windowViewport = window.GetViewport();

    // Window loop
    while (window.ProcessWindowEvents())
    {
        // Window resizing
        if (window.NeedsResizing())
        {
            commandQueue.Flush(window.GetBufferCount());
            window.ResizeNow(device);
            windowScissorRect = window.GetScissorRect();
            windowViewport = window.GetViewport();
        }

        // Prepare current buffer as RenderTarget
        window.GetCurrentBuffer()->SetResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET, commandList.ResourceBarrierPeekAndPush());
        commandList.OMPrepareRtDsViews(window.GetCurrentRtvCpuHandle());

        // BIND states
        pipelineState.Bind(commandList);
        rootSignatureConfiguration.Bind(commandList);
        commandList.IASetBuffer(&vertexBufferView, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        commandList.RSSetViewportAndRect(&windowViewport, &windowScissorRect);

        // DRAW triangles
        commandList.Draw(4);

        // Prepare current buffer for presenting
        window.GetCurrentBuffer()->SetResourceState(D3D12_RESOURCE_STATE_PRESENT, commandList.ResourceBarrierPeekAndPush());

        // Executing command list
        commandList.Close();
        commandQueue.Wait(commandQueue.Execute(commandList));
        commandList.Reset();

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
