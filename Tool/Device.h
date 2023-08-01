#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)
private:
	CDevice();
	~CDevice();
	// 1. 장치 정보를 조사할 객체를 생성
	// 2. 1번에서 생성한 객체로 장치의 지원 수준을 조사
	// 3. 그래픽 장치를 제어할 객체를 생성

public:
	HRESULT			Initialize();
	void			Render_Begin();
	void			Render_End(HWND hWnd = nullptr);
	void			Release();

private:
	void			Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice;

};

