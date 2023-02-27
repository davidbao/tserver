
#ifndef	__PUB_BUF_H__
#define	__PUB_BUF_H__

#include <cstdlib>
#include <cstring>

#ifdef  _WINDOWS32
#ifdef PUB_EXPORTS
#define PUB_API __declspec(dllexport)
#else
#define PUB_API __declspec(dllimport)
#endif
#else 
#define PUB_API
#endif

class PUB_API CBuffer
{
public:
	CBuffer();
	~CBuffer();
	
	//
	// consumer interface
	//
	inline char* GetBufPtr() const { return m_BufPtr; }
	inline int   GetLength() const { return m_DataLen; }
	inline int   GetBuffer(char** buf_ptr, int& buf_size) const 
	{
		*buf_ptr = m_BufPtr;
		buf_size = m_DataLen;
		return 0;
	}
	
        char* Detach();
        static void Free(void* pointer);

	
	//
	// producer interface
	//
	int SetBuffer(const char* buf_ptr, const int buf_size);
	int SetLength(const int buf_size);
	

private:
	char* m_BufPtr;
	int   m_BufLen;
	int   m_DataLen;
	int   m_Detached;
};


#endif
