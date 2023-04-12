// DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDemoDlg dialog
EEPROM_IC Type[9] = {S24C02,S24C04,S24C08,S24C16,S24C32,S24C64,S24C128,S24C256,S24C512};

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PID, m_PID);
	DDX_Text(pDX, IDC_VID, m_VID);
	DDX_Text(pDX, IDC_DSIZE, m_EEP_Size);
	DDV_MaxChars(pDX, m_EEP_Size, 5);
	DDX_Text(pDX, IDC_ADDR, m_EEP_Adress);
	DDV_MaxChars(pDX, m_EEP_Adress, 5);
	DDX_Text(pDX, IDC_DATA, m_EEP_Data);
	DDX_Text(pDX, IDC_REG_DSIZE, m_Reg_Size);
	DDV_MaxChars(pDX, m_Reg_Size, 2);
	DDX_Text(pDX, IDC_REG_ADDR, m_Reg_Adress);
	DDV_MaxChars(pDX, m_Reg_Adress, 2);
	DDX_Text(pDX, IDC_REG_DATA, m_Reg_Data);

	DDX_Control(pDX, IDC_MSG, m_MsgBox);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MONITOR, &CDemoDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_READ, &CDemoDlg::OnBnClickedEEPRead)
	ON_BN_CLICKED(IDC_WRITE, &CDemoDlg::OnBnClickedEEPWrite)
	ON_CBN_SELCHANGE(IDC_COMBOTYPE, &CDemoDlg::OnCbnSelchangeCombotype)
	ON_BN_CLICKED(IDC_REG_READ, &CDemoDlg::OnBnClickedRegRead)
	ON_BN_CLICKED(IDC_REG_WRITE, &CDemoDlg::OnBnClickedRegWrite)
	
END_MESSAGE_MAP()


// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_VID = _T("0d8c");
	m_PID = _T("01af");
	UpdateData(0);

	GetDlgItem(IDC_MONITOR)->SetWindowText(_T("Start"));
	((CComboBox*)GetDlgItem(IDC_COMBOTYPE))->SetCurSel(4);

	m_CHidControl.SetHWND(GetSafeHwnd());
	m_Monitor = FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if( m_Monitor)
	{
		m_CHidControl.CloseDevice();
		GetDlgItem(IDC_MONITOR)->SetWindowText(_T("Start"));
		m_Monitor=FALSE;
	}	

}


void CDemoDlg::OnBnClickedStart()
{
	if(!m_Monitor)
	{
		UpdateData(TRUE);
		WORD VID=(WORD)TranslateToNum(m_VID);
		WORD PID=(WORD)TranslateToNum(m_PID);
		
		if( m_CHidControl.OpenDevice( VID, PID ) && m_CHidControl.StartReadThread() )
		{
			m_MsgBox.InsertString(0,_T("Start Success!!"));
			GetDlgItem(IDC_MONITOR)->SetWindowText(_T("Stop"));
			
			int iPos=((CComboBox*)GetDlgItem(IDC_COMBOTYPE))->GetCurSel();
			m_CHidControl.SetEEPROMSize(Type[iPos]);

			m_Monitor=TRUE;
		}
		else
		{
			m_MsgBox.InsertString(0,_T("Start Fail ,Please check usb device!!"));
		}
	}
	else
	{
		
		m_CHidControl.CloseDevice();
		GetDlgItem(IDC_MONITOR)->SetWindowText(_T("Start"));
		m_Monitor=FALSE;
	}
	
	return;
}


