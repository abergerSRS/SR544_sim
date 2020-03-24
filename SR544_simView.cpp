
// SR544_simView.cpp : implementation of the CSR544simView class
//

#include "config.h"
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
#include "motor.h"
#include "frontpanel.h"
#include "display.h"
#include "flash_kinetis.h"
#include "hardware.h"
#include "remoteIfx.h"
#include "parser.h"

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
	ON_BN_CLICKED(IDC_BTN_0, &CSR544simView::OnBnClickedBtn0)
	ON_BN_CLICKED(IDC_BTN_1, &CSR544simView::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2, &CSR544simView::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3, &CSR544simView::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN_4, &CSR544simView::OnBnClickedBtn4)
	ON_BN_CLICKED(IDC_BTN_5, &CSR544simView::OnBnClickedBtn5)
	ON_BN_CLICKED(IDC_BTN_6, &CSR544simView::OnBnClickedBtn6)
	ON_BN_CLICKED(IDC_BTN_7, &CSR544simView::OnBnClickedBtn7)
	ON_BN_CLICKED(IDC_BTN_8, &CSR544simView::OnBnClickedBtn8)
	ON_BN_CLICKED(IDC_BTN_9, &CSR544simView::OnBnClickedBtn9)
	ON_BN_CLICKED(IDC_BTN_DP, &CSR544simView::OnBnClickedBtnDp)
	ON_BN_CLICKED(IDC_BTN_PM, &CSR544simView::OnBnClickedBtnPm)
	ON_BN_CLICKED(IDC_BTN_ENTER, &CSR544simView::OnBnClickedBtnEnter)
	ON_BN_CLICKED(IDC_BTN_BKSPC, &CSR544simView::OnBnClickedBtnBkspc)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CSR544simView::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_REL, &CSR544simView::OnBnClickedBtnRel)
	ON_BN_CLICKED(IDC_BTN_RUNSTOP, &CSR544simView::OnBnClickedBtnRunstop)
	ON_BN_CLICKED(IDC_BTN_STORE, &CSR544simView::OnBnClickedBtnStore)
	ON_BN_CLICKED(IDC_BTN_RECALL, &CSR544simView::OnBnClickedBtnRecall)
	ON_EN_CHANGE(IDC_REMTERM, &CSR544simView::OnEnChangeRemterm)
	ON_BN_CLICKED(IDC_SEND_REM, &CSR544simView::OnBnClickedSendRem)
END_MESSAGE_MAP()

// CSR544simView construction/destruction

CSR544simView::CSR544simView() noexcept
	: CFormView(IDD_SR544_SIM_FORM)
{
	// TODO: add construction code here
	onPlus_bmp = LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LED_PLUSON));
	offPlus_bmp = LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LED_PLUSOFF));
	onMinus_bmp = LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LED_MINUSON));
	offMinus_bmp = LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_LED_MINUSOFF));
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
	DDX_Control(pDX, IDC_LED_PLUS, mLedPlus);
	DDX_Control(pDX, IDC_LED_MINUS, mLedMinus);
	DDX_Control(pDX, IDC_RUN, mLedRun);
	DDX_Control(pDX, IDC_STOP, mLedStop);
	DDX_Control(pDX, IDC_LED_NUMENTRY, mLedNumEntry);
	DDX_Control(pDX, IDC_CHOP_LCK, mChopLck);
	DDX_Control(pDX, IDC_CHOP_UNLCK, mChopUnlck);
	DDX_Control(pDX, IDC_SEND_REM, mSendRem);
	DDX_Control(pDX, IDC_REMTERM, m_RemTermCtrl);
	DDX_Control(pDX, IDC_USB_LED, mUsbComm);
	DDX_Control(pDX, IDC_ERR_LED, mErrLed);
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
	seg[0].SubclassDlgItem(IDC_SEG0, this); 
	seg[1].SubclassDlgItem(IDC_SEG1, this);
	seg[2].SubclassDlgItem(IDC_SEG2, this);
	seg[3].SubclassDlgItem(IDC_SEG3, this);
	seg[4].SubclassDlgItem(IDC_SEG4, this);
	seg[5].SubclassDlgItem(IDC_SEG5, this);

	mLedPlus.SetBitmap(offPlus_bmp);
	mLedMinus.SetBitmap(offMinus_bmp);
			
	SetTimer(1000, 10, NULL); // SetTimer(timer ID, interval (ms), NULL)

	InitializeInstrument();
	InitializeFrontPanel();

	// initialize all 9 memory locations to the default startup state
	/*
	for (int i = 1; i <= 9; i++) {
		saveCurrentConfig(i);
	}
	*/
	
	UpdateDisplay();	// updates instrument model display variables
	refreshDisplay();	// simulates the hardware refresh (actually pushes changes to FP)
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
		UpdateDisplayTimers();
		if (IsDisplayDirty()) {
			refreshDisplay();
		}
		UpdateHardware();
	}

	CString str(_T(""));
	if (!IsTxEmpty(Usb)) {
		while (!IsTxEmpty(Usb))
		{
			str.AppendChar(TransmitCharacter(Usb));
		}
		appendOutput(str);
		// OutputDebugString(str);
		StartUsbBusy();
	}	

	CFormView::OnTimer(nIDEvent);
}

