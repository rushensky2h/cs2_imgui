#include "pch.h"
#include "render_d3d11.h"

#define RELEASE_D3D(p) if(p) { p->Release(); p = nullptr; }

void render_d3d11::create_render_target()
{
	ID3D11Texture2D *backbuffer;

	if (SUCCEEDED(swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID *>(&backbuffer))))
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		device_->CreateRenderTargetView(backbuffer, &desc, &render_target_view_);

		RELEASE_D3D(backbuffer);
	}
}

void render_d3d11::cleanup_render_target()
{
	RELEASE_D3D(render_target_view_);
}

void render_d3d11::set_render_targets()
{
	context_->OMSetRenderTargets(1, &render_target_view_, NULL);
}

void render_d3d11::set_swapchain(IDXGISwapChain *swapchain)
{
	swapchain_ = swapchain;

	if (FAILED(swapchain_->GetDevice(__uuidof(ID3D11Device), (void **)&device_)))
		console::get().print("device: failed to get device.\n");

	device_->GetImmediateContext(&context_);
}
