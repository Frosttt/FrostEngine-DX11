#include "FRException.h"
#include <sstream>

FRException::FRException(int line, const char* file) noexcept
: line(line),
	file(file),
	text()
{

}

FRException::FRException(int line, const char* file, const char* text) noexcept
	: line(line),
	file(file),
	text(text)
{

}

const char* FRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();

}

const char* FRException::GetType() const noexcept
{
	return "Frost Exception";
}

int FRException::GetLine() const noexcept
{
	return line;
}

std::string FRException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	if (!text.empty())
	{ 
		oss << text << std::endl;
	}
	oss << "[FILE]: " << file << std::endl
			<< "[LINE]: " << line;
		return oss.str();

}
