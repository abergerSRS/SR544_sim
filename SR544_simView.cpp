
// SR544_simView.cpp : implementation of the CSR544simView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SR544_sim.h"
#endif

#include "MainFrm.h"
#include "SR544_simDoc.h"
#include "SR544_simView.h"
#include "instrument.h"
#include "motorDrive.h"
#include "frontpanel.h"
#include "display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSR544simView

IMPLEMENT_DYNCREATE(CSR544simView, CFormView)

BEGIN_MESSAGE_MAP(CSR544simView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSR544simView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_SYNC, &CSR544simView::OnBnClickedBtnSync)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SRC, &CSR544simView::OnBnClickedBtnSrc)
	ON_BN_CLICKED(IDC_BTN_CTRL, &CSR544simView::OnBnClickedBtnCtrl)
	ON_BN_CLICKED(IDC_SELECT_L, &CSR544simView::OnBnClickedSelectL)
	ON_BN_CLICKED(IDC_SELECT_R, &CSR544simView::OnBnClickedSelectR)
END_MESSAGE_MAP()

// CSR544simView construction/destruction

CSR544simView::CSR544simView() noexcept
	: CFormView(IDD_SR544_SIM_FORM)
{
	// TODO: add construction code here

}

CSR544simView::~CSR544simView()
{
}

void CSR544simView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RISING_EDGE, mRisingEdge);
	DDX_Control(pDX, IDC_FALLING_EDGE, mFallingEdge);
	DDX_Control(pDX, IDC_SINE, mSineEdge);
	DDX_Control(pDX, IDC_INT_RATE, mIntRate);
	DDX_Control(pDX, IDC_VCO_IN, mVcoIn);
	DDX_Control(pDX, IDC_AC_LINE, mAcLine);
	DDX_Control(pDX, IDC_EXT_SYNC, mExtSync);
	DDX_Control(pDX, IDC_SHAFT, mShaft);
	DDX_Control(pDX, IDC_INNER, mInner);
	DDX_Control(pDX, IDC_OUTER, mOuter);
	DDX_Control(pDX, IDC_MULT_NM, mMult_NM);
	DDX_Control(pDX, IDC_MULT_1, mMult_1);
	DDX_Control(pDX, IDC_MON_OUTER, mMonOuter);
	DDX_Control(pDX, IDC_MON_INNER, mMonInner);
	DDX_Control(pDX, IDC_MON_SHAFT, mMonShaft);
	DDX_Control(pDX, IDC_MON_SRCE, mMonSource);
	DDX_Control(pDX, IDC_SET_INT, mSetIntRate);
	DDX_Control(pDX, IDC_SET_PHASE, mSetPhase);
	DDX_Control(pDX, IDC_SET_MULTN, mSetMultN);
	DDX_Control(pDX, IDC_SET_DIVM, mSetDivM);
	DDX_Control(pDX, IDC_SET_VCOFS, mSetVCOFS);
	DDX_Control(pDX, IDC_UNITS_HZ, mUnitsHz);
	DDX_Control(pDX, IDC_UNITS_DEG, mUnitsDeg);
	DDX_Control(pDX, IDC_UNITS_N, mUnitsN);
	DDX_Control(pDX, IDC_UNITS_RELPHASE, mUnitsRel);
}

BOOL CSR544simView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSR544simView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// SubclassDlgItem connects the CSevenSegment segN to the static display identified by IDC_SEGN
	seg0.SubclassDlgItem(IDC_SEG0, this); 
	seg1.SubclassDlgItem(IDC_SEG1, this);
	seg2.SubclassDlgItem(IDC_SEG2, this);
	seg3.SubclassDlgItem(IDC_SEG3, this);
	seg4.SubclassDlgItem(IDC_SEG4, this);
	seg5.SubclassDlgItem(IDC_SEG5, this);

	SetTimer(1000, 10, NULL);

	initInstrument();
	initFrontPanel();
	updateDisplay();	// updates instrument model
	refreshDisplay();	// simulates the hardware refresh (actually pushes changes to FP)

	seg0.setValue(0x00);
	seg1.setValue(0x00);
	seg2.setValue(0x00);
	seg3.setValue(0x00);
	seg4.setValue(0x00);
	seg5.setValue(0x00);
}