void CDemoDlg::OnBnClickedEEPRead()
{
	UpdateData(TRUE);

	DWORD   dwAddress=TranslateToNum(m_EEP_Adress);
	DWORD   dwSize   =TranslateToNum(m_EEP_Size);

	BYTE*  pReadData =NULL;
	TCHAR* pString =NULL;
	CString str, tmp;

	if( dwSize<=0 )
		return;
	
	pReadData =new BYTE[dwSize];
	pString =new TCHAR[dwSize];

	if( NULL!=pReadData && NULL!=pString )
	{
		memset( pReadData, 0, dwSize );
		memset( pString, 0, dwSize*sizeof(TCHAR) );
	}
	else
		return;

	if( m_CHidControl.EEPROMAccess( FALSE, dwAddress, pReadData, dwSize)  )
	{
		for(int i=0; i<dwSize; i++)
		{	
			tmp.Format( _T("%02X "), pReadData[i] );

			str +=tmp ;
			if( 0==(i+1)%16 && 0!=i )
				str +=_T("\r\n");
		}
		m_EEP_Data=str;	
		m_MsgBox.InsertString(0,_T("Read EEPROM Success!!"));
	}
	else
	{
		m_MsgBox.InsertString(0,_T("Read EEPROM Fail!!"));
	}

	SAFE_DELETE_ARRAY(pReadData);
	SAFE_DELETE_ARRAY(pString);

	UpdateData(FALSE);
}

void CDemoDlg::OnBnClickedEEPWrite()
{
	UpdateData(TRUE);

	BYTE* pWriteData =NULL;

	DWORD   dwAddress=TranslateToNum(m_EEP_Adress);
	DWORD   dwSize   =TranslateToNum(m_EEP_Size);

	pWriteData =new BYTE[dwSize];
	if( NULL!=pWriteData )
		memset( pWriteData, 0, dwSize );
	else 
		return;
	
	StringHexToByteArray( m_EEP_Data, pWriteData );

	if( m_CHidControl.EEPROMAccess( TRUE, dwAddress, pWriteData, dwSize ) )
	{
		m_MsgBox.InsertString(0,_T("Write EEPROM Success!!"));
	}
	else
	{
		m_MsgBox.InsertString(0,_T("Write EEPROM Fail!!"));
	}
	
	m_EEP_Size.Format( _T("%x"), dwSize );

	SAFE_DELETE_ARRAY(pWriteData);

	UpdateData(FALSE);
}

void CDemoDlg::OnCbnSelchangeCombotype()
{
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBOTYPE))->GetCurSel();
	m_CHidControl.SetEEPROMSize(Type[iPos]);
}


void CDemoDlg::OnBnClickedRegWrite()
{
	
	UpdateData(TRUE);
	BYTE* pWriteData =NULL;

	DWORD dwAddress=TranslateToNum(m_Reg_Adress);
	DWORD dwSize =TranslateToNum(m_Reg_Size);

	if( dwSize<=0 )
		return;

	pWriteData =new BYTE[dwSize];

	if( NULL!=pWriteData )
		memset( pWriteData, 0, dwSize );
	else
		return;

	StringHexToByteArray(m_Reg_Data, pWriteData);

	if( m_CHidControl.RegisterAccess(TRUE, dwAddress, pWriteData, dwSize) )
	{
		m_MsgBox.InsertString(0,_T("Write Reg Success!!"));
	}
	else
	{
		m_MsgBox.InsertString(0,_T("Write Reg Fail!!"));
	}


	SAFE_DELETE_ARRAY(pWriteData );
	
	UpdateData(FALSE);
}



void CDemoDlg::OnBnClickedRegRead()
{
	UpdateData(TRUE);

	BYTE* pReadData =NULL;
	
	DWORD   dwAddress=TranslateToNum(m_Reg_Adress);
	DWORD   dwSize   =TranslateToNum(m_Reg_Size);
	
	CString str, tmp;

	if( dwSize<=0 )
		return;

	pReadData =new BYTE[dwSize];

	if( NULL!=pReadData )
		memset( pReadData, 0, dwSize );
	else
		return;

	if( m_CHidControl.RegisterAccess(FALSE, dwAddress, pReadData, dwSize) )
	{
		for( int i=0; i<dwSize; ++i)
		{
			tmp.Format( _T("%02X "), pReadData[i] );

			str += tmp;
			if( 0==(i+1)%16 && 0!=i )
				str +=_T("\r\n");		
		}
		m_Reg_Data =str;
		m_MsgBox.InsertString(0,_T("Read Reg Success!!"));
	}
	else
	{
		m_MsgBox.InsertString(0,_T("Read Reg Fail!!"));
	}

	SAFE_DELETE_ARRAY(pReadData );

	UpdateData(FALSE);
}

