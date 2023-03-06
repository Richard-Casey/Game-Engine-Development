//#include <filesystem>
//#include <vector>
//#include <iostream>
//#include <imgui.h>
//#include <imgui_internal.h>
//#include <imgui_sdl.h>
//
//#include "bitmap.h"
//
//std::vector<Bitmap*> content;
//			std::string path = "../Assets";
//			for (const auto& entry : std::filesystem::directory_iterator(path)) //directory_iterator(path) //recursive_
//			{
//				if (entry.path().extension() == ".bmp" || entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
//				{
//					Bitmap* Asset = new Bitmap(pRenderer, entry.path().string(), 0, 0, true);
//					content.push_back(Asset);
//
//				}
//				else if (entry.is_directory())
//				{
//					std::cout << "dir " << entry << std::endl;
//				}
//				//debug
//				std::cout << entry.path() << std::endl;
//			}
//
//
//
//
//
//
//            
//ImGui::Begin("Editor");
//				ImGui::BeginChild("Content Window", ImVec2(), true);
//				ImGui::BeginTable("Content browser", 3);
//				;
//				for (int i = 0; i < content.size(); i++)
//				{
//					ImGui::PushID(i);
//
//					ImGui::ImageButton((ImTextureID)content[i]->GetTextureRef(), { 100,100 });
//
//
//					for dragging
//					/*if (ImGui::BeginDragDropSource())
//					{
//						AssetMousDrag = content[i];
//						ImGui::Image((ImTextureID)content[i]->GetTextureRef(), { 100,100 });
//						ImGui::EndDragDropSource();
//					}*/
//					ImGui::PopID();
//					ImGui::SameLine();
//				}
//
//				ImGui::EndTabItem();
//
//				ImGui::EndChild();
//				ImGui::End();