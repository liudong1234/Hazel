#pragma once

#include <string>

namespace Hazel
{
	class FileDialogs
	{
	public:
		//返回空字符串则是取消
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
