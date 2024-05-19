#pragma once
#include "FrWin.h"
#include <d3d11.h>
#include "FRException.h"
#include <vector>
#include "DxgiInfoManager.h"
#include "FrWRL.h"

class Renderer
{

#pragma region Exceptions
	public:
		class Exception : public FRException
		{
			using FRException::FRException;
		};
		class HRException : public Exception
		{
			public:
				HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
				const char* what() const noexcept override;
				const char* GetType() const noexcept override;
				HRESULT GetErrorCode() const noexcept;
				std::string GetErrorString() const noexcept;
				std::string GetErrorDescription() const noexcept;
				std::string GetErrorInfo() const noexcept;
				private:
				HRESULT hr;
				std::string info;
		};
		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};
		class DeviceRemovedException : public HRException
		{
			using HRException::HRException;
		public:
			const char* GetType() const noexcept override;
		private:
			std::string reason;
		};
#pragma endregion

	public:
	Renderer(HWND hWind);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;

	void ClearBuffer(float r, float g, float b, float a = 1.0f) noexcept;
	void DrawTestTriangle(float x, float y, float angle = 0, float uptime = 0);
	void EndFrame();
	private:

	#ifndef NDEBUG
	DxgiInfoManager infoManager;
	#endif

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice; // Used for allcation
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext; // Used for configuration and executing rendering commands
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

