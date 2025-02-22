//Copyright (C) 2024 Ehsan Kamrani 
//This file is licensed and distributed under MIT license

#pragma once
#include "afxcmn.h"


// CScriptEditorAddFunction dialog

class CScriptEditorAddFunction : public CDialog
{
	DECLARE_DYNAMIC(CScriptEditorAddFunction)
private:
	CInt m_index;
public:
	CScriptEditorAddFunction(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScriptEditorAddFunction();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCRIPT_EDITOR_ADD_FUNCTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listFunctions;
	CRichEditCtrl m_richFunctionName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListFunctions(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CVoid InsertItem(CChar* name);
private:
	CChar PlaySoundLoop[MAX_URI_SIZE];
	CChar PlaySoundOnce[MAX_URI_SIZE];
	CChar PauseSound[MAX_URI_SIZE];
	CChar StopSound[MAX_URI_SIZE];

	CChar ExecuteCyclicAnimation[MAX_URI_SIZE];
	CChar RemoveCyclicAnimation[MAX_URI_SIZE];
	CChar ExecuteNonCyclicAnimation[MAX_URI_SIZE];
	CChar ReverseExecuteNonCyclicAnimation[MAX_URI_SIZE];
	CChar RemoveNonCyclicAnimation[MAX_URI_SIZE];
	CChar GetAnimationClipDurationOfPrefabInstance[MAX_URI_SIZE];
	CChar PausePrefabInstanceAnimations[MAX_URI_SIZE];
	CChar ResumePrefabInstanceAnimations[MAX_URI_SIZE];

	CChar LoadVScene[MAX_URI_SIZE];
	CChar ExitGame[MAX_URI_SIZE];

	CChar ActivateThirdPersonCamera[MAX_URI_SIZE];
	CChar ActivateFirstPersonCamera[MAX_URI_SIZE];
	CChar ActivateImportedCamera[MAX_URI_SIZE];
	CChar ActivateImportedCameraOfPrefabInstance[MAX_URI_SIZE];
	CChar ActivateEngineCamera[MAX_URI_SIZE];
	CChar SetPhysicsCameraAngle[MAX_URI_SIZE];
	CChar GetPhysicsCameraAngle[MAX_URI_SIZE];
	CChar SetPhysicsCameraTilt[MAX_URI_SIZE];
	CChar SetPhysicsCameraMaxTilt[MAX_URI_SIZE];
	CChar SetPhysicsCameraMinTilt[MAX_URI_SIZE];
	CChar GetPhysicsCameraTilt[MAX_URI_SIZE];
	CChar GetPhysicsCameraMaxTilt[MAX_URI_SIZE];
	CChar GetPhysicsCameraMinTilt[MAX_URI_SIZE];
	CChar SetPhysicsCameraYaw[MAX_URI_SIZE];
	CChar GetPhysicsCameraYaw[MAX_URI_SIZE];
	CChar SetPhysicsCameraNearClipPlane[MAX_URI_SIZE];
	CChar SetPhysicsCameraFarClipPlane[MAX_URI_SIZE];
	CChar GetPhysicsCameraNearClipPlane[MAX_URI_SIZE];
	CChar GetPhysicsCameraFarClipPlane[MAX_URI_SIZE];

	CChar LoadResource[MAX_URI_SIZE];
	CChar DeleteResource[MAX_URI_SIZE];
	CChar DeleteAllResources[MAX_URI_SIZE];
	CChar PlayResourceSound[MAX_URI_SIZE];
	CChar PlayResourceSoundLoop[MAX_URI_SIZE];
	CChar PlayResourceSoundOnce[MAX_URI_SIZE];
	CChar StopResourceSound[MAX_URI_SIZE];
	CChar PauseResourceSound[MAX_URI_SIZE];
	CChar ShowCursorIcon[MAX_URI_SIZE];
	CChar HideCursorIcon[MAX_URI_SIZE];

	CChar PrintConsole[MAX_URI_SIZE];

	CChar ShowGUI[MAX_URI_SIZE];
	CChar HideGUI[MAX_URI_SIZE];

	CChar IsKeyDown[MAX_URI_SIZE];

	CChar ShowPrefabInstance[MAX_URI_SIZE];
	CChar HidePrefabInstance[MAX_URI_SIZE];

	CChar SetSelectionDistance[MAX_URI_SIZE];
	CChar GetSelectionDistance[MAX_URI_SIZE];
	CChar SelectPrefabInstances[MAX_URI_SIZE];

	//get
	CChar GetScreenWidth[MAX_URI_SIZE];
	CChar GetScreenHeight[MAX_URI_SIZE];
	CChar GetCursorX[MAX_URI_SIZE];
	CChar GetCursorY[MAX_URI_SIZE];
	CChar GetElapsedTime[MAX_URI_SIZE];
	CChar IsCharacterControllerLocked[MAX_URI_SIZE];
	CChar GetPrefabInstanceNameFromActor[MAX_URI_SIZE];

	CChar TranslatePrefabInstance[MAX_URI_SIZE];
	CChar RotatePrefabInstance[MAX_URI_SIZE];
	CChar ScalePrefabInstance[MAX_URI_SIZE];

	CChar GetPrefabInstanceTranslate[MAX_URI_SIZE];
	CChar GetPrefabInstanceRotate[MAX_URI_SIZE];
	CChar GetPrefabInstanceScale[MAX_URI_SIZE];

	CChar GetPrefabInstanceRadius[MAX_URI_SIZE];
	CChar GetDistanceOfPrefabInstanceFromPhysicsCamera[MAX_URI_SIZE];

	CChar EnableDepthOfField[MAX_URI_SIZE];
	CChar DisableDepthOfField[MAX_URI_SIZE];
	CChar SetDepthOfFieldFocalDistance[MAX_URI_SIZE];
	CChar SetDepthOfFieldFocalRange[MAX_URI_SIZE];

	CChar GetDepthOfFieldFocalDistance[MAX_URI_SIZE];
	CChar GetDepthOfFieldFocalRange[MAX_URI_SIZE];

	CChar EnableFog[MAX_URI_SIZE];
	CChar DisableFog[MAX_URI_SIZE];
	CChar SetFogColor[MAX_URI_SIZE];
	CChar SetFogDensity[MAX_URI_SIZE];

	CChar GetFogColor[MAX_URI_SIZE];
	CChar GetFogDensity[MAX_URI_SIZE];

	CChar EnableBloom[MAX_URI_SIZE];
	CChar DisableBloom[MAX_URI_SIZE];
	CChar SetBloomColor[MAX_URI_SIZE];
	CChar SetBloomIntensity[MAX_URI_SIZE];

	CChar GetBloomColor[MAX_URI_SIZE];
	CChar GetBloomIntensity[MAX_URI_SIZE];

	CChar EnableDirectionalShadow[MAX_URI_SIZE];
	CChar DisableDirectionalShadow[MAX_URI_SIZE];
	CChar SetDirectionalShadowAlgorithm[MAX_URI_SIZE];
	CChar SetDirectionalShadowNumberOfSplits[MAX_URI_SIZE];
	CChar SetDirectionalShadowWeightOfSplits[MAX_URI_SIZE];
	CChar SetDirectionalShadowNearClipPlane[MAX_URI_SIZE];
	CChar SetDirectionalShadowFarClipPlane[MAX_URI_SIZE];
	CChar SetDirectionalShadowResolution[MAX_URI_SIZE];
	CChar SetDirectionalShadowIntensity[MAX_URI_SIZE];
	CChar SetDirectionalShadowLight[MAX_URI_SIZE];

	CChar GetDirectionalShadowAlgorithm[MAX_URI_SIZE];
	CChar GetDirectionalShadowNumberOfSplits[MAX_URI_SIZE];
	CChar GetDirectionalShadowWeightOfSplits[MAX_URI_SIZE];
	CChar GetDirectionalShadowNearClipPlane[MAX_URI_SIZE];
	CChar GetDirectionalShadowFarClipPlane[MAX_URI_SIZE];
	CChar GetDirectionalShadowResolution[MAX_URI_SIZE];
	CChar GetDirectionalShadowIntensity[MAX_URI_SIZE];
	CChar GetDirectionalShadowLight[MAX_URI_SIZE];

	CChar SetLightAmbient[MAX_URI_SIZE];
	CChar SetLightDiffuse[MAX_URI_SIZE];
	CChar SetLightSpecular[MAX_URI_SIZE];
	CChar SetLightShininess[MAX_URI_SIZE];

	CChar GetLightAmbient[MAX_URI_SIZE];
	CChar GetLightDiffuse[MAX_URI_SIZE];
	CChar GetLightSpecular[MAX_URI_SIZE];
	CChar GetLightShininess[MAX_URI_SIZE];

	CChar SetPrefabInstanceAmbient[MAX_URI_SIZE];
	CChar SetPrefabInstanceDiffuse[MAX_URI_SIZE];
	CChar SetPrefabInstanceSpecular[MAX_URI_SIZE];
	CChar SetPrefabInstanceEmission[MAX_URI_SIZE];
	CChar SetPrefabInstanceShininess[MAX_URI_SIZE];
	CChar SetPrefabInstanceTransparency[MAX_URI_SIZE];
	CChar EnablePrefabInstanceMaterial[MAX_URI_SIZE];
	CChar DisablePrefabInstanceMaterial[MAX_URI_SIZE];
	CChar IsPrefabInstanceMaterialEnabled[MAX_URI_SIZE];

	CChar GetPrefabInstanceAmbient[MAX_URI_SIZE];
	CChar GetPrefabInstanceDiffuse[MAX_URI_SIZE];
	CChar GetPrefabInstanceSpecular[MAX_URI_SIZE];
	CChar GetPrefabInstanceEmission[MAX_URI_SIZE];
	CChar GetPrefabInstanceShininess[MAX_URI_SIZE];
	CChar GetPrefabInstanceTransparency[MAX_URI_SIZE];

	CChar SetPhysicsDefaultRestitution[MAX_URI_SIZE];
	CChar SetPhysicsDefaultSkinWidth[MAX_URI_SIZE];
	CChar SetPhysicsDefaultStaticFriction[MAX_URI_SIZE];
	CChar SetPhysicsDefaultDynamicFriction[MAX_URI_SIZE];
	CChar EnablePhysicsGravity[MAX_URI_SIZE];
	CChar DisablePhysicsGravity[MAX_URI_SIZE];
	CChar SetPhysicsGravity[MAX_URI_SIZE];
	CChar EnablePhysicsGroundPlane[MAX_URI_SIZE];
	CChar DisablePhysicsGroundPlane[MAX_URI_SIZE];
	CChar SetPhysicsGroundHeight[MAX_URI_SIZE];
	CChar SetDistanceBetweenPhysicsCameraAndCharacterController[MAX_URI_SIZE];
	CChar SetCharacterControllerCapsuleRadius[MAX_URI_SIZE];
	CChar SetCharacterControllerCapsuleHeight[MAX_URI_SIZE];
	CChar SetCharacterControllerForcePower[MAX_URI_SIZE];
	CChar SetCharacterControllerWalkSpeed[MAX_URI_SIZE];
	CChar SetCharacterControllerRunSpeed[MAX_URI_SIZE];
	CChar SetCharacterControllerSkinWidth[MAX_URI_SIZE];
	CChar SetCharacterControllerStepOffset[MAX_URI_SIZE];
	CChar SetCharacterControllerSlopeLimit[MAX_URI_SIZE];
	CChar SetCharacterControllerJumpPower[MAX_URI_SIZE];
	CChar EnableCharacterControllerJump[MAX_URI_SIZE];
	CChar DisableCharacterControllerJump[MAX_URI_SIZE];
	CChar EnablePhysicsDebugMode[MAX_URI_SIZE];
	CChar DisablePhysicsDebugMode[MAX_URI_SIZE];
	CChar SetCharacterControllerPosition[MAX_URI_SIZE];
	CChar GetCharacterControllerPosition[MAX_URI_SIZE];

	CChar GetDistanceBetweenPhysicsCameraAndCharacterController[MAX_URI_SIZE];

	CChar GetCharacterControllerCapsuleRadius[MAX_URI_SIZE];
	CChar GetCharacterControllerCapsuleHeight[MAX_URI_SIZE];
	CChar GetCharacterControllerForcePower[MAX_URI_SIZE];
	CChar GetCharacterControllerWalkSpeed[MAX_URI_SIZE];
	CChar GetCharacterControllerRunSpeed[MAX_URI_SIZE];
	CChar GetCharacterControllerStepOffset[MAX_URI_SIZE];
	CChar GetCharacterControllerJumpPower[MAX_URI_SIZE];

	CChar GetPhysicsDefaultRestitution[MAX_URI_SIZE];
	CChar GetPhysicsDefaultSkinWidth[MAX_URI_SIZE];
	CChar GetPhysicsDefaultStaticFriction[MAX_URI_SIZE];
	CChar GetPhysicsDefaultDynamicFriction[MAX_URI_SIZE];
	CChar GetPhysicsGravity[MAX_URI_SIZE];
	CChar GetPhysicsGroundHeight[MAX_URI_SIZE];
	CChar GetPhysicsCollisionFlags[MAX_URI_SIZE];

	CChar SetMultisamplingValue[MAX_URI_SIZE];
	CChar SetAnisotropicFilteringValue[MAX_URI_SIZE];
	CChar EnableVSync[MAX_URI_SIZE];
	CChar DisableVSync[MAX_URI_SIZE];
	CChar EnableGeneralWaterReflection[MAX_URI_SIZE];
	CChar DisableGeneralWaterReflection[MAX_URI_SIZE];
	CChar SetScreenResolution[MAX_URI_SIZE];
	CChar SaveGeneralProperties[MAX_URI_SIZE];
	CChar GetMultisamplingValue[MAX_URI_SIZE];
	CChar GetAnisotropicFilteringValue[MAX_URI_SIZE];
	CChar IsVSyncEnabled[MAX_URI_SIZE];
	CChar IsGeneralWaterReflectionEnabled[MAX_URI_SIZE];
	CChar GetScreenResolution[MAX_URI_SIZE];

	CChar GetVSceneScriptStringVariable[MAX_URI_SIZE];
	CChar GetVSceneScriptBoolVariable[MAX_URI_SIZE];
	CChar GetVSceneScriptIntVariable[MAX_URI_SIZE];
	CChar GetVSceneScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetVSceneScriptStringVariable[MAX_URI_SIZE];
	CChar SetVSceneScriptBoolVariable[MAX_URI_SIZE];
	CChar SetVSceneScriptIntVariable[MAX_URI_SIZE];
	CChar SetVSceneScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetSkyScriptStringVariable[MAX_URI_SIZE];
	CChar GetSkyScriptBoolVariable[MAX_URI_SIZE];
	CChar GetSkyScriptIntVariable[MAX_URI_SIZE];
	CChar GetSkyScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetSkyScriptStringVariable[MAX_URI_SIZE];
	CChar SetSkyScriptBoolVariable[MAX_URI_SIZE];
	CChar SetSkyScriptIntVariable[MAX_URI_SIZE];
	CChar SetSkyScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetTerrainScriptStringVariable[MAX_URI_SIZE];
	CChar GetTerrainScriptBoolVariable[MAX_URI_SIZE];
	CChar GetTerrainScriptIntVariable[MAX_URI_SIZE];
	CChar GetTerrainScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetTerrainScriptStringVariable[MAX_URI_SIZE];
	CChar SetTerrainScriptBoolVariable[MAX_URI_SIZE];
	CChar SetTerrainScriptIntVariable[MAX_URI_SIZE];
	CChar SetTerrainScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetPrefabInstanceScriptStringVariable[MAX_URI_SIZE];
	CChar GetPrefabInstanceScriptBoolVariable[MAX_URI_SIZE];
	CChar GetPrefabInstanceScriptIntVariable[MAX_URI_SIZE];
	CChar GetPrefabInstanceScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetPrefabInstanceScriptStringVariable[MAX_URI_SIZE];
	CChar SetPrefabInstanceScriptBoolVariable[MAX_URI_SIZE];
	CChar SetPrefabInstanceScriptIntVariable[MAX_URI_SIZE];
	CChar SetPrefabInstanceScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetGUIButtonScriptStringVariable[MAX_URI_SIZE];
	CChar GetGUIButtonScriptBoolVariable[MAX_URI_SIZE];
	CChar GetGUIButtonScriptIntVariable[MAX_URI_SIZE];
	CChar GetGUIButtonScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetGUIButtonScriptStringVariable[MAX_URI_SIZE];
	CChar SetGUIButtonScriptBoolVariable[MAX_URI_SIZE];
	CChar SetGUIButtonScriptIntVariable[MAX_URI_SIZE];
	CChar SetGUIButtonScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetTriggerScriptStringVariable[MAX_URI_SIZE];
	CChar GetTriggerScriptBoolVariable[MAX_URI_SIZE];
	CChar GetTriggerScriptIntVariable[MAX_URI_SIZE];
	CChar GetTriggerScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetTriggerScriptStringVariable[MAX_URI_SIZE];
	CChar SetTriggerScriptBoolVariable[MAX_URI_SIZE];
	CChar SetTriggerScriptIntVariable[MAX_URI_SIZE];
	CChar SetTriggerScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetWaterScriptStringVariable[MAX_URI_SIZE];
	CChar GetWaterScriptBoolVariable[MAX_URI_SIZE];
	CChar GetWaterScriptIntVariable[MAX_URI_SIZE];
	CChar GetWaterScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetWaterScriptStringVariable[MAX_URI_SIZE];
	CChar SetWaterScriptBoolVariable[MAX_URI_SIZE];
	CChar SetWaterScriptIntVariable[MAX_URI_SIZE];
	CChar SetWaterScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetVideoScriptStringVariable[MAX_URI_SIZE];
	CChar GetVideoScriptBoolVariable[MAX_URI_SIZE];
	CChar GetVideoScriptIntVariable[MAX_URI_SIZE];
	CChar GetVideoScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetVideoScriptStringVariable[MAX_URI_SIZE];
	CChar SetVideoScriptBoolVariable[MAX_URI_SIZE];
	CChar SetVideoScriptIntVariable[MAX_URI_SIZE];
	CChar SetVideoScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetAmbientSoundScriptStringVariable[MAX_URI_SIZE];
	CChar GetAmbientSoundScriptBoolVariable[MAX_URI_SIZE];
	CChar GetAmbientSoundScriptIntVariable[MAX_URI_SIZE];
	CChar GetAmbientSoundScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetAmbientSoundScriptStringVariable[MAX_URI_SIZE];
	CChar SetAmbientSoundScriptBoolVariable[MAX_URI_SIZE];
	CChar SetAmbientSoundScriptIntVariable[MAX_URI_SIZE];
	CChar SetAmbientSoundScriptDoubleVariable[MAX_URI_SIZE];

	CChar Get3DSoundScriptStringVariable[MAX_URI_SIZE];
	CChar Get3DSoundScriptBoolVariable[MAX_URI_SIZE];
	CChar Get3DSoundScriptIntVariable[MAX_URI_SIZE];
	CChar Get3DSoundScriptDoubleVariable[MAX_URI_SIZE];
	CChar Set3DSoundScriptStringVariable[MAX_URI_SIZE];
	CChar Set3DSoundScriptBoolVariable[MAX_URI_SIZE];
	CChar Set3DSoundScriptIntVariable[MAX_URI_SIZE];
	CChar Set3DSoundScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetLightScriptStringVariable[MAX_URI_SIZE];
	CChar GetLightScriptBoolVariable[MAX_URI_SIZE];
	CChar GetLightScriptIntVariable[MAX_URI_SIZE];
	CChar GetLightScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetLightScriptStringVariable[MAX_URI_SIZE];
	CChar SetLightScriptBoolVariable[MAX_URI_SIZE];
	CChar SetLightScriptIntVariable[MAX_URI_SIZE];
	CChar SetLightScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetCameraScriptStringVariable[MAX_URI_SIZE];
	CChar GetCameraScriptBoolVariable[MAX_URI_SIZE];
	CChar GetCameraScriptIntVariable[MAX_URI_SIZE];
	CChar GetCameraScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetCameraScriptStringVariable[MAX_URI_SIZE];
	CChar SetCameraScriptBoolVariable[MAX_URI_SIZE];
	CChar SetCameraScriptIntVariable[MAX_URI_SIZE];
	CChar SetCameraScriptDoubleVariable[MAX_URI_SIZE];

	CChar GetMainCharacterScriptStringVariable[MAX_URI_SIZE];
	CChar GetMainCharacterScriptBoolVariable[MAX_URI_SIZE];
	CChar GetMainCharacterScriptIntVariable[MAX_URI_SIZE];
	CChar GetMainCharacterScriptDoubleVariable[MAX_URI_SIZE];
	CChar SetMainCharacterScriptStringVariable[MAX_URI_SIZE];
	CChar SetMainCharacterScriptBoolVariable[MAX_URI_SIZE];
	CChar SetMainCharacterScriptIntVariable[MAX_URI_SIZE];
	CChar SetMainCharacterScriptDoubleVariable[MAX_URI_SIZE];

	CChar ShowGUIButton[MAX_URI_SIZE];
	CChar HideGUIButton[MAX_URI_SIZE];
	CChar ShowGUIImage[MAX_URI_SIZE];
	CChar HideGUIImage[MAX_URI_SIZE];
	CChar ShowGUIText[MAX_URI_SIZE];
	CChar HideGUIText[MAX_URI_SIZE];
	CChar ScaleGUIButton[MAX_URI_SIZE];
	CChar ScaleGUIImage[MAX_URI_SIZE];

	CChar SetGUIButtonPosition[MAX_URI_SIZE];
	CChar SetGUIImagePosition[MAX_URI_SIZE];
	CChar SetGUITextPosition[MAX_URI_SIZE];
	CChar GetGUIButtonPosition[MAX_URI_SIZE];
	CChar GetGUIImagePosition[MAX_URI_SIZE];
	CChar GetGUITextPosition[MAX_URI_SIZE];

	CChar SetGUIPosition[MAX_URI_SIZE];
	CChar GetGUIPosition[MAX_URI_SIZE];

	CChar AddForceToCharacterController[MAX_URI_SIZE];
	CChar AddForceToPrefabInstance[MAX_URI_SIZE];
	CChar AddTorqueToPrefabInstance[MAX_URI_SIZE];

	CChar GetPhysicsActorGroup[MAX_URI_SIZE];
	CChar SetPhysicsCollisionFlags[MAX_URI_SIZE];

	CChar GeneratePrefabInstance[MAX_URI_SIZE];
	CChar DeletePrefabInstance[MAX_URI_SIZE];

	CChar AttachPrefabInstanceToWater[MAX_URI_SIZE];
	CChar DetachPrefabInstanceFromWater[MAX_URI_SIZE];
	CChar PauseWaterAnimation[MAX_URI_SIZE];
	CChar ResumeWaterAnimation[MAX_URI_SIZE];

	CChar SetSoundVolume[MAX_URI_SIZE];
	CChar SetSoundPitch[MAX_URI_SIZE];
	CChar PlaySound[MAX_URI_SIZE];
	CChar SetSoundLoop[MAX_URI_SIZE];
	CChar SetSoundPosition[MAX_URI_SIZE];
	CChar SetSoundRollOff[MAX_URI_SIZE];
	CChar SetSoundReferenceDistance[MAX_URI_SIZE];
	CChar SetSoundMaxDistance[MAX_URI_SIZE];

	CChar GetSoundVolume[MAX_URI_SIZE];
	CChar GetSoundPitch[MAX_URI_SIZE];
	CChar GetSoundPlay[MAX_URI_SIZE];
	CChar GetSoundLoop[MAX_URI_SIZE];
	CChar GetSoundPosition[MAX_URI_SIZE];
	CChar GetSoundRollOff[MAX_URI_SIZE];
	CChar GetSoundReferenceDistance[MAX_URI_SIZE];
	CChar GetSoundMaxDistance[MAX_URI_SIZE];

	CChar SetGlobalSoundVolume[MAX_URI_SIZE];
	CChar GetGlobalSoundVolume[MAX_URI_SIZE];

	CChar PlayVideo[MAX_URI_SIZE];
	CChar PlayVideoLoop[MAX_URI_SIZE];
	CChar PlayVideoOnce[MAX_URI_SIZE];
	CChar StopVideo[MAX_URI_SIZE];

	CChar SetVideoLoop[MAX_URI_SIZE];
	CChar SetVideoVolume[MAX_URI_SIZE];

	CChar GetVideoPlay[MAX_URI_SIZE];
	CChar GetVideoLoop[MAX_URI_SIZE];
	CChar GetVideoVolume[MAX_URI_SIZE];
	CChar GetVideoDuration[MAX_URI_SIZE];

	//Stop Sounds
	CChar StopAllSounds[MAX_URI_SIZE];
	CChar StopAllAmbientSounds[MAX_URI_SIZE];
	CChar StopAll3DSounds[MAX_URI_SIZE];
	CChar StopAllResourceSounds[MAX_URI_SIZE];
	//Play Sounds
	CChar PlayAllSounds[MAX_URI_SIZE];
	CChar PlayAllPausedSounds[MAX_URI_SIZE];
	CChar PlayAllStoppedSounds[MAX_URI_SIZE];
	CChar PlayAllAmbientSounds[MAX_URI_SIZE];
	CChar PlayAll3DSounds[MAX_URI_SIZE];
	CChar PlayAllResourceSounds[MAX_URI_SIZE];
	CChar PlayAllStoppedAmbientSounds[MAX_URI_SIZE];
	CChar PlayAllStopped3DSounds[MAX_URI_SIZE];
	CChar PlayAllStoppedResourceSounds[MAX_URI_SIZE];
	CChar PlayAllPausedAmbientSounds[MAX_URI_SIZE];
	CChar PlayAllPaused3DSounds[MAX_URI_SIZE];
	CChar PlayAllPausedResourceSounds[MAX_URI_SIZE];
	//Play Sounds loop
	CChar PlayAllSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllPausedSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllStoppedSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllAmbientSoundsLoop[MAX_URI_SIZE];
	CChar PlayAll3DSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllResourceSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllStoppedAmbientSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllStopped3DSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllStoppedResourceSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllPausedAmbientSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllPaused3DSoundsLoop[MAX_URI_SIZE];
	CChar PlayAllPausedResourceSoundsLoop[MAX_URI_SIZE];
	//Play Sounds once
	CChar PlayAllSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllPausedSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllStoppedSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllAmbientSoundsOnce[MAX_URI_SIZE];
	CChar PlayAll3DSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllResourceSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllStoppedAmbientSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllStopped3DSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllStoppedResourceSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllPausedAmbientSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllPaused3DSoundsOnce[MAX_URI_SIZE];
	CChar PlayAllPausedResourceSoundsOnce[MAX_URI_SIZE];
	//Pause Sounds
	CChar PauseAllSounds[MAX_URI_SIZE];
	CChar PauseAllAmbientSounds[MAX_URI_SIZE];
	CChar PauseAll3DSounds[MAX_URI_SIZE];
	CChar PauseAllResourceSounds[MAX_URI_SIZE];

	//Pause game 
	CChar PauseGame[MAX_URI_SIZE];
	CChar PauseAllAnimationsOfPrefabInstances[MAX_URI_SIZE];
	CChar PauseMainCharacterAnimations[MAX_URI_SIZE];
	CChar PausePhysics[MAX_URI_SIZE];
	CChar PauseAnimationOfAllWaters[MAX_URI_SIZE];

	//resume game
	CChar ResumeGame[MAX_URI_SIZE];
	CChar ResumeAllAnimationsOfPrefabInstances[MAX_URI_SIZE];
	CChar ResumeMainCharacterAnimations[MAX_URI_SIZE];
	CChar ResumePhysics[MAX_URI_SIZE];
	CChar ResumeAnimationOfAllWaters[MAX_URI_SIZE];

	//lock/unlock character
	CChar LockCharacterController[MAX_URI_SIZE];
	CChar UnlockCharacterController[MAX_URI_SIZE];

	//menu cursor
	CChar ShowMenuCursor[MAX_URI_SIZE];
	CChar HideMenuCursor[MAX_URI_SIZE];
	CChar SetMenuCursorSize[MAX_URI_SIZE];
	CChar GetMenuCursorSize[MAX_URI_SIZE];;

	//Pause script Update event of game objects
	CChar PauseAllUpdateEvents[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAllPrefabInstances[MAX_URI_SIZE];
	CChar PauseUpdateEventOfVSceneScript[MAX_URI_SIZE];
	CChar PauseUpdateEventOfSky[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAllWaters[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAllLights[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAll3DSounds[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAllAmbientSounds[MAX_URI_SIZE];
	CChar PauseUpdateEventOfMainCharacter[MAX_URI_SIZE];
	CChar PauseUpdateEventOfTerrain[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAllEngineCameras[MAX_URI_SIZE];

	//Resume script Update event of game objects
	CChar ResumeAllUpdateEvents[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAllPrefabInstances[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfVSceneScript[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfSky[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAllWaters[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAllLights[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAll3DSounds[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAllAmbientSounds[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfMainCharacter[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfTerrain[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAllEngineCameras[MAX_URI_SIZE];

	//Pause script Update event of individual game objects
	CChar PauseUpdateEventOfPrefabInstance[MAX_URI_SIZE];
	CChar PauseUpdateEventOfWater[MAX_URI_SIZE];
	CChar PauseUpdateEventOfLight[MAX_URI_SIZE];
	CChar PauseUpdateEventOf3DSound[MAX_URI_SIZE];
	CChar PauseUpdateEventOfAmbientSound[MAX_URI_SIZE];
	CChar PauseUpdateEventOfEngineCamera[MAX_URI_SIZE];

	//Resume script Update event of individual game objects
	CChar ResumeUpdateEventOfPrefabInstance[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfWater[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfLight[MAX_URI_SIZE];
	CChar ResumeUpdateEventOf3DSound[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfAmbientSound[MAX_URI_SIZE];
	CChar ResumeUpdateEventOfEngineCamera[MAX_URI_SIZE];

	CChar SetTerrainAmbient[MAX_URI_SIZE];
	CChar SetTerrainDiffuse[MAX_URI_SIZE];
	CChar SetTerrainSpecular[MAX_URI_SIZE];
	CChar SetTerrainShininess[MAX_URI_SIZE];

	CChar GetTerrainAmbient[MAX_URI_SIZE];
	CChar GetTerrainDiffuse[MAX_URI_SIZE];
	CChar GetTerrainSpecular[MAX_URI_SIZE];
	CChar GetTerrainShininess[MAX_URI_SIZE];

	//Engine Camera
	CChar SetEngineCameraPosition[MAX_URI_SIZE];
	CChar SetEngineCameraPan[MAX_URI_SIZE];
	CChar SetEngineCameraTilt[MAX_URI_SIZE];
	CChar SetEngineCameraNearClipPlane[MAX_URI_SIZE];
	CChar SetEngineCameraFarClipPlane[MAX_URI_SIZE];
	CChar SetEngineCameraAngle[MAX_URI_SIZE];

	CChar GetEngineCameraPosition[MAX_URI_SIZE];
	CChar GetEngineCameraPan[MAX_URI_SIZE];
	CChar GetEngineCameraTilt[MAX_URI_SIZE];
	CChar GetEngineCameraNearClipPlane[MAX_URI_SIZE];
	CChar GetEngineCameraFarClipPlane[MAX_URI_SIZE];
	CChar GetEngineCameraAngle[MAX_URI_SIZE];

	//Water
	CChar SetWaterPosition[MAX_URI_SIZE];
	CChar SetWaterRotation[MAX_URI_SIZE];
	CChar SetWaterScale[MAX_URI_SIZE];
	CChar SetWaterLightPosition[MAX_URI_SIZE];
	CChar SetWaterUnderwaterColor[MAX_URI_SIZE];
	CChar SetWaterUnderwaterFogDensity[MAX_URI_SIZE];
	CChar SetWaterTransparency[MAX_URI_SIZE];
	CChar SetWaterFlowSpeed[MAX_URI_SIZE];
	CChar SetWaterUV[MAX_URI_SIZE];
	CChar SetWaterVisible[MAX_URI_SIZE];
	CChar SetWaterInvisible[MAX_URI_SIZE];
	CChar EnableWaterShadow[MAX_URI_SIZE];
	CChar DisableWaterShadow[MAX_URI_SIZE];
	CChar EnableWaterSunReflection[MAX_URI_SIZE];
	CChar DisableWaterSunReflection[MAX_URI_SIZE];

	CChar GetWaterPosition[MAX_URI_SIZE];
	CChar GetWaterRotation[MAX_URI_SIZE];
	CChar GetWaterScale[MAX_URI_SIZE];
	CChar GetWaterLightPosition[MAX_URI_SIZE];
	CChar GetWaterUnderwaterColor[MAX_URI_SIZE];
	CChar GetWaterUnderwaterFogDensity[MAX_URI_SIZE];
	CChar GetWaterTransparency[MAX_URI_SIZE];
	CChar GetWaterFlowSpeed[MAX_URI_SIZE];
	CChar GetWaterUV[MAX_URI_SIZE];
	CChar IsWaterVisible[MAX_URI_SIZE];
	CChar IsWaterShadowEnabled[MAX_URI_SIZE];
	CChar IsWaterSunReflectionEnabled[MAX_URI_SIZE];

	//Sky
	CChar SetSkyPosition[MAX_URI_SIZE];
	CChar EnableSkyFog[MAX_URI_SIZE];
	CChar DisableSkyFog[MAX_URI_SIZE];

	CChar GetSkyPosition[MAX_URI_SIZE];
	CChar IsSkyFogEnabled[MAX_URI_SIZE];

	//Save/Load
	CChar CreateFolder[MAX_URI_SIZE];
	CChar RemoveFolder[MAX_URI_SIZE];
	CChar RemoveFile[MAX_URI_SIZE];
	CChar OpenFileForReading[MAX_URI_SIZE];
	CChar OpenFileForWriting[MAX_URI_SIZE];
	CChar CloseFile[MAX_URI_SIZE];
	CChar ReadBoolVariableFromFile[MAX_URI_SIZE];
	CChar WriteBoolVariableToFile[MAX_URI_SIZE];
	CChar ReadFloatVariableFromFile[MAX_URI_SIZE];
	CChar WriteFloatVariableToFile[MAX_URI_SIZE];
	CChar ReadIntVariableFromFile[MAX_URI_SIZE];
	CChar WriteIntVariableToFile[MAX_URI_SIZE];
	CChar ReadStringVariableFromFile[MAX_URI_SIZE];
	CChar WriteStringVariableToFile[MAX_URI_SIZE];

	CImageList m_image;

public:
	afx_msg void OnBnClickedCancel();
};
