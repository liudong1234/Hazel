#include "hzpch.h"
#include "ContentBrowserPanel.h"
#include "imgui/imgui.h"

namespace Hazel
{
	static const std::filesystem::path s_AssetsDirPath = "Assets";
	static const std::string s_DirImgPath = "Resources/Icons/ContentBrowser/DirImg.png";
	static const std::string s_FileImgPath = "Resources/Icons/ContentBrowser/FileImg.png";

	ContentBrowserPanel::ContentBrowserPanel():
		m_CurDirectory(s_AssetsDirPath)
	{
		this->m_FileImg = Texture2D::Create(std::string(s_FileImgPath));
		this->m_DirImg = Texture2D::Create(std::string(s_DirImgPath));
	}
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurDirectory.string() != s_AssetsDirPath)
		{
			if (ImGui::Button("<-"))
				m_CurDirectory = m_CurDirectory.parent_path();
		}
		static float padding = 16.0f;
		static float buttonSize = 75.0f;
		float cellSize = padding + buttonSize;
		float viewWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(viewWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;
		
		ImGui::Columns(columnCount, 0, false);
		for (auto& directoryDir : std::filesystem::directory_iterator(m_CurDirectory))
		{
			//与cherno略有不同
			std::filesystem::path curpath = directoryDir.path();
			//std::string pathString = curpath.filename().string();
			auto relativePath = std::filesystem::relative(curpath, s_AssetsDirPath);
			std::string pathString = relativePath.filename().string();
			Ref<Texture2D> ico = std::filesystem::is_directory(curpath) ? this->m_DirImg : this->m_FileImg;
			ImGui::PushID(pathString.c_str());
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)ico->GetRendererID(), { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = directoryDir.path().c_str();
				ImGui::SetDragDropPayload("Content browser", itemPath, wcslen(itemPath + 1) * sizeof(itemPath), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (std::filesystem::is_directory(curpath))
				{
					m_CurDirectory = curpath;
				}
			}
			ImGui::TextWrapped(pathString.c_str());

			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size", &buttonSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);


		ImGui::End();
	}
}
