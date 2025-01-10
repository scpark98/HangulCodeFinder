
// HangulCodeFinderDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "HangulCodeFinder.h"
#include "HangulCodeFinderDlg.h"
#include "afxdialogex.h"

#include <thread>
#include "../Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHangulCodeFinderDlg 대화 상자



CHangulCodeFinderDlg::CHangulCodeFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HANGULCODEFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHangulCodeFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_UTF8, m_check_utf8);
	DDX_Control(pDX, IDC_COMBO_EXTENSION, m_combo_extension);
	DDX_Control(pDX, IDC_COMBO_EXCEPT, m_combo_except);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_COMBO_EDITOR, m_combo_editor);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_STATIC_MATCHED, m_static_matched);
}

BEGIN_MESSAGE_MAP(CHangulCodeFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHangulCodeFinderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHangulCodeFinderDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CHangulCodeFinderDlg::OnNMRClickList)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_UTF8, &CHangulCodeFinderDlg::OnBnClickedCheckUtf8)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTENSION, &CHangulCodeFinderDlg::OnCbnSelchangeComboExtension)
	ON_CBN_EDITCHANGE(IDC_COMBO_EXCEPT, &CHangulCodeFinderDlg::OnCbnEditchangeComboExcept)
	ON_CBN_SELCHANGE(IDC_COMBO_EDITOR, &CHangulCodeFinderDlg::OnCbnSelchangeComboEditor)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHangulCodeFinderDlg::OnNMDblclkList)
END_MESSAGE_MAP()


// CHangulCodeFinderDlg 메시지 처리기

