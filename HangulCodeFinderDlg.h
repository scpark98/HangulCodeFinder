
// HangulCodeFinderDlg.h: 헤더 파일
//

#pragma once

#include "../Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"
#include "../Common/ResizeCtrl.h"
#include "../Common/CSliderCtrl/SCSliderCtrl/SCSliderCtrl.h"
#include "../Common/CStatic/SCStatic/SCStatic.h"
#include "../Common/CComboBox/SCComboBox/SCComboBox.h"

// CHangulCodeFinderDlg 대화 상자
class CHangulCodeFinderDlg : public CDialogEx
{
// 생성입니다.
public:
	CHangulCodeFinderDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CResizeCtrl		m_resize;

	enum ENUM_LIST_COLUMN
	{
		//m_list.set_headings(_T("파일명,100;라인,60;문자열,200;경로,200"));
		col_filename = 0,
		col_linenum,
		col_string,
		col_folder,
	};

	void		init_list();

	enum ENUM_EDITOR
	{
		editor_vs2015 = 0,
		editor_vs2019,
		editor_vs2022,
		editor_vscode,
		editor_intellij,
		editor_notepadpp,
		editor_default,
		editor_custom,
	};

	std::deque<CString>		m_files;
	std::deque<CString>		m_dropList;
	CString					m_droppedFolder;
	void					thread_make_list();


	std::deque<CString>		m_dq_editor;
	void					init_combo_editor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HANGULCODEFINDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonBrowseFolder();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnNMRClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckUtf8();
	afx_msg void OnCbnSelchangeComboExtension();
	afx_msg void OnCbnEditchangeComboExcept();
	afx_msg void OnCbnSelchangeComboEditor();
	afx_msg void OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CButton m_check_utf8;
	CSCComboBox m_combo_extension;
	CSCComboBox m_combo_except;
	CVtListCtrlEx m_list;
	CSCComboBox m_combo_editor;
	CSCSliderCtrl m_slider;
	CSCStatic m_static_matched;
};
