// ViewMeetings.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewMeetings.h"

#include "DialogNewMeeting.h"
#include "ViewCharts.h"
#include "DialogFilterMeetings.h"

#include <Shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp ;

/////////////////////////////////////////////////////////////////////////////
// CViewMeetings

IMPLEMENT_DYNCREATE(CViewMeetings, CFormView)

CViewMeetings::CViewMeetings()
	: CFormView(CViewMeetings::IDD)
{
	//{{AFX_DATA_INIT(CViewMeetings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCompanyID = -1;
	m_From.SetStatus(COleDateTime::invalid);
	m_To.SetStatus(COleDateTime::invalid);
	m_nRec = -1;
	m_nDec = -1;
	m_nSort = 0;
}

CViewMeetings::~CViewMeetings()
{
}

void CViewMeetings::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewMeetings)
	DDX_Control(pDX, IDC_LIST_MAIN, m_ListMeetings);
	DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
	DDX_Control(pDX, IDC_COMBO_REC, m_comboRec);
	DDX_Control(pDX, IDC_COMBO_DEC, m_comboDec);
	DDX_Control(pDX, IDC_EDIT_DESC, m_editDesc);
	DDX_Control(pDX, IDC_EDIT_ANALYST, m_editAnalyst);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_comboCompany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewMeetings, CFormView)
	//{{AFX_MSG_MAP(CViewMeetings)
	ON_COMMAND(ID_MEETINGS_DELETE, OnMeetingsDelete)
	ON_COMMAND(ID_MEETINGS_FILTER, OnMeetingsFilter)
	ON_COMMAND(ID_MEETINGS_HISTORY, OnMeetingsHistory)
	ON_COMMAND(ID_MEETINGS_NEW, OnMeetingsNew)
	ON_COMMAND(ID_MEETINGS_SAVE, OnMeetingsSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, OnButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILE, OnButtonAddFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_FILE, OnButtonDeleteFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALLFILES, OnButtonDeleteAllfiles)
	ON_LBN_SELCHANGE(IDC_LIST_MAIN, OnSelchangeListMain)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_MEETINGS_COMPANY, OnMeetingsCompany)
	ON_COMMAND(ID_MEETINGS_CHART, OnMeetingsChart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewMeetings diagnostics

#ifdef _DEBUG
void CViewMeetings::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewMeetings::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewMeetings message handlers

void CViewMeetings::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();	

	// Fill up the companies combo
	int nIndex = m_comboCompany.AddString("- New Idea -");
	m_comboCompany.SetItemData(nIndex, 0);

	CArray<int,int> arIds;
	CStringArray arTickers;
	CSQLCompany::GetCompanyIDAndTickers(arIds, arTickers, theApp.m_Universe.GetDate());
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nID = arIds[i];
		CString sTicker = arTickers[i];

		nIndex = m_comboCompany.AddString(sTicker);
		m_comboCompany.SetItemData(nIndex, nID);
	}

	Load();

	if(m_ListMeetings.GetCount() > 0)
	{
		m_ListMeetings.SetCurSel(0);
		SetData();
	}
}

void CViewMeetings::OnMeetingsDelete() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}

	int nRet = AfxMessageBox("Are you sure you want to delete the Meeting information and its attached files? You cannot undo the operation.", MB_ICONQUESTION|MB_YESNO);
	if(nRet == IDNO)
		return;

	SMeeting* pM = m_arMeetings[nSel];
	if(!CSQLMeetings::DeleteMeeting(pM->id, true))
	{
		AfxMessageBox("Unable to delete the Meeting from the Data-Base. Please try again.");
		return;
	}

	AfxMessageBox("The Meeting Information is successfully deleted from the Data-Base.");
	
	Load();
	if(m_ListMeetings.GetCount() > 0)
	{
		m_ListMeetings.SetCurSel(0);
		SetData();
	}
}

