//Copyright (C) 2024 Ehsan Kamrani 
//This file is licensed and distributed under MIT license

// Add3DSound.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngineEditor.h"
#include "Add3DSound.h"
#include  "AFXPRIV.H"
#include "ViewScript.h"

// CAdd3DSound dialog

IMPLEMENT_DYNAMIC(CAdd3DSound, CDialog)

CAdd3DSound::CAdd3DSound(CWnd* pParent /*=NULL*/)
	: CDialog(CAdd3DSound::IDD, pParent)
{
	m_loop = m_play = CTrue;
	m_create = CFalse; //I assume the user wants to edit the dialog data at the beginning. To change it, use the SetCreate() function.
	m_editMode = CFalse;
	m_scriptUpdated = CFalse;
	m_hasScript = CFalse;
}

CAdd3DSound::~CAdd3DSound()
{
	m_strScript.ReleaseBuffer();
}

void CAdd3DSound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_X_POS, m_editBox3DSoundPosX);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_Y_POS, m_editBox3DSoundPosY);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_Z_POS, m_editBox3DSoundPosZ);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_ROLLOFF, m_editBox3DSoundRolloff);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_PITCH, m_editBox3DSoundPitch);
	DDX_Control(pDX, IDC_COMBO_3D_SOUND_LOOP, m_combo3DSoundLoop);
	DDX_Control(pDX, IDC_COMBO_3D_SOUND_PLAY, m_combo3DSoundPlay);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_DATA, m_editBox3DSoundData);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_NAME, m_editBox3DSoundName);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_REFERENCE_DISTANCE, m_editBox3DSoundReferenceDistance);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_MAX_DISTANCE, m_editBox3DSoundMaxDistance);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_VOLUME, m_editBox3DSoundVolume);
	DDX_Control(pDX, IDC_EDIT_3D_SOUND_SCRIPT, m_editBoxScript);
}

