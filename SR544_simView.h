
// SR544_simView.h : interface of the CSR544simView class
//
#include "SevenSegment.h"
#include "instrument.h"

#pragma once


class CSR544simView : public CFormView
{
protected: // create from serialization only
	CSR544simView() noexcept;
	DECLARE_DYNCREATE(CSR544simView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SR544_SIM_FORM };
#endif

// Attributes
public:
	CSR544simDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSR544simView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	// Sync Edge Check Boxes
	CButton mRisingEdge;
	CButton mFallingEdge;
	CButton mSineEdge;
	// Source Check Boxes
	CButton mIntRate;
	CButton mVcoIn;
	CButton mAcLine;
	CButton mExtSync;
	// Control Check Boxes
	CButton mShaft;
	CButton mInner;
	CButton mOuter;
	// Multiplier Check Boxes
	CButton mMult_NM;
	CButton mMult_1;
	// Monitor Check Boxes
	CButton mMonOuter;
	CButton mMonInner;
	CButton mMonShaft;
	CButton mMonSource;
	// Settings Check Boxes
	CButton mSetIntRate;
	CButton mSetPhase;
	CButton mSetMultN;
	CButton mSetDivM;
	CButton mSetVCOFS;
	// Units Check Boxes
	CButton mUnitsHz;
	CButton mUnitsDeg;
	CButton mUnitsN;
	CButton mUnitsRel;

	CButton mLedNumEntry;

	// 7 segment displays
	CSevenSegment seg[6];

	// Plus/Minus LEDs
	CStatic mLedPlus;
	CStatic mLedMinus;
	HBITMAP onPlus_bmp; 
	HBITMAP offPlus_bmp;
	HBITMAP onMinus_bmp;
	HBITMAP offMinus_bmp;

	// Motor LEDs
	CButton mLedRun;
	CButton mLedStop;
	CButton mChopLck;
	CButton mChopUnlck;
	
	// Buttons
	afx_msg void OnBnClickedBtnSync();
	afx_msg void OnBnClickedBtnSrc();	
	afx_msg void OnBnClickedBtnCtrl();	
	afx_msg void OnBnClickedSelectL();
	afx_msg void OnBnClickedSelectR();
	afx_msg void OnBnClickedBtnRel();
	afx_msg void OnBnClickedBtnRunstop();
	// Numeric Entry
	afx_msg void OnBnClickedBtn0();
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
	afx_msg void OnBnClickedBtn4();
	afx_msg void OnBnClickedBtn5();
	afx_msg void OnBnClickedBtn6();
	afx_msg void OnBnClickedBtn7();
	afx_msg void OnBnClickedBtn8();
	afx_msg void OnBnClickedBtn9();
	afx_msg void OnBnClickedBtnDp();
	afx_msg void OnBnClickedBtnPm();
	afx_msg void OnBnClickedBtnEnter();
	afx_msg void OnBnClickedBtnBkspc();
	afx_msg void OnBnClickedBtnCancel();	
	// Save/Recall Configuration
	afx_msg void OnBnClickedBtnStore();
	afx_msg void OnBnClickedBtnRecall();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void appendOutput(CString &str);
	void refreshDisplay(void);
	void refresh7seg(void);
	void refreshPlusMinus(void);	
	
	afx_msg void OnEnChangeRemterm();
	CString m_RemTerm;
	afx_msg void OnBnClickedSendRem();
	CButton mSendRem;
	CEdit m_RemTermCtrl;
	CButton mUsbComm;
	CButton mErrLed;
};

#ifndef _DEBUG  // debug version in SR544_simView.cpp
inline CSR544simDoc* CSR544simView::GetDocument() const
   { return reinterpret_cast<CSR544simDoc*>(m_pDocument); }
#endif