void CViewMeetings::OnMeetingsFilter() 
{
	CDialogFilterMeetings dlg;
	
	dlg.m_nCompanyID = m_nCompanyID;
	dlg.m_From = m_From;
	dlg.m_To = m_To;
	dlg.m_nRec = m_nRec;
	dlg.m_nDec = m_nDec;
	dlg.m_nSort = m_nSort;
	if(dlg.DoModal() != IDOK)
		return;

	m_nCompanyID = dlg.m_nCompanyID;
	m_From = dlg.m_From;
	m_To = dlg.m_To;
	m_nRec = dlg.m_nRec;
	m_nDec = dlg.m_nDec;
	m_nSort = dlg.m_nSort;

	Load();
	if(m_ListMeetings.GetCount() > 0)
	{
		m_ListMeetings.SetCurSel(0);
		SetData();
	}
}

void CViewMeetings::FilterByCompany(const char* szTicker)
{
	int nID = CSQLCompany::GetCompanyIDByTicker(szTicker);
	if(nID == -1)
	{
		AfxMessageBox("Internal Error: Invalid Ticker.", MB_ICONINFORMATION);
		return;
	}

	m_nCompanyID = nID;
	Load();
	if(m_ListMeetings.GetCount() > 0)
	{
		m_ListMeetings.SetCurSel(0);
		SetData();
	}
}

void CViewMeetings::OnMeetingsHistory() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}

	SMeeting* pMeeting = m_arMeetings[nSel]; 
	if(!pMeeting)
		return;

	if(pMeeting->nCompanyID == 0)
	{
		AfxMessageBox("This meeting was about a 'New Idea'. No specific Company was discussed.");
		return;
	}
	
	CCompany* pCompany = NULL;
	CString sTicker = CSQLCompany::GetCompanyTickerByID(pMeeting->nCompanyID);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker);
	if(nIndex == -1)
		return;
	pCompany = theApp.m_Universe.GetCompanyByTicker(nIndex);
	if(!pCompany)
		return;

	theApp.NewViewHistory(pCompany);
}

void CViewMeetings::OnMeetingsNew() 
{
	CDialogNewMeeting dlg;
	if(dlg.DoModal() != IDOK)
		return;
	
	int nID = CSQLMeetings::UpdateMeeting(dlg.m_nCompanyID, dlg.m_Date, dlg.m_sAnalyst, dlg.m_nRecommend, dlg.m_nDecision, dlg.m_sDesc);
	if(nID == 0)
	{
		AfxMessageBox("Unable to add Meeting information into the Data-base. Please try again.");
		return;
	}

	// Add the documents
	for(int i=0; i<dlg.m_arFiles.GetSize(); i++)
	{
		if(CSQLMeetings::AddDocument(nID, dlg.m_arFiles[i], "") <= 0)
		{
			CString s; s.Format("Unable to add the file '%s' into the Data-Base. The Meeting information isn't saved completely.");
			AfxMessageBox(s);
		}
	}

	AfxMessageBox("The Meeting information and attached files are successfully added into the Data-Base.");
}

void CViewMeetings::OnMeetingsSave() 
{
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("Please select a company, or specify 'New Idea'");
		return;
	}
	int nCompanyID = m_comboCompany.GetItemData(nSel);

	CString sDate;
	m_editDate.GetWindowText(sDate);
	COleDateTime Date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(Date.GetStatus() != COleDateTime::valid)
	{
		AfxMessageBox("Please specify a valid date.");
		return;
	}

	CString sAnalyst;
	m_editAnalyst.GetWindowText(sAnalyst);
	if(sAnalyst.IsEmpty())
	{
		AfxMessageBox("Please enter Analyst Name");
		return;
	}

	int nRecommend = m_comboRec.GetCurSel();
	if(nRecommend == -1)
	{
		AfxMessageBox("Please select a 'Recommendation'.");
		return;
	}

	int nDecision = m_comboDec.GetCurSel();
	if(nDecision == -1)
	{
		AfxMessageBox("Please select a 'Decision'.");
		return;
	}

	CString sDesc;
	m_editDesc.GetWindowText(sDesc);
	if(sDesc.IsEmpty())
	{
		AfxMessageBox("Please enter a Description.");
		return;
	}

	int nID = CSQLMeetings::UpdateMeeting(nCompanyID, Date, sAnalyst, nRecommend, nDecision, sDesc);
	if(nID == 0)
	{
		AfxMessageBox("Unable to add Meeting information into the Data-base. Please try again.");
		return;
	}	
	
	AfxMessageBox("The information is successfully saved into the Data-base.");
	
	nSel = m_ListMeetings.GetCurSel();
	Load();
	if(m_ListMeetings.GetCount() > nSel)
	{
		m_ListMeetings.SetCurSel(nSel);
		SetData();
	}
}

