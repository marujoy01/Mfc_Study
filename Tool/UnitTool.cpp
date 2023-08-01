// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strSrc(_T(""))
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
{

}

CUnitTool::~CUnitTool()
{
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();

}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strSrc);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);

	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)

	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)

	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnCreateUnit)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);	// ���̾�α� �ڽ��κ��� �Էµ� ���� ����

	m_strSrc = m_strTemp;

	UpdateData(FALSE);	// ������ ����� ������ ���̾�α� �ڽ��� �ݿ�

}


void CUnitTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : ����Ʈ �ڽ����� ������ ����� �ε����� ��ȯ

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : �ش� �ε����� ���ڿ��� ������ �Լ�
	m_ListBox.GetText(iIndex, strFindName);

	auto		iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp	  = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);
	
	UpdateData(FALSE);

}


void CUnitTool::OnCreateUnit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	UNITDATA*	pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iHp = m_iHp;
	pUnit->iAttack = m_iAttack;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;



	// AddString : ����Ʈ �ڽ��� ���ڿ��� �߰�
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}



BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		bBitmap = (HBITMAP)LoadImage(nullptr, 
												L"../Texture/JusinLogo1.bmp",
												IMAGE_BITMAP, 
												100, 
												50,
												LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_Bitmap.SetBitmap(bBitmap);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
