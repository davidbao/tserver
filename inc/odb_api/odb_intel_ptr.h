#ifndef _ODB_INTEL_PTR
#define _ODB_INTEL_PTR

#include <vector>

template <class T> class CIntelPtr
{
private:
	T *m_Ptr;
	CIntelPtr(const CIntelPtr &);
	CIntelPtr& operator=(const CIntelPtr &);
public:
	CIntelPtr(){m_Ptr = NULL;}
	void SetPtr(T* t_ptr)
	{
		if(m_Ptr !=NULL)
		{
			delete m_Ptr;
		}
		m_Ptr = t_ptr;
	}
	~CIntelPtr()
	{
		delete m_Ptr;
	}
};

template <class T> class CIntelPtrArray
{
private:
	std::vector<T *>m_PtrArray;
	CIntelPtrArray(const CIntelPtrArray &);
	CIntelPtrArray& operator =(const CIntelPtrArray&);
public:
	CIntelPtrArray(){}
	void  push_back(T* t_ptr)
	{
		m_PtrArray.push_back(t_ptr);
		return;
	}
	~CIntelPtrArray()
	{
		int i;
		int vec_size = m_PtrArray.size();
		
		for(i=0;i<vec_size;i++)
		{
			delete m_PtrArray[i];
		}
	}

	T* operator[](int pos)
	{
		if(pos>=0 && pos<(int)(m_PtrArray.size()))
			return m_PtrArray[pos];
		else 
			return NULL;
	}

	int GetSize()
	{
		return m_PtrArray.size();
	}
};

#endif
