#include "pch.h"
#include "hooks.h"

#include "common/offsets.h"
#include "render_d3d11/render_d3d11.h"

#define RELEASE_D3D(p) if(p) { p->Release(); p = nullptr; }

bool state = true;

hook_data hook_present;
hook_data hook_resizebuffers;

WNDPROC g_wndproc{};

HRESULT WINAPI hooked_present(IDXGISwapChain *thisptr, UINT SyncInterval, UINT Flags)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (state)
	{
		ImGui::Begin("Debug");
		ImGui::End();
	}

	render_d3d11::get().set_render_targets();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return ((present_t)hook_present.m_orig_)(thisptr, SyncInterval, Flags);
}

HRESULT WINAPI hooked_resizebuffers(IDXGISwapChain *thisptr, UINT buffer, UINT w, UINT h, DXGI_FORMAT format, UINT flags)
{
	auto hr = ((resize_buffers_t)hook_resizebuffers.m_orig_)(thisptr, buffer, w, h, format, flags);

	render_d3d11::get().cleanup_render_target();

	ImGui_ImplDX11_InvalidateDeviceObjects();

	if (SUCCEEDED(hr))
	{
		render_d3d11::get().set_swapchain(thisptr);
		render_d3d11::get().create_render_target();
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	return hr;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT hooked_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYDOWN)
	{
		if (wparam == VK_INSERT)
			state = !state;
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return FALSE;

	return CallWindowProc(g_wndproc, hwnd, msg, wparam, lparam);
}

void hooks::init()
{
    if (MH_Initialize() != MH_OK)
    {
        console::get().print("MH not initialized.\n");
        return;
    }

    console::get().print("Install hooks...\n");
	g_wndproc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)hooked_wndproc);

	render_d3d11::get().set_swapchain(g_swapchain->pSwapChain);
	render_d3d11::get().create_render_target();

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_hwnd);
	ImGui_ImplDX11_Init(render_d3d11::get().get_device(), render_d3d11::get().get_context());

	if (hook_present.init((FARPROC)(get_v_func(g_swapchain->pSwapChain, 8)), (FARPROC)hooked_present))
		hook_present.hook();

	if (hook_resizebuffers.init((FARPROC)(get_v_func(g_swapchain->pSwapChain, 13)), (FARPROC)hooked_resizebuffers))
		hook_resizebuffers.hook();
}

void hooks::shutdown()
{
	SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)g_wndproc);

    console::get().print("Remove hooks...\n");

    hook_present.clear();
	hook_resizebuffers.clear();

	render_d3d11::get().cleanup_render_target();

	auto ctx = ImGui::GetCurrentContext();

	if (ctx && ctx->Initialized)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

    MH_Uninitialize();
}
