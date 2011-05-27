#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// you should consider CriticalSectionLocker whenever possible instead of
// directly working with CriticalSection class - it is safer
class CriticalSection
{
public:
	void Initialize() 	{ m_critRegion.OwningThread = 0; InitializeCriticalSection(&m_critRegion); }
	void Delete() 	  	{ DeleteCriticalSection(&m_critRegion); }

	// enter the section
	void Enter() 	  	
	{ 
		ULONG_PTR ownerThreadId = (ULONG_PTR)m_critRegion.OwningThread;
		UNREFERENCED_PARAMETER(ownerThreadId);
		EnterCriticalSection(&m_critRegion);
	}

	// try enter the section
	bool TryEnter()   	{ return (TryEnterCriticalSection(&m_critRegion) != 0); }

	// leave the critical section
	void Leave() 	  	{ LeaveCriticalSection(&m_critRegion); }

private:
	CRITICAL_SECTION m_critRegion;
};

class CriticalSectionLocker
{
public:
	CriticalSectionLocker(CriticalSection& cs)
		: m_critsect(cs)
	{
		m_critsect.Enter();
	}

	~CriticalSectionLocker()
	{
		m_critsect.Leave();
	}
	
	CriticalSectionLocker & operator=( const CriticalSectionLocker & ) {}

private:
	CriticalSection& m_critsect;
};