#include "ImguiSystem.h"
#include "EngineSystem.h"
#include "WindowSystem.h"
#include "ToolSystemMap.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ImguiSystem::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
}

void ImguiSystem::Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


    static int image_current_idx = 0; // Here we store our selection data as an index.
    static int fbx_current_idx = 0; // Here we store our selection data as an index.
    static int iMapSize = 4;
    static float fMapDistance = 1.0f;
    static bool bWireFrame = false;
    static bool bMapPicking = false;
    static bool bOjbectPicking = false;
    static bool bSculptPicking = false;
    ImGui::Begin("Demo");
    {
        {
            if (ImGui::Checkbox("WireFrame", &bWireFrame))
            {
                ~bWireFrame;
                _ToolSystemMap.SetWireframe(bWireFrame);
            }

            if (ImGui::Checkbox("MapPicking", &bMapPicking))
            {
                ~bMapPicking;
                _ToolSystemMap.SelectImage(image_current_idx, bMapPicking);
            }

            if (ImGui::Checkbox("OjbectPicking", &bOjbectPicking))
            {
                ~bOjbectPicking;
                _ToolSystemMap.SelectObject(bOjbectPicking);
            }

            if (ImGui::Checkbox("SculptPicking", &bSculptPicking))
            {
                ~bSculptPicking;
                _ToolSystemMap.SelectSculpt(bSculptPicking);
            }
        }
        ImGui::Dummy({ 0, 10 });

        {
            if (ImGui::Button("CreateObject"))
                _ToolSystemMap.CreateFbxObject(_ToolSystemMap.m_ListFbx[fbx_current_idx], _CameraSystem.GetCurrentCamera()->m_vCameraPos);

        }
        ImGui::Dummy({ 0, 10 });

        {
            if (ImGui::Button("CreateMap"))
            {
                _ToolSystemMap.CreateSimpleMap(iMapSize + 1, iMapSize + 1, fMapDistance, image_current_idx);
            }
            ImGui::InputInt("MapSize", &iMapSize);
            ImGui::InputFloat("ShellDistance", &fMapDistance);
        }
        ImGui::Dummy({ 0, 10 });

        {
            if (ImGui::Button("DeleteMap"))
            {
                _ToolSystemMap.DeleteSimpleMap();
            }
        }
        ImGui::Dummy({ 0, 10 });

       
        {
            if (ImGui::Button("open"))
            {
                ifd::FileDialog::Instance().Open("ShaderOpenDialog", "Open a shader", "*.sav {.sav}", true);
                _ToolSystemMap.OpenFile();
            }
        }
        ImGui::Dummy({ 0, 10 });

        {
            if (ImGui::Button("save"))
            {
                ifd::FileDialog::Instance().Save("ShaderSaveDialog", "Save a shader", "*.txt {.txt}");
                //_ToolSystemMap.SaveFile();
            }
        }
        ImGui::Dummy({ 0, 10 });

    }
    ImGui::End();
   
    //// 0.∏ﬁ¿Œ
    //{
    //    ImGui::Begin("##main");
    //    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
    //    ImGui::SetNextWindowSize({ ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y });
    //    ImGui::Image(srv, { ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y });
    //    ImGui::End();
    //}
    //

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   /* if (m_show_demo_window)
        ImGui::ShowDemoWindow(&m_show_demo_window);*/
   
    //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;
    //    
    //    
    //    ImGui::Begin("Hello, world2!");                          // Create a window called "Hello, world!" and append into it.

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &m_show_another_window);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3("clear color", (float*)&m_clear_color); // Edit 3 floats representing a color

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //    ImGui::End();
    //}

    //// 3. Show another simple window.
    //if (m_show_another_window)
    //{
    //    ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        m_show_another_window = false;
    //    ImGui::End();
    //}

    // Simple window
    ImGui::Begin("Control Panel");
    if (ImGui::Button("Open ImgFile"))
        ifd::FileDialog::Instance().Open("ImageOpenDialog", "Open a Image", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*", true);
    /*if (ImGui::Button("Open directory"))
        ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Open a directory", "");
    if (ImGui::Button("Save file"))
        ifd::FileDialog::Instance().Save("ShaderSaveDialog", "Save a shader", "*.sprj {.sprj}");*/
  
    if (ImGui::BeginListBox("listbox 1"))
    {
        for (int n = 0; n < _ToolSystemMap.m_ListTexture.size(); n++)
        {
            std::string fullpath = _towm(_ToolSystemMap.m_ListTexture[n]);

            std::string content = GetSplitName(fullpath);
                
            const bool is_selected = (image_current_idx == n);
            if (ImGui::Selectable(content.c_str(), is_selected))
            {
                image_current_idx = n;
                _ToolSystemMap.SelectImage(image_current_idx, bMapPicking);
            }
                
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    if (ImGui::Button("Open Fbxfile"))
        ifd::FileDialog::Instance().Open("FbxOpenDialog", "Open a Fbx", "Fbx file (*.fbx;*.FBX){.fbx,.FBX},.*", true);
    if (ImGui::BeginListBox("listbox 2"))
    {
        for (int n = 0; n < _ToolSystemMap.m_ListFbx.size(); n++)
        {
            std::string fullpath = _towm(_ToolSystemMap.m_ListFbx[n]);

            std::string content = GetSplitName(fullpath);

            const bool is_selected = (fbx_current_idx == n);
            if (ImGui::Selectable(content.c_str(), is_selected))
            {
                fbx_current_idx = n;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    ImGui::End();

    std::string objectname;
    static float scale[3] = { 0.0f, 0.0f, 0.0f };
    static float rotation[3] = { 0.0f, 0.0f, 0.0f };
    static float position[3] = { 0.0f, 0.0f, 0.0f };
    Object* pObject;
    // Simple window
    ImGui::Begin("Control Panel2");
    {
        {
            if (_ToolSystemMap.m_pQuadTree != nullptr)
            {
                if (_ToolSystemMap.m_pQuadTree->GetPickingObject() != nullptr)
                {
                    pObject = _ToolSystemMap.m_pQuadTree->GetPickingObject();
                    XMVECTOR v_scale, v_rotation, v_translation;
                    XMMatrixDecompose(&v_scale, &v_rotation, &v_translation, pObject->constantData.matWorld);
                    scale[0] = v_scale.m128_f32[0]; scale[1] = v_scale.m128_f32[1]; scale[2] = v_scale.m128_f32[2];
                    rotation[0] = v_rotation.m128_f32[0]; rotation[1] = v_rotation.m128_f32[1]; rotation[2] = v_rotation.m128_f32[2];
                    position[0] = v_translation.m128_f32[0]; position[1] = v_translation.m128_f32[1]; position[2] = v_translation.m128_f32[2];
                }
            }
            if (ImGui::Button("Object"))
            {
               
            }
            ImGui::InputFloat3("scale", scale);
            ImGui::InputFloat3("rotation", rotation);
            ImGui::InputFloat3("position", position);
        }
        ImGui::Dummy({ 0, 10 });
    }
    ImGui::End();

    // Using the generic BeginListBox() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
    
   
    // file dialogs
    if (ifd::FileDialog::Instance().IsDone("ImageOpenDialog")) {
        if (ifd::FileDialog::Instance().HasResult()) {
            const std::vector<std::filesystem::path>& res = ifd::FileDialog::Instance().GetResults();
            for(int idx = 0; idx < res.size(); idx++)
                _ToolSystemMap.m_ListTexture.push_back(res[idx].wstring()); 
            //for (const auto& r : res) // ShaderOpenDialog supports multiselection
            //{
            //   printf("OPEN[%s]\n", r.u8string().c_str()); 
            //}
        }
        ifd::FileDialog::Instance().Close();
    }


    // file dialogs
    if (ifd::FileDialog::Instance().IsDone("FbxOpenDialog")) {
        if (ifd::FileDialog::Instance().HasResult()) {
            const std::vector<std::filesystem::path>& res = ifd::FileDialog::Instance().GetResults();
            for (int idx = 0; idx < res.size(); idx++)
                _ToolSystemMap.m_ListFbx.push_back(res[idx].wstring());
            //for (const auto& r : res) // ShaderOpenDialog supports multiselection
            //{
            //   printf("OPEN[%s]\n", r.u8string().c_str()); 
            //}
        }
        ifd::FileDialog::Instance().Close();
    }

    /*if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
        if (ifd::FileDialog::Instance().HasResult()) {
            std::string res = ifd::FileDialog::Instance().GetResult().u8string();
            printf("DIRECTORY[%s]\n", res.c_str());
        }
        ifd::FileDialog::Instance().Close();
    }*/

    if (ifd::FileDialog::Instance().IsDone("ShaderSaveDialog")) {
        if (ifd::FileDialog::Instance().HasResult()) {
            std::wstring res = ifd::FileDialog::Instance().GetResult().wstring();
            _ToolSystemMap.SaveFile(res);
            //printf("SAVE[%s]\n", res.c_str());
        }
        ifd::FileDialog::Instance().Close();
    }

}

void ImguiSystem::Render()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

ImguiSystem::ImguiSystem()
{
    std::cout << "Initialize : ImguiSystem" << std::endl;
    m_show_demo_window = true;
    m_show_another_window = false;
    m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // ImFileDialog requires you to set the CreateTexture and DeleteTexture
    ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
        /*GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (void*)tex;*/
        return nullptr;
    };
    ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
        /*GLuint texID = (GLuint)((uintptr_t)tex);
        glDeleteTextures(1, &texID);*/
        return nullptr;
    };
}

ImguiSystem::~ImguiSystem()
{
    std::cout << "Release : ImguiSystem" << std::endl;
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