BOOL CHangulCodeFinderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);

	m_resize.Add(IDC_STATIC_ENCODING, 100, 0, 0, 0);
	m_resize.Add(IDC_CHECK_UTF8, 100, 0, 0, 0);

	m_resize.Add(IDC_LIST, 0, 0, 100, 100);
	m_resize.Add(IDC_SLIDER, 0, 100, 100, 0);
	m_resize.Add(IDC_STATIC_MATCHED, 0, 100, 0, 0);
	m_resize.Add(IDC_STATIC_EDITOR, 0, 100, 0, 0);
	m_resize.Add(IDC_COMBO_EDITOR, 0, 100, 0, 0);
	m_resize.Add(IDOK, 100, 100, 0, 0);
	m_resize.Add(IDCANCEL, 100, 100, 0, 0);

	m_combo_extension.load_history(&theApp, _T("setting\\m_combo_extension"));
	m_combo_except.load_history(&theApp, _T("setting\\m_combo_except"));

	m_combo_extension.add(_T("h;cpp"));
	m_combo_extension.add(_T("js;vue;env"));
	if (m_combo_extension.GetCurSel() < 0)
		m_combo_extension.SetCurSel(0);

	m_check_utf8.SetCheck(theApp.GetProfileInt(_T("setting"), _T("m_check_utf8"), true));

	m_slider.set_style(CSCSliderCtrl::style_progress_line);

	init_combo_editor();

	init_list();

	DragAcceptFiles();

	RestoreWindowPosition(&theApp, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHangulCodeFinderDlg::init_combo_editor()
{
	m_dq_editor.push_back(_T("C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\IDE\\devenv.exe"));
	m_dq_editor.push_back(_T("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\devenv.exe"));
	m_dq_editor.push_back(_T("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe"));
	m_dq_editor.push_back(get_known_folder(CSIDL_LOCAL_APPDATA) + _T("\\Programs\\Microsoft VS Code\\Code.exe"));
	m_dq_editor.push_back(_T("C:\\Program Files\\JetBrains\\IntelliJ IDEA 2023.3.2\\bin\\idea64.exe"));
	m_dq_editor.push_back(_T("C:\\Program Files\\Notepad++\\notepad++.exe"));

	m_combo_editor.AddString(_T("Visual Studio 2015"));
	m_combo_editor.AddString(_T("Visual Studio 2019"));
	m_combo_editor.AddString(_T("Visual Studio 2022"));
	m_combo_editor.AddString(_T("VSCode"));
	m_combo_editor.AddString(_T("IntelliJ"));
	m_combo_editor.AddString(_T("Notepad++"));
	m_combo_editor.AddString(_T("Default text editor"));
	//m_combo_editor.AddString(_T("Select user editor..."));

	int index = theApp.GetProfileInt(_T("setting"), _T("m_combo_editor index"), 0);

	if (index < 0 || index >= m_combo_editor.GetCount())
		index = 0;

	m_combo_editor.SetCurSel(index);
}

void CHangulCodeFinderDlg::init_list()
{
	m_list.set_headings(_T("파일명,100;라인,60;문자열,200;경로,300"));
	m_list.set_font_name(theApp.GetProfileString(_T("m_list"), _T("font name"), _T("맑은 고딕")));

	m_list.set_header_height(24);
	m_list.set_line_height(20);

	m_list.set_column_data_type(col_linenum, CVtListCtrlEx::column_data_type_numeric);
	m_list.set_column_text_align(col_linenum, LVCFMT_RIGHT);

	m_list.set_back_alt_color(Gdiplus::Color::LightGray);
	m_list.load_column_width(&theApp, _T("m_list"));
}

void CHangulCodeFinderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHangulCodeFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHangulCodeFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHangulCodeFinderDlg::OnBnClickedOk()
{
	m_list.delete_all_items();

	bool is_utf8 = m_check_utf8.GetCheck();

	//drag&drop이 아닌 폴더를 찾아서 확인을 누른 경우
	if (m_files.size() == 0)
	{
		m_list.set_text_on_empty(_T("검색 결과가 존재하지 않습니다.\n대상 확장자, 인코딩 등을 확인한 후 검색하고자 하는 파일/폴더를 드롭하면 검색이 시작됩니다."));
		return;
	}

	m_slider.SetRange(0, m_files.size());
	m_slider.ShowWindow(SW_SHOW);

	for (int i = 0; i < m_files.size(); i++)
	{
		m_slider.SetPos(i + 1);

		if (m_files[i].Find(_T("KoinoViewer.cpp")) >= 0)
		{
			m_slider.SetPos(i + 1);
		}

		FILE* fp = NULL;

		int text_encoding = text_encoding_ansi;

		if (is_utf8)
		{
			_tfopen_s(&fp, m_files[i], _T("rt")CHARSET);
			text_encoding = text_encoding_unicode;
		}
		else
		{
			text_encoding = file_open(&fp, _T("rt"), m_files[i]);
		}

		if (text_encoding == text_encoding_unknown)
		{
			TRACE(_T("fail to open %s\n"), m_files[i]);
			continue;
		}

		char	cline[2048] = { 0, };
		TCHAR	tline[2048] = { 0, };
		CString sline;
		CString temp;
		int		line = 0;
		int		index;
		bool	multiline_comment = false;

		while (true)
		{
			if (text_encoding == text_encoding_ansi)
			{
				if (fgets(cline, 2048, fp) == 0)
					break;
				sline = cline;
			}
			else
			{
				if (_fgetts(tline, 2048, fp) == 0)
					break;
				sline = tline;
			}

			line++;

			sline.Trim();

			if (sline.IsEmpty())
				continue;

			if (multiline_comment)
			{
				int end_comment_pos = sline.Find(_T("*/"));
				if (end_comment_pos < 0)
					continue;

				sline = sline.Mid(end_comment_pos + 2).Trim();
				multiline_comment = false;

				if (sline.IsEmpty())
					continue;
			}

			//주석처리된 라인이면 스킵
			CString comment = is_comment(sline);
			if (!comment.IsEmpty())
			{
				if (comment == _T("/*"))
					multiline_comment = true;
				continue;
			}

			temp = sline;

			//문자열 내에서만 한글이 포함되어 있는지 검사한다.
			while (true)
			{
				bool double_quote = true;

				//문자열 시작 위치 찾기. 겹따옴표, 홀따옴표 
				int dq_start = temp.Find(_T("\""));
				if (dq_start < 0)
				{
					dq_start = temp.Find(_T("'"));
					if (dq_start < 0)
						break;

					double_quote = false;
				}

				temp = temp.Mid(dq_start + 1);
				dq_start = 0;

				//문자열 끝 위치 찾기
				int dq_end = temp.Find((double_quote ? _T("\"") : _T("'")));
				if (dq_end < 0)
					break;

				int hangul_count = 0;

				if (is_hangul(temp.Mid(dq_start, dq_end - dq_start), true, &hangul_count) && (hangul_count > 0))
				{
					TRACE(_T("file = %s, %d line. %s\n"), m_files[i], line, temp);
					index = m_list.insert_item(-1, 0, get_part(m_files[i], fn_name), i2S(line), sline, get_part(m_files[i], fn_folder));
					break;
				}
				temp = temp.Mid(dq_end + 1);
			}
		}

		fclose(fp);
	}

	m_static_matched.set_textf(-1, _T("총 %d 파일. 결과 %s건"), m_files.size(), i2S(m_list.size(), true));
	m_slider.ShowWindow(SW_HIDE);
}


void CHangulCodeFinderDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_extension.save_history(&theApp, _T("setting\\m_combo_extension"));
	m_combo_except.save_history(&theApp, _T("setting\\m_combo_except"));

	CDialogEx::OnCancel();
}


void CHangulCodeFinderDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list.save_column_width(&theApp, _T("m_list"));
	SaveWindowPosition(&theApp, this);
}


void CHangulCodeFinderDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];
	UINT drop_num = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	m_files.clear();
	m_dropList.clear();
	m_list.delete_all_items();
	m_slider.SetRange(0, 100);
	m_slider.SetPos(0);


	//하나의 폴더가 드롭되면 해당 폴더내의 모든 폴더+파일들을 리스팅하는 것이므로
	//m_droppedFolder는 해당 폴더까지의 fullpath가 된다.
	//두개 이상의 파일 또는 폴더가 드롭되면 해당 폴더들이 존재하는 폴더가 m_droppedFolder가 된다.
	for (int i = 0; i < drop_num; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);
		m_dropList.push_back(sfile);
	}

	if (m_dropList.size() == 0)
		return;

	if (drop_num == 1)
	{
		m_droppedFolder = m_dropList[0];
	}
	else
	{
		m_droppedFolder = GetParentDirectory(m_dropList[0]);
	}

	if (!IsFolder(m_droppedFolder))
		m_droppedFolder = get_part(m_droppedFolder, fn_folder);

	std::thread th(&CHangulCodeFinderDlg::thread_make_list, this);
	th.detach();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CHangulCodeFinderDlg::thread_make_list()
{
	int i;
	long t0 = clock();
	CString str;
	FILE* fp = NULL;
	CString ext_filters;
	CString except_lists;

	m_combo_extension.GetWindowText(ext_filters);
	m_combo_except.GetWindowText(except_lists);

	m_files.clear();

	m_slider.SetRange(0, m_dropList.size());

	for (i = 0; i < m_dropList.size(); i++)
	{
		m_slider.SetPos(i + 1);

		std::deque<CString> files;

		if (PathIsDirectory(m_dropList[i]))
		{
			//약 2,000개 파일 검색 소요시간
			//FindAllFiles	: 2,902ms
			//find_all_files: 163ms (94%정도 시간단축)
			if (false)
			{
				//FindAllFiles(m_dropList[i], &m_files, _T(""), _T(""), true);
			}
			else
			{
				//files = find_all_files(m_dropList[i]);
				FindAllFiles(m_dropList[i], &files, _T("*"), ext_filters, true, false, except_lists);
				m_files.insert(m_files.end(), files.begin(), files.end());
			}
		}
		else
		{
			m_files.push_back(m_dropList[i]);
		}
	}

	for (i = 0; i < m_files.size(); i++)
		TRACE(_T("%d = %s\n"), i, m_files[i]);

	m_slider.SetPos(0);

	if (m_files.size() == 0)
	{
	}
	else if (m_files.size() == 1)
	{
		str.Format(_T("한글 코드 검색 - %s"), m_files[0]);
		SetWindowText(str);
	}
	else
	{
		str.Format(_T("한글 코드 검색 - %s 외 %d 파일"), m_files[0], m_files.size() - 1);
		SetWindowText(str);
	}

	m_static_matched.set_textf(-1, _T("총 %d파일."), m_files.size());

	OnBnClickedOk();
}