void CViewMeetings::OnButtonOpenFile() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}
	SMeeting* pM = m_arMeetings[nSel];

	nSel = m_ListFiles.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No document is selected.");
		return;
	}
	int nID = ( (SFile*)m_ListFiles.GetItemData(nSel) )->nID;

	CString sComp, sDate, sFold;
	nSel = m_comboCompany.GetCurSel();
	m_comboCompany.GetLBText(nSel, sComp);
	sDate = pM->date.Format("%Y_%m_%d");
	sFold = sDate + "_" + sComp;

	//////////////////////////////////////////////////////////////////
	// create target folder
	//////////////////////////////////////////////////////////////////
	TCHAR szAppDataPath[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_APPDATA, false);

	BOOL bRet;
	CString sDir; 
	
	sDir.Format("%s\\Cornerstone IP", szAppDataPath);
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return;

	sDir += "\\Fair Value Model";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return;

	sDir += "\\Temp";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return;

	sDir += "\\" + sFold + "\\";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return;

	char szName[256];
	char szDesc[512];
	CString sPath = CSQLMeetings::GetDocument(nID, sDir, szName, szDesc);
	if(sPath.IsEmpty())
	{
		AfxMessageBox("Unable to download the file. Please try again.");
		return;
	}

	ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
}

void CViewMeetings::OnButtonAddFile() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}
	SMeeting* pM = m_arMeetings[nSel];

	static char szFilters[] ="All Files (*.*)|*.*|PDF Files (*.pdf)|*.pdf|Word Files (*.doc)|*.doc|Excel Files (*.xls)|*.xls||";

	CFileDialog fileDlg (true, "", "", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	if(CSQLMeetings::AddDocument(pM->id, fileDlg.GetPathName(), "") <= 0)
	{
		AfxMessageBox("Unable to add the document into the Data-Base. Please try again.");
		return;
	}

	SetData();
}

void CViewMeetings::OnButtonDeleteFile() 
{
	int nSel = m_ListFiles.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No file is selected.");
		return;
	}

	int nRet = AfxMessageBox("Are you sure you want to delete the specified document? You cannot undo this operation.", MB_ICONQUESTION|MB_YESNO);
	if(nRet == IDNO)
		return;

	int nID = ( (SFile*)m_ListFiles.GetItemData(nSel) )->nID;
	if(!CSQLMeetings::DeleteDocument(nID))
	{
		AfxMessageBox("Unablie to delete the document from the Data-Base. Please try again.");
		return;
	}

	SetData();
}

void CViewMeetings::OnButtonDeleteAllfiles() 
{
	int nRet = AfxMessageBox("Are you sure you want to delete all document for the meeting? You cannot undo this operation.", MB_ICONQUESTION|MB_YESNO);
	if(nRet == IDNO)
		return;
	
	for(int i=0; i<m_ListFiles.GetCount(); i++)
	{
		int nID = m_ListFiles.GetItemData(i);
		if(!CSQLMeetings::DeleteDocument(nID))
		{
			AfxMessageBox("Unablie to delete the document from the Data-Base. Please try again.");
			SetData();
			return;
		}	
	}

	SetData();
}

void CViewMeetings::Load()
{
	for(int i=0; i<m_arMeetings.GetSize(); i++)
		delete m_arMeetings[i];
	m_arMeetings.RemoveAll();
	m_ListMeetings.ResetContent();
	
	CSQLMeetings::GetMeetings(m_nCompanyID, m_From, m_To, m_nRec, m_nDec, m_nSort, m_arMeetings);

	for(int i=0; i<m_arMeetings.GetSize(); i++)
		m_ListMeetings.AddString((LPCTSTR)m_arMeetings[i]);
}