BOOL CAdd3DSound::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_create )
	{
		m_str3DSoundPosX = "0.0f";
		m_str3DSoundPosY = "2.0f";
		m_str3DSoundPosZ = "0.0f";
		m_str3DSoundRolloff = "1.0f";
		m_str3DSoundPitch = "1.0f";
		m_str3DSoundVolume = "1.0f";
		m_str3DSoundReferenceDistance = "10.0f";
		m_str3DSoundMaxDistance = "1.0f";

		CChar ThreeDSoundPath[MAX_NAME_SIZE];
		sprintf(ThreeDSoundPath, "%s%s", g_pathProperties.m_soundPath, "default3D.ogg" );
		m_str3DSoundDataPath = ThreeDSoundPath;
		m_str3DSoundPureDataPath = "default3D";
	}

	if (m_editMode)
	{
		if (m_hasScript)
		{
			m_editBoxScript.SetWindowTextA(m_strScript);
		}
	}

	m_editBox3DSoundPosX.SetWindowTextA( m_str3DSoundPosX );
	m_editBox3DSoundPosY.SetWindowTextA( m_str3DSoundPosY );
	m_editBox3DSoundPosZ.SetWindowTextA( m_str3DSoundPosZ );
	m_editBox3DSoundRolloff.SetWindowTextA( m_str3DSoundRolloff );
	m_editBox3DSoundPitch.SetWindowTextA( m_str3DSoundPitch );
	m_editBox3DSoundVolume.SetWindowTextA( m_str3DSoundVolume );
	m_editBox3DSoundReferenceDistance.SetWindowTextA( m_str3DSoundReferenceDistance );
	m_editBox3DSoundMaxDistance.SetWindowTextA( m_str3DSoundMaxDistance );
	m_editBox3DSoundName.SetWindowTextA( m_str3DSoundName);
	m_editBox3DSoundData.SetWindowTextA( m_str3DSoundDataPath);

	m_combo3DSoundPlay.InsertString( 0, "True" );
	m_combo3DSoundPlay.InsertString( 1, "False" );
	if( m_play )
		m_combo3DSoundPlay.SetCurSel( 0 );
	else
		m_combo3DSoundPlay.SetCurSel( 1 );

	m_combo3DSoundPlay.UpdateWindow();

	m_combo3DSoundLoop.InsertString( 0, "True" );
	m_combo3DSoundLoop.InsertString( 1, "False" );
	if( m_loop )
		m_combo3DSoundLoop.SetCurSel( 0 );
	else
		m_combo3DSoundLoop.SetCurSel( 1 );

	m_combo3DSoundLoop.UpdateWindow();

	m_str3DSoundTempName = m_str3DSoundName;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAdd3DSound, CDialog)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_X_POS, &CAdd3DSound::OnEnChangeEdit3DSoundXPos)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_Y_POS, &CAdd3DSound::OnEnChangeEdit3DSoundYPos)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_Z_POS, &CAdd3DSound::OnEnChangeEdit3DSoundZPos)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_PITCH, &CAdd3DSound::OnEnChangeEdit3DSoundPitch)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_ROLLOFF, &CAdd3DSound::OnEnChangeEdit3DSoundRolloff)
	ON_CBN_SELCHANGE(IDC_COMBO_3D_SOUND_LOOP, &CAdd3DSound::OnCbnSelchangeCombo3DSoundLoop)
	ON_CBN_SELCHANGE(IDC_COMBO_3D_SOUND_PLAY, &CAdd3DSound::OnCbnSelchangeCombo3DSoundPlay)
	ON_BN_CLICKED(IDC_BUTTON_3D_SOUND_DATA, &CAdd3DSound::OnBnClickedButton3DSoundData)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_NAME, &CAdd3DSound::OnEnChangeEdit3DSoundName)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_REFERENCE_DISTANCE, &CAdd3DSound::OnEnChangeEdit3DSoundReferenceDistance)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_MAX_DISTANCE, &CAdd3DSound::OnEnChangeEdit3DSoundMaxDistance)
	ON_EN_CHANGE(IDC_EDIT_3D_SOUND_VOLUME, &CAdd3DSound::OnEnChangeEdit3DSoundVolume)
	ON_BN_CLICKED(IDOK, &CAdd3DSound::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_ADD_SCRIPT, &CAdd3DSound::OnBnClickedBtnAddScript)
	ON_BN_CLICKED(IDC_BTN_REMOVE_SCRIPT, &CAdd3DSound::OnBnClickedBtnRemoveScript)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_SCRIPT, &CAdd3DSound::OnBnClickedButtonViewScript)
END_MESSAGE_MAP()


// CAdd3DSound message handlers

void CAdd3DSound::OnEnChangeEdit3DSoundName()
{
	m_editBox3DSoundName.GetWindowTextA( m_str3DSoundName );
}

void CAdd3DSound::OnEnChangeEdit3DSoundXPos()
{
	m_editBox3DSoundPosX.GetWindowTextA( m_str3DSoundPosX );
	m_f3DSoundPos[0] = atof( m_str3DSoundPosX );
}

void CAdd3DSound::OnEnChangeEdit3DSoundYPos()
{
	m_editBox3DSoundPosY.GetWindowTextA( m_str3DSoundPosY );
	m_f3DSoundPos[1] = atof( m_str3DSoundPosY );
}

void CAdd3DSound::OnEnChangeEdit3DSoundZPos()
{
	m_editBox3DSoundPosZ.GetWindowTextA( m_str3DSoundPosZ );
	m_f3DSoundPos[2] = atof( m_str3DSoundPosZ );
}

void CAdd3DSound::OnEnChangeEdit3DSoundPitch()
{
	m_editBox3DSoundPitch.GetWindowTextA( m_str3DSoundPitch );
	m_f3DSoundPitch = atof( m_str3DSoundPitch );
}

