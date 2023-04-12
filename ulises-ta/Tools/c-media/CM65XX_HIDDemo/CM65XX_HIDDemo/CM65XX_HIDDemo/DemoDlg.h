// DemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "HidControl.h"

// CDemoDlg dialog
class CDemoDlg : public CDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEEPRead();
	afx_msg void OnBnClickedEEPWrite();
	afx_msg void OnCbnSelchangeCombotype();
	
	afx_msg void OnBnClickedRegRead();
	afx_msg void OnBnClickedRegWrite();
	void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_Monitor;
	CString m_PID;
	CString m_VID;
	CString m_EEP_Size;
	CString m_EEP_Adress;
	CString m_EEP_Data;	
	CString m_Reg_Size;
	CString m_Reg_Adress;
	CString m_Reg_Data;	
	CListBox m_MsgBox;
	
	HidControl m_CHidControl;
};
