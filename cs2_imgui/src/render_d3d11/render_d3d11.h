#pragma once

#include "framework.h"

class render_d3d11
{
protected:
	render_d3d11() = default;
	render_d3d11(const render_d3d11&) = delete;
	render_d3d11 &operator=(const render_d3d11&) = delete;

public:
	static render_d3d11 &get()
	{
		static render_d3d11 instance;
		return instance;
	}

	void set_swapchain(IDXGISwapChain* swapchain);

	auto get_device() const					{ return device_; }
	auto get_context() const				{ return context_; }
	auto get_render_target_view() const		{ return render_target_view_; }

	void create_render_target();
	void cleanup_render_target();
	void set_render_targets();

private:
	IDXGISwapChain			*swapchain_{};
	ID3D11Device			*device_{};
	ID3D11RenderTargetView	*render_target_view_{};
	ID3D11DeviceContext		*context_{};
};