void CSR544simView::appendOutput(CString &str)
{
	CMainFrame *pFrame = (CMainFrame *)EnsureParentFrame();
	COutputWnd *pOutput = pFrame->getOutputWnd();
	pOutput->appendDebug(str);
}

void CSR544simView::refreshDisplay(void)
{	
	/*----------------------------------------------------
	This function will be replaced by SPI communication 
	to front panel hardware
	----------------------------------------------------*/

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
	mUsbComm.SetCheck(getLED(LED_USB));

	// refresh plus/minus
	refreshPlusMinus();

	// refresh Units
	mUnitsHz.SetCheck(getLED(LED_HZ));
	mUnitsDeg.SetCheck(getLED(LED_DEG));
	mUnitsN.SetCheck(getLED(LED_N));
	mUnitsRel.SetCheck(getLED(LED_REL));

	// refresh motor LEDs
	mLedRun.SetCheck(getLED(LED_RUN));
	mLedStop.SetCheck(getLED(LED_STOP));
	mChopLck.SetCheck(getLED(LED_CHOP_LCK));
	mChopUnlck.SetCheck(getLED(LED_CHOP_ULCK));

	// refresh Numeric Entry LED
	mLedNumEntry.SetCheck(getLED(LED_NUMENTRY));

	// refresh Usb Comm LED
	mUsbComm.SetCheck(getLED(LED_USB));

	// referesh Error LED
	mErrLed.SetCheck(getLED(LED_ERR));

	// refresh 7-seg displays
	refresh7seg();
	
	DisplayIsClean();
}

void CSR544simView::refresh7seg(void) {
	for (int i = 0; i < NUM_DIGITS; i++) {
		seg[i].setValue(getSegDigit(i));
	}
}

void CSR544simView::refreshPlusMinus(void) {
	if (getLED(LED_PLUS)) {
		mLedPlus.SetBitmap(onPlus_bmp);		
	} else {
		mLedPlus.SetBitmap(offPlus_bmp);
	}

	if (getLED(LED_MINUS)) {
		mLedMinus.SetBitmap(onMinus_bmp);
	} else {
		mLedMinus.SetBitmap(offMinus_bmp);
	}
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


void CSR544simView::OnBnClickedBtn0()
{
	onButton(BTN_0);
}


void CSR544simView::OnBnClickedBtn1()
{
	onButton(BTN_1);
}


void CSR544simView::OnBnClickedBtn2()
{
	onButton(BTN_2);
}


void CSR544simView::OnBnClickedBtn3()
{
	onButton(BTN_3);
}


void CSR544simView::OnBnClickedBtn4()
{
	onButton(BTN_4);
}


void CSR544simView::OnBnClickedBtn5()
{
	onButton(BTN_5);
}


void CSR544simView::OnBnClickedBtn6()
{
	onButton(BTN_6);
}


void CSR544simView::OnBnClickedBtn7()
{
	onButton(BTN_7);
}


void CSR544simView::OnBnClickedBtn8()
{
	onButton(BTN_8);
}


void CSR544simView::OnBnClickedBtn9()
{
	onButton(BTN_9);
}


void CSR544simView::OnBnClickedBtnDp()
{
	onButton(BTN_POINT);
}


void CSR544simView::OnBnClickedBtnPm()
{
	onButton(BTN_PLUS_MINUS);
}


void CSR544simView::OnBnClickedBtnEnter()
{
	onButton(BTN_ENTER);
}


void CSR544simView::OnBnClickedBtnBkspc()
{
	onButton(BTN_BS);
}


void CSR544simView::OnBnClickedBtnCancel()
{
	onButton(BTN_CANCEL);
}

void CSR544simView::OnBnClickedBtnRel()
{
	onButton(BTN_RELPHASE);
}


void CSR544simView::OnBnClickedBtnRunstop()
{
	onButton(BTN_RUNSTOP);
}


void CSR544simView::OnBnClickedBtnStore()
{
	onButton(BTN_STORE);
}


void CSR544simView::OnBnClickedBtnRecall()
{
	onButton(BTN_RECALL);
}


void CSR544simView::OnEnChangeRemterm()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSR544simView::OnBnClickedSendRem()
{
	CString command = _T("");
		
	m_RemTermCtrl.GetWindowTextW(command);

	int length = m_RemTermCtrl.GetWindowTextLengthW();

	for (int i = 0; i < length; i++) {
		ReceiveCharacter(Usb, toupper(command[i]));
	}
	ReceiveCharacter(Usb, '\r');
	StartUsbBusy();

	do_commands();
}