void CViewMeetings::SetData()
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
		return;

	SMeeting *pM = m_arMeetings[nSel];

	// CompanyID
	for(int i=0; i<m_comboCompany.GetCount(); i++)
	{
		if(pM->nCompanyID == (int)m_comboCompany.GetItemData(i))
		{
			m_comboCompany.SetCurSel(i);
			break;
		}
	}

	// Date
	m_editDate.SetWindowText(pM->date.Format("%m/%d/%Y"));

	// Analyst Name
	m_editAnalyst.SetWindowText(pM->szAnalyst);

	// Recommendation
	for(int i=0; i<m_comboRec.GetCount(); i++)
	{
		if(pM->nRecommendation == i)
		{
			m_comboRec.SetCurSel(i);
			break;
		}
	}

	// Decision
	for(int i=0; i<m_comboDec.GetCount(); i++)
	{
		if(pM->nDecision == i)
		{
			m_comboDec.SetCurSel(i);
			break;
		}
	}

	// Description
	m_editDesc.SetWindowText(pM->szDesc);

	// documents
	for(int i=0; i<m_ListFiles.GetCount(); i++)
		delete ( (SFile*)m_ListFiles.GetItemData(i) );
	m_ListFiles.ResetContent();
	

	CArray<int,int> arIds;
	CStringArray arNames;
	int nCount = CSQLMeetings::GetDocumentsForMeeting(pM->id, arIds, arNames);
	for(int i=0; i<nCount; i++)
	{
		SFile* pS = new SFile;
		pS->sName = arNames[i];
		pS->nID = arIds[i];
		
		m_ListFiles.AddString((LPCTSTR)pS);
	}
}

void CViewMeetings::OnSelchangeListMain() 
{
	SetData();	
}

void CViewMeetings::OnDestroy() 
{
	CFormView::OnDestroy();
	
	for(int i=0; i<m_arMeetings.GetSize(); i++)
		delete m_arMeetings[i];
	m_arMeetings.RemoveAll();

	for(int i=0; i<m_ListFiles.GetCount(); i++)
		delete ( (SFile*)m_ListFiles.GetItemData(i) );
	m_ListFiles.ResetContent();


	
}

HBRUSH CViewMeetings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CAPTION)
	{
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		strcpy(lf.lfFaceName,"Courier New");
		lf.lfHeight = (long)16;
		lf.lfWeight = 400;
		CFont font; font.CreateFontIndirect(&lf);
		pDC->SelectObject(&font);


		static CBrush br(RGB(0,0,0));
		return br;

	}

	return hbr;
}

void CViewMeetings::OnMeetingsCompany() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}

	SMeeting* pMeeting = m_arMeetings[nSel]; 
	if(!pMeeting)
		return;

	if(pMeeting->nCompanyID == 0)
	{
		AfxMessageBox("This meeting was about a 'New Idea'. No specific Company was discussed.");
		return;
	}
	
	CCompany* pCompany = NULL;
	CString sTicker = CSQLCompany::GetCompanyTickerByID(pMeeting->nCompanyID);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker);
	if(nIndex == -1)
		return;
	pCompany = theApp.m_Universe.GetCompanyByTicker(nIndex);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
}

void CViewMeetings::OnMeetingsChart() 
{
	int nSel = m_ListMeetings.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Meeting is selected.");
		return;
	}

	SMeeting* pMeeting = m_arMeetings[nSel]; 
	if(!pMeeting)
		return;

	if(pMeeting->nCompanyID == 0)
	{
		AfxMessageBox("This meeting was about a 'New Idea'. No specific Company was discussed.");
		return;
	}
	
	CCompany* pCompany = NULL;
	CString sTicker = CSQLCompany::GetCompanyTickerByID(pMeeting->nCompanyID);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker);
	if(nIndex == -1)
		return;
	pCompany = theApp.m_Universe.GetCompanyByTicker(nIndex);
	if(!pCompany)
		return;

	CViewCharts* pView = theApp.NewViewCharts();

	CString sTokens;
	sTokens.Format("%s_REPV %s_NEPV %s_Price", sTicker, sTicker, sTicker);

	pView->SetCharts(sTokens, "", true);	
}
