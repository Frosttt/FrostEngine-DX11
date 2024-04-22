#include "Renderer.h"
#include "FRException.h"
#include "dxerr.h"
#include <iostream>
#include <sstream>
#include "GraphicsExceptionMacros.h"
#include <d3dcompiler.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace wrl = Microsoft::WRL;
//namespace dx = DirectX;

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
	wrl::ComPtr<ID3D11Texture2D> pBackBuffer;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
	//{
	//	FR_EXCEPT();
	//}

}


void Renderer::ClearBuffer(float r, float g, float b, float a) noexcept
{
	const float color[] = {r, g, b, a};
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Renderer::DrawTestTriangle()
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	struct Vertex
	{
		float x;
		float y;
	};

	// create vertex buffer (2d triangle)
	const Vertex vertices[] =
	{
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f},
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	// Build buffer description
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // what kind of binding
	bd.Usage = D3D11_USAGE_DEFAULT; // What is allowed to use this data
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	// Self explanatory
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	
	wrl::ComPtr<ID3DBlob> pBlob;

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// Bind Pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// Create Vertext Shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO( D3DReadFileToBlob(L"VertexShader.cso", &pBlob) );
	GFX_THROW_INFO( pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader) );

	// Bind Vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr,0u);

	// iput vertext layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	GFX_THROW_INFO( pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	) );

	// bind input vertext layout
	pContext->IASetInputLayout( pInputLayout.Get());



	// bind render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	// Set primitive topology
	// https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies
	pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 


	GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
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
