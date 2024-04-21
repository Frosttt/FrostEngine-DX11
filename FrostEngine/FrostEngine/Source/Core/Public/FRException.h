#pragma once
#include <exception>
#include <string>

class FRException : public std::exception
{
	public:
		FRException(int line, const char* file) noexcept;
		FRException(int line, const char* file, const char* text) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		std::string GetOriginString() const noexcept;
	private:
		int line;
		std::string file;
		std::string text;
	protected:
		mutable std::string whatBuffer;
};

#define FR_EXCEPT() FRException(__LINE__, __FILE__)
#define FR_EXCEPT_TXT(text) FRException(__LINE__, __FILE__, text)
