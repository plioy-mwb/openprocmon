#include <conio.h>
#include <atltime.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "../../sdk/procmonsdk/sdk.hpp"

 

class CMyEvent : public IEventCallback
{
public:
	virtual BOOL DoEvent(const CRefPtr<CEventView> pEventView)
	{

		time_t when		= pEventView->GetStartTime().QuadPart;
		CString name	= pEventView->GetProcessName();
		PLOG_ENTRY plog = pEventView->GetPreEventEntry();
		CString disp	= StrMapOperation(plog);
	
		time_t t		= time(&when);
		tm* tmm			= localtime(&t);
		std::wstringstream wss;
		wss << std::put_time(tmm, L"%F %T");

		if (!name.Compare(L"node.exe"))
			LogMessage(L_INFO, TEXT("%s | %s |%-36s| %s "), wss.str().c_str(), name.GetBuffer(), disp, pEventView->GetPath().GetBuffer());

		//m_viewList.push_back(pEventView);
		return TRUE;
	}
};


int main()
{

	CEventMgr&			Optmgr				= Singleton<CEventMgr>::getInstance();
	CMonitorContoller&	Monitormgr			= Singleton<CMonitorContoller>::getInstance();
	CDrvLoader&			Drvload				= Singleton<CDrvLoader>::getInstance();
	
	if (!Drvload.Init(TEXT("PROCMON24"), TEXT("procmon.sys"))) {
		LogMessage(L_ERROR, TEXT("Cannot load procmon driver"));
		return -1;
	}

	Optmgr.RegisterCallback(new CMyEvent);

	if (!Monitormgr.Connect()){
		LogMessage(L_ERROR, TEXT("Cannot connect to procmon driver"));
		return -1;
	}
	
	Monitormgr.SetMonitor(TRUE, TRUE, TRUE);
	if (!Monitormgr.Start()){
		LogMessage(L_ERROR, TEXT("Cannot start the mointor"));
		return -1;
	}

	int ch=_getch();

	LogMessage(L_INFO, TEXT("!!!!!exit monitor!!!!"));

	/*

	Monitormgr.Stop();

	LogMessage(L_INFO, TEXT("!!!!!monitor stop press any key to start!!!!"));
	_getch();

	Monitormgr.Start();

	_getch();
	*/
	Monitormgr.Stop();
	Monitormgr.Destory();

	return 0;
}
