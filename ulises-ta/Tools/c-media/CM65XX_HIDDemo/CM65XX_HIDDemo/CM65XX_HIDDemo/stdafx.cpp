// stdafx.cpp : source file that includes just the standard includes
// Demo.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


DWORD TranslateToNum(LPCTSTR szHex_)//only accept 4 BYTE
{
	TCHAR szHex[MAX_PATH]=_T("");
	_tcscpy_s( szHex, MAX_PATH, szHex_);
	_tcsupr_s( szHex, MAX_PATH );
	int nSize=_tcslen(szHex);
	nSize=nSize > 8 ? 8 :nSize;
	DWORD wCode=0;
	int   nHex =0;
	for(int i=0;i<nSize;i++){
		switch(szHex[i])
		{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				 wCode+=(szHex[i]-'0') <<((nSize-1-i)*4);
				 break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				 wCode+=((szHex[i]-'A')+10) <<((nSize-1-i)*4);
				 break;
		}
	}
	return wCode;
}

DWORD StringHexToByteArray(CString Str, PBYTE pData)
{
	int Len =0;
    BYTE result = 0;
    ULONG RET = 0;
    CString temp1, temp2;
    CHAR t =_T('');

    Str.MakeUpper();
    Str.TrimLeft();
    Str.TrimRight();
    Len = Str.GetLength();

    int lastPos = 0, Pos = Str.Find(' ');

    while(Pos != -1)
    {
        temp1 = Str.Mid(lastPos, Pos - lastPos);
        if(temp1 == _T(" "))
        {
            lastPos ++;
            Pos = Str.Find(' ', lastPos+1);
            continue;
        }

        result = 0;
        for(int i = 0; i < temp1.GetLength(); i++)
        {
            temp2 = temp1.Mid(i,1);
            if (temp2.FindOneOf(_T("0123456789")) != -1)
            {
                t = temp2.GetAt(0);
                t = t - '0';
                result = result * 16 + t;
            }
            if (temp2.FindOneOf(_T("ABCDEF")) != -1)
            {
                t = temp2.GetAt(0);
                t = t - 'A' + 10;
                result = result * 16 + t;
            }
        }
        *pData = result;
        pData++;
        RET++;
        lastPos = Pos;
        Pos = Str.Find(' ', lastPos+1);
    }

    if (Str.GetLength() > 0 && Len != lastPos)
    {
        temp1 = Str.Mid(lastPos, Len - lastPos);
        result = 0;
        for(int i = 0; i < temp1.GetLength(); i++)
        {
            temp2 = temp1.Mid(i,1);
            if (temp2.FindOneOf(_T("0123456789")) != -1)
            {
                t = temp2.GetAt(0);
                t = t - '0';
                result = result * 16 + t;
            }
            if (temp2.FindOneOf(_T("ABCDEF")) != -1)
            {
                t = temp2.GetAt(0);
                t = t - 'A' + 10;
                result = result * 16 + t;
            }
        }
        *pData = result;
        pData++;
        RET++;
    }

    return RET;
}