void CHangulCodeFinderDlg::OnNMRClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int item = -1;// = pNMItemActivate->iItem;
	int subItem = -1;// = pNMItemActivate->iSubItem;	<== invalid index returned when user clicked out of columns

	if (!m_list.get_index_from_point(pNMItemActivate->ptAction, item, subItem, false) ||
		item < 0 || subItem < 0)
		return;

	TRACE(_T("rclick = %d, %d\n"), item, subItem);
	m_list.set_back_color(item, subItem, Gdiplus::Color::RoyalBlue);
	Wait(200);
	m_list.set_back_color(item, subItem, Gdiplus::Color::RoyalBlue, true);

	copy_to_clipboard(m_hWnd, m_list.get_text(item, subItem));

	*pResult = 0;
}


void CHangulCodeFinderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list.save_column_width(&theApp, _T("m_list"));
}


void CHangulCodeFinderDlg::OnBnClickedCheckUtf8()
{
	theApp.WriteProfileInt(_T("setting"), _T("m_check_utf8"), m_check_utf8.GetCheck());
}


void CHangulCodeFinderDlg::OnCbnSelchangeComboExtension()
{
	int index = m_combo_extension.GetCurSel();
	theApp.WriteProfileInt(_T("setting"), _T("m_combo_extension index"), index);


}


void CHangulCodeFinderDlg::OnCbnEditchangeComboExcept()
{
	CString text;
	m_combo_except.GetWindowText(text);
	theApp.WriteProfileString(_T("setting"), _T("m_combo_except string"), text);
}


void CHangulCodeFinderDlg::OnCbnSelchangeComboEditor()
{
	int index = m_combo_editor.GetCurSel();

	if (index < 0 || index >= m_combo_editor.GetCount())
		return;


	//직접 에디터를 선택할 수 있다.
	theApp.WriteProfileInt(_T("setting"), _T("m_combo_editor index"), index);
}


void CHangulCodeFinderDlg::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_list.get_selected_index();
	int editor_index = m_combo_editor.GetCurSel();
	CString line = m_list.get_text(index, col_linenum);
	CString file = concat_path(m_list.get_text(index, col_folder), m_list.get_text(index, col_filename));

	CString param;


	if (!PathFileExists(m_dq_editor[editor_index]))
	{
		AfxMessageBox(m_dq_editor[editor_index] + _T("\n\n위 경로에 에디터가 존재하지 않습니다. 다른 에디터를 선택하세요."));
		return;
	}


	switch (editor_index)
	{
	case editor_vs2015:
	case editor_vs2019:
	case editor_vs2022:
		//"/Edit"을 붙이면 기존 실행중인 vs에 표시되나 goto가 안먹고  안붙이면 goto가 동작한다;
		param.Format(_T("\"%s\" /Command \"edit.goto %s\""), file, line);
		break;
	case editor_vscode:
		param.Format(_T("--goto \"%s:%s:0\""), file, line);
		break;
	case editor_intellij:
		param.Format(_T("--line %s \"%s\""), line, file);
		break;
	case editor_notepadpp:
		param.Format(_T("\"%s\" -n%s"), file, line);
		break;
	default:
		param = file;
	}

	if (!param.IsEmpty())
		ShellExecute(m_hWnd, _T("open"), m_dq_editor[editor_index], param, 0, SW_SHOWNORMAL);

	*pResult = 0;
}


BOOL CHangulCodeFinderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_DELETE:
			if (GetFocus() == &m_list)
			{
				m_list.delete_selected_items();
				return true;
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