void CAdd3DSound::OnEnChangeEdit3DSoundRolloff()
{
	m_editBox3DSoundRolloff.GetWindowTextA( m_str3DSoundRolloff );
	m_f3DSoundRolloff = atof( m_str3DSoundRolloff );
}

void CAdd3DSound::OnEnChangeEdit3DSoundVolume()
{
	m_editBox3DSoundVolume.GetWindowTextA( m_str3DSoundVolume );
	m_f3DSoundVolume = atof( m_str3DSoundVolume );
}

void CAdd3DSound::OnEnChangeEdit3DSoundReferenceDistance()
{
	m_editBox3DSoundReferenceDistance.GetWindowTextA( m_str3DSoundReferenceDistance );
	m_f3DSoundReferenceDistance = atof( m_str3DSoundReferenceDistance );
}

void CAdd3DSound::OnEnChangeEdit3DSoundMaxDistance()
{
	m_editBox3DSoundMaxDistance.GetWindowTextA( m_str3DSoundMaxDistance );
	m_f3DSoundMaxDistance = atof( m_str3DSoundMaxDistance );
}

void CAdd3DSound::OnCbnSelchangeCombo3DSoundLoop()
{
	CInt curSel = m_combo3DSoundLoop.GetCurSel();

	switch( curSel )
	{
	case 0: //true;
		m_loop = CTrue;
		break;
	case 1: //false;
		m_loop = CFalse;
		break;
	}
}

void CAdd3DSound::OnCbnSelchangeCombo3DSoundPlay()
{
	CInt curSel = m_combo3DSoundPlay.GetCurSel();
	switch( curSel )
	{
	case 0: //true;
		m_play = CTrue;
		break;
	case 1: //false;
		m_play = CFalse;
		break;
	}
}

void CAdd3DSound::OnBnClickedButton3DSoundData()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgOpen(TRUE, _T("*.ogg"), _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("Ogg File (*.ogg)|*.ogg||"), NULL, NULL);
	if( IDOK == dlgOpen.DoModal() )
	{
		m_str3DSoundDataPath = (CString)dlgOpen.GetPathName();
		m_str3DSoundPureDataPath = dlgOpen.GetFileTitle();

		m_editBox3DSoundData.SetWindowText( m_str3DSoundDataPath );
	}
}

