#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)
private:
	CDevice();
	~CDevice();
	// 1. ��ġ ������ ������ ��ü�� ����
	// 2. 1������ ������ ��ü�� ��ġ�� ���� ������ ����
	// 3. �׷��� ��ġ�� ������ ��ü�� ����

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

