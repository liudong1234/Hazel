#pragma once
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurDirectory;
		Ref<Texture2D> m_FileImg;
		Ref<Texture2D> m_DirImg;
	};
}