void CAdd3DSound::OnOK()
{
	CBool compare = CFalse;
	if( !m_str3DSoundName.IsEmpty() )
	{
		compare = CTrue;
		if( m_editMode )
		{
			CChar name[MAX_NAME_SIZE];
			CChar tempName[MAX_NAME_SIZE];
			Cpy(name, (LPCSTR)m_str3DSoundName);
			Cpy(tempName, (LPCSTR)m_str3DSoundTempName);

			StringToUpper(name);
			StringToUpper(tempName);

			if (Cmp(name, tempName))
				compare = CFalse;
		}
	}
	else
		compare = CFalse;

	if( compare )
	{
		for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
		{
			CChar engineObjectCapsName[MAX_NAME_SIZE];
			Cpy(engineObjectCapsName, (*it).c_str());
			StringToUpper(engineObjectCapsName);

			CChar currentObjectName[MAX_NAME_SIZE];
			Cpy(currentObjectName, (LPCSTR)m_str3DSoundName);
			StringToUpper(currentObjectName);

			if (Cmp(currentObjectName, engineObjectCapsName))
			{
				MessageBox("This name already exists. Please select another name!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
				return;
			}
		}
	}

	if(	m_str3DSoundPosX.IsEmpty() || m_str3DSoundPosY.IsEmpty() || m_str3DSoundPosZ.IsEmpty() || m_str3DSoundPitch.IsEmpty() 
		|| m_str3DSoundName.IsEmpty() || m_str3DSoundDataPath.IsEmpty() || m_str3DSoundRolloff.IsEmpty() ||	m_str3DSoundMaxDistance.IsEmpty()
		|| m_str3DSoundVolume.IsEmpty() )
	{
		MessageBox( "Please Fill In All Of The Required Fields", "Vanda Engine Error", MB_OK | MB_ICONERROR );
		return;
	}

	if (!m_str3DSoundName.IsEmpty())
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, (LPCSTR)m_str3DSoundName);
		StringToUpper(name);

		if (Cmp(name, "THIS"))
		{
			MessageBox("'this' is a reserved name. Please select another name!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}
	}

	if (m_f3DSoundVolume > 1.0f || m_f3DSoundVolume < 0.0f)
	{
		MessageBox("Volume must be in [0,1] range", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (m_f3DSoundReferenceDistance < 0.0)
	{
		MessageBox("Reference distance must be greater than or equal to 0.0", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (m_f3DSoundMaxDistance < 0.0)
	{
		MessageBox("Max distance must be greater than or equal to 0.0", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (m_f3DSoundRolloff < 0.0)
	{
		MessageBox("Rolloff must be greater than or equal to 0.0", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (m_f3DSoundPitch <= 0.0)
	{
		MessageBox("Pitch must be greater than 0.0", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}



INT_PTR CAdd3DSound::DoModal()
{
	CDialogTemplate dlt;
	INT_PTR nResult;
	// load dialog template
	if (!dlt.Load(MAKEINTRESOURCE(CAdd3DSound::IDD))) return -1;
	// set the font, for example "Arial", 10 pts.
	dlt.SetFont("Arial", 8);
	// get pointer to the modified dialog template
	LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
	// let MFC know that you are using your own template
	m_lpszTemplateName = NULL;
	InitModalIndirect(pdata);
	// display dialog box
	nResult = CDialog::DoModal();
	// unlock memory object
	GlobalUnlock(dlt.m_hTemplate);
	return nResult;
}

void CAdd3DSound::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CAdd3DSound::OnBnClickedBtnAddScript()
{
	CFileDialog dlgOpen(TRUE, _T("*.lua"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("LUA File (*.lua)|*.lua||"), NULL, NULL);
	if (IDOK == dlgOpen.DoModal())
	{
		g_testScript = CTrue;
		CString m_string;
		m_string = (CString)dlgOpen.GetPathName();

		lua_close(g_lua);
		g_lua = LuaNewState();
		LuaOpenLibs(g_lua);
		LuaRegisterFunctions(g_lua);

		int s = luaL_loadfile(g_lua, m_string);
		if (s == 0) {
			// execute Lua program
			s = LuaExecuteProgram(g_lua);
		}
		if (s == 0)
		{
			m_editBoxScript.SetWindowText(m_string);
			m_strScript = m_string;
			m_scriptUpdated = CTrue;
			m_hasScript = CTrue;
			PrintInfo("\nScript loaded scuccessfully", COLOR_GREEN);
		}
		else
		{
			MessageBox("Script contains error(s).\nPlease use script editor to fix the issue(s)", "Error", MB_OK | MB_ICONERROR);
		}
		g_testScript = CFalse;
	}
}


void CAdd3DSound::OnBnClickedBtnRemoveScript()
{
	if (!m_hasScript)
	{
		MessageBox("No script was found!", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (m_hasScript)
	{
		if (MessageBox("Remove current script?", "Warning", MB_YESNO) == IDYES)
		{
			m_editBoxScript.SetWindowTextA("\n");
			m_strScript.Empty();
			m_hasScript = CFalse;
		}
	}
}


void CAdd3DSound::OnBnClickedButtonViewScript()
{
	if (!m_hasScript)
	{
		MessageBox("Please add a script!", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	CViewScript* dlg = CNew(CViewScript);
	dlg->SetScriptPath(m_strScript.GetBuffer(m_strScript.GetLength()));
	m_strScript.ReleaseBuffer();
	dlg->DoModal();
	CDelete(dlg);
}
