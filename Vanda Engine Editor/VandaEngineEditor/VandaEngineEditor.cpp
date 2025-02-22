//Copyright (C) 2024 Ehsan Kamrani 
//This file is licensed and distributed under MIT license
// VandaEngine.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VandaEngineEditor.h"
#include "VandaEngineEditorDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVandaEngineApp

BEGIN_MESSAGE_MAP(CVandaEngineApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVandaEngineApp construction

CVandaEngineApp::CVandaEngineApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVandaEngineApp object

CVandaEngineApp theApp;


// CVandaEngineApp initialization

BOOL CVandaEngineApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	SetRegistryKey(_T("Vanda Engine"));
	CVandaEngineDlg dlg;
	m_pMainWnd = &dlg;
	ex_pVandaEngineDlg = &dlg;
	AfxInitRichEdit2();
	CCommandLineInfo oInfo;
	ParseCommandLine(oInfo);
	// DON'T display a new MDI child window during startup!!!
	//oInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	//if (!ProcessShellCommand(oInfo))
	//	return FALSE;
	if(!oInfo.m_strFileName.IsEmpty())
	{
		g_openVINFile = CTrue;
		Cpy( g_fileNameInCommandLine, oInfo.m_strFileName.GetBuffer( oInfo.m_strFileName.GetLength() ) );
		oInfo.m_strFileName.ReleaseBuffer();

		std::string fullStr( GetCommandLine()  );
		std::string subStr;
		subStr.append( "''" );
		subStr.append(oInfo.m_strFileName.GetString());
		subStr.append( "''" );
		std::string tempStr = fullStr.substr( 0, fullStr.length() - subStr.length() );
		CChar tempStrChar[MAX_NAME_SIZE];
		Cpy( tempStrChar, tempStr.c_str() );
		CChar* afterPath = GetAfterPath( tempStrChar );
		std::string afterPathStr( afterPath);
		std::string finalStr = tempStr.substr(0, tempStr.length() - afterPathStr.length() - 1);
		finalStr.erase(0, 1);
		SetCurrentDirectory( finalStr.c_str() );
	}

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}


	return FALSE;
}
