#pragma once
/*SystemBlock*/
#include "WindowSystem.h"
#include "EngineSystem.h"
#include "InputSystem.h"
#include "ImguiSystem.h"
#include "ObjectManager.h"
#include "CameraSystem.h"

class EntrySystem : public WindowSystem
{
public:
	/*Winodw Event*/
	// WindowSystem을(를) 통해 상속됨
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnFocus() override;
	virtual void OnKillFocus() override;
	virtual void OnSize() override;
	virtual void OnDestroy() override;
	virtual LRESULT MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	void Update();
	void Render();

	/*Constructor Block*/
public:
	EntrySystem();
	~EntrySystem();
	
	/*Variable Block*/
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	ConstantBuffer* m_pConstantBuffer;
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	
	Texture* m_pTexture;
	Camera* m_pCamera;
	Object* m_pObject;
};

