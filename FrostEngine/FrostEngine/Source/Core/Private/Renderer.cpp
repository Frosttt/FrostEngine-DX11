#include "Renderer.h"
#include "FRException.h"
#include "dxerr.h"
#include <iostream>
#include <sstream>
#include "GraphicsExceptionMacros.h"

#pragma comment(lib, "d3d11.lib")



Renderer::Renderer(HWND hWind)
	: pDevice(nullptr)
	, pSwapChain(nullptr)
	, pContext(nullptr)
	, pTarget(nullptr)
{

	DXGI_SWAP_CHAIN_DESC swapdesc = {};
	swapdesc.BufferDesc.Width = 0;
	swapdesc.BufferDesc.Height = 0;
	swapdesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapdesc.BufferDesc.RefreshRate.Numerator = 0;
	swapdesc.BufferDesc.RefreshRate.Denominator = 0;
	swapdesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapdesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapdesc.SampleDesc.Count = 1;
	swapdesc.SampleDesc.Quality = 0;
	swapdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.BufferCount = 1;
	swapdesc.OutputWindow = hWind;
	swapdesc.Windowed = TRUE;
	swapdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapdesc.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	HRESULT hr;

		GFX_THROW_INFO (D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapdesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget));
	pBackBuffer->Release();
	//{
	//	FR_EXCEPT();
	//}

}

Renderer::~Renderer()
{
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}

	if (pSwapChain != nullptr)
	{
		pSwapChain->Release();
	}

	if (pContext != nullptr)
	{
		pContext->Release();
	}

	if (!pTarget)
	{
		pTarget->Release();
	}

}

void Renderer::ClearBuffer(float r, float g, float b, float a) noexcept
{
	const float color[] = {r, g, b, a};
	pContext->ClearRenderTargetView(pTarget, color);
}

void Renderer::EndFrame()
{
	HRESULT hr;

#ifndef NDEBUG
	infoManager.Set();
#endif // !NDEBUG

	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}



// Graphics exception stuff


Renderer::HRException::HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	if (!info.empty())
	{
		info.pop_back();
	}
}


const char* Renderer::HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Renderer::HRException::GetType() const noexcept
{
	return "Frost Graphics Exception";
}

HRESULT Renderer::HRException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Renderer::HRException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Renderer::HRException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Renderer::HRException::GetErrorInfo() const noexcept
{
	return info;
}

Renderer::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Renderer::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Renderer::InfoException::GetType() const noexcept
{
	return "Frost Graphics Info Exception";
}

std::string Renderer::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Renderer::DeviceRemovedException::GetType() const noexcept
{
	return "Frost Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
