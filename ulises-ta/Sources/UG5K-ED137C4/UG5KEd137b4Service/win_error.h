// win_error.h: interface for the win_error class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN_ERROR_H__83E2A587_D438_46FA_B050_171353892B1F__INCLUDED_)
#define AFX_WIN_ERROR_H__83E2A587_D438_46FA_B050_171353892B1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
 #include <windows.h>
#else
#endif

#include <stdexcept> 
#include <string> 
#include <sstream>

/** */
class win_error : public std::runtime_error
{
private:
    unsigned long m_error;
    mutable std::string m_msg;
protected:
    virtual const char *Prefix() const
    {
#ifdef _WIN32
        return "_WIN32";
#else
	return "LINUX";
#endif
    }

public:
    win_error(const char *s="")
#ifdef _WIN32
		:runtime_error(s), m_error(GetLastError())
#else
		:runtime_error(s), m_error(0)		// todo. obtener ultimo error en linux
#endif
    {
    }

    win_error(int lastError, const char *s="")
        :runtime_error(s), m_error(lastError)
    {
    }

	~win_error() throw()
	{
	}

    int GetCode() const throw()
    {
        return m_error;
    }

    virtual const char *what() const throw()
    {
        try
        {
            if (m_msg.empty())
            {
#ifdef _WIN32
                std::ostringstream os;
                os << Prefix() << "(" << m_error << ") " << runtime_error::what();
                char buf[256];
                if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, m_error, 0, buf, sizeof(buf), 0))
                {
                    os << ": " << buf;
                }
                m_msg = os.str();
#else
				// todo.
#endif
            }
            
            return m_msg.c_str();
        }
        catch (...)
        {
            return "error on error";
        }
    }

};

/** */
inline void TestBool(bool val, const char *txt="")
{
    if (!val)
        throw win_error(txt);
}

#ifdef _WIN32
	inline void TestHandle(HANDLE val, const char *txt="")
	{
		if (!val || val==INVALID_HANDLE_VALUE)
			throw win_error(txt);
	}
#endif

	/** */
inline void TestPointer(void *val, const char *txt="")
{
    if (!val)
        throw win_error(txt);
}

#ifdef _WIN32
	inline void TestHResult(HRESULT val, const char *txt="")
	{
		if (FAILED(val))
			throw win_error(val, txt);
	}
#endif

#ifdef _MMRESULT_
	inline void TestMMResult(MMRESULT val, const char *txt="")
	{
		if (val!=0)
			throw win_error(val, txt);

	}
#endif //_MMRESULT_

template<class T> void TestError(T val, T err, const char *txt="")
{
    if (val==err)
        throw win_error(txt);
}



#endif // !defined(AFX_WIN_ERROR_H__83E2A587_D438_46FA_B050_171353892B1F__INCLUDED_)
