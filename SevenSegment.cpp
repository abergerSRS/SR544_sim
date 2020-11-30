// SevenSegment.cpp : implementation file
//

#include "stdafx.h"
#include "SR544_sim.h"
#include "SevenSegment.h"


// CSevenSegment

IMPLEMENT_DYNAMIC(CSevenSegment, CStatic)

CSevenSegment::CSevenSegment()
{
	val = 0x00;
	selected = false;
}

CSevenSegment::~CSevenSegment()
{
}


BEGIN_MESSAGE_MAP(CSevenSegment, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CSevenSegment message handlers




void CSevenSegment::setValue(unsigned char value)
{
	val = 0;

	if (value & 0x10)
		val |= 0x01;
	if (value & 0x20)
		val |= 0x02;
	if (value & 0x40)
		val |= 0x04;
	if (value & 0x01)
		val |= 0x08;
	if (value & 0x02)
		val |= 0x10;
	if (value & 0x08)
		val |= 0x20;
	if (value & 0x04)
		val |= 0x40;
	if (value & 0x80)
		val |= 0x80;

	this->Invalidate(FALSE);
}

void CSevenSegment::OnPaint()
{
	CPaintDC dc(this); // device context for painting	

	CRect rect;
	this->GetWindowRect(&rect);
	// Subtrack off border width
	rect.bottom = rect.bottom - 3;
	rect.right = rect.right - 3;
	dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));

	int segWidth = rect.Width() - 13;
	int segHeight = rect.Height() - 13;
	int segTop = 4;
	int segLeft = 6;

	CPen selectedGreenPen(PS_SOLID, 3, RGB(0, 175, 0));
	CPen onGreenPen(PS_SOLID, 3, RGB(0, 255, 0));
	CPen offGreenPen(PS_SOLID, 3, RGB(0, 50, 0));

	CBrush selectedGreenBrush(RGB(0, 175, 0));
	CBrush onGreenBrush(RGB(0, 255, 0));
	CBrush offGreenBrush(RGB(0, 100, 0));

	CPen *oldPen = dc.SelectObject(&offGreenPen);

	//if (type == SEVENSEG) {
		if (val & 0x01) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.MoveTo(segLeft, segTop);
		dc.LineTo(segLeft + segWidth, segTop);

		if (val & 0x02) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft + segWidth - 1, segTop + segHeight / 2);

		if (val & 0x04) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft + segWidth - 2, segTop + segHeight);

		if (val & 0x08) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft - 2, segTop + segHeight);

		if (val & 0x10) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft - 1, segTop + segHeight / 2);

		if (val & 0x20) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft, segTop);

		if (val & 0x40) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.MoveTo(segLeft - 1, segTop + segHeight / 2);
		dc.LineTo(segLeft + segWidth - 1, segTop + segHeight / 2);
	//}
	/*
	else { // PLUSMINUS_1
		if (val & 0x02) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.MoveTo(segLeft + segWidth, segTop);
		dc.LineTo(segLeft + segWidth - 1, segTop + segHeight / 2);

		if (val & 0x04) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.LineTo(segLeft + segWidth - 2, segTop + segHeight);

		if (val & 0x20) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.MoveTo(segLeft + segWidth / 2 - 2, segTop + 1);
		dc.LineTo(segLeft + segWidth / 2 - 3, segTop + segHeight / 2 - 3);
		dc.MoveTo(segLeft + 1, segTop + segHeight / 4 - 1);
		dc.LineTo(segLeft + segWidth - 6, segTop + segHeight / 4 - 1);

		if (val & 0x40) {
			if (selected)
				dc.SelectObject(&selectedGreenPen);
			else
				dc.SelectObject(&onGreenPen);
		}
		else
			dc.SelectObject(&offGreenPen);
		dc.MoveTo(segLeft + 1, segTop + segHeight / 2);
		dc.LineTo(segLeft + segWidth - 6, segTop + segHeight / 2);
	}
	*/

	CBrush *oldBrush = dc.SelectObject(&offGreenBrush);
	if (val & 0x80) {
		if (selected) {
			dc.SelectObject(&selectedGreenBrush);
			dc.SelectObject(&selectedGreenPen);
		}
		else {
			dc.SelectObject(&onGreenBrush);
			dc.SelectObject(&onGreenPen);
		}
	}
	else {
		dc.SelectObject(&offGreenBrush);
		dc.SelectObject(&offGreenPen);
	}
	dc.Ellipse(CRect(segLeft + segWidth + 2, segTop + segHeight + 2,
		segLeft + segWidth + 5, segTop + segHeight + 5));

	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
	// Do not call CStatic::OnPaint() for painting messages
}
