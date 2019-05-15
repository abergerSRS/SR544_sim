#pragma once


// CSevenSegment

class CSevenSegment : public CStatic
{
	DECLARE_DYNAMIC(CSevenSegment)

public:
	CSevenSegment();
	virtual ~CSevenSegment();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	unsigned char val;
	void setValue(unsigned char value);
	
};