// CSR544simView printing


void CSR544simView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSR544simView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSR544simView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSR544simView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSR544simView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CSR544simView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSR544simView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSR544simView diagnostics

#ifdef _DEBUG
void CSR544simView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSR544simView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSR544simDoc* CSR544simView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSR544simDoc)));
	return (CSR544simDoc*)m_pDocument;
}
#endif //_DEBUG


// CSR544simView message handlers

void CSR544simView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000) {
		// updateDisplay()
		if (isDisplayDirty()) {
			refreshDisplay();
		}		
	}

	CFormView::OnTimer(nIDEvent);
}

void CSR544simView::appendOutput(CString &str)
{
	CMainFrame *pFrame = (CMainFrame *)GetParentFrame();
	COutputWnd *pOutput = pFrame->getOutputWnd();
	pOutput->appendDebug(str);
}

void CSR544simView::refreshDisplay(void)
{	
	// refresh Sync Edge LEDs
	mRisingEdge.SetCheck(getLED(LED_SYNC_RISE));
	mFallingEdge.SetCheck(getLED(LED_SYNC_FALL));
	mSineEdge.SetCheck(getLED(LED_SYNC_SINE));

	// refresh Source LEDs
	mIntRate.SetCheck(getLED(LED_SRC_INT));
	mVcoIn.SetCheck(getLED(LED_SRC_VCO));
	mAcLine.SetCheck(getLED(LED_SRC_AC));
	mExtSync.SetCheck(getLED(LED_SRC_EXT));

	// refresh Control LEDs
	mShaft.SetCheck(getLED(LED_CTRL_SHAFT));
	mInner.SetCheck(getLED(LED_CTRL_INNER));
	mOuter.SetCheck(getLED(LED_CTRL_OUTER));

	// refresh Multiplier LEDs
	mMult_NM.SetCheck(getLED(LED_MULT_NM));
	mMult_1.SetCheck(getLED(LED_MULT_1));

	// refresh Main Display
	mMonOuter.SetCheck(getLED(LED_MON_OUTER));
	mMonInner.SetCheck(getLED(LED_MON_INNER));
	mMonShaft.SetCheck(getLED(LED_MON_SHAFT));
	mMonSource.SetCheck(getLED(MD_MON_SOURCE));
	mSetIntRate.SetCheck(getLED(MD_SET_INTRATE));
	mSetPhase.SetCheck(getLED(MD_SET_PHASE));
	mSetMultN.SetCheck(getLED(MD_SET_MULTN));
	mSetDivM.SetCheck(getLED(MD_SET_DIVM));
	mSetVCOFS.SetCheck(getLED(MD_SET_VCOFS));

	// refresh Units
	mUnitsHz.SetCheck(getLED(LED_HZ));
	mUnitsDeg.SetCheck(getLED(LED_DEG));
	mUnitsN.SetCheck(getLED(LED_N));
	mUnitsRel.SetCheck(getLED(LED_REL));

	// refresh 7-seg displays
	seg0.setValue(getSegDigit(0));
	seg1.setValue(getSegDigit(1));
	seg2.setValue(getSegDigit(2));
	seg3.setValue(getSegDigit(3));
	seg4.setValue(getSegDigit(4));
	seg5.setValue(getSegDigit(5));
	
	DisplayIsClean();
}

void CSR544simView::OnBnClickedBtnSync()
{
	// TODO: Add your control notification handler code here
	// Example for writing Debug output 
	/*
	OutputDebugString(_T("Sync clicked\n"));
	*/

	// Example for writing Debug output to Main Form window
	/*
	CString str(_T("Hello"));
	str += " World";
	str.Format(_T("%s -> This is a number: %d"), str, 5);

	appendOutput(CString(_T("Send to output")));
	appendOutput(str);
	*/

	onButton(BTN_SYNCEDGE);
}

void CSR544simView::OnBnClickedBtnSrc()
{
	onButton(BTN_SOURCE);
}


void CSR544simView::OnBnClickedBtnCtrl()
{
	onButton(BTN_CONTROL);
}


void CSR544simView::OnBnClickedSelectL()
{
	onButton(BTN_SELECT_L);
}


void CSR544simView::OnBnClickedSelectR()
{
	onButton(BTN_SELECT_R);
}
