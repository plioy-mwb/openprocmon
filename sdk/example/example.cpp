// sdktest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <conio.h>
#include <atltime.h>
#include "../../sdk/procmonsdk/sdk.hpp"

class CMyEvent : public IEventCallback
{
public:
	virtual BOOL DoEvent(const CRefPtr<CEventView> pEventView)
	{

		ULONGLONG Time = pEventView->GetStartTime().QuadPart;
		CString name = pEventView->GetProcessName();
		PLOG_ENTRY plog = pEventView->GetPreEventEntry();
		CString disp = StrMapOperation(plog);
		if (!name.Compare(L"node.exe"))
		LogMessage(L_INFO, TEXT("%llu | %s |%-36s| %s "),
			Time,
			name.GetBuffer(),
			disp, //pEventView->GetEventOperator(),
			pEventView->GetPath().GetBuffer()
		);
		//m_viewList.push_back(pEventView);
		return TRUE;
	}
};


int main()
{

	CEventMgr&			Optmgr				= Singleton<CEventMgr>::getInstance();
	CMonitorContoller&	Monitormgr			= Singleton<CMonitorContoller>::getInstance();
	CDrvLoader&			Drvload				= Singleton<CDrvLoader>::getInstance();
	
	if(!Drvload.Init(TEXT("PROCMON24"), TEXT("procmon.sys"))){
		return -1;
	}
	Optmgr.RegisterCallback(new CMyEvent);

	//
	// Try to connect to procmon driver
	//
	
	if (!Monitormgr.Connect()){
		LogMessage(L_ERROR, TEXT("Cannot connect to procmon driver"));
		return -1;
	}
	
	//
	// try to start monitor
	//
	
	Monitormgr.SetMonitor(TRUE, TRUE, FALSE);
	if (!Monitormgr.Start()){
		LogMessage(L_ERROR, TEXT("Cannot start the mointor"));
		return -1;
	}

	_getch();
	
	//
	// try to stop the monitor
	//
	
	Monitormgr.Stop();

	LogMessage(L_INFO, TEXT("!!!!!monitor stop press any key to start!!!!"));
	_getch();

	Monitormgr.Start();

	_getch();

	Monitormgr.Stop();
	Monitormgr.Destory();

	return 0;
}
