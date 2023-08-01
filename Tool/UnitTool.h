#pragma once
#include "afxwin.h"

#include "Include.h"
// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnListBox();
	afx_msg void OnCreateUnit();
	
public:
	// �ƹ� �ǹ� ���� ����
	CString m_strTemp;
	CString m_strSrc;
	CString m_strName;
	int m_iHp;
	int m_iAttack;

public:
	CListBox m_ListBox;

public:
	map<CString, UNITDATA*>		m_mapUnitData;

};
