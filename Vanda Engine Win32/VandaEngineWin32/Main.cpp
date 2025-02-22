//Copyright (C) 2024 Ehsan Kamrani 
//This file is licensed and distributed under MIT license

#include "stdafx.h"
#include "main.h"
#include "GraphicsEngine/Animation.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
#include <sstream>
#include "graphicsEngine\\imagelib.h"

//#include <thread>
//Vanda C Functions to be used in Lua scripts
CGeometry* GetGeometryFromScenes(const CChar * name, const CChar * DocURI)
{
	for (CUInt i = 0; i<g_geometries.size(); i++)
	{
		if (ICmp(g_geometries[i]->GetName(), name) &&
			ICmp(g_geometries[i]->GetDocURI(), DocURI))
			return g_geometries[i];
	}
	return NULL;
}

CInt PlaySound(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString);

		if (Cmp(luaToString, "THIS"))
		{
			if (g_current3DSound)
			{
				Cpy(luaToString, g_current3DSound->GetName());
				StringToUpper(luaToString);
			}
			else if (g_currentAmbientSound)
			{
				Cpy(luaToString, g_currentAmbientSound->GetName());
				StringToUpper(luaToString);
			}
			else
			{
				//PrintInfo("\nPlaySound Error: Couldn't find current sound object", COLOR_RED);
				continue;
			}
		}

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engine3DSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(g_engine3DSounds[i]->GetLoop());
				g_engine3DSounds[i]->SetLoop(g_engine3DSounds[i]->GetLoop());
				g_engine3DSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "%s%s%s", "\nSound '", g_engine3DSounds[i]->GetName(), "' is playing continiously.");
				//PrintInfo(temp, COLOR_GREEN);
				foundTarget = CTrue;
				break;
			}
		}

		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(g_engineAmbientSounds[i]->GetLoop());
				g_engineAmbientSounds[i]->SetLoop(g_engineAmbientSounds[i]->GetLoop());
				g_engineAmbientSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "%s%s%s", "\nSound '", g_engineAmbientSounds[i]->GetName(), "' is playing continiously.");
				//PrintInfo(temp, COLOR_GREEN);
				foundTarget = CTrue;
				break;
			}
		}

		if (!foundTarget)
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nPlaySound() Error: Couldn't find Sound '", lua_tostring(L, n), "' to be played.");
			//PrintInfo(temp, COLOR_RED);
		}
	}

	return 0; // number of return values
}

CInt PlaySoundLoop(lua_State *L)
{
	int argc = lua_gettop(L);


	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString);

		if (Cmp(luaToString, "THIS"))
		{
			if (g_current3DSound)
			{
				Cpy(luaToString, g_current3DSound->GetName());
				StringToUpper(luaToString);
			}
			else if (g_currentAmbientSound)
			{
				Cpy(luaToString, g_currentAmbientSound->GetName());
				StringToUpper(luaToString);
			}
			else
			{
				//PrintInfo("\nPlaySoundLoop Error: Couldn't find current sound object", COLOR_RED);
				continue;
			}
		}

		for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engine3DSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engine3DSounds[i]->SetLoop( CTrue );
				g_engine3DSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
				break;
			}
		}

		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engineAmbientSounds[i]->SetLoop(CTrue);
				g_engineAmbientSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
				break;
			}
		}

	}
	return 0; // number of return values
}

CInt PlaySoundOnce(lua_State *L)
{
	int argc = lua_gettop(L);


	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString);

		if (Cmp(luaToString, "THIS"))
		{
			if (g_current3DSound)
			{
				Cpy(luaToString, g_current3DSound->GetName());
				StringToUpper(luaToString);
			}
			else if (g_currentAmbientSound)
			{
				Cpy(luaToString, g_currentAmbientSound->GetName());
				StringToUpper(luaToString);
			}
			else
			{
				//PrintInfo("\nPlaySoundOnce Error: Couldn't find current sound object", COLOR_RED);
				continue;
			}
		}

		for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engine3DSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engine3DSounds[i]->SetLoop(CFalse);
				g_engine3DSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
				break;
			}
		}

		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engineAmbientSounds[i]->SetLoop(CFalse);
				g_engineAmbientSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
				break;
			}
		}

	}
	return 0; // number of return values
}


CInt PauseSound(lua_State *L)
{
	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString);

		if (Cmp(luaToString, "THIS"))
		{
			if (g_current3DSound)
			{
				Cpy(luaToString, g_current3DSound->GetName());
				StringToUpper(luaToString);
			}
			else if (g_currentAmbientSound)
			{
				Cpy(luaToString, g_currentAmbientSound->GetName());
				StringToUpper(luaToString);
			}
			else
			{
				//PrintInfo("\nPauseSound Error: Couldn't find current sound object", COLOR_RED);
				continue;
			}
		}

		for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engine3DSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engine3DSounds[i]->SetPlay(CFalse);
				g_soundSystem->PauseALSound(*(g_engine3DSounds[i]->GetSoundSource()));
				break;
			}
		}

		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engineAmbientSounds[i]->SetPlay(CFalse);
				g_soundSystem->PauseALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
				break;
			}
		}

	}
	return 0; // number of return values
}

CInt StopSound(lua_State *L)
{
	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString);

		if (Cmp(luaToString, "THIS"))
		{
			if (g_current3DSound)
			{
				Cpy(luaToString, g_current3DSound->GetName());
				StringToUpper(luaToString);
			}
			else if (g_currentAmbientSound)
			{
				Cpy(luaToString, g_currentAmbientSound->GetName());
				StringToUpper(luaToString);
			}
			else
			{
				//PrintInfo("\nStopSound Error: Couldn't find current sound object", COLOR_RED);
				continue;
			}
		}

		for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engine3DSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engine3DSounds[i]->SetPlay(CFalse);
				g_soundSystem->StopALSound(*(g_engine3DSounds[i]->GetSoundSource()));
				break;
			}
		}

		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar soundName[MAX_NAME_SIZE];
			Cpy(soundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(soundName);

			if (Cmp(soundName, luaToString))
			{
				g_engineAmbientSounds[i]->SetPlay(CFalse);
				g_soundSystem->StopALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
				break;
			}
		}

	}
	return 0; // number of return values
}

CInt ExecuteCyclicAnimation(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for ExecuteCyclicAnimation()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}
					scene->ExecuteCyclicAnimation(index, (CFloat)lua_tonumber(L, 3), (CFloat)lua_tonumber(L, 4));
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}
					scene->ExecuteCyclicAnimation(index, (CFloat)lua_tonumber(L, 3), (CFloat)lua_tonumber(L, 4));
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt RemoveCyclicAnimation(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for RemoveCyclicAnimation()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->RemoveCyclicAnimation(index, (CFloat)lua_tonumber(L, 3));
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->RemoveCyclicAnimation(index, (CFloat)lua_tonumber(L, 3));
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ExecuteNonCyclicAnimation(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for ExecuteNonCyclicAnimation()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					CFloat weight = 1.0f;
					CInt lockResult;
					CBool lock = CFalse;
					if (argc > 4)
						weight = (CFloat)lua_tonumber(L, 5);
					if (argc > 5)
						lockResult = lua_toboolean(L, 6);
					if (lockResult)
						lock = CTrue;
					else
						lock = CFalse;
					scene->ExecuteNonCyclicAnimation(index, (CFloat)lua_tonumber(L, 3), (CFloat)lua_tonumber(L, 4), weight, lock);
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current instance prefab", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					CFloat weight = 1.0f;
					CInt lockResult;
					CBool lock = CFalse;
					if (argc > 4)
						weight = (CFloat)lua_tonumber(L, 5);
					if (argc > 5)
						lockResult = lua_toboolean(L, 6);
					if (lockResult)
						lock = CTrue;
					else
						lock = CFalse;
					scene->ExecuteNonCyclicAnimation(index, (CFloat)lua_tonumber(L, 3), (CFloat)lua_tonumber(L, 4), weight, lock);
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ReverseExecuteNonCyclicAnimation(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for ReverseExecuteNonCyclicAnimation()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->ReverseExecuteNonCyclicAnimation(index);
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current instance prefab", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->ReverseExecuteNonCyclicAnimation(index);
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//First argument: prefab instance name
//Second argument: animation clip name
CInt RemoveNonCyclicAnimation(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for RemoveNonCyclicAnimation()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->RemoveNonCyclicAnimation(index);
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current instance prefab", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					scene->SetAnimationStatus(eANIM_PLAY);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					scene->RemoveNonCyclicAnimation(index);
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//First Argument: Prefab Instance Name
//Second Argument: Animation Clip Name
CInt GetAnimationClipDurationOfPrefabInstance(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetAnimationClipDurationOfPrefabInstance()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					lua_pushnumber(L, scene->m_animationClips[index]->GetDuration());
					return 1;
				}
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current instance prefab", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);

					CChar luaToString2[MAX_NAME_SIZE];
					Cpy(luaToString2, lua_tostring(L, 2));
					StringToUpper(luaToString2);

					CBool foundAnimationTarget = CFalse;
					CInt index;
					for (CInt k = 0; k < scene->GetNumClips(); k++)
					{
						CChar animationName[MAX_NAME_SIZE];
						Cpy(animationName, scene->m_animationClips[k]->GetName());
						StringToUpper(animationName);
						if (Cmp(luaToString2, animationName))
						{
							index = k;
							foundAnimationTarget = CTrue;
							break;
						}
					}
					if (!foundAnimationTarget)
					{
						CChar temp[MAX_NAME_SIZE];
						sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString2, "'");
						//PrintInfo(temp, COLOR_RED);
						return 0;
					}

					lua_pushnumber(L, scene->m_animationClips[index]->GetDuration());
					return 1;
				}
			}
		}
	}
	if (!foundPrefabInstance)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt ShowPrefabInstance(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for ShowPrefabInstance()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
			g_currentInstancePrefab->SetVisible(CTrue);
		//else
		//	PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetVisible(CTrue);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt HidePrefabInstance(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for HidePrefabInstance()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
			g_currentInstancePrefab->SetVisible(CFalse);
		//else
		//	PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetVisible(CFalse);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt PausePrefabInstanceAnimations(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for PausePrefabInstanceAnimations()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}

			g_currentInstancePrefab->SetUpdateAnimation(CFalse);

		}
		else
		{
			//PrintInfo("\nPausePrefabInstanceAnimations() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
				}
			}

			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}

			g_instancePrefab[i]->SetUpdateAnimation(CFalse);

		}
	}

	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumePrefabInstanceAnimations(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for ResumePrefabInstanceAnimations()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_currentInstancePrefab->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);
				}
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}

			g_currentInstancePrefab->SetUpdateAnimation(CTrue);

		}
		else
		{
			//PrintInfo("\nResumePrefabInstanceAnimations() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 4; j++)
			{
				CBool condition = CFalse;
				if (j < 3)
				{
					if (prefab && prefab->GetHasLod(j))
						condition = CTrue;
				}
				else
				{
					if (prefab && g_instancePrefab[i]->GetHasCollider())
						condition = CTrue;
				}
				if (condition /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);
				}
			}

			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}

			g_instancePrefab[i]->SetUpdateAnimation(CTrue);

		}
	}

	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt LoadVScene(lua_State *L)
{
	//if( g_testScript )
	//	return 0;
	g_clickedNew = g_clickedOpen = CTrue;

	int argc = lua_gettop(L);
	if( argc < 1 )
	{
		//PrintInfo( "\nPlease specify 1 argument for LoadVScene()", COLOR_RED );
		return 0;
	}
	Cpy( g_currentVSceneNameViaScript, lua_tostring(L, 1) );
	g_loadSceneViaScript = CTrue;
	return 0;
}

CInt ExitGame(lua_State *L)
{
	g_main->SetExitGame(CTrue);
	return 0;
}

CInt ActivateThirdPersonCamera(lua_State *L)
{
	if (g_mainCharacter)
	{
		g_render.SetActiveInstanceCamera(NULL);
		g_currentCameraType = eCAMERA_PHYSX;
		g_mainCharacter->SetCameraType(ePHYSX_CAMERA_THIRD_PERSON);
		//PrintInfo("\nDefault third person PhysX camera was activated.", COLOR_YELLOW);
		return 0;
	}
	else
	{
		//PrintInfo("\nCouldn't find main character", COLOR_RED);
		return 0;
	}
}

CInt ActivateFirstPersonCamera(lua_State *L)
{
	if (g_mainCharacter)
	{
		g_render.SetActiveInstanceCamera(NULL);
		g_currentCameraType = eCAMERA_PHYSX;
		g_mainCharacter->SetCameraType(ePHYSX_CAMERA_FIRST_PERSON);
		//PrintInfo("\nDefault first person PhysX camera was activated.", COLOR_YELLOW);
		return 0;
	}
	else
	{
		//PrintInfo("\nCouldn't find main character", COLOR_RED);
		return 0;
	}
}

//First Argument: Imported Camera Name
//Second Argument: End Time. Should Be Positive Value. Arbitrary Argument
CInt ActivateImportedCamera(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify at least 1 argument for ActivateImportedCamera()", COLOR_RED);
		return 0;
	}

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_importedCameraInstances.size(); i++)
	{
		CChar camera[MAX_NAME_SIZE];
		Cpy(camera, g_importedCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(camera);

		if (Cmp(camera, luaToString))
		{
			CFloat end_time = 0.0f;
			CBool enableTimer = CFalse;
			if (argc > 1)
			{
				end_time = (CFloat)lua_tonumber(L, 2);
				if (end_time <= 0.0f)
				{
					enableTimer = CFalse;
					end_time = 0.0f;
				}
				else
				{
					enableTimer = CTrue;
				}
			}

			g_render.SetActiveInstanceCamera(g_importedCameraInstances[i]);
			g_importedCameraInstances[i]->SetIsTimerEnabled(enableTimer);
			g_importedCameraInstances[i]->SetEndTime(end_time);
			g_currentCameraType = eCAMERA_COLLADA;
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCamera '", g_importedCameraInstances[i]->m_abstractCamera->GetName(), "' was activated.");
			//PrintInfo(temp, COLOR_RED);

			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCoudn't find camera '", luaToString, "' to be activated.");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

//First Argument: Prefab Instance Name. Use "this" to refer to current prefab instance
//Second Argument: Imported camera name of prefab
//Third Argument: End Time. Should Be Positive Value. Arbitrary Argument
CInt ActivateImportedCameraOfPrefabInstance(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify at least 2 arguments for ActivateImportedCameraOfPrefabInstance()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;
	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	//camera name of current prefab
	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	CBool foundCameraTarget = CFalse;

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			CPrefab* prefab = g_currentInstancePrefab->GetPrefab();

			for (CUInt j = 0; j < 3; j++)
			{
				if (prefab && prefab->GetHasLod(j) /*&& g_currentInstancePrefab->GetSceneVisible(j)*/)
				{
					scene = g_currentInstancePrefab->GetScene(j);

					CUInt index;
					for (CUInt k = 0; k < scene->m_cameraInstances.size(); k++)
					{
						CChar cameraName[MAX_NAME_SIZE];
						Cpy(cameraName, scene->m_cameraInstances[k]->m_abstractCamera->GetPureName());
						StringToUpper(cameraName);
						if (Cmp(luaToString2, cameraName))
						{
							index = k;
							foundCameraTarget = CTrue;
							break;
						}
					}

					if (foundCameraTarget)
					{
						CFloat end_time = 0.0f;
						CBool enableTimer = CFalse;
						if (argc > 2)
						{
							end_time = (CFloat)lua_tonumber(L, 3);
							if (end_time <= 0.0f)
							{
								enableTimer = CFalse;
								end_time = 0.0f;
							}
							else
							{
								enableTimer = CTrue;
							}
						}

						g_render.SetActiveInstanceCamera(scene->m_cameraInstances[index]);
						scene->m_cameraInstances[index]->SetIsTimerEnabled(enableTimer);
						scene->m_cameraInstances[index]->SetEndTime(end_time);
						g_currentCameraType = eCAMERA_COLLADA;
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "%s%s%s", "\nCamera '", scene->m_cameraInstances[index]->m_abstractCamera->GetPureName(), "' was activated.");
						//PrintInfo(temp, COLOR_GREEN);
					}
				}
				if (foundCameraTarget)
					break;
			}
			if (!scene)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Prefab Instance '", g_currentInstancePrefab->GetName(), "' Is Invisible");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
			if (!foundCameraTarget)
			{
				//CChar temp[MAX_NAME_SIZE];
				//sprintf(temp, "\n%s%s%s", "Couldn't find the camera '", luaToString2, "'");
				//PrintInfo(temp, COLOR_RED);
				return 0;
			}
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();

			for (CUInt j = 0; j < 3; j++)
			{
				if (prefab && prefab->GetHasLod(j) /*&& g_instancePrefab[i]->GetSceneVisible(j)*/)
				{
					scene = g_instancePrefab[i]->GetScene(j);

					CUInt index;
					for (CUInt k = 0; k < scene->m_cameraInstances.size(); k++)
					{
						CChar cameraName[MAX_NAME_SIZE];
						Cpy(cameraName, scene->m_cameraInstances[k]->m_abstractCamera->GetPureName());
						StringToUpper(cameraName);
						if (Cmp(luaToString2, cameraName))
						{
							index = k;
							foundCameraTarget = CTrue;
							break;
						}
					}
					if (foundCameraTarget)
					{
						CFloat end_time = 0.0f;
						CBool enableTimer = CFalse;
						if (argc > 2)
						{
							end_time = (CFloat)lua_tonumber(L, 3);
							if (end_time <= 0.0f)
							{
								enableTimer = CFalse;
								end_time = 0.0f;
							}
							else
							{
								enableTimer = CTrue;
							}
						}

						g_render.SetActiveInstanceCamera(scene->m_cameraInstances[index]);
						scene->m_cameraInstances[index]->SetIsTimerEnabled(enableTimer);
						scene->m_cameraInstances[index]->SetEndTime(end_time);
						g_currentCameraType = eCAMERA_COLLADA;
						//CChar temp[MAX_NAME_SIZE];
						//sprintf(temp, "%s%s%s", "\nCamera '", scene->m_cameraInstances[index]->m_abstractCamera->GetPureName(), "' was activated.");
						//PrintInfo(temp, COLOR_GREEN);
					}
				}
				if (foundCameraTarget)
					break;
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Prefab Instance '", luaToString, "' Is Invisible");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	if (!foundCameraTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the camera '", luaToString2, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//First Argument: Engine Camera Name
//Second Argument: End Time. Should Be Positive Value. Arbitrary Argument

CInt ActivateEngineCamera(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify at least 1 argument for ActivateEngineCamera()", COLOR_RED);
		return 0;
	}

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			Cpy(luaToString, g_currentEngineCamera->m_abstractCamera->GetName());
			StringToUpper(luaToString);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nActivateEngineCamera() Error: Couldn't find current camera");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	CBool foundTarget = CFalse;
	CInt index = -1;
	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar camera[MAX_NAME_SIZE];
		Cpy(camera, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(camera);

		if (Cmp(camera, luaToString))
		{
			CFloat end_time = 0.0f;
			CBool enableTimer = CFalse;
			if (argc > 1)
			{
				end_time = (CFloat)lua_tonumber(L, 2);
				if (end_time <= 0.0f)
				{
					enableTimer = CFalse;
					end_time = 0.0f;
				}
				else
				{
					enableTimer = CTrue;
				}
			}

			g_render.SetActiveInstanceCamera(g_engineCameraInstances[i]);
			g_engineCameraInstances[i]->SetActive(CTrue);
			g_engineCameraInstances[i]->SetIsTimerEnabled(enableTimer);
			g_engineCameraInstances[i]->SetEndTime(end_time);
			g_currentCameraType = eCAMERA_ENGINE;
			index = i;
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCamera '", g_engineCameraInstances[i]->m_abstractCamera->GetName(), "' was activated.");
			//PrintInfo(temp, COLOR_RED);

			foundTarget = CTrue;
			break;
		}
	}
	if (foundTarget)
	{
		for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
		{
			if (i != index)
				g_engineCameraInstances[i]->SetActive(CFalse);
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCoudn't find camera '", luaToString, "' to be activated.");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

//Argument: Angle in degrees
CInt SetPhysicsCameraAngle(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraAngle()", COLOR_RED);
		return 0;
	}
	if (g_camera)
	{
		CFloat angle = (CFloat)lua_tonumber(L, 1);
		g_camera->m_cameraManager->SetAngle(angle);
	}
	return 0;
}

CInt GetPhysicsCameraAngle(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	if (g_camera)
	{
		CFloat angle = g_camera->m_cameraManager->GetAngle();
		lua_pushnumber(L, angle);
		return 1;
	}
	return 0;
}

CInt SetPhysicsCameraTilt(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraTilt()", COLOR_RED);
		return 0;
	}
	if (g_camera)
	{
		CFloat value = (CFloat)lua_tonumber(L, 1);
		g_camera->m_perspectiveCameraTilt = NxMath::degToRad(value);
		g_camera->m_perspectiveCurrentCameraTilt = NxMath::degToRad(value);
	}
	return 0;
}

CInt SetPhysicsCameraMaxTilt(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraMaxTilt()", COLOR_RED);
		return 0;
	}
	if (g_camera)
	{
		CFloat value = (CFloat)lua_tonumber(L, 1);
		g_camera->m_perspectiveCameraMaxTilt = NxMath::degToRad(value);
	}
	return 0;
}

CInt SetPhysicsCameraMinTilt(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraMinTilt()", COLOR_RED);
		return 0;
	}
	if (g_camera)
	{
		CFloat value = (CFloat)lua_tonumber(L, 1);
		g_camera->m_perspectiveCameraMinTilt = NxMath::degToRad(value);
	}
	return 0;
}

CInt GetPhysicsCameraTilt(lua_State *L)
{
	if (g_camera)
	{
		lua_pushnumber(L, NxMath::radToDeg(g_camera->m_perspectiveCurrentCameraTilt));
		return 1;
	}

	return 0;
}

CInt GetPhysicsCameraMaxTilt(lua_State *L)
{
	if (g_camera)
	{
		lua_pushnumber(L, NxMath::radToDeg(g_camera->m_perspectiveCameraMaxTilt));
		return 1;
	}

	return 0;
}

CInt GetPhysicsCameraMinTilt(lua_State *L)
{
	if (g_camera)
	{
		lua_pushnumber(L, NxMath::radToDeg(g_camera->m_perspectiveCameraMinTilt));
		return 1;
	}

	return 0;
}

CInt SetPhysicsCameraYaw(lua_State *L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraYaw()", COLOR_RED);
		return 0;
	}
	if (g_camera)
	{
		CFloat value = (CFloat)lua_tonumber(L, 1);
		g_camera->m_perspectiveCameraYaw = NxMath::degToRad(value);
	}

	return 0;
}

CInt GetPhysicsCameraYaw(lua_State *L)
{
	if (g_camera)
	{
		lua_pushnumber(L, NxMath::radToDeg(g_camera->m_perspectiveCameraYaw));
		return 1;
	}

	return 0;
}

CInt SetPhysicsCameraNearClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraNearClipPlane()", COLOR_RED);
		return 0;
	}

	CFloat ncp;

	ncp = (CFloat)lua_tonumber(L, 1);

	if (ncp < 0.0f)
	{
		//PrintInfo("\nError: argument of SetPhysicsCameraNearClipPlane() must be greater than 0.0", COLOR_RED);
		return 0;
	}

	g_cameraProperties.m_playModePerspectiveNCP = ncp;

	return 0;
}

CInt SetPhysicsCameraFarClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsCameraFarClipPlane()", COLOR_RED);
		return 0;
	}

	CFloat fcp;

	fcp = (CFloat)lua_tonumber(L, 1);

	if (fcp < 0.0f)
	{
		//PrintInfo("\nError: argument of SetPhysicsCameraFarClipPlane() must be greater than 0.0", COLOR_RED);
		return 0;
	}

	g_cameraProperties.m_playModePerspectiveFCP = fcp;

	return 0;
}

CInt GetPhysicsCameraNearClipPlane(lua_State* L)
{
	CFloat ncp = g_cameraProperties.m_playModePerspectiveNCP;

	lua_pushnumber(L, ncp);

	return 1;
}

CInt GetPhysicsCameraFarClipPlane(lua_State* L)
{
	CFloat fcp = g_cameraProperties.m_playModePerspectiveFCP;

	lua_pushnumber(L, fcp);

	return 1;
}


CInt LoadResource(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify 2 arguments for LoadResource()", MB_OK | MB_ICONERROR);
		return 0;
	}

	CChar luaToString1[MAX_NAME_SIZE];
	Cpy(luaToString1, lua_tostring(L, 1));
	StringToUpper(luaToString1); //package name

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	CChar fileName[MAX_NAME_SIZE];
	Cpy(fileName, "Assets/Resources/");

	Append(fileName, luaToString1);
	Append(fileName, "/");
	Append(fileName, luaToString2);

	CChar extension[MAX_NAME_SIZE];
	if (GetFileExtension(luaToString2))
		Cpy(extension, GetFileExtension(luaToString2));
	else
		return 0;

	CChar name[MAX_NAME_SIZE];
	Cpy(name, luaToString1);
	Append(name, "_");
	Append(name, luaToString2);
	StringToUpper(name);

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CChar old_name[MAX_NAME_SIZE];
		Cpy(old_name, g_resourceFiles[i]->GetName());
		StringToUpper(old_name);

		if (Cmp(name, old_name))
			return 0;
	}

	if (Cmp(extension, ".OGG"))
	{
		CResourceFile* oggFile = CNew(CResourceFile);
		if (oggFile->LoadOGGFile(fileName))
		{
			oggFile->SetName(name);
			oggFile->GetSoundSource()->SetName(name);
			oggFile->SetDirectoryName(luaToString1);
			oggFile->SetFileName(luaToString2);
			g_resourceFiles.push_back(oggFile);
		}
		else
		{
			CDelete(oggFile);
		}
	}
	else if (Cmp(extension, ".DDS"))
	{
		CResourceFile* ddsFile = CNew(CResourceFile);
		if (ddsFile->LoadDDSFile(fileName))
		{
			ddsFile->SetName(name);
			ddsFile->SetDirectoryName(luaToString1);
			ddsFile->SetFileName(luaToString2);
			g_resourceFiles.push_back(ddsFile);
		}
		else
		{
			CDelete(ddsFile);
		}
	}
	else if (Cmp(extension, ".AVI"))
	{
		CResourceFile* aviFile = CNew(CResourceFile);
		if (aviFile->LoadAVIFile(fileName))
		{
			aviFile->SetName(name);
			aviFile->SetDirectoryName(luaToString1);
			aviFile->SetFileName(luaToString2);
			g_resourceFiles.push_back(aviFile);
		}
		else
		{
			CDelete(aviFile);
		}
	}

	return 0;
}

CInt DeleteResource(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for DeleteResource()", COLOR_RED);
		return 0;
	}

	CChar luaToString1[MAX_NAME_SIZE];
	Cpy(luaToString1, lua_tostring(L, 1));
	StringToUpper(luaToString1); //directory name

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2); //file name

	for (CUInt j = 0; j < g_resourceFiles.size(); j++)
	{
		if (Cmp(g_resourceFiles[j]->GetDirectoryName(), luaToString1) && Cmp(g_resourceFiles[j]->GetFileName(), luaToString2))
		{
			CDelete(g_resourceFiles[j]);
			g_resourceFiles.erase(g_resourceFiles.begin() + j);
			break;
		}
	}
	return 0;
}

CInt DeleteAllResources(lua_State* L)
{
	for (CUInt j = 0; j < g_resourceFiles.size(); j++)
		CDelete(g_resourceFiles[j]);
	g_resourceFiles.clear();

	if (g_main && g_main->GetCursorIcon())
		g_main->GetCursorIcon()->SetVisible(CFalse);
	return 0;
}

CInt PlayResourceSound(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify at least 1 argument for PlayResourceSound()", COLOR_RED);
		return 0;
	}

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString); //package name

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_resourceFiles.size(); i++)
		{
			if (g_resourceFiles[i]->GetSoundSource())
			{
				CChar string[MAX_NAME_SIZE];
				Cpy(string, g_resourceFiles[i]->GetSoundSource()->GetName());
				StringToUpper(string); //package name

				if (Cmp(string, luaToString))
				{
					g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
					//CChar temp[MAX_NAME_SIZE];
					//sprintf(temp, "%s%s%s", "\nSound '", g_resourceFiles[i]->GetSoundSource()->GetName(), "' was played.");
					//PrintInfo(temp, COLOR_GREEN);
					foundTarget = CTrue;
					break;
				}
			}
		}
		if (!foundTarget)
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nPlayResourceSound() Error: Couldn't find the sound '", lua_tostring(L, n), "' to be played.");
			//PrintInfo(temp, COLOR_RED);
		}
	}

	return 0;
}

CInt PlayResourceSoundLoop(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify at least 1 argument for PlayResourceSoundLoop()", MB_OK | MB_ICONERROR);
		return 0;
	}

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString); //package name

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_resourceFiles.size(); i++)
		{
			if (g_resourceFiles[i]->GetSoundSource())
			{
				CChar string[MAX_NAME_SIZE];
				Cpy(string, g_resourceFiles[i]->GetSoundSource()->GetName());
				StringToUpper(string); //package name

				if (Cmp(string, luaToString))
				{
					g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
					g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
					foundTarget = CTrue;
					break;
				}
			}
		}
		if (!foundTarget)
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCouldn't find the sound '", lua_tostring(L, n), "' to be played.");
			MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
		}
	}

	return 0;
}

CInt PlayResourceSoundOnce(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify at least 1 argument for PlayResourceSoundLoop()", MB_OK | MB_ICONERROR);

		return 0;
	}

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString); //package name

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_resourceFiles.size(); i++)
		{
			if (g_resourceFiles[i]->GetSoundSource())
			{
				CChar string[MAX_NAME_SIZE];
				Cpy(string, g_resourceFiles[i]->GetSoundSource()->GetName());
				StringToUpper(string); //package name

				if (Cmp(string, luaToString))
				{
					g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
					g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));

					foundTarget = CTrue;
					break;
				}
			}
		}
		if (!foundTarget)
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCouldn't find the sound '", lua_tostring(L, n), "' to be played once.");
			MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
		}
	}

	return 0;
}

CInt StopResourceSound(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify at least 1 argument for StopResourceSound()", MB_OK | MB_ICONERROR);

		return 0;
	}

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString); //package name

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_resourceFiles.size(); i++)
		{
			if (g_resourceFiles[i]->GetSoundSource())
			{
				CChar string[MAX_NAME_SIZE];
				Cpy(string, g_resourceFiles[i]->GetSoundSource()->GetName());
				StringToUpper(string); //package name

				if (Cmp(string, luaToString))
				{
					g_soundSystem->StopALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
					foundTarget = CTrue;
					break;
				}
			}
		}
		if (!foundTarget)
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCouldn't find the sound '", lua_tostring(L, n), "' to be stopped.");
			MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
		}
	}

	return 0;
}

CInt PauseResourceSound(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify at least 1 argument for StopResourceSound()", MB_OK | MB_ICONERROR);

		return 0;
	}

	for (int n = 1; n <= argc; ++n)
	{
		CChar luaToString[MAX_NAME_SIZE];
		Cpy(luaToString, lua_tostring(L, n));
		StringToUpper(luaToString); //package name

		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_resourceFiles.size(); i++)
		{
			if (g_resourceFiles[i]->GetSoundSource())
			{
				CChar string[MAX_NAME_SIZE];
				Cpy(string, g_resourceFiles[i]->GetSoundSource()->GetName());
				StringToUpper(string); //package name

				if (Cmp(string, luaToString))
				{
					g_soundSystem->PauseALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
					foundTarget = CTrue;
					break;
				}
			}
		}
		if (!foundTarget)
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "\nCouldn't find the sound '", lua_tostring(L, n), "' to be paused.");
			MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
		}
	}

	return 0;
}

CInt ShowGUI(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify 1 argument for ShowGUI()", MB_OK | MB_ICONERROR);
		return 0;
	}
	CBool foundTarget = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, luaToString))
		{
			g_guis[i]->SetVisible(CTrue);

			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCoudn't find the GUI '", luaToString, "' to be activated.");
		MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
	}

	return 0;
}

CInt HideGUI(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify 1 argument for HideGUI()", MB_OK | MB_ICONERROR);
		return 0;
	}
	CBool foundTarget = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, luaToString))
		{
			g_guis[i]->SetVisible(CFalse);

			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCoudn't find the GUI '", luaToString, "' to be deactivated.");
		MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
	}

	return 0;
}

CInt ShowCursorIcon(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify 2 arguments for ShowCursorIcon()", MB_OK | MB_ICONERROR);
		return 0;
	}

	CFloat size = (CFloat)lua_tonumber(L, 2);
	if (size <= 0.0)
	{
		//PrintInfo("\nShowCursorIcon() Error: cursor size must be greater than 0.0", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString); //package name

	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		if (g_resourceFiles[i]->GetImage())
		{
			CChar string[MAX_NAME_SIZE];
			Cpy(string, g_resourceFiles[i]->GetName());
			StringToUpper(string); //package name

			if (Cmp(string, luaToString))
			{
				g_main->GetCursorIcon()->SetVisible(CTrue);
				g_main->GetCursorIcon()->SetName(string);
				g_main->GetCursorIcon()->SetImage(g_resourceFiles[i]->GetImage());
				g_main->GetCursorIcon()->SetSize(lua_tonumber(L, 2));

				foundTarget = CTrue;
				break;
			}
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCouldn't find the image '", lua_tostring(L, 1), "' to be shown.");
		MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
	}

	return 0;
}

CInt HideCursorIcon(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		MessageBoxA(NULL, "Vanda Engine Error", "Please specify 1 argument for HideCursorIcon()", MB_OK | MB_ICONERROR);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString); //package name

	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		if (g_resourceFiles[i]->GetImage())
		{
			CChar string[MAX_NAME_SIZE];
			Cpy(string, g_resourceFiles[i]->GetName());
			StringToUpper(string); //package name

			if (Cmp(string, luaToString))
			{
				g_main->GetCursorIcon()->SetVisible(CFalse);
				foundTarget = CTrue;
				break;
			}
		}
	}
	if (!foundTarget)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "\nCouldn't find the image '", lua_tostring(L, 1), "' to be hidden.");
		MessageBoxA(NULL, "Vanda Engine Error", temp, MB_OK | MB_ICONERROR);
	}

	return 0;
}

CInt PrintConsole(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PrintConsole", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	//PrintInfo(luaToString);

	return 0;
}

CInt IsKeyDown(lua_State *L)
{
	if (g_main && g_main->m_loadScene) return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for IsKeyDown()", COLOR_RED);
		lua_pushboolean(L, 0);
		return 0;
	}

	CChar keyCode[MAX_NAME_SIZE];
	Cpy(keyCode, lua_tostring(L, 1)); //DirecInput Key Code
	StringToUpper(keyCode);

	g_input.Update();

	if (Cmp(keyCode, "0"))
	{
		if (g_input.ButtonDown(0)) //left click
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "1"))
	{
		if (g_input.ButtonDown(1)) //right click
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "2"))
	{
		if (g_input.ButtonDown(2)) //middle button click
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_ESCAPE"))
	{
		if (g_input.KeyDown(DIK_ESCAPE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_1"))
	{
		if (g_input.KeyDown(DIK_1))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_2"))
	{
		if (g_input.KeyDown(DIK_2))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_3"))
	{
		if (g_input.KeyDown(DIK_3))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_4"))
	{
		if (g_input.KeyDown(DIK_4))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_5"))
	{
		if (g_input.KeyDown(DIK_5))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_6"))
	{
		if (g_input.KeyDown(DIK_6))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_7"))
	{
		if (g_input.KeyDown(DIK_7))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_8"))
	{
		if (g_input.KeyDown(DIK_8))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_9"))
	{
		if (g_input.KeyDown(DIK_9))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_0"))
	{
		if (g_input.KeyDown(DIK_0))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_MINUS"))
	{
		if (g_input.KeyDown(DIK_MINUS))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_EQUALS"))
	{
		if (g_input.KeyDown(DIK_EQUALS))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_BACK"))
	{
		if (g_input.KeyDown(DIK_BACK))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_TAB"))
	{
		if (g_input.KeyDown(DIK_TAB))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_Q"))
	{
		if (g_input.KeyDown(DIK_Q))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_W"))
	{
		if (g_input.KeyDown(DIK_W))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_E"))
	{
		if (g_input.KeyDown(DIK_E))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_R"))
	{
		if (g_input.KeyDown(DIK_R))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_T"))
	{
		if (g_input.KeyDown(DIK_T))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_Y"))
	{
		if (g_input.KeyDown(DIK_Y))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_U"))
	{
		if (g_input.KeyDown(DIK_U))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_I"))
	{
		if (g_input.KeyDown(DIK_I))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_O"))
	{
		if (g_input.KeyDown(DIK_O))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_P"))
	{
		if (g_input.KeyDown(DIK_P))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LBRACKET"))
	{
		if (g_input.KeyDown(DIK_LBRACKET))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RBRACKET"))
	{
		if (g_input.KeyDown(DIK_RBRACKET))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RETURN"))
	{
		if (g_input.KeyDown(DIK_RETURN))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LCONTROL"))
	{
		if (g_input.KeyDown(DIK_LCONTROL))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_A"))
	{
		if (g_input.KeyDown(DIK_A))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_S"))
	{
		if (g_input.KeyDown(DIK_S))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_D"))
	{
		if (g_input.KeyDown(DIK_D))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F"))
	{
		if (g_input.KeyDown(DIK_F))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_G"))
	{
		if (g_input.KeyDown(DIK_G))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_H"))
	{
		if (g_input.KeyDown(DIK_H))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_J"))
	{
		if (g_input.KeyDown(DIK_J))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_K"))
	{
		if (g_input.KeyDown(DIK_K))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_L"))
	{
		if (g_input.KeyDown(DIK_L))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SEMICOLON"))
	{
		if (g_input.KeyDown(DIK_SEMICOLON))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_APOSTROPHE"))
	{
		if (g_input.KeyDown(DIK_APOSTROPHE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_GRAVE"))
	{
		if (g_input.KeyDown(DIK_GRAVE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LSHIFT"))
	{
		if (g_input.KeyDown(DIK_LSHIFT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_BACKSLASH"))
	{
		if (g_input.KeyDown(DIK_BACKSLASH))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_Z"))
	{
		if (g_input.KeyDown(DIK_Z))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_X"))
	{
		if (g_input.KeyDown(DIK_X))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_C"))
	{
		if (g_input.KeyDown(DIK_C))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_V"))
	{
		if (g_input.KeyDown(DIK_V))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_B"))
	{
		if (g_input.KeyDown(DIK_B))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_N"))
	{
		if (g_input.KeyDown(DIK_N))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_M"))
	{
		if (g_input.KeyDown(DIK_M))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_COMMA"))
	{
		if (g_input.KeyDown(DIK_COMMA))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_PERIOD"))
	{
		if (g_input.KeyDown(DIK_PERIOD))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SLASH"))
	{
		if (g_input.KeyDown(DIK_SLASH))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RSHIFT"))
	{
		if (g_input.KeyDown(DIK_RSHIFT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_MULTIPLY"))
	{
		if (g_input.KeyDown(DIK_MULTIPLY))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LMENU"))
	{
		if (g_input.KeyDown(DIK_LMENU))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SPACE"))
	{
		if (g_input.KeyDown(DIK_SPACE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_CAPITAL"))
	{
		if (g_input.KeyDown(DIK_CAPITAL))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F1"))
	{
		if (g_input.KeyDown(DIK_F1))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F2"))
	{
		if (g_input.KeyDown(DIK_F2))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F3"))
	{
		if (g_input.KeyDown(DIK_F3))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	if (Cmp(keyCode, "DIK_F4"))
	{
		if (g_input.KeyDown(DIK_F4))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F5"))
	{
		if (g_input.KeyDown(DIK_F5))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F6"))
	{
		if (g_input.KeyDown(DIK_F6))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F7"))
	{
		if (g_input.KeyDown(DIK_F7))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F8"))
	{
		if (g_input.KeyDown(DIK_F8))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F9"))
	{
		if (g_input.KeyDown(DIK_F9))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F10"))
	{
		if (g_input.KeyDown(DIK_F10))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMLOCK"))
	{
		if (g_input.KeyDown(DIK_NUMLOCK))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SCROLL"))
	{
		if (g_input.KeyDown(DIK_SCROLL))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD7"))
	{
		if (g_input.KeyDown(DIK_NUMPAD7))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD8"))
	{
		if (g_input.KeyDown(DIK_NUMPAD8))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD9"))
	{
		if (g_input.KeyDown(DIK_NUMPAD9))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SUBTRACT"))
	{
		if (g_input.KeyDown(DIK_SUBTRACT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD4"))
	{
		if (g_input.KeyDown(DIK_NUMPAD4))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD5"))
	{
		if (g_input.KeyDown(DIK_NUMPAD5))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD6"))
	{
		if (g_input.KeyDown(DIK_NUMPAD6))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_ADD"))
	{
		if (g_input.KeyDown(DIK_ADD))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD1"))
	{
		if (g_input.KeyDown(DIK_NUMPAD1))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD2"))
	{
		if (g_input.KeyDown(DIK_NUMPAD2))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD3"))
	{
		if (g_input.KeyDown(DIK_NUMPAD3))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPAD0"))
	{
		if (g_input.KeyDown(DIK_NUMPAD0))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_DECIMAL"))
	{
		if (g_input.KeyDown(DIK_DECIMAL))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F11"))
	{
		if (g_input.KeyDown(DIK_F11))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F12"))
	{
		if (g_input.KeyDown(DIK_F12))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F13"))
	{
		if (g_input.KeyDown(DIK_F13))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F14"))
	{
		if (g_input.KeyDown(DIK_F14))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_F15"))
	{
		if (g_input.KeyDown(DIK_F15))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_KANA"))
	{
		if (g_input.KeyDown(DIK_KANA))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_CONVERT"))
	{
		if (g_input.KeyDown(DIK_CONVERT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NOCONVERT"))
	{
		if (g_input.KeyDown(DIK_NOCONVERT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_YEN"))
	{
		if (g_input.KeyDown(DIK_YEN))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPADEQUALS"))
	{
		if (g_input.KeyDown(DIK_NUMPADEQUALS))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_CIRCUMFLEX"))
	{
		if (g_input.KeyDown(DIK_CIRCUMFLEX))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_AT"))
	{
		if (g_input.KeyDown(DIK_AT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_COLON"))
	{
		if (g_input.KeyDown(DIK_COLON))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_UNDERLINE"))
	{
		if (g_input.KeyDown(DIK_UNDERLINE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_KANJI"))
	{
		if (g_input.KeyDown(DIK_KANJI))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_STOP"))
	{
		if (g_input.KeyDown(DIK_STOP))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_AX"))
	{
		if (g_input.KeyDown(DIK_AX))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_UNLABELED"))
	{
		if (g_input.KeyDown(DIK_UNLABELED))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPADENTER"))
	{
		if (g_input.KeyDown(DIK_NUMPADENTER))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RCONTROL"))
	{
		if (g_input.KeyDown(DIK_RCONTROL))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NUMPADCOMMA"))
	{
		if (g_input.KeyDown(DIK_NUMPADCOMMA))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_DIVIDE"))
	{
		if (g_input.KeyDown(DIK_DIVIDE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SYSRQ"))
	{
		if (g_input.KeyDown(DIK_SYSRQ))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RMENU"))
	{
		if (g_input.KeyDown(DIK_RMENU))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_PAUSE"))
	{
		if (g_input.KeyDown(DIK_PAUSE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_HOME"))
	{
		if (g_input.KeyDown(DIK_HOME))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_UP"))
	{
		if (g_input.KeyDown(DIK_UP))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_PRIOR"))
	{
		if (g_input.KeyDown(DIK_PRIOR))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LEFT"))
	{
		if (g_input.KeyDown(DIK_LEFT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RIGHT"))
	{
		if (g_input.KeyDown(DIK_RIGHT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_END"))
	{
		if (g_input.KeyDown(DIK_END))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_DOWN"))
	{
		if (g_input.KeyDown(DIK_DOWN))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_NEXT"))
	{
		if (g_input.KeyDown(DIK_NEXT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_INSERT"))
	{
		if (g_input.KeyDown(DIK_INSERT))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_DELETE"))
	{
		if (g_input.KeyDown(DIK_DELETE))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_LWIN"))
	{
		if (g_input.KeyDown(DIK_LWIN))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_RWIN"))
	{
		if (g_input.KeyDown(DIK_RWIN))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_APPS"))
	{
		if (g_input.KeyDown(DIK_APPS))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_POWER"))
	{
		if (g_input.KeyDown(DIK_POWER))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}
	else if (Cmp(keyCode, "DIK_SLEEP"))
	{
		if (g_input.KeyDown(DIK_SLEEP))
		{
			lua_pushboolean(L, 1); //true
			return 1;
		}
	}


	lua_pushboolean(L, 0); //false
	return 1;
}

CInt SetSelectionDistance(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetSelectionDistance()", COLOR_RED);
		return 0;
	}

	CFloat distance = CFloat(lua_tonumber(L, 1));
	if (distance <= 0.0)
	{
		//PrintInfo("\nSetSelectionDistance() Error: distance must be greater than 0.0", COLOR_RED);
		return 0;
	}

	g_main->SetSelectionDistance(CFloat(lua_tonumber(L, 1)));
	return 0;
}

CInt GetSelectionDistance(lua_State *L)
{
	CFloat selectionDistance = g_main->GetSelectionDistance();
	lua_pushnumber(L, selectionDistance);
	return 1;
}

CInt SelectPrefabInstances(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 argument for SelectPrefabInstances()", COLOR_RED);
		return 0;
	}
	CDouble mouseXPos = CDouble(lua_tonumber(L, 1));
	CDouble mouseYPos = CDouble(lua_tonumber(L, 2));
	CDouble selectionWidth = CDouble(lua_tonumber(L, 3));
	CDouble selectionHeight = CDouble(lua_tonumber(L, 4));

	CChar* selectedName = g_main->SelectPrefabInstances(mouseXPos, mouseYPos, selectionWidth, selectionHeight);
	lua_pushstring(L, selectedName);
	return 1;
}

CInt GetScreenWidth(lua_State* L)
{
	CFloat width = (CFloat)g_width;
	lua_pushnumber(L, width);
	return 1;
}

CInt GetScreenHeight(lua_State* L)
{
	CFloat height = (CFloat)g_height;
	lua_pushnumber(L, height);
	return 1;
}

CInt GetCursorX(lua_State* L)
{
	CFloat posX = g_main->GetCursorX();
	lua_pushnumber(L, posX);
	return 1;

}

CInt GetCursorY(lua_State* L)
{
	CFloat posY = g_main->GetCursorY();
	lua_pushnumber(L, posY);
	return 1;
}

CInt GetElapsedTime(lua_State* L)
{
	lua_pushnumber(L, g_elapsedTime);
	return 1;
}

CInt IsCharacterControllerLocked(lua_State* L)
{
	CBool isCharacterControlledLocked = g_currentVSceneProperties.m_lockCharacterController;
	if (isCharacterControlledLocked)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

//First Argument: Physics Actor Name
CInt GetPrefabInstanceNameFromActor(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceNameFromActor()", COLOR_RED);
		return 0;
	}

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];

	if (lua_tostring(L, 1) == NULL) return 0;

	Cpy(luaToString, lua_tostring(L, 1)); //Physics Actor Name- First Argument

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		if (prefab)
		{
			for (CUInt j = 0; j < 3; j++)
			{
				if (prefab->GetHasLod(j))
				{
					CScene* scene = g_instancePrefab[i]->GetScene(j);
					for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
					{
						if (scene->m_instanceGeometries[k]->m_hasPhysX)
						{
							if (Cmp(luaToString, scene->m_instanceGeometries[k]->m_physXName))
							{
								lua_pushstring(L, g_instancePrefab[i]->GetName());
								return 1;
							}
						}
					}
				}
			}
			if (g_instancePrefab[i]->GetHasCollider())
			{
				CScene* scene = g_instancePrefab[i]->GetScene(3);
				for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
				{
					if (scene->m_instanceGeometries[k]->m_hasPhysX)
					{
						if (Cmp(luaToString, scene->m_instanceGeometries[k]->m_physXName))
						{
							lua_pushstring(L, g_instancePrefab[i]->GetName());
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}


//First argument: prefab instance name
//2nd, 3rd, and 4th arguments: x positio, y position, z position of prefab instance
CInt TranslatePrefabInstance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for TranslatePrefabInstance()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CFloat xPos = (CFloat)lua_tonumber(L, 2);
	CFloat yPos = (CFloat)lua_tonumber(L, 3);
	CFloat zPos = (CFloat)lua_tonumber(L, 4);
	CVec3f pos(xPos, yPos, zPos);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			if (g_currentInstancePrefab->GetScene(0) && g_currentInstancePrefab->GetScene(0)->IsTransformable())
				g_currentInstancePrefab->SetTranslate(pos);
		}
		else
		{
		//	PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->IsTransformable())
			{
				g_instancePrefab[i]->SetTranslate(pos);
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//First argument: prefab instance name
//2nd, 3rd, and 4th arguments: x rotation, y rotation, z rotation of prefab instance
CInt RotatePrefabInstance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for RotatePrefabInstance()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CFloat xRot = (CFloat)lua_tonumber(L, 2);
	CFloat yRot = (CFloat)lua_tonumber(L, 3);
	CFloat zRot = (CFloat)lua_tonumber(L, 4);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			if (g_currentInstancePrefab->GetScene(0) && g_currentInstancePrefab->GetScene(0)->IsTransformable())
			{
				CVec4f rotation(xRot, yRot, zRot, g_currentInstancePrefab->GetRotate().w);
				g_currentInstancePrefab->SetRotate(rotation);
			}
		}
		else
		{
		//	PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->IsTransformable())
			{
				CVec4f rotation(xRot, yRot, zRot, g_instancePrefab[i]->GetRotate().w);
				g_instancePrefab[i]->SetRotate(rotation);
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//First argument: prefab instance name
//2nd, 3rd, and 4th arguments: x scale, y scale, z scale of prefab instance
CInt ScalePrefabInstance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for ScalePrefabInstance()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CFloat xScale = (CFloat)lua_tonumber(L, 2);
	CFloat yScale = (CFloat)lua_tonumber(L, 3);
	CFloat zScale = (CFloat)lua_tonumber(L, 4);
	CVec3f scale(xScale, yScale, zScale);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			if (g_currentInstancePrefab->GetScene(0) && g_currentInstancePrefab->GetScene(0)->IsTransformable())
			{
				g_currentInstancePrefab->SetScale(scale);
			}
		}
		else
		{
		//	PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->IsTransformable())
			{
				g_instancePrefab[i]->SetScale(scale);
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceTranslate(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for GetPrefabInstanceTranslate()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			CFloat x = g_currentInstancePrefab->GetTranslate().x;
			CFloat y = g_currentInstancePrefab->GetTranslate().y;
			CFloat z = g_currentInstancePrefab->GetTranslate().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CFloat x = g_instancePrefab[i]->GetTranslate().x;
			CFloat y = g_instancePrefab[i]->GetTranslate().y;
			CFloat z = g_instancePrefab[i]->GetTranslate().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceRotate(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for GetPrefabInstanceRotate()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			CFloat x = g_currentInstancePrefab->GetRotate().x;
			CFloat y = g_currentInstancePrefab->GetRotate().y;
			CFloat z = g_currentInstancePrefab->GetRotate().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}


	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CFloat x = g_instancePrefab[i]->GetRotate().x;
			CFloat y = g_instancePrefab[i]->GetRotate().y;
			CFloat z = g_instancePrefab[i]->GetRotate().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceScale(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for GetPrefabInstanceScale()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			CFloat x = g_currentInstancePrefab->GetScale().x;
			CFloat y = g_currentInstancePrefab->GetScale().y;
			CFloat z = g_currentInstancePrefab->GetScale().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CFloat x = g_instancePrefab[i]->GetScale().x;
			CFloat y = g_instancePrefab[i]->GetScale().y;
			CFloat z = g_instancePrefab[i]->GetScale().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceRadius(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for GetPrefabInstanceRadius()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			CFloat radius = g_currentInstancePrefab->GetRadius();

			lua_pushnumber(L, radius);

			return 1;
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CFloat radius = g_instancePrefab[i]->GetRadius();

			lua_pushnumber(L, radius);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetDistanceOfPrefabInstanceFromPhysicsCamera(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 arguments for GetDistanceOfPrefabInstanceFromPhysicsCamera()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			CFloat distance = g_currentInstancePrefab->GetDistanceFromCamera();

			lua_pushnumber(L, distance);

			return 1;
		}
		else
		{
			//PrintInfo("\nCouldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			CFloat distance = g_instancePrefab[i]->GetDistanceFromCamera();

			lua_pushnumber(L, distance);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


//depth of field
CInt EnableDepthOfField(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_dofProperties.m_enable = CTrue;
	return 0;
}

CInt DisableDepthOfField(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_dofProperties.m_enable = CFalse;
	return 0;
}

CInt SetDepthOfFieldFocalDistance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDepthOfFieldFocalDistance()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);
	if (value < 0.0f)
	{
		//PrintInfo("\nSetDepthOfFieldFocalDistance() Error: distance must be equal to or greater than 0.0", COLOR_RED);
		return 0;
	}

	g_main->m_dof.m_focalDistance = g_dofProperties.m_dofFocalDistance = value;

	return 0;
}

CInt SetDepthOfFieldFocalRange(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDepthOfFieldFocalRange()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);
	if (value < 0.0f)
	{
		//PrintInfo("\nSetDepthOfFieldFocalRange() Error: range must be equal to or greater than 0.0", COLOR_RED);
		return 0;
	}

	g_main->m_dof.m_focalRange = g_dofProperties.m_dofFocalRange = value;

	return 0;
}

CInt GetDepthOfFieldFocalDistance(lua_State* L)
{
	CFloat focalDistance = g_dofProperties.m_dofFocalDistance;

	lua_pushnumber(L, focalDistance);

	return 1;
}

CInt GetDepthOfFieldFocalRange(lua_State* L)
{
	CFloat focalRange = g_dofProperties.m_dofFocalRange;

	lua_pushnumber(L, focalRange);

	return 1;
}


//fog
CInt EnableFog(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_fogProperties.m_enable = CTrue;
	return 0;
}

CInt DisableFog(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_fogProperties.m_enable = CFalse;
	return 0;
}

CInt SetFogColor(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetFogColor()", COLOR_RED);
		return 0;
	}

	CFloat value[4];

	for (CUInt i = 1; i < 4; i++)
	{
		value[i - 1] = (CFloat)lua_tonumber(L, i);
		if (value[i - 1] < 0.0f || value[i - 1] > 1.0f)
		{
			//PrintInfo("\nError: All arguments of SetFogColor() must be between 0.0 and 1.0", COLOR_RED);
			return 0;
		}
		g_fogProperties.m_fogColor[i - 1] = value[i - 1];
	}
	g_fogProperties.m_fogColor[3] = 1.0f;

	CFog fog;
	fog.SetColor(g_fogProperties.m_fogColor);

	return 0;
}

CInt SetFogDensity(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetFogDensity()", COLOR_RED);
		return 0;
	}

	CFloat density;

	density = (CFloat)lua_tonumber(L, 1);

	if (density < 0.0f)
	{
		//PrintInfo("\nError: argument of SetFogDensity() must be greater than 0.0", COLOR_RED);
		return 0;
	}
	g_fogProperties.m_fogDensity = density;

	CFog fog;
	fog.SetDensity(g_fogProperties.m_fogDensity);

	return 0;
}

CInt GetFogColor(lua_State* L)
{
	CFloat r = g_fogProperties.m_fogColor[0];
	CFloat g = g_fogProperties.m_fogColor[1];
	CFloat b = g_fogProperties.m_fogColor[2];

	lua_pushnumber(L, r);
	lua_pushnumber(L, g);
	lua_pushnumber(L, b);

	return 3;
}

CInt GetFogDensity(lua_State* L)
{
	CFloat density = g_fogProperties.m_fogDensity;

	lua_pushnumber(L, density);

	return 1;
}

//bloom
CInt EnableBloom(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_bloomProperties.m_enable = CTrue;
	return 0;
}

CInt DisableBloom(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_bloomProperties.m_enable = CFalse;
	return 0;
}

CInt SetBloomColor(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetBloomColor()", COLOR_RED);
		return 0;
	}

	CFloat value[3];
	for (CUInt i = 1; i < 4; i++)
	{
		value[i - 1] = (CFloat)lua_tonumber(L, i);
		if (value[i - 1] < 0.0f || value[i - 1] > 1.0f)
		{
			//PrintInfo("\nError: All arguments of SetBloomColor() must be between 0.0 and 1.0", COLOR_RED);
			return 0;
		}
		g_bloomProperties.m_bloomColor[i - 1] = value[i - 1];
	}

	return 0;
}

CInt SetBloomIntensity(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetBloomIntensity()", COLOR_RED);
		return 0;
	}

	CFloat intensity;

	intensity = (CFloat)lua_tonumber(L, 1);
	if (intensity < 0.0f || intensity > 1.0f)
	{
		//PrintInfo("\nError: argument of SetBloomIntensity() must be between 0.0 and 1.0", COLOR_RED);
		return 0;
	}

	g_bloomProperties.m_bloomIntensity = intensity;

	return 0;
}

CInt GetBloomColor(lua_State* L)
{
	CFloat r = g_bloomProperties.m_bloomColor[0];
	CFloat g = g_bloomProperties.m_bloomColor[1];
	CFloat b = g_bloomProperties.m_bloomColor[2];

	lua_pushnumber(L, r);
	lua_pushnumber(L, g);
	lua_pushnumber(L, b);

	return 3;
}

CInt GetBloomIntensity(lua_State* L)
{
	CFloat intensity = g_bloomProperties.m_bloomIntensity;

	lua_pushnumber(L, intensity);

	return 1;
}

//shadow
CInt EnableDirectionalShadow(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_shadowProperties.m_enable = CTrue;

	return 0;
}

CInt DisableDirectionalShadow(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	g_shadowProperties.m_enable = CFalse;

	return 0;
}

CInt SetDirectionalShadowAlgorithm(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowAlgorithm()", COLOR_RED);
		return 0;
	}
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "SHADOW_SINGLE_HL"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_SINGLE_HL;
	}
	else if (Cmp(luaToString, "SHADOW_SINGLE"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_SINGLE;
	}
	else if (Cmp(luaToString, "SHADOW_MULTI_LEAK"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_MULTI_LEAK;
	}
	else if (Cmp(luaToString, "SHADOW_MULTI_NOLEAK"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_MULTI_NOLEAK;
	}
	else if (Cmp(luaToString, "SHADOW_PCF"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_PCF;
	}
	else if (Cmp(luaToString, "SHADOW_PCF_TRILIN"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_PCF_TRILIN;
	}
	else if (Cmp(luaToString, "SHADOW_PCF_4TAP"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_PCF_4TAP;
	}
	else if (Cmp(luaToString, "SHADOW_PCF_8TAP"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_PCF_8TAP;
	}
	else if (Cmp(luaToString, "SHADOW_PCF_GAUSSIAN"))
	{
		g_shadowProperties.m_shadowType = eSHADOW_PCF_GAUSSIAN;
	}
	else
	{
		//PrintInfo("\nError: SetDirectionalShadowAlgorithm() argument is invalid", COLOR_RED);
	}

	return 0;
}

CInt SetDirectionalShadowNumberOfSplits(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_dynamicShadowMap == NULL)
		return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowNumberOfSplits()", COLOR_RED);
		return 0;
	}
	int splits = (CInt)lua_tonumber(L, 1);

	if (splits == 1)
	{
		g_shadowProperties.m_shadowSplits = eSHADOW_1_SPLIT;
	}
	else if (splits == 2)
	{
		g_shadowProperties.m_shadowSplits = eSHADOW_2_SPLITS;
	}
	else if (splits == 3)
	{
		g_shadowProperties.m_shadowSplits = eSHADOW_3_SPLITS;
	}
	else if (splits == 4)
	{
		g_shadowProperties.m_shadowSplits = eSHADOW_4_SPLITS;
	}
	else
	{
		//PrintInfo("\nError:SetDirectionalShadowNumberOfSplits() argument must be 1, 2, 3 or 4", COLOR_RED);
		return 0;
	}

	switch (g_shadowProperties.m_shadowSplits)
	{
	case eSHADOW_1_SPLIT:
		g_dynamicShadowMap->cur_num_splits = 1;
		break;
	case eSHADOW_2_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 2;
		break;
	case eSHADOW_3_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 3;
		break;
	case eSHADOW_4_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 4;
		break;
	default:
		break;
	}

	return 0;
}

CInt SetDirectionalShadowWeightOfSplits(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_dynamicShadowMap == NULL)
		return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowWeightOfSplits()", COLOR_RED);
		return 0;
	}

	CFloat weight = (CFloat)lua_tonumber(L, 1);

	if (weight < 0.0f || weight > 1.0f)
	{
		//PrintInfo("\nError: argument of SetDirectionalShadowWeightOfSplits() must be between 0.0 and 1.0", COLOR_RED);
		return 0;
	}

	g_shadowProperties.m_shadowSplitWeight = weight;

	g_dynamicShadowMap->split_weight = g_shadowProperties.m_shadowSplitWeight;

	return 0;
}

CInt SetDirectionalShadowNearClipPlane(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowNearClipPlane()", COLOR_RED);
		return 0;
	}

	CFloat ncp = (CFloat)lua_tonumber(L, 1);

	if (ncp <= 0.0f)
	{
		//PrintInfo("\nError: SetDirectionalShadowNearClipPlane() argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_shadowProperties.m_shadowNearClipPlane = ncp;

	return 0;
}

CInt SetDirectionalShadowFarClipPlane(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowFarClipPlane()", COLOR_RED);
		return 0;
	}

	CFloat fcp = (CFloat)lua_tonumber(L, 1);

	if (fcp <= 0.0f)
	{
		//PrintInfo("\nError: SetDirectionalShadowFarClipPlane() argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_shadowProperties.m_shadowFarClipPlane = fcp;

	return 0;
}

CInt SetDirectionalShadowResolution(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_dynamicShadowMap == NULL)
		return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowResolution()", COLOR_RED);
		return 0;
	}
	int resolution = (CInt)lua_tonumber(L, 1);

	if (resolution == 1024)
	{
		g_shadowProperties.m_shadowResolution = eSHADOW_1024;
	}
	else if (resolution == 2048)
	{
		g_shadowProperties.m_shadowResolution = eSHADOW_2048;
	}
	else if (resolution == 4096)
	{
		g_shadowProperties.m_shadowResolution = eSHADOW_4096;
	}
	else
	{
		//PrintInfo("\nError:SetDirectionalShadowResolution() argument must be 1024, 2048 or 4096", COLOR_RED);
		return 0;
	}

	switch (g_shadowProperties.m_shadowResolution)
	{
	case eSHADOW_1024:
		g_dynamicShadowMap->depth_size = 1024;
		break;
	case eSHADOW_2048:
		g_dynamicShadowMap->depth_size = 2048;
		break;
	case eSHADOW_4096:
		g_dynamicShadowMap->depth_size = 4096;
		break;
	default:
		break;
	}

	g_dynamicShadowMap->RegenerateDepthTex(g_dynamicShadowMap->depth_size);

	return 0;
}

CInt SetDirectionalShadowIntensity(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowIntensity()", COLOR_RED);
		return 0;
	}

	CFloat intensity = (CFloat)lua_tonumber(L, 1);

	if (intensity < 0.0f || intensity > 1.0f)
	{
		//PrintInfo("\nError: SetDirectionalShadowIntensity() argument must be between 0.0 and 1.0", COLOR_RED);
		return 0;
	}

	g_shadowProperties.m_intensity = intensity;

	return 0;
}

CInt SetDirectionalShadowLight(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDirectionalShadowLight()", COLOR_RED);
		return 0;
	}
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool foundTarget = CFalse;
	for (CUInt j = 0; j < g_engineLights.size(); j++)
	{
		if (g_engineLights[j]->m_abstractLight->GetType() == eLIGHTTYPE_DIRECTIONAL)
		{
			if (Cmp(g_engineLights[j]->m_abstractLight->GetName(), luaToString))
			{
				foundTarget = CTrue;
				break;
			}
		}
	}
	if (!foundTarget)
	{
		//PrintInfo("\nSetDirectionalShadowLight() Error:light name does not exist", COLOR_RED);
		return 0;
	}

	Cpy(g_shadowProperties.m_directionalLightName, luaToString);

	return 0;
}

CInt GetDirectionalShadowAlgorithm(lua_State* L)
{
	CChar algorithm[MAX_NAME_SIZE];

	if (g_shadowProperties.m_shadowType == eSHADOW_SINGLE_HL)
	{
		Cpy(algorithm, "SHADOW_SINGLE_HL");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_SINGLE)
	{
		Cpy(algorithm, "SHADOW_SINGLE");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_MULTI_LEAK)
	{
		Cpy(algorithm, "SHADOW_MULTI_LEAK");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_MULTI_NOLEAK)
	{
		Cpy(algorithm, "SHADOW_MULTI_NOLEAK");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_PCF)
	{
		Cpy(algorithm, "SHADOW_PCF");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_PCF_TRILIN)
	{
		Cpy(algorithm, "SHADOW_PCF_TRILIN");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_PCF_4TAP)
	{
		Cpy(algorithm, "SHADOW_PCF_4TAP");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_PCF_8TAP)
	{
		Cpy(algorithm, "SHADOW_PCF_8TAP");
	}
	else if (g_shadowProperties.m_shadowType == eSHADOW_PCF_GAUSSIAN)
	{
		Cpy(algorithm, "SHADOW_PCF_GAUSSIAN");
	}

	lua_pushstring(L, algorithm);

	return 1;
}

CInt GetDirectionalShadowNumberOfSplits(lua_State* L)
{
	CInt splits = 0;

	if (g_shadowProperties.m_shadowSplits == eSHADOW_1_SPLIT)
	{
		splits = 1;
	}
	else if (g_shadowProperties.m_shadowSplits == eSHADOW_2_SPLITS)
	{
		splits = 2;
	}
	else if (g_shadowProperties.m_shadowSplits == eSHADOW_3_SPLITS)
	{
		splits = 3;
	}
	else if (g_shadowProperties.m_shadowSplits == eSHADOW_4_SPLITS)
	{
		splits = 4;
	}

	lua_pushinteger(L, splits);

	return 1;
}

CInt GetDirectionalShadowWeightOfSplits(lua_State* L)
{
	CFloat value = g_shadowProperties.m_shadowSplitWeight;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetDirectionalShadowNearClipPlane(lua_State* L)
{
	CFloat value = g_shadowProperties.m_shadowNearClipPlane;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetDirectionalShadowFarClipPlane(lua_State* L)
{
	CFloat value = g_shadowProperties.m_shadowFarClipPlane;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetDirectionalShadowResolution(lua_State* L)
{
	CInt resolution = 0;

	if (g_shadowProperties.m_shadowResolution == eSHADOW_1024)
	{
		resolution = 1024;
	}
	else if (g_shadowProperties.m_shadowResolution == eSHADOW_2048)
	{
		resolution = 2048;
	}
	else if (g_shadowProperties.m_shadowResolution == eSHADOW_4096)
	{
		resolution = 4096;
	}

	lua_pushinteger(L, resolution);

	return 1;
}

CInt GetDirectionalShadowIntensity(lua_State* L)
{
	CFloat value = g_shadowProperties.m_intensity;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetDirectionalShadowLight(lua_State* L)
{
	lua_pushstring(L, g_shadowProperties.m_directionalLightName);

	return 1;
}


CInt SetLightAmbient(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetLightAmbient()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetLightAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetLightAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetLightAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->SetAmbient(Color);
		}
		else
		{
			//PrintInfo("\nSetLightAmbient Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	CBool foundTarget = CFalse;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			g_engineLights[i]->m_abstractLight->SetAmbient(Color);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetLightAmbient() Error: Couldn't find light '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetLightDiffuse(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetLightDiffuse()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetLightDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetLightDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetLightDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->SetDiffuse(Color);
		}
		else
		{
			//PrintInfo("\nSetLightDiffuse Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	CBool foundTarget = CFalse;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			g_engineLights[i]->m_abstractLight->SetDiffuse(Color);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetLightDiffuse() Error: Couldn't find light '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetLightSpecular(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetLightSpecular()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetLightSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetLightSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetLightSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->SetSpecular(Color);
		}
		else
		{
			//PrintInfo("\nSetLightSpecular Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	CBool foundTarget = CFalse;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			g_engineLights[i]->m_abstractLight->SetSpecular(Color);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetLightSpecular() Error: Couldn't find light '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetLightShininess(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetLightShininess()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat shininess = (CFloat)lua_tonumber(L, 2);

	if (shininess < 0.0)
	{
		//PrintInfo("\nSetLightShininess() Error: shininess must be equal to or greater than 0.0", COLOR_RED);
		return 0;
	}

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->SetShininess(shininess);
		}
		else
		{
			//PrintInfo("\nSetLightShininess Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	CBool foundTarget = CFalse;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			g_engineLights[i]->m_abstractLight->SetShininess(shininess);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetLightShininess() Error: Couldn't find light '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt GetLightAmbient(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetLightAmbient()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			CFloat x = g_currentLight->GetAmbient()[0];
			CFloat y = g_currentLight->GetAmbient()[1];
			CFloat z = g_currentLight->GetAmbient()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetLightAmbient Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			CFloat x = g_engineLights[i]->m_abstractLight->GetAmbient()[0];
			CFloat y = g_engineLights[i]->m_abstractLight->GetAmbient()[1];
			CFloat z = g_engineLights[i]->m_abstractLight->GetAmbient()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}

	//CChar message[MAX_URI_SIZE];
	//sprintf(message, "\nGetLightAmbient Error: Couldn't find light '%s'", lua_tostring(L, 1));
	//PrintInfo(message, COLOR_RED);

	return 0;
}

CInt GetLightDiffuse(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetLightDiffuse()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			CFloat x = g_currentLight->GetDiffuse()[0];
			CFloat y = g_currentLight->GetDiffuse()[1];
			CFloat z = g_currentLight->GetDiffuse()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetLightDiffuse Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			CFloat x = g_engineLights[i]->m_abstractLight->GetDiffuse()[0];
			CFloat y = g_engineLights[i]->m_abstractLight->GetDiffuse()[1];
			CFloat z = g_engineLights[i]->m_abstractLight->GetDiffuse()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}

	//CChar message[MAX_URI_SIZE];
	//sprintf(message, "\nGetLightDiffuse Error: Couldn't find light '%s'", lua_tostring(L, 1));
	//PrintInfo(message, COLOR_RED);

	return 0;
}

CInt GetLightSpecular(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetLightSpecular()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			CFloat x = g_currentLight->GetSpecular()[0];
			CFloat y = g_currentLight->GetSpecular()[1];
			CFloat z = g_currentLight->GetSpecular()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetLightSpecular Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			CFloat x = g_engineLights[i]->m_abstractLight->GetSpecular()[0];
			CFloat y = g_engineLights[i]->m_abstractLight->GetSpecular()[1];
			CFloat z = g_engineLights[i]->m_abstractLight->GetSpecular()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}

	//CChar message[MAX_URI_SIZE];
	//sprintf(message, "\nGetLightSpecular Error: Couldn't find light '%s'", lua_tostring(L, 1));
	//PrintInfo(message, COLOR_RED);

	return 0;
}

CInt GetLightShininess(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetLightShininess()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			CFloat val = g_currentLight->GetShininess();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetLightShininess Error: Couldn't find current light", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);

		if (Cmp(luaToString, lightName))
		{
			CFloat val = g_engineLights[i]->m_abstractLight->GetShininess();
			lua_pushnumber(L, val);
			return 1;
		}
	}

	//CChar message[MAX_URI_SIZE];
	//sprintf(message, "\nGetLightShininess Error: Couldn't find light '%s'", lua_tostring(L, 1));
	//PrintInfo(message, COLOR_RED);

	return 0;
}

CInt SetPrefabInstanceAmbient(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetPrefabInstanceAmbient()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetPrefabInstanceAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetPrefabInstanceAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetAmbient(Color);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceAmbient() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetAmbient(Color);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceAmbient() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceDiffuse(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetPrefabInstanceDiffuse()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetPrefabInstanceDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetPrefabInstanceDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetDiffuse(Color);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceDiffuse() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetDiffuse(Color);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceDiffuse() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceSpecular(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetPrefabInstanceSpecular()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetPrefabInstanceSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetPrefabInstanceSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetSpecular(Color);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceSpecular() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetSpecular(Color);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceSpecular() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceEmission(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetPrefabInstanceEmission()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceEmission() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetPrefabInstanceEmission() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetPrefabInstanceEmission() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetEmission(Color);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceEmission() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetEmission(Color);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceEmission() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceShininess(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetPrefabInstanceShininess()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat shininess = (CFloat)lua_tonumber(L, 2);
	if (shininess < 0.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceShininess() must be 0.0 or higher", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetShininess(shininess);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceShininess() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetShininess(shininess);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceShininess() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceTransparency(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetPrefabInstanceTransparency()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat transparency = (CFloat)lua_tonumber(L, 2);
	if (transparency < 0.0f || transparency > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetPrefabInstanceTransparency() must be in the range [0.0,1.0]", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetTransparency(transparency);
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceTransparency() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetTransparency(transparency);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceTransparency() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt EnablePrefabInstanceMaterial(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for EnablePrefabInstanceMaterial()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
			g_currentInstancePrefab->EnableMaterial();
		//else
		//	PrintInfo("\nEnablePrefabInstanceMaterial() Error: Couldn't find current prefab instance", COLOR_RED);
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->EnableMaterial();
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nEnablePrefabInstanceMaterial() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt DisablePrefabInstanceMaterial(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for DisablePrefabInstanceMaterial()", COLOR_RED);
		return 0;
	}
	CBool foundPrefabInstance = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
			g_currentInstancePrefab->DisableMaterial();
		//else
		//	PrintInfo("\nDisablePrefabInstanceMaterial Error: Couldn't find current prefab instance", COLOR_RED);
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->DisableMaterial();
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nDisablePrefabInstanceMaterial() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt IsPrefabInstanceMaterialEnabled(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for IsPrefabInstanceMaterialEnabled()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			if (g_currentInstancePrefab->IsMaterialEnabled())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
		else
		{
			//PrintInfo("\nIsPrefabInstanceMaterialEnabled() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			if (g_instancePrefab[i]->IsMaterialEnabled())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nIsPrefabInstanceMaterialEnabled() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceAmbient(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceAmbient()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat ambientColor[3];

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			for (CUInt i = 0; i < 3; i++)
			{
				ambientColor[i] = g_currentInstancePrefab->GetAmbient()[i];
				lua_pushnumber(L, ambientColor[i]);
			}

			return 3;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceAmbient() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			for (CUInt j = 0; j < 3; j++)
			{
				ambientColor[j] = g_instancePrefab[i]->GetAmbient()[j];
				lua_pushnumber(L, ambientColor[j]);
			}

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceAmbient() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetPrefabInstanceDiffuse(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceDiffuse()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat diffuseColor[3];

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			for (CUInt i = 0; i < 3; i++)
			{
				diffuseColor[i] = g_currentInstancePrefab->GetDiffuse()[i];
				lua_pushnumber(L, diffuseColor[i]);
			}

			return 3;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceDiffuse() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			for (CUInt j = 0; j < 3; j++)
			{
				diffuseColor[j] = g_instancePrefab[i]->GetDiffuse()[j];
				lua_pushnumber(L, diffuseColor[j]);
			}

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceDiffuse() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceSpecular(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceSpecular()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat specularColor[3];

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			for (CUInt i = 0; i < 3; i++)
			{
				specularColor[i] = g_currentInstancePrefab->GetSpecular()[i];
				lua_pushnumber(L, specularColor[i]);
			}

			return 3;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceSpecular() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			for (CUInt j = 0; j < 3; j++)
			{
				specularColor[j] = g_instancePrefab[i]->GetSpecular()[j];
				lua_pushnumber(L, specularColor[j]);
			}

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceSpecular() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceEmission(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceEmission()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat emissionColor[3];

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			for (CUInt i = 0; i < 3; i++)
			{
				emissionColor[i] = g_currentInstancePrefab->GetEmission()[i];
				lua_pushnumber(L, emissionColor[i]);
			}

			return 3;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceEmission() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			for (CUInt j = 0; j < 3; j++)
			{
				emissionColor[j] = g_instancePrefab[i]->GetEmission()[j];
				lua_pushnumber(L, emissionColor[j]);
			}

			return 3;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceEmission() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceShininess(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceShininess()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat shininess = 0.0;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			shininess = g_currentInstancePrefab->GetShininess();
			lua_pushnumber(L, shininess);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceShininess() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			shininess = g_instancePrefab[i]->GetShininess();
			lua_pushnumber(L, shininess);
			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceShininess() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceTransparency(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPrefabInstanceTransparency()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundPrefabInstance = CFalse;

	CFloat transparency = 0.0;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			transparency = g_currentInstancePrefab->GetTransparency();
			lua_pushnumber(L, transparency);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceTransparency() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			transparency = g_instancePrefab[i]->GetTransparency();
			lua_pushnumber(L, transparency);
			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceTransparency() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}



CInt SetPhysicsDefaultRestitution(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsRestitution()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
		value = 0.0f;

	if (value > 1.0f)
		value = 1.0f;

	g_physXProperties.m_fDefaultRestitution = value;
	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(g_physXProperties.m_fDefaultRestitution);

	std::vector<std::string>physics_actor_with_material;

	for (CUInt p = 0; p < g_instancePrefab.size(); p++)
	{
		for (CUInt i = 0; i < 3; i++)
		{
			if (g_instancePrefab[p]->GetPrefab()->GetHasLod(i))
			{
				CScene* scene = g_instancePrefab[p]->GetScene(i);
				if (!scene) continue;
				if (!scene->m_isTrigger)
				{
					for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
					{
						if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
						{
							if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
							{
								physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
							}
						}
					}
				}
			}
		}
		if (g_instancePrefab[p]->GetHasCollider())
		{
			CScene* scene = g_instancePrefab[p]->GetScene(3);
			for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
			{
				if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
				{
					if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
					{
						physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
					}
				}
			}
		}
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		CBool skipActor = CFalse;
		for (CUInt j = 0; j < physics_actor_with_material.size(); j++)
		{
			if (gPhysXscene->getActors()[i]->getName() && Cmp(gPhysXscene->getActors()[i]->getName(), physics_actor_with_material[j].c_str()))
			{
				skipActor = CTrue;
				break;
			}
		}
		if (skipActor)
			continue;

		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
		currentActor->getShapes()[0]->setMaterial(defaultMaterial->getMaterialIndex());
	}

	physics_actor_with_material.clear();

	return 0;
}

CInt SetPhysicsDefaultSkinWidth(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsDefaultSkinWidth()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetPhysicsDefaultSkinWidth() Error: Skin width must be greater than 0.0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fDefaultSkinWidth = value;
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, g_physXProperties.m_fDefaultSkinWidth);

	std::vector<std::string>physics_actor_with_material;

	for (CUInt p = 0; p < g_instancePrefab.size(); p++)
	{
		for (CUInt i = 0; i < 3; i++)
		{
			if (g_instancePrefab[p]->GetPrefab()->GetHasLod(i))
			{
				CScene* scene = g_instancePrefab[p]->GetScene(i);
				if (!scene) continue;
				if (!scene->m_isTrigger)
				{
					for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
					{
						if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
						{
							if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
							{
								physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
							}
						}
					}
				}
			}
		}
		if (g_instancePrefab[p]->GetHasCollider())
		{
			CScene* scene = g_instancePrefab[p]->GetScene(3);
			for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
			{
				if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
				{
					if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
					{
						physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
					}
				}
			}
		}
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		CBool skipActor = CFalse;
		for (CUInt j = 0; j < physics_actor_with_material.size(); j++)
		{
			if (gPhysXscene->getActors()[i]->getName() && Cmp(gPhysXscene->getActors()[i]->getName(), physics_actor_with_material[j].c_str()))
			{
				skipActor = CTrue;
				break;
			}
		}
		if (skipActor)
			continue;

		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
		currentActor->getShapes()[0]->setSkinWidth(g_physXProperties.m_fDefaultSkinWidth);
	}

	physics_actor_with_material.clear();

	return 0;
}

CInt SetPhysicsDefaultStaticFriction(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsDefaultStaticFriction()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
		value = 0.0f;

	g_physXProperties.m_fDefaultStaticFriction = value;
	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setStaticFriction(g_physXProperties.m_fDefaultStaticFriction);

	std::vector<std::string>physics_actor_with_material;

	for (CUInt p = 0; p < g_instancePrefab.size(); p++)
	{
		for (CUInt i = 0; i < 3; i++)
		{
			if (g_instancePrefab[p]->GetPrefab()->GetHasLod(i))
			{
				CScene* scene = g_instancePrefab[p]->GetScene(i);
				if (!scene) continue;
				if (!scene->m_isTrigger)
				{
					for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
					{
						if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
						{
							if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
							{
								physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
							}
						}
					}
				}
			}
		}
		if (g_instancePrefab[p]->GetHasCollider())
		{
			CScene* scene = g_instancePrefab[p]->GetScene(3);
			for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
			{
				if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
				{
					if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
					{
						physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
					}
				}
			}
		}
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		CBool skipActor = CFalse;
		for (CUInt j = 0; j < physics_actor_with_material.size(); j++)
		{
			if (gPhysXscene->getActors()[i]->getName() && Cmp(gPhysXscene->getActors()[i]->getName(), physics_actor_with_material[j].c_str()))
			{
				skipActor = CTrue;
				break;
			}
		}
		if (skipActor)
			continue;

		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
		currentActor->getShapes()[0]->setMaterial(defaultMaterial->getMaterialIndex());
	}

	physics_actor_with_material.clear();

	return 0;
}

CInt SetPhysicsDefaultDynamicFriction(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsDefaultDynamicFriction()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
		value = 0.0f;

	g_physXProperties.m_fDefaultDynamicFriction = value;
	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setDynamicFriction(g_physXProperties.m_fDefaultDynamicFriction);

	std::vector<std::string>physics_actor_with_material;

	for (CUInt p = 0; p < g_instancePrefab.size(); p++)
	{
		for (CUInt i = 0; i < 3; i++)
		{
			if (g_instancePrefab[p]->GetPrefab()->GetHasLod(i))
			{
				CScene* scene = g_instancePrefab[p]->GetScene(i);
				if (!scene) continue;
				if (!scene->m_isTrigger)
				{
					for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
					{
						if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
						{
							if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
							{
								physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
							}
						}
					}
				}
			}
		}
		if (g_instancePrefab[p]->GetHasCollider())
		{
			CScene* scene = g_instancePrefab[p]->GetScene(3);
			for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
			{
				if (scene->m_instanceGeometries[j]->GetHasPhysXActor())
				{
					if (scene->m_instanceGeometries[j]->HasPhysicsMaterial())
					{
						physics_actor_with_material.push_back(scene->m_instanceGeometries[j]->GetPhysXActorName());
					}
				}
			}
		}
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		CBool skipActor = CFalse;
		for (CUInt j = 0; j < physics_actor_with_material.size(); j++)
		{
			if (gPhysXscene->getActors()[i]->getName() && Cmp(gPhysXscene->getActors()[i]->getName(), physics_actor_with_material[j].c_str()))
			{
				skipActor = CTrue;
				break;
			}
		}
		if (skipActor)
			continue;

		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
		currentActor->getShapes()[0]->setMaterial(defaultMaterial->getMaterialIndex());
	}

	physics_actor_with_material.clear();

	return 0;
}

CInt EnablePhysicsGravity(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (!gPhysXscene)
	{
		return 0;
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
	}

	g_physXProperties.m_bApplyGravity = CTrue;
	g_nx->m_defaultGravity = NxVec3(g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ);
	gPhysXscene->setGravity(NxVec3(g_nx->m_defaultGravity.x, g_nx->m_defaultGravity.y, g_nx->m_defaultGravity.z));

	return 0;
}

CInt DisablePhysicsGravity(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (!gPhysXscene)
	{
		return 0;
	}

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
	}

	g_physXProperties.m_bApplyGravity = CFalse;
	g_nx->m_defaultGravity = NxVec3(0.0f, 0.0f, 0.0f);
	gPhysXscene->setGravity(NxVec3(0.0f));

	return 0;
}

CInt SetPhysicsGravity(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPhysicsGravity()", COLOR_RED);
		return 0;
	}

	CFloat value1 = (CFloat)lua_tonumber(L, 1);
	CFloat value2 = (CFloat)lua_tonumber(L, 2);
	CFloat value3 = (CFloat)lua_tonumber(L, 3);

	g_physXProperties.m_fGravityX = value1;
	g_physXProperties.m_fGravityY = value2;
	g_physXProperties.m_fGravityZ = value3;

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
	}

	if (g_physXProperties.m_bApplyGravity)
	{
		g_nx->m_defaultGravity = NxVec3(g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ);
		gPhysXscene->setGravity(NxVec3(g_nx->m_defaultGravity.x, g_nx->m_defaultGravity.y, g_nx->m_defaultGravity.z));
	}

	return 0;
}

CInt EnablePhysicsGroundPlane(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bGroundPlane = CTrue;

	g_nx->ReleaseGroundPlane();

	g_nx->CreateGroundPlane(g_physXProperties.m_fGroundHeight);
	NxVec3 rot0(0, 0, 0);
	NxVec3 rot1(0, 0, 0);
	NxVec3 rot2(0, 0, 0);
	NxMat33 rot(rot0, rot1, rot2);

	CPhysXMaterial physicsMaterial;
	g_nx->m_groundBox = g_nx->CreateBox(NxVec3(0.0f, g_physXProperties.m_fGroundHeight - 0.5, 0.0f), NxVec3(100.0f, 0.01, 100.0f), 0, rot, NULL, CFalse, CFalse, physicsMaterial);
	g_nx->SetActorCollisionGroup(g_nx->m_groundBox, GROUP_GROUND);

	g_nx->gControllers->reportSceneChanged();
	gPhysXscene->simulate(EPSILON);
	gPhysXscene->flushStream();
	gPhysXscene->fetchResults(NX_ALL_FINISHED, true);

	return 0;
}

CInt DisablePhysicsGroundPlane(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bGroundPlane = CFalse;

	g_nx->ReleaseGroundPlane();

	g_nx->gControllers->reportSceneChanged();
	gPhysXscene->simulate(EPSILON);
	gPhysXscene->flushStream();
	gPhysXscene->fetchResults(NX_ALL_FINISHED, true);

	return 0;
}

CInt SetPhysicsGroundHeight(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetPhysicsGroundHeight()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);
	g_physXProperties.m_fGroundHeight = value;

	if (g_physXProperties.m_bGroundPlane)
	{
		g_nx->ReleaseGroundPlane();

		g_nx->CreateGroundPlane(g_physXProperties.m_fGroundHeight);
		NxVec3 rot0(0, 0, 0);
		NxVec3 rot1(0, 0, 0);
		NxVec3 rot2(0, 0, 0);
		NxMat33 rot(rot0, rot1, rot2);

		CPhysXMaterial physicsMaterial;

		g_nx->m_groundBox = g_nx->CreateBox(NxVec3(0.0f, g_physXProperties.m_fGroundHeight - 0.5, 0.0f), NxVec3(100.0f, 0.01, 100.0f), 0, rot, NULL, CFalse, CFalse, physicsMaterial);
		g_nx->SetActorCollisionGroup(g_nx->m_groundBox, GROUP_GROUND);
	}

	g_nx->gControllers->reportSceneChanged();
	gPhysXscene->simulate(EPSILON);
	gPhysXscene->flushStream();
	gPhysXscene->fetchResults(NX_ALL_FINISHED, true);

	return 0;
}

CInt SetDistanceBetweenPhysicsCameraAndCharacterController(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetDistanceBetweenPhysicsCameraAndCharacterController()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value <= 0.0f)
	{
		//PrintInfo("\nSetDistanceBetweenPhysicsCameraAndCharacterController() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCameraCharacterDistance = value;
	g_nx->gDesiredDistance = value;

	return 0;
}

CInt SetCharacterControllerCapsuleRadius(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerCapsuleRadius()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value <= 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerCapsuleRadius() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCapsuleRadius = value;
	g_nx->gControllers->setRadius(g_physXProperties.m_fCapsuleRadius);

	return 0;
}

CInt SetCharacterControllerCapsuleHeight(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerCapsuleHeight()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value <= 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerCapsuleHeight() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCapsuleHeight = value;
	g_nx->gControllers->setHeight(g_physXProperties.m_fCapsuleHeight);

	return 0;
}

CInt SetCharacterControllerForcePower(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerForcePower()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerForcePower() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCharacterPower = value;

	return 0;
}

CInt SetCharacterControllerWalkSpeed(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerWalkSpeed()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerWalkSpeed() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCharacterWalkSpeed = value;
	g_nx->gCharacterWalkSpeed = g_physXProperties.m_fCharacterWalkSpeed;

	return 0;
}

CInt SetCharacterControllerRunSpeed(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerRunSpeed()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerRunSpeed() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCharacterRunSpeed = value;
	g_nx->gCharacterRunSpeed = g_physXProperties.m_fCharacterRunSpeed;

	return 0;
}

CInt SetCharacterControllerSkinWidth(lua_State* L)
{
	return 0;
}

CInt SetCharacterControllerStepOffset(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerStepOffset()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerStepOffset() Error: argument must be equal to or greater than 0.0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fCharacterStepOffset = value;
	g_nx->gControllers->setStepOffset(g_physXProperties.m_fCharacterStepOffset);

	return 0;
}

CInt SetCharacterControllerSlopeLimit(lua_State* L)
{
	return 0;
}

CInt SetCharacterControllerJumpPower(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetCharacterControllerJumpPower()", COLOR_RED);
		return 0;
	}

	CFloat value = (CFloat)lua_tonumber(L, 1);

	if (value < 0.0f)
	{
		//PrintInfo("\nSetCharacterControllerJumpPower() Error: argument must be greater than 0", COLOR_RED);
		return 0;
	}

	g_physXProperties.m_fJumpPower = value;

	return 0;
}

CInt EnableCharacterControllerJump(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bJumping = CTrue;

	return 0;
}

CInt DisableCharacterControllerJump(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bJumping = CFalse;

	return 0;
}

CInt EnablePhysicsDebugMode(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bDebugMode = CTrue;
	g_nx->debugMode = g_physXProperties.m_bDebugMode;

	return 0;
}

CInt DisablePhysicsDebugMode(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	g_physXProperties.m_bDebugMode = CFalse;
	g_nx->debugMode = g_physXProperties.m_bDebugMode;

	return 0;
}

CInt SetCharacterControllerPosition(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetCharacterControllerPosition()", COLOR_RED);
		return 0;
	}

	CFloat value1 = (CFloat)lua_tonumber(L, 1);
	CFloat value2 = (CFloat)lua_tonumber(L, 2);
	CFloat value3 = (CFloat)lua_tonumber(L, 3);

	NxExtendedVec3 nxPos(value1, value2, value3);
	g_nx->SetCharacterPos(nxPos);

	return 0;
}

CInt GetCharacterControllerPosition(lua_State* L)
{
	if (!gPhysXscene)
	{
		return 0;
	}

	//if (g_testScript)
	//	return 0;

	CFloat x = g_nx->GetRawCharacterPos().x;
	CFloat y = g_nx->GetRawCharacterPos().y;
	CFloat z = g_nx->GetRawCharacterPos().z;

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

CInt GetDistanceBetweenPhysicsCameraAndCharacterController(lua_State* L)
{
	CFloat distance = g_physXProperties.m_fCameraCharacterDistance;

	lua_pushnumber(L, distance);

	return 1;
}

CInt GetCharacterControllerCapsuleRadius(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCapsuleRadius;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerCapsuleHeight(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCapsuleHeight;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerForcePower(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCharacterPower;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerWalkSpeed(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCharacterWalkSpeed;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerRunSpeed(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCharacterRunSpeed;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerStepOffset(lua_State* L)
{
	CFloat value = g_physXProperties.m_fCharacterStepOffset;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetCharacterControllerJumpPower(lua_State* L)
{
	CFloat value = g_physXProperties.m_fJumpPower;

	lua_pushnumber(L, value);

	return 1;
}


CInt GetPhysicsDefaultRestitution(lua_State* L)
{
	CFloat value = g_physXProperties.m_fDefaultRestitution;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetPhysicsDefaultSkinWidth(lua_State* L)
{
	CFloat value = g_physXProperties.m_fDefaultSkinWidth;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetPhysicsDefaultStaticFriction(lua_State* L)
{
	CFloat value = g_physXProperties.m_fDefaultStaticFriction;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetPhysicsDefaultDynamicFriction(lua_State* L)
{
	CFloat value = g_physXProperties.m_fDefaultDynamicFriction;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetPhysicsGravity(lua_State* L)
{
	CFloat x = g_physXProperties.m_fGravityX;
	CFloat y = g_physXProperties.m_fGravityY;
	CFloat z = g_physXProperties.m_fGravityZ;

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

CInt GetPhysicsGroundHeight(lua_State* L)
{
	CFloat value = g_physXProperties.m_fGroundHeight;

	lua_pushnumber(L, value);

	return 1;
}

CInt GetPhysicsCollisionFlags(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetPhysicsCollisionFlags()", COLOR_RED);
		return 0;
	}

	if (!gPhysXscene)
		return 0;

	CChar name1[MAX_NAME_SIZE];
	Cpy(name1, lua_tostring(L, 1));
	StringToUpper(name1);

	CChar name2[MAX_NAME_SIZE];
	Cpy(name2, lua_tostring(L, 2));
	StringToUpper(name2);

	std::vector <CInt> group1;
	std::vector <CInt> group2;

	if (Cmp(name1, "KINEMATIC"))
	{
		group1.push_back(GROUP_COLLIDABLE_NON_PUSHABLE);
		group1.push_back(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT);
	}
	else if (Cmp(name1, "DYNAMIC"))
	{
		group1.push_back(GROUP_COLLIDABLE_PUSHABLE);
	}
	else if (Cmp(name1, "TRIGGER"))
	{
		group1.push_back(GROUP_TRIGGER);
	}
	else if (Cmp(name1, "STATIC"))
	{
		group1.push_back(GROUP_STATIC);
	}
	else if (Cmp(name1, "GROUND"))
	{
		group1.push_back(GROUP_GROUND);
	}

	if (Cmp(name2, "KINEMATIC"))
	{
		group2.push_back(GROUP_COLLIDABLE_NON_PUSHABLE);
		group2.push_back(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT);
	}
	else if (Cmp(name2, "DYNAMIC"))
	{
		group2.push_back(GROUP_COLLIDABLE_PUSHABLE);
	}
	else if (Cmp(name2, "TRIGGER"))
	{
		group2.push_back(GROUP_TRIGGER);
	}
	else if (Cmp(name2, "STATIC"))
	{
		group2.push_back(GROUP_STATIC);
	}
	else if (Cmp(name2, "GROUND"))
	{
		group2.push_back(GROUP_GROUND);
	}

	for (CUInt i = 0; i < group1.size(); i++)
	{
		for (CUInt j = 0; j < group2.size(); j++)
		{
			CBool value = gPhysXscene->getGroupCollisionFlag(group1[i], group2[j]);

			if (value)
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}

	//PrintInfo("\nGetPhysicsCollisionFlags() Error: group1 and/or group2 string is invalid", COLOR_RED);
	return 0;
}


CInt SetMultisamplingValue(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetMultisamplingValue()", COLOR_RED);
		return 0;
	}

	CInt value = (CInt)lua_tonumber(L, 1);

	if (value == 0 || value == 2 || value == 4 || value == 8 || value == 16)
	{
		g_options.m_numSamples = value;
	}
	else
	{
		//PrintInfo("\nSetMultisamplingValue() Error: Accepted values are 0, 2, 4, 8 or 16", COLOR_RED);
	}

	return 0;
}

CInt SetAnisotropicFilteringValue(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetAnisotropicFilteringValue()", COLOR_RED);
		return 0;
	}

	CInt value = (CInt)lua_tonumber(L, 1);

	if (value == 0 || value == 2 || value == 4 || value == 8 || value == 16)
	{
		if (GLEW_EXT_texture_filter_anisotropic)
		{
			g_options.m_anisotropy = value;
		}
		else
		{
			g_options.m_anisotropy = 0;
		}
	}
	else
	{
		//PrintInfo("\nSetAnisotropicFilteringValue() Error: Accepted values are 0, 2, 4, 8 or 16", COLOR_RED);
	}

	return 0;
}

CInt EnableVSync(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (WGLEW_EXT_swap_control)
	{
		g_options.m_disableVSync = CFalse;
		wglSwapIntervalEXT(1);
	}

	return 0;
}

CInt DisableVSync(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (WGLEW_EXT_swap_control)
	{
		g_options.m_disableVSync = CTrue;
		wglSwapIntervalEXT(0);
	}

	return 0;
}

CInt EnableGeneralWaterReflection(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_render.m_useWaterReflection)
	{
		g_options.m_enableWaterReflection = CTrue;
	}

	return 0;
}

CInt DisableGeneralWaterReflection(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_render.m_useWaterReflection)
	{
		g_options.m_enableWaterReflection = CFalse;
	}

	return 0;
}

//Sets screen resolution
//Accepted values are:
//0    : Current display resolution
//800  : 800x600
//1024 : 1024x768
//1280 : 1280x720 � HD / 720p.
//1920 : 1920x1080 � FHD(Full HD) / 1080p.
//2560 : 2560x1440 � QHD / WQHD(Quad HD) / 1440p.
//3840 : 3840x2160 � UHD(Ultra HD) / 4K 2160p.
//7680 : 7680�4320 � FUHD(Full Ultra HD) / 8K 4320p.
CInt SetScreenResolution(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetScreenResolution()", COLOR_RED);
		return 0;
	}

	CInt value = (CInt)lua_tonumber(L, 1);

	if (value == 0 || value == 800 || value == 1024 || value == 1280 || value == 1920 || value == 2560 || value == 3840 || value == 7680)
	{
		if (value == 0)
		{
			g_options.m_useCurrentResolution = CTrue;
		}
		else if (value == 800)
		{
			g_options.m_width = 800;
			g_options.m_height = 600;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 1024)
		{
			g_options.m_width = 1024;
			g_options.m_height = 768;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 1280)
		{
			g_options.m_width = 1280;
			g_options.m_height = 720;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 1920)
		{
			g_options.m_width = 1920;
			g_options.m_height = 1080;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 2560)
		{
			g_options.m_width = 2560;
			g_options.m_height = 1440;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 3840)
		{
			g_options.m_width = 3840;
			g_options.m_height = 2160;
			g_options.m_useCurrentResolution = CFalse;
		}
		else if (value == 7680)
		{
			g_options.m_width = 7680;
			g_options.m_height = 4320;
			g_options.m_useCurrentResolution = CFalse;
		}
	}
	else
	{
		//PrintInfo("\nSetScreenResolution() Error: Accepted values are 0, 800, 1024, 1280, 1920, 2560, 3840 or 7680", COLOR_RED);
	}
	return 0;
}

CInt SaveGeneralProperties(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	FILE *filePtr;
	filePtr = fopen("Assets/config/conf_win32.dat", "wb");
	fwrite(&g_options, sizeof(COptions), 1, filePtr);
	fclose(filePtr);

	return 0;
}

CInt GetMultisamplingValue(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	lua_pushnumber(L, g_options.m_numSamples);

	return 1;
}

CInt GetAnisotropicFilteringValue(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	lua_pushnumber(L, g_options.m_anisotropy);

	return 1;
}

CInt IsVSyncEnabled(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_options.m_disableVSync)
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L, 1);

	return 1;
}

CInt IsGeneralWaterReflectionEnabled(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_options.m_enableWaterReflection)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

CInt GetScreenResolution(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	if (g_options.m_useCurrentResolution)
		lua_pushnumber(L, 0); //current display resolution
	else
		lua_pushnumber(L, g_options.m_width);

	return 1;
}

CInt GetVSceneScriptStringVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVSceneScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar* value = NULL;
	if (g_VSceneScript)
	{
		value = g_VSceneScript->GetScriptStringVariable(luaToString);
	}
	else
	{
		return 0;
	}

	lua_pushstring(L, value);

	free(value);

	return 1;
}

CInt GetVSceneScriptBoolVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVSceneScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool value;
	if (g_VSceneScript)
	{
		value = g_VSceneScript->GetScriptBoolVariable(luaToString);
	}
	lua_pushboolean(L, value);

	return 1;
}

CInt GetVSceneScriptIntVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVSceneScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	if (g_VSceneScript)
	{
		value = g_VSceneScript->GetScriptIntVariable(luaToString);
	}
	lua_pushinteger(L, value);

	return 1;
}

CInt GetVSceneScriptDoubleVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVSceneScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	if (g_VSceneScript)
	{
		value = g_VSceneScript->GetScriptDoubleVariable(luaToString);
	}
	lua_pushnumber(L, value);

	return 1;
}

CInt SetVSceneScriptStringVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetVSceneScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar value[MAX_URI_SIZE];
	Cpy(value, lua_tostring(L, 2));
	if (g_VSceneScript)
	{
		g_VSceneScript->SetScriptStringVariable(luaToString, value);
	}

	return 0;
}

CInt SetVSceneScriptBoolVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetVSceneScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool bValue;
	CInt iValue;
	iValue = lua_toboolean(L, 2);
	if (iValue)
		bValue = CTrue;
	else
		bValue = CFalse;
	if (g_VSceneScript)
	{
		g_VSceneScript->SetScriptBoolVariable(luaToString, bValue);
	}

	return 0;
}

CInt SetVSceneScriptIntVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetVSceneScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	value = lua_tointeger(L, 2);
	if (g_VSceneScript)
	{
		g_VSceneScript->SetScriptIntVariable(luaToString, value);
	}

	return 0;
}

CInt SetVSceneScriptDoubleVariable(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetVSceneScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	value = lua_tonumber(L, 2);
	if (g_VSceneScript)
	{
		g_VSceneScript->SetScriptDoubleVariable(luaToString, value);
	}

	return 0;
}

CInt GetSkyScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSkyScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar* value = NULL;
	if (g_skyDome)
	{
		value = g_skyDome->GetScriptStringVariable(luaToString);
	}
	else
	{
		return 0;
	}

	lua_pushstring(L, value);

	free(value);

	return 1;
}

CInt GetSkyScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSkyScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool value;
	if (g_skyDome)
	{
		value = g_skyDome->GetScriptBoolVariable(luaToString);
	}
	lua_pushboolean(L, value);

	return 1;
}

CInt GetSkyScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSkyScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	if (g_skyDome)
	{
		value = g_skyDome->GetScriptIntVariable(luaToString);
	}
	lua_pushinteger(L, value);

	return 1;
}

CInt GetSkyScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSkyScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	if (g_skyDome)
	{
		value = g_skyDome->GetScriptDoubleVariable(luaToString);
	}
	lua_pushnumber(L, value);

	return 1;
}

CInt SetSkyScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetSkyScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar value[MAX_URI_SIZE];
	Cpy(value, lua_tostring(L, 2));
	if (g_skyDome)
	{
		g_skyDome->SetScriptStringVariable(luaToString, value);
	}

	return 0;
}

CInt SetSkyScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetSkyScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool bValue;
	CInt iValue;
	iValue = lua_toboolean(L, 2);
	if (iValue)
		bValue = CTrue;
	else
		bValue = CFalse;
	if (g_skyDome)
	{
		g_skyDome->SetScriptBoolVariable(luaToString, bValue);
	}

	return 0;
}

CInt SetSkyScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetSkyScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	value = lua_tointeger(L, 2);
	if (g_skyDome)
	{
		g_skyDome->SetScriptIntVariable(luaToString, value);
	}

	return 0;
}

CInt SetSkyScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetSkyScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	value = lua_tonumber(L, 2);
	if (g_skyDome)
	{
		g_skyDome->SetScriptDoubleVariable(luaToString, value);
	}

	return 0;
}


CInt GetTerrainScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetTerrainScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar* value = NULL;
	if (g_terrain)
	{
		value = g_terrain->GetScriptStringVariable(luaToString);
	}
	else
	{
		return 0;
	}

	lua_pushstring(L, value);

	free(value);

	return 1;
}

CInt GetTerrainScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetTerrainScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool value;
	if (g_terrain)
	{
		value = g_terrain->GetScriptBoolVariable(luaToString);
	}
	lua_pushboolean(L, value);

	return 1;
}

CInt GetTerrainScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetTerrainScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	if (g_terrain)
	{
		value = g_terrain->GetScriptIntVariable(luaToString);
	}
	lua_pushinteger(L, value);

	return 1;
}

CInt GetTerrainScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetTerrainScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	if (g_terrain)
	{
		value = g_terrain->GetScriptDoubleVariable(luaToString);
	}
	lua_pushnumber(L, value);

	return 1;
}

CInt SetTerrainScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetTerrainScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar value[MAX_URI_SIZE];
	Cpy(value, lua_tostring(L, 2));
	if (g_terrain)
	{
		g_terrain->SetScriptStringVariable(luaToString, value);
	}

	return 0;
}

CInt SetTerrainScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetTerrainScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool bValue;
	CInt iValue;
	iValue = lua_toboolean(L, 2);
	if (iValue)
		bValue = CTrue;
	else
		bValue = CFalse;
	if (g_terrain)
	{
		g_terrain->SetScriptBoolVariable(luaToString, bValue);
	}

	return 0;
}

CInt SetTerrainScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetTerrainScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	value = lua_tointeger(L, 2);
	if (g_terrain)
	{
		g_terrain->SetScriptIntVariable(luaToString, value);
	}

	return 0;
}

CInt SetTerrainScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetTerrainScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	value = lua_tonumber(L, 2);
	if (g_terrain)
	{
		g_terrain->SetScriptDoubleVariable(luaToString, value);
	}

	return 0;
}


CInt GetPrefabInstanceScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetPrefabInstanceScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			value = g_currentInstancePrefab->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceScriptStringVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			value = g_instancePrefab[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetPrefabInstanceScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			value = g_currentInstancePrefab->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceScriptBoolVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			value = g_instancePrefab[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetPrefabInstanceScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			value = g_currentInstancePrefab->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceScriptIntVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			value = g_instancePrefab[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPrefabInstanceScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetPrefabInstanceScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			value = g_currentInstancePrefab->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetPrefabInstanceScriptDoubleVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			value = g_instancePrefab[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetPrefabInstanceScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPrefabInstanceScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetScriptStringVariable(variable, value);

			return 0;
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceScriptStringVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPrefabInstanceScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetScriptBoolVariable(variable, value);

			return 0;
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceScriptBoolVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPrefabInstanceScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetScriptIntVariable(variable, value);

			return 0;
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceScriptIntVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetPrefabInstanceScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPrefabInstanceScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetScriptDoubleVariable(variable, value);

			return 0;
		}
		else
		{
			//PrintInfo("\nSetPrefabInstanceScriptDoubleVariable() Error: Couldn't find current prefab instance", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetPrefabInstanceScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetGUIButtonScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for GetGUIButtonScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					value = g_guis[i]->m_guiButtons[j]->GetScriptStringVariable(variableName);

					lua_pushstring(L, value);

					free(value);

					return 1;

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptStringVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptStringVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt GetGUIButtonScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for GetGUIButtonScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CBool value;

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					value = g_guis[i]->m_guiButtons[j]->GetScriptBoolVariable(variableName);

					lua_pushboolean(L, value);

					return 1;

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptBoolVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptBoolVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt GetGUIButtonScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for GetGUIButtonScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CInt value;

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					value = g_guis[i]->m_guiButtons[j]->GetScriptIntVariable(variableName);

					lua_pushinteger(L, value);

					return 1;

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptIntVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptIntVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt GetGUIButtonScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for GetGUIButtonScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CDouble value;

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					value = g_guis[i]->m_guiButtons[j]->GetScriptDoubleVariable(variableName);

					lua_pushnumber(L, value);

					return 1;

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptDoubleVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonScriptDoubleVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}


CInt SetGUIButtonScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIButtonScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 4));

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetScriptStringVariable(variableName, value);

					return 0;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptStringVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptStringVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt SetGUIButtonScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIButtonScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 4);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetScriptBoolVariable(variableName, value);

					return 0;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptBoolVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptBoolVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt SetGUIButtonScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIButtonScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CInt value;
	value = lua_tointeger(L, 4);

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetScriptIntVariable(variableName, value);

					return 0;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptIntVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptIntVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}

CInt SetGUIButtonScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIButtonScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CChar variableName[MAX_NAME_SIZE];
	Cpy(variableName, lua_tostring(L, 3));

	CDouble value;
	value = lua_tonumber(L, 4);

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetScriptDoubleVariable(variableName, value);

					return 0;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptDoubleVariable() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonScriptDoubleVariable() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	return 0;
}


CInt GetTriggerScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetTriggerScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigger Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			value = g_triggers[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetTriggerScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetTriggerScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetTriggerScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigge Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			value = g_triggers[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetTriggerScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetTriggerScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetTriggerScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigge Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			value = g_triggers[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetTriggerScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetTriggerScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetTriggerScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigge Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			value = g_triggers[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetTriggerScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt SetTriggerScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTriggerScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigger Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			g_triggers[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetTriggerScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetTriggerScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTriggerScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigger Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			g_triggers[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetTriggerScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetTriggerScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTriggerScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigger Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			g_triggers[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetTriggerScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetTriggerScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTriggerScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundTrigger = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Trigger Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CChar triggerName[MAX_NAME_SIZE];
		Cpy(triggerName, g_triggers[i]->GetName());
		StringToUpper(triggerName);
		if (Cmp(triggerName, luaToString))
		{
			foundTrigger = CTrue;
			g_triggers[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundTrigger)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetTriggerScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' trigger");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetWaterScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetWaterScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			value = g_engineWaters[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetWaterScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetWaterScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			value = g_engineWaters[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetWaterScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetWaterScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			value = g_engineWaters[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetWaterScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetWaterScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			value = g_engineWaters[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetWaterScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetWaterScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetWaterScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetWaterScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetWaterScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetWaterScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetWaterScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetWaterScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetWaterScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetVideoScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetVideoScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			value = g_engineVideos[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetVideoScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetVideoScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetVideoScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			value = g_engineVideos[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetVideoScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetVideoScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetVideoScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			value = g_engineVideos[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetVideoScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetVideoScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetVideoScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			value = g_engineVideos[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetVideoScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetVideoScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetVideoScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			g_engineVideos[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetVideoScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetVideoScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetVideoScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			g_engineVideos[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetVideoScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetVideoScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetVideoScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			g_engineVideos[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetVideoScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetVideoScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetVideoScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundVideo = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Video Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar videoName[MAX_NAME_SIZE];
		Cpy(videoName, g_engineVideos[i]->GetName());
		StringToUpper(videoName);
		if (Cmp(videoName, luaToString))
		{
			foundVideo = CTrue;
			g_engineVideos[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundVideo)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetVideoScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' video");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetAmbientSoundScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetAmbientSoundScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			value = g_engineAmbientSounds[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetAmbientSoundScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetAmbientSoundScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetAmbientSoundScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			value = g_engineAmbientSounds[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetAmbientSoundScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetAmbientSoundScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetAmbientSoundScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			value = g_engineAmbientSounds[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetAmbientSoundScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetAmbientSoundScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetAmbientSoundScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			value = g_engineAmbientSounds[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetAmbientSoundScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetAmbientSoundScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetAmbientSoundScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			g_engineAmbientSounds[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetAmbientSoundScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetAmbientSoundScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetAmbientSoundScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			g_engineAmbientSounds[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetAmbientSoundScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetAmbientSoundScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetAmbientSoundScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			g_engineAmbientSounds[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetAmbientSoundScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetAmbientSoundScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetAmbientSoundScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //AmbientSound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;
			g_engineAmbientSounds[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetAmbientSoundScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' ambient sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt Get3DSoundScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for Get3DSoundScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			value = g_engine3DSounds[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGet3DSoundScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Get3DSoundScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for Get3DSoundScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			value = g_engine3DSounds[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGet3DSoundScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Get3DSoundScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for Get3DSoundScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			value = g_engine3DSounds[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGet3DSoundScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Get3DSoundScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for Get3DSoundScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			value = g_engine3DSounds[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGet3DSoundScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Set3DSoundScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for Set3DSoundScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			g_engine3DSounds[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSet3DSoundScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Set3DSoundScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for Set3DSoundScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			g_engine3DSounds[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSet3DSoundScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Set3DSoundScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for Set3DSoundScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			g_engine3DSounds[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSet3DSoundScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt Set3DSoundScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for Set3DSoundScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3D Sound Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar ThreeDSoundName[MAX_NAME_SIZE];
		Cpy(ThreeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(ThreeDSoundName);
		if (Cmp(ThreeDSoundName, luaToString))
		{
			found3DSound = CTrue;
			g_engine3DSounds[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSet3DSoundScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' 3D sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetLightScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetLightScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			value = g_engineLights[i]->m_abstractLight->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetLightScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetLightScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetLightScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			value = g_engineLights[i]->m_abstractLight->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetLightScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetLightScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetLightScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			value = g_engineLights[i]->m_abstractLight->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetLightScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetLightScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetLightScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			value = g_engineLights[i]->m_abstractLight->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetLightScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetLightScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetLightScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			g_engineLights[i]->m_abstractLight->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetLightScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetLightScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetLightScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			g_engineLights[i]->m_abstractLight->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetLightScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetLightScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetLightScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			g_engineLights[i]->m_abstractLight->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetLightScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetLightScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetLightScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;
			g_engineLights[i]->m_abstractLight->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetLightScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetCameraScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetCameraScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar* value = NULL;

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			value = g_engineCameraInstances[i]->GetScriptStringVariable(variable);

			lua_pushstring(L, value);

			free(value);

			return 1;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetCameraScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetCameraScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetCameraScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CBool value;

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			value = g_engineCameraInstances[i]->GetScriptBoolVariable(variable);

			lua_pushboolean(L, value);

			return 1;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetCameraScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' Camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetCameraScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetCameraScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			value = g_engineCameraInstances[i]->GetScriptIntVariable(variable);

			lua_pushinteger(L, value);

			return 1;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetCameraScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetCameraScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetCameraScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			value = g_engineCameraInstances[i]->GetScriptDoubleVariable(variable);

			lua_pushnumber(L, value);

			return 1;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nGetCameraScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetCameraScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetCameraScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CChar value[MAX_NAME_SIZE];
	Cpy(value, lua_tostring(L, 3));

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			g_engineCameraInstances[i]->SetScriptStringVariable(variable, value);

			return 0;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetCameraScriptStringVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetCameraScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetCameraScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt result;

	CBool value;
	result = lua_toboolean(L, 3);

	if (result)
		value = CTrue;
	else
		value = CFalse;

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			g_engineCameraInstances[i]->SetScriptBoolVariable(variable, value);

			return 0;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetCameraScriptBoolVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetCameraScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetCameraScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CInt value;
	value = lua_tointeger(L, 3);

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			g_engineCameraInstances[i]->SetScriptIntVariable(variable, value);

			return 0;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetCameraScriptIntVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetCameraScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetCameraScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CBool foundCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Name- First Argument
	StringToUpper(luaToString);

	CChar variable[MAX_NAME_SIZE];
	Cpy(variable, lua_tostring(L, 2));

	CDouble value;
	value = lua_tonumber(L, 3);

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCamera = CTrue;
			g_engineCameraInstances[i]->SetScriptDoubleVariable(variable, value);

			return 0;
		}
	}
	if (!foundCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nSetCameraScriptDoubleVariable() Error: %s%s%s", "Couldn't find '", luaToString, "' camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt GetMainCharacterScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetMainCharacterScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar* value = NULL;
	if (g_mainCharacter)
	{
		value = g_mainCharacter->GetScriptStringVariable(luaToString);
	}
	else
	{
		return 0;
	}

	lua_pushstring(L, value);

	free(value);

	return 1;
}

CInt GetMainCharacterScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetMainCharacterScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool value;
	if (g_mainCharacter)
	{
		value = g_mainCharacter->GetScriptBoolVariable(luaToString);
	}
	lua_pushboolean(L, value);

	return 1;
}

CInt GetMainCharacterScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetMainCharacterScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	if (g_mainCharacter)
	{
		value = g_mainCharacter->GetScriptIntVariable(luaToString);
	}
	lua_pushinteger(L, value);

	return 1;
}

CInt GetMainCharacterScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetMainCharacterScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	if (g_mainCharacter)
	{
		value = g_mainCharacter->GetScriptDoubleVariable(luaToString);
	}
	lua_pushnumber(L, value);

	return 1;
}


CInt SetMainCharacterScriptStringVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetMainCharacterScriptStringVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar value[MAX_URI_SIZE];
	Cpy(value, lua_tostring(L, 2));
	if (g_mainCharacter)
	{
		g_mainCharacter->SetScriptStringVariable(luaToString, value);
	}

	return 0;
}

CInt SetMainCharacterScriptBoolVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetMainCharacterScriptBoolVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CBool bValue;
	CInt iValue;
	iValue = lua_toboolean(L, 2);
	if (iValue)
		bValue = CTrue;
	else
		bValue = CFalse;
	if (g_mainCharacter)
	{
		g_mainCharacter->SetScriptBoolVariable(luaToString, bValue);
	}

	return 0;
}

CInt SetMainCharacterScriptIntVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetMainCharacterScriptIntVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CInt value;
	value = lua_tointeger(L, 2);
	if (g_mainCharacter)
	{
		g_mainCharacter->SetScriptIntVariable(luaToString, value);
	}

	return 0;
}

CInt SetMainCharacterScriptDoubleVariable(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 argument for SetMainCharacterScriptDoubleVariable()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CDouble value;
	value = lua_tonumber(L, 2);
	if (g_mainCharacter)
	{
		g_mainCharacter->SetScriptDoubleVariable(luaToString, value);
	}

	return 0;
}


CInt ShowGUIButton(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for ShowGUIButton()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nShowGUIButton() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->Show();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIButton() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIButton() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt HideGUIButton(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for HideGUIButton()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nHideGUIButton() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->Hide();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIButton() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIButton() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ShowGUIImage(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for ShowGUIImage()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIImage = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar imageName[MAX_NAME_SIZE];
	Cpy(imageName, lua_tostring(L, 2));
	StringToUpper(imageName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nShowGUIImage() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				CChar image[MAX_NAME_SIZE];
				Cpy(image, g_guis[i]->m_guiImages[j]->GetName());
				StringToUpper(image);

				if (Cmp(image, imageName))
				{
					foundGUIImage = CTrue;

					g_guis[i]->m_guiImages[j]->Show();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIImage() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIImage)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIImage() Error: Couldn't find GUI image '", imageName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt HideGUIImage(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for HideGUIImage()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIImage = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar imageName[MAX_NAME_SIZE];
	Cpy(imageName, lua_tostring(L, 2));
	StringToUpper(imageName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nHideGUIImage() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				CChar image[MAX_NAME_SIZE];
				Cpy(image, g_guis[i]->m_guiImages[j]->GetName());
				StringToUpper(image);

				if (Cmp(image, imageName))
				{
					foundGUIImage = CTrue;

					g_guis[i]->m_guiImages[j]->Hide();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIImage() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIImage)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIImage() Error: Couldn't find GUI image '", imageName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ShowGUIText(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for ShowGUIText()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIText = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar textName[MAX_NAME_SIZE];
	Cpy(textName, lua_tostring(L, 2));
	StringToUpper(textName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nShowGUIText() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
			{
				CChar text[MAX_NAME_SIZE];
				Cpy(text, g_guis[i]->m_guiTexts[j]->GetName());
				StringToUpper(text);

				if (Cmp(text, textName))
				{
					foundGUIText = CTrue;

					g_guis[i]->m_guiTexts[j]->Show();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIText() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIText)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nShowGUIText() Error: Couldn't find GUI text '", textName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt HideGUIText(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for HideGUIText()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIText = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar textName[MAX_NAME_SIZE];
	Cpy(textName, lua_tostring(L, 2));
	StringToUpper(textName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nHideGUIText() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
			{
				CChar text[MAX_NAME_SIZE];
				Cpy(text, g_guis[i]->m_guiTexts[j]->GetName());
				StringToUpper(text);

				if (Cmp(text, textName))
				{
					foundGUIText = CTrue;

					g_guis[i]->m_guiTexts[j]->Hide();

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIText() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIText)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nHideGUIText() Error: Couldn't find GUI text '", textName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ScaleGUIButton(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for ScaleGUIButton()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CFloat scale;
	scale = (CFloat)lua_tonumber(L, 3);
	if (scale < 1.0)
	{
		//PrintInfo("\nScaleGUIButton() Error: scale variable must be equal or greater than 1.0", COLOR_RED);
		return 0;
	}

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nScaleGUIButton() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetScale(scale);

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nScaleGUIButton() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nScaleGUIButton() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ScaleGUIImage(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for ScaleGUIImage()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIImage = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar imageName[MAX_NAME_SIZE];
	Cpy(imageName, lua_tostring(L, 2));
	StringToUpper(imageName);

	CFloat scale;
	scale = (CFloat)lua_tonumber(L, 3);
	if (scale < 1.0)
	{
		//PrintInfo("\nScaleGUIImage() Error: scale variable must be equal or greater than 1.0", COLOR_RED);
		return 0;
	}

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nScaleGUIImage() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				CChar image[MAX_NAME_SIZE];
				Cpy(image, g_guis[i]->m_guiImages[j]->GetName());
				StringToUpper(image);

				if (Cmp(image, imageName))
				{
					foundGUIImage = CTrue;

					g_guis[i]->m_guiImages[j]->SetScale(scale);

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nScaleGUIImage() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIImage)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nScaleGUIImage() Error: Couldn't find GUI image '", imageName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt SetGUIButtonPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIButtonPosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	CFloat x, y;
	x = (CFloat)lua_tonumber(L, 3);
	y = (CFloat)lua_tonumber(L, 4);
	CVec2f pos(x, y);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nSetGUIButtonPosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					g_guis[i]->m_guiButtons[j]->SetPosition(pos);

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonPosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIButtonPosition() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetGUIImagePosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUIImagePosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIImage = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar imageName[MAX_NAME_SIZE];
	Cpy(imageName, lua_tostring(L, 2));
	StringToUpper(imageName);

	CFloat x, y;
	x = (CFloat)lua_tonumber(L, 3);
	y = (CFloat)lua_tonumber(L, 4);
	CVec2f pos(x, y);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nSetGUIImagePosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				CChar image[MAX_NAME_SIZE];
				Cpy(image, g_guis[i]->m_guiImages[j]->GetName());
				StringToUpper(image);

				if (Cmp(image, imageName))
				{
					foundGUIImage = CTrue;

					g_guis[i]->m_guiImages[j]->SetPosition(pos);

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIImagePosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIImage)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIImagePosition() Error: Couldn't find GUI image '", imageName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetGUITextPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetGUITextPosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIText = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar textName[MAX_NAME_SIZE];
	Cpy(textName, lua_tostring(L, 2));
	StringToUpper(textName);

	CFloat x, y;
	x = (CFloat)lua_tonumber(L, 3);
	y = (CFloat)lua_tonumber(L, 4);
	CVec2f pos(x, y);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nSetGUITextPosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
			{
				CChar text[MAX_NAME_SIZE];
				Cpy(text, g_guis[i]->m_guiTexts[j]->GetName());
				StringToUpper(text);

				if (Cmp(text, textName))
				{
					foundGUIText = CTrue;

					g_guis[i]->m_guiTexts[j]->SetPosition(pos);

					break;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUITextPosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIText)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUITextPosition() Error: Couldn't find GUI text '", textName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetGUIButtonPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetGUIButtonPosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIButton = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar buttonName[MAX_NAME_SIZE];
	Cpy(buttonName, lua_tostring(L, 2));
	StringToUpper(buttonName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nGetGUIButtonPosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				CChar button[MAX_NAME_SIZE];
				Cpy(button, g_guis[i]->m_guiButtons[j]->GetName());
				StringToUpper(button);

				if (Cmp(button, buttonName))
				{
					foundGUIButton = CTrue;

					CVec2f pos;
					pos = g_guis[i]->m_guiButtons[j]->GetPosition();
					lua_pushnumber(L, pos.x);
					lua_pushnumber(L, pos.y);

					return 2;

				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonPosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIButton)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIButtonPosition() Error: Couldn't find GUI button '", buttonName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetGUIImagePosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetGUIImagePosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIImage = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar imageName[MAX_NAME_SIZE];
	Cpy(imageName, lua_tostring(L, 2));
	StringToUpper(imageName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nGetGUIImagePosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				CChar image[MAX_NAME_SIZE];
				Cpy(image, g_guis[i]->m_guiImages[j]->GetName());
				StringToUpper(image);

				if (Cmp(image, imageName))
				{
					foundGUIImage = CTrue;

					CVec2f pos;
					pos = g_guis[i]->m_guiImages[j]->GetPosition();
					lua_pushnumber(L, pos.x);
					lua_pushnumber(L, pos.y);
					return 2;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIImagePosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIImage)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIImagePosition() Error: Couldn't find GUI image '", imageName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetGUITextPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for GetGUITextPosition()", COLOR_RED);
		return 0;
	}

	CBool foundGUI = CFalse;
	CBool foundGUIText = CFalse;

	CChar GUIName[MAX_NAME_SIZE];
	Cpy(GUIName, lua_tostring(L, 1)); //GUI Name- First Argument
	StringToUpper(GUIName);

	CChar textName[MAX_NAME_SIZE];
	Cpy(textName, lua_tostring(L, 2));
	StringToUpper(textName);

	if (Cmp(GUIName, "THIS"))
	{
		if (g_currentGUI)
		{
			Cpy(GUIName, g_currentGUI->GetName());
			StringToUpper(GUIName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s%s%s", "\nGetGUITextPosition() Error: Couldn't find current GUI");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, GUIName))
		{
			foundGUI = CTrue;

			for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
			{
				CChar text[MAX_NAME_SIZE];
				Cpy(text, g_guis[i]->m_guiTexts[j]->GetName());
				StringToUpper(text);

				if (Cmp(text, textName))
				{
					foundGUIText = CTrue;

					CVec2f pos;
					pos = g_guis[i]->m_guiTexts[j]->GetPosition();
					lua_pushnumber(L, pos.x);
					lua_pushnumber(L, pos.y);

					return 2;
				}
			}

			break;
		}
	}
	if (!foundGUI)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUITextPosition() Error: Couldn't find GUI '", GUIName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	if (!foundGUIText)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUITextPosition() Error: Couldn't find GUI text '", textName, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetGUIPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetGUIPosition()", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat xPos = (CFloat)lua_tonumber(L, 2);
	CFloat yPos = (CFloat)lua_tonumber(L, 3);
	if (xPos < -100.0f || xPos > 100.0f || yPos < -100.0f || yPos > 100.0f)
	{
		//CChar message[MAX_URI_SIZE];
		//sprintf(message, "\nSetGUIPosition(%s, %.2f, %.2f) error: input values must be in[-100, 100] range", lua_tostring(L, 1), xPos, yPos);
		//PrintInfo(message, COLOR_RED);
		return 0;
	}

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		CFloat x = (CFloat)lua_tonumber(L, 2);
		CFloat y = (CFloat)lua_tonumber(L, 3);

		CVec2f pos; pos.x = x; pos.y = y;

		if (Cmp(gui, luaToString))
		{
			g_guis[i]->SetPosition(pos);

			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetGUIPosition() Error: Couldn't find the GUI '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;

}

CInt GetGUIPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetGUIPosition()", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;
	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CChar gui[MAX_NAME_SIZE];
		Cpy(gui, g_guis[i]->GetName());
		StringToUpper(gui);

		if (Cmp(gui, luaToString))
		{
			CVec2f position = g_guis[i]->GetPosition();
			lua_pushnumber(L, position.x);
			lua_pushnumber(L, position.y);
			return 2;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nGetGUIPosition() Error: Couldn't find GUI '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt AddForceToCharacterController(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 5)
	{
		//PrintInfo("\nPlease specify 5 arguments for AddForceToCharacterController()", COLOR_RED);
		return 0;
	}

	g_nx->bPushCharacter = CTrue;

	g_main->m_forceDirection.x = (CFloat)lua_tonumber(L, 1);
	g_main->m_forceDirection.y = (CFloat)lua_tonumber(L, 2);
	g_main->m_forceDirection.z = (CFloat)lua_tonumber(L, 3);

	g_main->m_forceSpeed = (CFloat)lua_tonumber(L, 4);

	g_main->m_forceDecreaseValue = (CFloat)lua_tonumber(L, 5);

	return 0;
}

CInt AddForceToPrefabInstance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 5)
	{
		//PrintInfo("\nPlease specify 5 arguments for AddForceToPrefabInstance()", COLOR_RED);
		return 0;
	}

	CChar name[MAX_NAME_SIZE];
	Cpy(name, lua_tostring(L, 1));
	StringToUpper(name);

	CFloat forceX = (CFloat)lua_tonumber(L, 2);
	CFloat forceY = (CFloat)lua_tonumber(L, 3);
	CFloat forceZ = (CFloat)lua_tonumber(L, 4);
	CFloat forcePower = (CFloat)lua_tonumber(L, 5);

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", name))
	{
			if (g_currentInstancePrefab)
			{
				g_currentInstancePrefab->AddPhysicsForce(forceX, forceY, forceZ, forcePower);
			}
			else
			{
				//PrintInfo("\nAddForceToPrefabInstance() Error: Couldn't find current prefab instance", COLOR_RED);
			}
			return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, name))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->AddPhysicsForce(forceX, forceY, forceZ, forcePower);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nAddForceToPrefabInstance() Error: %s%s%s", "Couldn't find '", name, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt AddTorqueToPrefabInstance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;

	int argc = lua_gettop(L);
	if (argc < 5)
	{
		//PrintInfo("\nPlease specify 5 arguments for AddTorqueToPrefabInstance()", COLOR_RED);
		return 0;
	}

	CChar name[MAX_NAME_SIZE];
	Cpy(name, lua_tostring(L, 1));
	StringToUpper(name);

	CFloat torqueX = (CFloat)lua_tonumber(L, 2);
	CFloat torqueY = (CFloat)lua_tonumber(L, 3);
	CFloat torqueZ = (CFloat)lua_tonumber(L, 4);
	CFloat torquePower = (CFloat)lua_tonumber(L, 5);

	CBool foundPrefabInstance = CFalse;

	if (Cmp("THIS", name))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->AddPhysicsTorque(torqueX, torqueY, torqueZ, torquePower);
		}
		else
		{
			//PrintInfo("\nAddTorqueToPrefabInstance() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, name))
		{
			foundPrefabInstance = CTrue;
			g_instancePrefab[i]->AddPhysicsTorque(torqueX, torqueY, torqueZ, torquePower);
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\nAddTorqueToPrefabInstance() Error: %s%s%s", "Couldn't find '", name, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetPhysicsActorGroup(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetPhysicsActorGroup()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar name[MAX_NAME_SIZE];
	Cpy(name, lua_tostring(L, 1));
	StringToUpper(name);

	if (gPhysXscene)
	{
		for (CUInt j = 0; j < gPhysXscene->getNbActors(); j++)
		{
			if (!gPhysXscene->getActors()[j]->getName()) continue; //main character

			CChar actorName[MAX_NAME_SIZE];
			Cpy(actorName, gPhysXscene->getActors()[j]->getName());
			StringToUpper(actorName);

			if (Cmp(name, actorName))
			{
				NxActor* currentActor = gPhysXscene->getActors()[j];

				if (currentActor->getGroup() == GROUP_COLLIDABLE_NON_PUSHABLE || currentActor->getGroup() == GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT)
				{
					lua_pushstring(L, "KINEMATIC");
				}
				else if (currentActor->getGroup() == GROUP_COLLIDABLE_PUSHABLE)
				{
					lua_pushstring(L, "DYNAMIC");
				}
				else if (currentActor->getGroup() == GROUP_TRIGGER)
				{
					lua_pushstring(L, "TRIGGER");
				}
				else if (currentActor->getGroup() == GROUP_STATIC)
				{
					lua_pushstring(L, "STATIC");
				}
				else if (currentActor->getGroup() == GROUP_GROUND)
				{
					lua_pushstring(L, "GROUND");
				}
				return 1;
			}
		}
	}

	return 0;
}

CInt SetPhysicsCollisionFlags(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetPhysicsCollisionFlags()", COLOR_RED);
		return 0;
	}

	if (!gPhysXscene)
		return 0;

	CChar name1[MAX_NAME_SIZE];
	Cpy(name1, lua_tostring(L, 1));
	StringToUpper(name1);

	CChar name2[MAX_NAME_SIZE];
	Cpy(name2, lua_tostring(L, 2));
	StringToUpper(name2);

	CBool bFlag;
	CInt iFlag = lua_toboolean(L, 3);
	if (iFlag)
		bFlag = CTrue;
	else
		bFlag = CFalse;

	std::vector <CInt> group1;
	std::vector <CInt> group2;

	if (Cmp(name1, "KINEMATIC"))
	{
		group1.push_back(GROUP_COLLIDABLE_NON_PUSHABLE);
		group1.push_back(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT);
	}
	else if (Cmp(name1, "DYNAMIC"))
	{
		group1.push_back(GROUP_COLLIDABLE_PUSHABLE);
	}
	else if (Cmp(name1, "TRIGGER"))
	{
		group1.push_back(GROUP_TRIGGER);
	}
	else if (Cmp(name1, "STATIC"))
	{
		group1.push_back(GROUP_STATIC);
	}
	else if (Cmp(name1, "GROUND"))
	{
		group1.push_back(GROUP_GROUND);
	}

	if (Cmp(name2, "KINEMATIC"))
	{
		group2.push_back(GROUP_COLLIDABLE_NON_PUSHABLE);
		group2.push_back(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT);
	}
	else if (Cmp(name2, "DYNAMIC"))
	{
		group2.push_back(GROUP_COLLIDABLE_PUSHABLE);
	}
	else if (Cmp(name2, "TRIGGER"))
	{
		group2.push_back(GROUP_TRIGGER);
	}
	else if (Cmp(name2, "STATIC"))
	{
		group2.push_back(GROUP_STATIC);
	}
	else if (Cmp(name2, "GROUND"))
	{
		group2.push_back(GROUP_GROUND);
	}

	for (CUInt i = 0; i < group1.size(); i++)
	{
		for (CUInt j = 0; j < group2.size(); j++)
		{
			gPhysXscene->setGroupCollisionFlag(group1[i], group2[j], bFlag);
		}
	}

	return 0;
}

CInt GeneratePrefabInstance(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 10)
	{
		//PrintInfo("\nPlease specify 10 arguments for GeneratePrefabInstance()", COLOR_RED);
		return 0;
	}

	CChar name[MAX_NAME_SIZE];
	Cpy(name, lua_tostring(L, 1));
	StringToUpper(name);

	CFloat xPos = (CFloat)lua_tonumber(L, 2);
	CFloat yPos = (CFloat)lua_tonumber(L, 3);
	CFloat zPos = (CFloat)lua_tonumber(L, 4);
	CVec3f pos(xPos, yPos, zPos);

	CFloat xRot = (CFloat)lua_tonumber(L, 5);
	CFloat yRot = (CFloat)lua_tonumber(L, 6);
	CFloat zRot = (CFloat)lua_tonumber(L, 7);
	CVec4f rot(xRot, yRot, zRot, 0.0f);

	CFloat xScale = (CFloat)lua_tonumber(L, 8);
	CFloat yScale = (CFloat)lua_tonumber(L, 9);
	CFloat zScale = (CFloat)lua_tonumber(L, 10);
	CVec3f scale(xScale, yScale, zScale);

	CBool foundPrefab = CFalse;

	static int index = 1;

	CPrefab* currentPrefab = NULL;
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_prefab[i]->GetName());
		StringToUpper(prefabName);

		if (Cmp(name, prefabName))
		{
			currentPrefab = g_prefab[i];
			break;
		}
	}

	if (!currentPrefab)
	{
		for (CUInt i = 0; i < g_resourcePrefab.size(); i++)
		{
			CChar prefabName[MAX_NAME_SIZE];
			Cpy(prefabName, g_resourcePrefab[i]->GetName());
			StringToUpper(prefabName);

			if (Cmp(name, prefabName))
			{
				CPrefab* new_prefab = CNew(CPrefab);
				new_prefab->SetName(g_resourcePrefab[i]->GetName());
				new_prefab->SetPackageName(g_resourcePrefab[i]->GetPackageName());
				new_prefab->SetPrefabName(g_resourcePrefab[i]->GetPrefabName());
				g_prefab.push_back(new_prefab);
				currentPrefab = new_prefab;
				break;
			}
		}
	}

	if (!currentPrefab)
	{
		//CChar message[MAX_URI_SIZE];
		//sprintf(message, "\nGeneratePrefabInstance() Error: Couldn't find '%s%s", lua_tostring(L, 1), "' prefab");
		//PrintInfo(message, COLOR_RED);
		return 0;
	}

	CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
	g_currentInstancePrefab = new_instance_prefab;

	CChar instanceName[MAX_NAME_SIZE];
	sprintf(instanceName, "%i%s%s%s%s", index, "pi_", currentPrefab->GetPackageName(), "_", currentPrefab->GetPrefabName());
	index++;

	new_instance_prefab->SetName(instanceName);
	new_instance_prefab->SetTranslate(pos);
	new_instance_prefab->SetRotate(rot);
	new_instance_prefab->SetScale(scale);

	currentPrefab->AddInstance(new_instance_prefab);
	currentPrefab->SetCurrentInstance(new_instance_prefab);
	new_instance_prefab->SetPrefab(currentPrefab);
	new_instance_prefab->SetNameIndex(); //for selection only
	new_instance_prefab->GenQueryIndex();
	new_instance_prefab->SetWater(NULL);
	g_instancePrefab.push_back(new_instance_prefab);
	Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
	g_main->InsertPrefab(currentPrefab);
	new_instance_prefab->UpdateBoundingBox(CTrue);
	new_instance_prefab->CalculateDistance();
	new_instance_prefab->UpdateIsStaticOrAnimated();
	new_instance_prefab->SetLightCooked(CFalse);
	new_instance_prefab->SetHasScript(currentPrefab->GetHasScript());
	new_instance_prefab->SetScript(currentPrefab->GetScript());
	if (new_instance_prefab->GetHasScript())
	{
		new_instance_prefab->LoadLuaFile();
	}

	if (new_instance_prefab->GetIsStatic())
		g_octree->ResetState();

	//generate physics
	CScene* scene = NULL;

	for (CUInt j = 0; j < 3; j++)
	{
		if (currentPrefab && currentPrefab->GetHasLod(j))
		{
			scene = new_instance_prefab->GetScene(j);
			if (scene)
			{
				for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
				{
					CPhysXMaterial physicsMaterial;
					physicsMaterial.HasMaterial = scene->m_instanceGeometries[k]->HasPhysicsMaterial();
					physicsMaterial.Restitution = scene->m_instanceGeometries[k]->GetPhysicsRestitution();
					physicsMaterial.SkinWidth = scene->m_instanceGeometries[k]->GetPhysicsSkinWidth();
					physicsMaterial.StaticFriction = scene->m_instanceGeometries[k]->GetPhysicsStaticFriction();
					physicsMaterial.DynamicFriction = scene->m_instanceGeometries[k]->GetPhysicsDynamicFriction();

					if (scene->m_instanceGeometries[k]->m_hasPhysX && scene->m_controllers.size())
					{
						scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, g_currentInstancePrefab);
					}
					else if (scene->m_instanceGeometries[k]->m_hasPhysX)
					{
						scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, NULL);
					}
				}
			}
		}
	}
	if (g_currentInstancePrefab->GetHasCollider())
	{
		scene = new_instance_prefab->GetScene(3);
		if (scene)
		{
			for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
			{
				CPhysXMaterial physicsMaterial;
				physicsMaterial.HasMaterial = scene->m_instanceGeometries[k]->HasPhysicsMaterial();
				physicsMaterial.Restitution = scene->m_instanceGeometries[k]->GetPhysicsRestitution();
				physicsMaterial.SkinWidth = scene->m_instanceGeometries[k]->GetPhysicsSkinWidth();
				physicsMaterial.StaticFriction = scene->m_instanceGeometries[k]->GetPhysicsStaticFriction();
				physicsMaterial.DynamicFriction = scene->m_instanceGeometries[k]->GetPhysicsDynamicFriction();

				if (scene->m_instanceGeometries[k]->m_hasPhysX && scene->m_controllers.size())
				{
					scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, g_currentInstancePrefab);
				}
				else if (scene->m_instanceGeometries[k]->m_hasPhysX)
				{
					scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, NULL);
				}
			}
		}
	}

	//update max radius
	g_maxInstancePrefabRadius = -1.f;
	for (CUInt j = 0; j < g_instancePrefab.size(); j++)
	{
		if (g_instancePrefab[j]->GetScene(0) && g_instancePrefab[j]->GetScene(0)->CastShadow())
		{
			if (g_instancePrefab[j]->GetRadius() > g_maxInstancePrefabRadius)
				g_maxInstancePrefabRadius = g_instancePrefab[j]->GetRadius();
		}
	}

	lua_pushstring(L, instanceName);
	return 1;
}


CInt DeletePrefabInstance(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for DeletePrefabInstance()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar name[MAX_NAME_SIZE];
	Cpy(name, lua_tostring(L, 1));
	StringToUpper(name);

	if (Cmp(name, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			Cpy(name, g_currentInstancePrefab->GetName());
			StringToUpper(name);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "\nDeletePrefabInstance() Error: Couldn't find current Prefab instance");
			//PrintInfo(temp, COLOR_RED);

			return 0;
		}
	}

	g_main->m_prefabInstanceNamesToBeDeleted.push_back(name);

	return 0;
}

CInt AttachPrefabInstanceToWater(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for AttachPrefabInstanceToWater()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar prefabInstanceName[MAX_NAME_SIZE];
	Cpy(prefabInstanceName, lua_tostring(L, 1));
	StringToUpper(prefabInstanceName);

	if (lua_tostring(L, 2) == NULL) return 0;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 2));
	StringToUpper(waterName);

	CBool foundPrefabInstance = CFalse;
	CBool foundWater = CFalse;

	if (Cmp(prefabInstanceName, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			foundPrefabInstance = CTrue;

			for (CUInt i = 0; i < g_engineWaters.size(); i++)
			{
				CChar currentWaterName[MAX_NAME_SIZE];
				Cpy(currentWaterName, g_engineWaters[i]->GetName());
				StringToUpper(currentWaterName);

				if (Cmp(waterName, currentWaterName))
				{
					foundWater = CTrue;

					CBool isCurrentPrefabInstanceAttached = CFalse;
					for (CUInt j = 0; j < g_engineWaters[i]->GetNumPrefabInstances(); j++)
					{
						if (Cmp(g_engineWaters[i]->GetPrefabInstance(j)->GetName(), g_currentInstancePrefab->GetName()))
						{
							isCurrentPrefabInstanceAttached = CTrue;
							break;
						}
					}
					if (!isCurrentPrefabInstanceAttached)
					{
						g_engineWaters[i]->AddPrefabInstance(g_currentInstancePrefab);
						g_currentInstancePrefab->SetWater(g_engineWaters[i]);
						g_currentInstancePrefab->UpdateBoundingBoxForWater(g_engineWaters[i]->GetHeight());
					}

					break;
				}
			}
		}
	}
	else if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				CChar currentPrefabInstanceName[MAX_NAME_SIZE];
				Cpy(currentPrefabInstanceName, g_instancePrefab[i]->GetName());
				StringToUpper(currentPrefabInstanceName);

				if (Cmp(prefabInstanceName, currentPrefabInstanceName))
				{
					foundPrefabInstance = CTrue;

					CBool isCurrentPrefabInstanceAttached = CFalse;
					for (CUInt j = 0; j < g_currentWater->GetNumPrefabInstances(); j++)
					{
						if (Cmp(g_currentWater->GetPrefabInstance(j)->GetName(), g_instancePrefab[i]->GetName()))
						{
							isCurrentPrefabInstanceAttached = CTrue;
							break;
						}
					}
					if (!isCurrentPrefabInstanceAttached)
					{
						g_currentWater->AddPrefabInstance(g_instancePrefab[i]);
						g_instancePrefab[i]->SetWater(g_currentWater);
						g_instancePrefab[i]->UpdateBoundingBoxForWater(g_currentWater->GetHeight());
					}

					break;
				}
			}
		}
	}
	else
	{
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			CChar currentWaterName[MAX_NAME_SIZE];
			Cpy(currentWaterName, g_engineWaters[i]->GetName());
			StringToUpper(currentWaterName);

			if (Cmp(waterName, currentWaterName))
			{
				foundWater = CTrue;

				for (CUInt j = 0; j < g_instancePrefab.size(); j++)
				{
					CChar currentPrefabInstanceName[MAX_NAME_SIZE];
					Cpy(currentPrefabInstanceName, g_instancePrefab[j]->GetName());
					StringToUpper(currentPrefabInstanceName);

					if (Cmp(prefabInstanceName, currentPrefabInstanceName))
					{
						foundPrefabInstance = CTrue;

						CBool isCurrentPrefabInstanceAttached = CFalse;
						for (CUInt k = 0; k < g_engineWaters[i]->GetNumPrefabInstances(); k++)
						{
							if (Cmp(g_engineWaters[i]->GetPrefabInstance(k)->GetName(), g_instancePrefab[j]->GetName()))
							{
								isCurrentPrefabInstanceAttached = CTrue;
								break;
							}
						}
						if (!isCurrentPrefabInstanceAttached)
						{
							g_engineWaters[i]->AddPrefabInstance(g_instancePrefab[j]);
							g_instancePrefab[j]->SetWater(g_engineWaters[i]);
							g_instancePrefab[j]->UpdateBoundingBoxForWater(g_engineWaters[i]->GetHeight());
						}

						break;
					}
				}

				break;
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "AttachPrefabInstanceToWater() warning : Couldn't find '", lua_tostring(L, 1), "' prefab instance");
		//PrintInfo(temp, COLOR_RED);
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "AttachPrefabInstanceToWater() warning : Couldn't find '", lua_tostring(L, 2), "' water");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt DetachPrefabInstanceFromWater(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for DetachPrefabInstanceFromWater()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar prefabInstanceName[MAX_NAME_SIZE];
	Cpy(prefabInstanceName, lua_tostring(L, 1));
	StringToUpper(prefabInstanceName);

	if (lua_tostring(L, 2) == NULL) return 0;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 2));
	StringToUpper(waterName);

	CBool foundPrefabInstance = CFalse;
	CBool foundWater = CFalse;

	if (Cmp(prefabInstanceName, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			foundPrefabInstance = CTrue;

			for (CUInt i = 0; i < g_engineWaters.size(); i++)
			{
				CChar currentWaterName[MAX_NAME_SIZE];
				Cpy(currentWaterName, g_engineWaters[i]->GetName());
				StringToUpper(currentWaterName);

				if (Cmp(waterName, currentWaterName))
				{
					foundWater = CTrue;

					CBool isCurrentPrefabInstanceAttached = CFalse;
					CInt index = -1;
					for (CUInt j = 0; j < g_engineWaters[i]->GetNumPrefabInstances(); j++)
					{
						if (Cmp(g_engineWaters[i]->GetPrefabInstance(j)->GetName(), g_currentInstancePrefab->GetName()))
						{
							isCurrentPrefabInstanceAttached = CTrue;
							index = j;
							break;
						}
					}
					if (isCurrentPrefabInstanceAttached)
					{
						g_engineWaters[i]->RemovePrefabInstance(index);
						g_currentInstancePrefab->SetWater(NULL);
					}

					break;
				}
			}
		}
	}
	else if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				CChar currentPrefabInstance[MAX_NAME_SIZE];
				Cpy(currentPrefabInstance, g_instancePrefab[i]->GetName());
				StringToUpper(currentPrefabInstance);

				if (Cmp(prefabInstanceName, currentPrefabInstance))
				{
					foundPrefabInstance = CTrue;

					CBool isCurrentPrefabInstanceAttached = CFalse;
					CInt index = -1;
					for (CUInt j = 0; j < g_currentWater->GetNumPrefabInstances(); j++)
					{
						if (Cmp(g_currentWater->GetPrefabInstance(j)->GetName(), g_instancePrefab[i]->GetName()))
						{
							isCurrentPrefabInstanceAttached = CTrue;
							index = j;
							break;
						}
					}
					if (isCurrentPrefabInstanceAttached)
					{
						g_currentWater->RemovePrefabInstance(index);
						g_instancePrefab[i]->SetWater(NULL);
					}

					break;
				}
			}
		}
	}
	else
	{
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			CChar currentWaterName[MAX_NAME_SIZE];
			Cpy(currentWaterName, g_engineWaters[i]->GetName());
			StringToUpper(currentWaterName);

			if (Cmp(waterName, currentWaterName))
			{
				foundWater = CTrue;

				for (CUInt j = 0; j < g_instancePrefab.size(); j++)
				{
					CChar currentPrefabInstanceName[MAX_NAME_SIZE];
					Cpy(currentPrefabInstanceName, g_instancePrefab[j]->GetName());
					StringToUpper(currentPrefabInstanceName);

					if (Cmp(prefabInstanceName, currentPrefabInstanceName))
					{
						foundPrefabInstance = CTrue;

						CBool isCurrentPrefabInstanceAttached = CFalse;
						CInt index = -1;
						for (CUInt k = 0; k < g_engineWaters[i]->GetNumPrefabInstances(); k++)
						{
							if (Cmp(g_engineWaters[i]->GetPrefabInstance(k)->GetName(), g_instancePrefab[j]->GetName()))
							{
								isCurrentPrefabInstanceAttached = CTrue;
								index = k;
								break;
							}
						}
						if (isCurrentPrefabInstanceAttached)
						{
							g_engineWaters[i]->RemovePrefabInstance(index);
							g_instancePrefab[j]->SetWater(NULL);
						}

						break;
					}
				}

				break;
			}
		}
	}
	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "DetachPrefabInstanceFromWater() warning : Couldn't find '", lua_tostring(L, 1), "' prefab instance");
		//PrintInfo(temp, COLOR_RED);
	}
	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "DetachPrefabInstanceFromWater() warning : Couldn't find '", lua_tostring(L, 2), "' water");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt PauseWaterAnimation(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseWaterAnimation()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetUpdateAnimation(CFalse);
		}
		else
		{
			//PrintInfo("\nPauseWaterAnimation() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetUpdateAnimation(CFalse);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseWaterAnimation() Error: Couldn't find '", waterName, "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeWaterAnimation(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeWaterAnimation()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetUpdateAnimation(CTrue);
		}
		else
		{
			//PrintInfo("\nResumeWaterAnimation() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetUpdateAnimation(CTrue);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeWaterAnimation() Error: Couldn't find '", waterName, "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


CInt SetSoundVolume(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundVolume()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat volume = (CFloat)lua_tonumber(L, 2);

	if (volume < 0.0 || volume > 1.0)
	{
		//PrintInfo("\nSetSoundVolume() Error: value must be in [0,1] range", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundVolume Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetVolume(volume);
			g_engine3DSounds[i]->SetVolume(volume);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar currentSoundName[MAX_NAME_SIZE];
			Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(currentSoundName);

			if (Cmp(currentSoundName, soundName))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetVolume(volume);
				g_engineAmbientSounds[i]->SetVolume(volume);
				foundTarget = CTrue;
				break;
			}
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s", "\nSetSoundVolume() Error: Couldn't find Sound '", lua_tostring(L, 1));
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}


CInt SetSoundPitch(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundPitch()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat pitch = (CFloat)lua_tonumber(L, 2);

	if (pitch <= 0.0)
	{
		//PrintInfo("\nSetSoundPitch() Error: Pitch must be greater than 0.0", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundPitch Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetPitch(pitch);
			g_engine3DSounds[i]->SetPitch(pitch);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar currentSoundName[MAX_NAME_SIZE];
			Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(currentSoundName);

			if (Cmp(currentSoundName, soundName))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetPitch(pitch);
				g_engineAmbientSounds[i]->SetPitch(pitch);
				foundTarget = CTrue;
				break;
			}
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundPitch() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}


CInt SetSoundLoop(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundLoop()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CInt isLoop = (CFloat)lua_toboolean(L, 2);
	CBool loop = CFalse;
	if (isLoop)
		loop = CTrue;

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundLoop Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetLooping(loop);
			g_engine3DSounds[i]->SetLoop(loop);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		{
			CChar currentSoundName[MAX_NAME_SIZE];
			Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
			StringToUpper(currentSoundName);

			if (Cmp(currentSoundName, soundName))
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(loop);
				g_engineAmbientSounds[i]->SetLoop(loop);
				foundTarget = CTrue;
				break;
			}
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundLoop() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}


//This function is only Valid for 3D sounds
CInt SetSoundPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetSoundPosition()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat PosX = (CFloat)lua_tonumber(L, 2);
	CFloat PosY = (CFloat)lua_tonumber(L, 3);
	CFloat PosZ = (CFloat)lua_tonumber(L, 4);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundPosition Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			CFloat position[3] = { PosX, PosY, PosZ };
			g_engine3DSounds[i]->GetSoundSource()->SetSoundPosition(position);
			g_engine3DSounds[i]->SetPosition(position);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundPosition() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

//This function is only Valid for 3D sounds
CInt SetSoundRollOff(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundRollOff()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat RollOff = (CFloat)lua_tonumber(L, 2);

	if (RollOff < 0.0)
	{
		//PrintInfo("\nSetSoundRollOff() Error: RollOff must be greater than or equal to 0.0", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundRollOff Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetRolloff(RollOff);
			g_engine3DSounds[i]->SetRolloff(RollOff);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundRollOff() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

//This function is only Valid for 3D sounds
CInt SetSoundReferenceDistance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundReferenceDistance()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat ReferenceDistance = (CFloat)lua_tonumber(L, 2);

	if (ReferenceDistance < 0.0)
	{
		//PrintInfo("\nSetSoundReferenceDistance() Error: reference distance must be greater than or equal to 0.0", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundReferenceDistance Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetReferenceDistance(ReferenceDistance);
			g_engine3DSounds[i]->SetReferenceDistance(ReferenceDistance);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundReferenceDistance() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

//This function is only Valid for 3D sounds
CInt SetSoundMaxDistance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetSoundMaxDistance()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CFloat MaxDistance = (CFloat)lua_tonumber(L, 2);

	if (MaxDistance < 0.0)
	{
		//PrintInfo("\nSetSoundMaxDistance() Error: max distance must be greater than or equal to 0.0", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nSetSoundMaxDistance Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			g_engine3DSounds[i]->GetSoundSource()->SetMaxDistance(MaxDistance);
			g_engine3DSounds[i]->SetMaxDistance(MaxDistance);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetSoundMaxDistance() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetGlobalSoundVolume(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetGlobalSoundVolume()", COLOR_RED);
		return 0;
	}

	CFloat volume = (CFloat)lua_tonumber(L, 1);

	if (volume < 0.0 || volume > 1.0)
	{
		//PrintInfo("\nSetGlobalSoundVolume() Error: value must be in [0,1] range", COLOR_RED);
		return 0;
	}

	g_currentVSceneProperties.m_globalSoundVolume = volume;
	g_main->m_soundSystem->SetListenerGain(volume);

	return 0;
}

CInt GetSoundVolume(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundVolume()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundVolume Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engine3DSounds[i]->GetVolume());
			return 1;
		}
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engineAmbientSounds[i]->GetVolume());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundVolume() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetSoundPitch(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundPitch()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundPitch Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engine3DSounds[i]->GetPitch());
			return 1;
		}
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engineAmbientSounds[i]->GetPitch());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundPitch() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetSoundPlay(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundPlay()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundPlay Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			if (g_engine3DSounds[i]->GetPlay())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			if (g_engineAmbientSounds[i]->GetPlay())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundPlay() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetSoundLoop(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundLoop()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else if (g_currentAmbientSound)
		{
			Cpy(soundName, g_currentAmbientSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundLoop Error: Couldn't find current sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			if (g_engine3DSounds[i]->GetLoop())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			if (g_engineAmbientSounds[i]->GetLoop())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundLoop() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

//This function is only Valid for 3D sounds
CInt GetSoundPosition(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundPosition()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundPosition Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			CFloat x = g_engine3DSounds[i]->GetPosition()[0];
			CFloat y = g_engine3DSounds[i]->GetPosition()[1];
			CFloat z = g_engine3DSounds[i]->GetPosition()[2];
			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);
			return 3;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundPosition() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

//This function is only Valid for 3D sounds
CInt GetSoundRollOff(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundRollOff()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundRollOff Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engine3DSounds[i]->GetRolloff());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundRollOff() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

//This function is only Valid for 3D sounds
CInt GetSoundReferenceDistance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundReferenceDistance()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundReferenceDistance Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engine3DSounds[i]->GetReferenceDistance());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundReferenceDistance() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

//This function is only Valid for 3D sounds
CInt GetSoundMaxDistance(lua_State* L)
{
	//if (g_testScript)
	//	return 0;
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetSoundMaxDistance()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar soundName[MAX_NAME_SIZE];
	Cpy(soundName, lua_tostring(L, 1));
	StringToUpper(soundName);

	CBool foundTarget = CFalse;

	if (Cmp(soundName, "THIS"))
	{
		if (g_current3DSound)
		{
			Cpy(soundName, g_current3DSound->GetName());
			StringToUpper(soundName);
		}
		else
		{
			//PrintInfo("\nGetSoundMaxDistance Error: Couldn't find current 3D sound object", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar currentSoundName[MAX_NAME_SIZE];
		Cpy(currentSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(currentSoundName);

		if (Cmp(currentSoundName, soundName))
		{
			lua_pushnumber(L, g_engine3DSounds[i]->GetMaxDistance());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetSoundMaxDistance() Error: Couldn't find Sound '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetGlobalSoundVolume(lua_State* L)
{
	lua_pushnumber(L, g_currentVSceneProperties.m_globalSoundVolume);
	return 1;
}

CInt PlayVideo(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PlayVideo()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nPlayVideo() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetPlay(false);
			g_engineVideos[i]->SetPlay(true);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nPlayVideo() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;

}

CInt PlayVideoLoop(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PlayVideoLoop()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nPlayVideoLoop() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetPlay(false);
			g_engineVideos[i]->SetPlay(true);
			g_engineVideos[i]->SetLoop(true);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nPlayVideoLoop() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt PlayVideoOnce(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PlayVideoOnce()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nPlayVideoOnce() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetPlay(false);
			g_engineVideos[i]->SetPlay(true);
			g_engineVideos[i]->SetLoop(false);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nPlayVideoOnce() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt StopVideo(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for StopVideo()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nStopVideo() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetPlay(false);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nStopVideo() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetVideoLoop(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetVideoLoop()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CInt isLoop = (CFloat)lua_toboolean(L, 2);
	CBool loop = CFalse;
	if (isLoop)
		loop = CTrue;

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nSetVideoLoop() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetLoop(loop);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetVideoLoop() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetVideoVolume(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetVideoVolume()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CFloat volume = (CFloat)lua_tonumber(L, 2);

	if (volume < 0.0 || volume > 1.0)
	{
		//PrintInfo("\nSetVideoVolume() Error: value must be in [0,1] range", COLOR_RED);
		return 0;
	}

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nSetVideoVolume() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			g_engineVideos[i]->SetVolume(volume);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetVideoVolume() Error: Couldn't find video '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt GetVideoPlay(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVideoPlay()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nGetVideoPlay() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			if (g_engineVideos[i]->GetPlay())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetVideoPlay() Error: Couldn't find video '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetVideoLoop(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVideoLoop()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nGetVideoLoop() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			if (g_engineVideos[i]->GetLoop())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetVideoLoop() Error: Couldn't find video '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetVideoVolume(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVideoVolume()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nGetVideoVolume() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			lua_pushnumber(L, g_engineVideos[i]->GetVolume());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetVideoVolume() Error: Couldn't find video '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

CInt GetVideoDuration(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetVideoDuration()", COLOR_RED);
		return 0;
	}

	if (lua_tostring(L, 1) == NULL) return 0;

	CChar videoName[MAX_NAME_SIZE];
	Cpy(videoName, lua_tostring(L, 1));
	StringToUpper(videoName);

	CBool foundTarget = CFalse;

	if (Cmp(videoName, "THIS"))
	{
		if (g_currentVideo)
		{
			Cpy(videoName, g_currentVideo->GetName());
			StringToUpper(videoName);
		}
		else
		{
			//CChar temp[MAX_NAME_SIZE];
			//sprintf(temp, "%s", "\nGetVideoDuration() Error: Couldn't find current video");
			//PrintInfo(temp, COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CChar currentVideoName[MAX_NAME_SIZE];
		Cpy(currentVideoName, g_engineVideos[i]->GetName());
		StringToUpper(currentVideoName);

		if (Cmp(currentVideoName, videoName))
		{
			lua_pushnumber(L, g_engineVideos[i]->GetDuration());
			return 1;
		}
	}

	//CChar temp[MAX_NAME_SIZE];
	//sprintf(temp, "%s%s%s", "\nGetVideoDuration() Error: Couldn't find video '", lua_tostring(L, 1), "'");
	//PrintInfo(temp, COLOR_RED);

	return 0;
}

//Stop Sounds
CInt StopAllSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt StopAllAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt StopAll3DSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt StopAllResourceSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->StopALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

//Play Sounds
CInt PlayAllSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAll3DSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllResourceSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStopped3DSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedResourceSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPaused3DSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedResourceSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

//Play Sounds Loop
CInt PlayAllSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
			g_engineAmbientSounds[i]->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
			g_engine3DSounds[i]->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
			g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engineAmbientSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engine3DSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_PAUSED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engineAmbientSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engine3DSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_STOPPED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllAmbientSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
			g_engineAmbientSounds[i]->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAll3DSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
			g_engine3DSounds[i]->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllResourceSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
			g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedAmbientSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engineAmbientSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStopped3DSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engine3DSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedResourceSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_STOPPED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

				g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedAmbientSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engineAmbientSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPaused3DSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CTrue);
				g_engine3DSounds[i]->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedResourceSoundsLoop(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_PAUSED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CTrue);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CTrue);

				g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

//Play Sounds once
CInt PlayAllSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
			g_engineAmbientSounds[i]->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
			g_engine3DSounds[i]->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
			g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engineAmbientSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engine3DSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_PAUSED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engineAmbientSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engine3DSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_STOPPED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllAmbientSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
			g_engineAmbientSounds[i]->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAll3DSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
			g_engine3DSounds[i]->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllResourceSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
			g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

			g_soundSystem->PlayALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedAmbientSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engineAmbientSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStopped3DSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_STOPPED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engine3DSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllStoppedResourceSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_STOPPED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

				g_soundSystem->PlayALStoppedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedAmbientSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engineAmbientSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engineAmbientSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engineAmbientSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPaused3DSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_engine3DSounds[i]->GetSoundSource())) == AL_PAUSED)
			{
				g_engine3DSounds[i]->GetSoundSource()->SetLooping(CFalse);
				g_engine3DSounds[i]->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PlayAllPausedResourceSoundsOnce(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			if (g_soundSystem->GetSourceState(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource())) == AL_PAUSED)
			{
				g_resourceFiles[i]->GetSoundSource()->GetSoundSource()->SetLooping(CFalse);
				g_resourceFiles[i]->GetSoundSource()->SetLoop(CFalse);

				g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
			}
		}
	}

	m_soundexception.clear();
	return 0;
}

//Pause Sounds
CInt PauseAllSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PauseAllAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PauseAll3DSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_soundexception[j].c_str()))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}

CInt PauseAllResourceSounds(lua_State* L)
{
	std::vector<std::string> m_soundexception;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_soundexception.push_back(name);
	}

	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_soundexception.size(); j++)
		{
			//resource sound names are uppercase
			CChar currentName[MAX_NAME_SIZE];
			Cpy(currentName, m_soundexception[j].c_str());
			StringToUpper(currentName);

			if (Cmp(g_resourceFiles[i]->GetName(), currentName))
			{
				foundException = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}

	m_soundexception.clear();
	return 0;
}


//Pause game 
CInt PauseGame(lua_State* L)
{
	g_main->PauseGame();

	//pause all sounds of current VScene
	g_main->PauseSounds();

	return 0;
}

CInt PauseAllAnimationsOfPrefabInstances(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateAnimation(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseAllAnimationsOfPrefabInstances warning: Couldn't find '%s' prefab instance object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseMainCharacterAnimations(lua_State* L)
{
	g_main->m_pauseMainCharacterAnimations = CTrue;
	return 0;
}

CInt PausePhysics(lua_State* L)
{
	g_main->m_pausePhysics = CTrue;
	return 0;
}

CInt PauseAnimationOfAllWaters(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateAnimation(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseAnimationOfAllWaters warning: Couldn't find '%s' water object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

//resume game
CInt ResumeGame(lua_State* L)
{
	g_main->ResumeGame();

	g_timer->GetElapsedSeconds(CTrue);

	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
	}
	g_nx->gControllers->reportSceneChanged();

	//resume all sounds of current VScene
	g_main->ResumeSounds();

	return 0;
}

CInt ResumeAllAnimationsOfPrefabInstances(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateAnimation(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeAllAnimationsOfPrefabInstances warning: Couldn't find '%s' prefab instance object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeMainCharacterAnimations(lua_State* L)
{
	g_main->m_pauseMainCharacterAnimations = CFalse;
	return 0;
}

CInt ResumePhysics(lua_State* L)
{
	for (CUInt i = 0; i < gPhysXscene->getNbActors(); i++)
	{
		NxActor* currentActor = gPhysXscene->getActors()[i];
		if (currentActor->isSleeping())
			currentActor->wakeUp();
	}
	g_nx->gControllers->reportSceneChanged();

	g_main->m_pausePhysics = CFalse;

	return 0;
}

CInt ResumeAnimationOfAllWaters(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateAnimation(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeAnimationOfAllWaters warning: Couldn't find '%s' water object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

//lock/unlock character
CInt LockCharacterController(lua_State* L)
{
	g_currentVSceneProperties.m_lockCharacterController = CTrue;

	return 0;
}

CInt UnlockCharacterController(lua_State* L)
{
	g_currentVSceneProperties.m_lockCharacterController = CFalse;

	return 0;
}

//show/hide menu cursor
CInt ShowMenuCursor(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc > 0)
	{
		CInt size;
		size = lua_tointeger(L, 1);

		if (size <= 0)
		{
			//PrintInfo("\nShowMenuCursor() Error: cursor size must be greater than 0", COLOR_RED);
			return 0;
		}

		g_currentVSceneProperties.m_menuCursorSize = size;
	}

	g_main->m_mousePosition.x = (CFloat)g_width / 2.f;
	g_main->m_mousePosition.y = (CFloat)g_height / 2.f;

	g_main->m_showMenuCursor = CTrue;

	return 0;
}

CInt HideMenuCursor(lua_State* L)
{
	g_main->m_showMenuCursor = CFalse;

	return 0;
}

CInt SetMenuCursorSize(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc > 0)
	{
		CInt size;
		size = lua_tointeger(L, 1);

		if (size < 0)
		{
			//PrintInfo("\nSetMenuCursorSize() Error: cursor size must be greater than 0", COLOR_RED);
			return 0;
		}

		g_currentVSceneProperties.m_menuCursorSize = size;
	}
	return 0;
}

CInt GetMenuCursorSize(lua_State* L)
{
	lua_pushinteger(L, g_currentVSceneProperties.m_menuCursorSize);
	return 1;
}


//Pause script Update event of game objects
CInt PauseAllUpdateEvents(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateEvent(CFalse);
	}

	if (g_VSceneScript)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_VSceneScript->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_VSceneScript->SetUpdateEvent(CFalse);
	}

	if (g_skyDome)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_skyDome->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_skyDome->SetUpdateEvent(CFalse);
	}
	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineLights[i]->m_abstractLight->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineLights[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engine3DSounds[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineAmbientSounds[i]->SetUpdateEvent(CFalse);
	}

	if (g_mainCharacter)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_mainCharacter->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_mainCharacter->SetUpdateEvent(CFalse);
	}

	if (g_terrain)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_terrain->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_terrain->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineCameraInstances[i]->m_abstractCamera->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineCameraInstances[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseAllUpdateEvents warning: Couldn't find '%s' object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfAllPrefabInstances(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAllPrefabInstances warning: Couldn't find '%s' prefab instance object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfVSceneScript(lua_State* L)
{
	if (g_VSceneScript)
		g_VSceneScript->SetUpdateEvent(CFalse);

	return 0;
}

CInt PauseUpdateEventOfSky(lua_State* L)
{
	if (g_skyDome)
		g_skyDome->SetUpdateEvent(CFalse);

	return 0;
}

CInt PauseUpdateEventOfAllWaters(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAllWaters warning: Couldn't find '%s' water object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfAllLights(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineLights[i]->m_abstractLight->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineLights[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAllLights warning: Couldn't find '%s' light object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfAll3DSounds(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engine3DSounds[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAll3DSounds warning: Couldn't find '%s' 3D sound object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfAllAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineAmbientSounds[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAllAmbientSounds warning: Couldn't find '%s' ambient sound object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt PauseUpdateEventOfMainCharacter(lua_State* L)
{
	if (g_mainCharacter)
		g_mainCharacter->SetUpdateEvent(CFalse);

	return 0;
}

CInt PauseUpdateEventOfTerrain(lua_State* L)
{
	if (g_terrain)
		g_terrain->SetUpdateEvent(CFalse);

	return 0;
}

CInt PauseUpdateEventOfAllEngineCameras(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineCameraInstances[i]->m_abstractCamera->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineCameraInstances[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nPauseUpdateEventOfAllEngineCameras warning: Couldn't find '%s' camera object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

//Resume script Update event of game objects
CInt ResumeAllUpdateEvents(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateEvent(CTrue);
	}

	if (g_VSceneScript)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_VSceneScript->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_VSceneScript->SetUpdateEvent(CTrue);
	}

	if (g_skyDome)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_skyDome->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_skyDome->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineLights[i]->m_abstractLight->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineLights[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engine3DSounds[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineAmbientSounds[i]->SetUpdateEvent(CTrue);
	}

	if (g_mainCharacter)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_mainCharacter->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_mainCharacter->SetUpdateEvent(CTrue);
	}

	if (g_terrain)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_terrain->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (!foundException)
			g_terrain->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineCameraInstances[i]->m_abstractCamera->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineCameraInstances[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeAllUpdateEvents warning: Couldn't find '%s' game object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfAllPrefabInstances(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_instancePrefab[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_instancePrefab[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAllPrefabInstances warning: Couldn't find '%s' prefab instance object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfVSceneScript(lua_State* L)
{
	if (g_VSceneScript)
		g_VSceneScript->SetUpdateEvent(CTrue);

	return 0;
}

CInt ResumeUpdateEventOfSky(lua_State* L)
{
	if (g_skyDome)
		g_skyDome->SetUpdateEvent(CTrue);

	return 0;
}

CInt ResumeUpdateEventOfAllWaters(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineWaters[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineWaters[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAllWaters warning: Couldn't find '%s' water object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfAllLights(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineLights[i]->m_abstractLight->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineLights[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAllLights warning: Couldn't find '%s' light object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfAll3DSounds(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engine3DSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engine3DSounds[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAll3DSounds warning: Couldn't find '%s' 3D sound object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfAllAmbientSounds(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineAmbientSounds[i]->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineAmbientSounds[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAllAmbientSounds warning: Couldn't find '%s' ambient sound object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}

CInt ResumeUpdateEventOfMainCharacter(lua_State* L)
{
	if (g_mainCharacter)
		g_mainCharacter->SetUpdateEvent(CTrue);

	return 0;
}

CInt ResumeUpdateEventOfTerrain(lua_State* L)
{
	if (g_terrain)
		g_terrain->SetUpdateEvent(CTrue);

	return 0;
}

CInt ResumeUpdateEventOfAllEngineCameras(lua_State* L)
{
	std::vector<std::string> m_exception;
	std::vector<CBool>m_foundException;

	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, lua_tostring(L, n));
		m_exception.push_back(name);
		m_foundException.push_back(CFalse);
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CBool foundException = CFalse;
		for (CUInt j = 0; j < m_exception.size(); j++)
		{
			if (Cmp(g_engineCameraInstances[i]->m_abstractCamera->GetName(), m_exception[j].c_str()))
			{
				foundException = CTrue;
				m_foundException[j] = CTrue;
				break;
			}
		}
		if (foundException)
			continue;

		g_engineCameraInstances[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < m_foundException.size(); i++)
	{
		if (!m_foundException[i])
		{
			//CChar message[MAX_URI_SIZE];
			//sprintf(message, "\nResumeUpdateEventOfAllEngineCameras warning: Couldn't find '%s' camera object", m_exception[i].c_str());
			//PrintInfo(message, COLOR_YELLOW);
		}
	}

	m_exception.clear();

	return 0;
}


//Pause script Update event of individual game objects
CInt PauseUpdateEventOfPrefabInstance(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOfPrefabInstance()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetUpdateEvent(CFalse);

		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOfPrefabInstance() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;

			g_instancePrefab[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOfPrefabInstance() Error: Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt PauseUpdateEventOfWater(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOfWater()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentWater)
		{
			g_currentWater->SetUpdateEvent(CFalse);

		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOfWater() Error: Couldn't find current water", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;

			g_engineWaters[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOfWater() Error: Couldn't find '", luaToString, "' Water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt PauseUpdateEventOfLight(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOfLight()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->m_instanceLight->SetUpdateEvent(CFalse);
		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOfLight() Error: Couldn't find current light", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;

			g_engineLights[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOfLight() Error: Couldn't find '", luaToString, "' Light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt PauseUpdateEventOf3DSound(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOf3DSound()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3DSound Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_current3DSound)
		{
			g_current3DSound->SetUpdateEvent(CFalse);

		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOf3DSound() Error: Couldn't find current 3D sound", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar threeDSoundName[MAX_NAME_SIZE];
		Cpy(threeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(threeDSoundName);
		if (Cmp(threeDSoundName, luaToString))
		{
			found3DSound = CTrue;

			g_engine3DSounds[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOf3DSound() Error: Couldn't find '", luaToString, "' 3D Sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt PauseUpdateEventOfAmbientSound(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOfAmbientSound()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Ambient Sound Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentAmbientSound)
		{
			g_currentAmbientSound->SetUpdateEvent(CFalse);

		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOfAmbientSound() Error: Couldn't find current ambient sound", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;

			g_engineAmbientSounds[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOfAmbientSound() Error: Couldn't find '", luaToString, "' Ambient Sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt PauseUpdateEventOfEngineCamera(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for PauseUpdateEventOfEngineCamera()", COLOR_RED);
		return 0;
	}

	CBool foundEngineCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Engine Camera Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetUpdateEvent(CFalse);

		}
		else
		{
			//PrintInfo("\nPauseUpdateEventOfEngineCamera() Error: Couldn't find current engine camera", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar engineCameraName[MAX_NAME_SIZE];
		Cpy(engineCameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(engineCameraName);
		if (Cmp(engineCameraName, luaToString))
		{
			foundEngineCamera = CTrue;

			g_engineCameraInstances[i]->SetUpdateEvent(CFalse);
		}
	}

	if (!foundEngineCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "PauseUpdateEventOfEngineCamera() Error: Couldn't find '", luaToString, "' Engine Camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//Resume script Update event of individual game objects
CInt ResumeUpdateEventOfPrefabInstance(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOfPrefabInstance()", COLOR_RED);
		return 0;
	}

	CBool foundPrefabInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Prefab Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetUpdateEvent(CTrue);

		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOfPrefabInstance() Error: Couldn't find current prefab instance", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_instancePrefab[i]->GetName());
		StringToUpper(prefabName);
		if (Cmp(prefabName, luaToString))
		{
			foundPrefabInstance = CTrue;

			g_instancePrefab[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!foundPrefabInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOfPrefabInstance() Error: Couldn't find '", luaToString, "' Prefab Instance");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeUpdateEventOfWater(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOfWater()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Water Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentWater)
		{
			g_currentWater->SetUpdateEvent(CTrue);

		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOfWater() Error: Couldn't find current water", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);
		if (Cmp(waterName, luaToString))
		{
			foundWater = CTrue;

			g_engineWaters[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOfWater() Error: Couldn't find '", luaToString, "' Water");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeUpdateEventOfLight(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOfLight()", COLOR_RED);
		return 0;
	}

	CBool foundLight = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Light Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentLight)
		{
			g_currentLight->m_instanceLight->SetUpdateEvent(CTrue);
		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOfLight() Error: Couldn't find current light", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		CChar lightName[MAX_NAME_SIZE];
		Cpy(lightName, g_engineLights[i]->m_abstractLight->GetName());
		StringToUpper(lightName);
		if (Cmp(lightName, luaToString))
		{
			foundLight = CTrue;

			g_engineLights[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!foundLight)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOfLight() Error: Couldn't find '", luaToString, "' Light");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeUpdateEventOf3DSound(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOf3DSound()", COLOR_RED);
		return 0;
	}

	CBool found3DSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //3DSound Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_current3DSound)
		{
			g_current3DSound->SetUpdateEvent(CTrue);

		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOf3DSound() Error: Couldn't find current 3D sound", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CChar threeDSoundName[MAX_NAME_SIZE];
		Cpy(threeDSoundName, g_engine3DSounds[i]->GetName());
		StringToUpper(threeDSoundName);
		if (Cmp(threeDSoundName, luaToString))
		{
			found3DSound = CTrue;

			g_engine3DSounds[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!found3DSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOf3DSound() Error: Couldn't find '", luaToString, "' 3D Sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeUpdateEventOfAmbientSound(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOfAmbientSound()", COLOR_RED);
		return 0;
	}

	CBool foundAmbientSound = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Ambient Sound Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentAmbientSound)
		{
			g_currentAmbientSound->SetUpdateEvent(CTrue);

		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOfAmbientSound() Error: Couldn't find current ambient sound", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CChar ambientSoundName[MAX_NAME_SIZE];
		Cpy(ambientSoundName, g_engineAmbientSounds[i]->GetName());
		StringToUpper(ambientSoundName);
		if (Cmp(ambientSoundName, luaToString))
		{
			foundAmbientSound = CTrue;

			g_engineAmbientSounds[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!foundAmbientSound)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOfAmbientSound() Error: Couldn't find '", luaToString, "' Ambient Sound");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt ResumeUpdateEventOfEngineCamera(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for ResumeUpdateEventOfEngineCamera()", COLOR_RED);
		return 0;
	}

	CBool foundEngineCamera = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Engine Camera Name- First Argument
	StringToUpper(luaToString);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetUpdateEvent(CTrue);

		}
		else
		{
			//PrintInfo("\nResumeUpdateEventOfEngineCamera() Error: Couldn't find current engine camera", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar engineCameraName[MAX_NAME_SIZE];
		Cpy(engineCameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(engineCameraName);
		if (Cmp(engineCameraName, luaToString))
		{
			foundEngineCamera = CTrue;

			g_engineCameraInstances[i]->SetUpdateEvent(CTrue);
		}
	}

	if (!foundEngineCamera)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "ResumeUpdateEventOfEngineCamera() Error: Couldn't find '", luaToString, "' Engine Camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//Terrain
CInt SetTerrainAmbient(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTerrainAmbient()", COLOR_RED);
		return 0;
	}

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 1);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nFirst argument of SetTerrainAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 2);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetTerrainAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 3);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nThird argument of SetTerrainAmbient() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (g_terrain)
		g_terrain->SetAmbientColor(Color);

	return 0;
}

CInt SetTerrainDiffuse(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTerrainDiffuse()", COLOR_RED);
		return 0;
	}

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 1);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nFirst argument of SetTerrainDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 2);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetTerrainDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 3);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nThird argument of SetTerrainDiffuse() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (g_terrain)
		g_terrain->SetDiffuseColor(Color);

	return 0;
}

CInt SetTerrainSpecular(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetTerrainSpecular()", COLOR_RED);
		return 0;
	}

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 1);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nFirst argument of SetTerrainSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 2);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetTerrainSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 3);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nThird argument of SetTerrainSpecular() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[4] = { R, G, B, 1.0f };

	if (g_terrain)
		g_terrain->SetSpecularColor(Color);

	return 0;
}

CInt SetTerrainShininess(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetTerrainShininess()", COLOR_RED);
		return 0;
	}

	CFloat shininess = (CFloat)lua_tonumber(L, 1);
	if (shininess < 0.0f)
	{
		//PrintInfo("\nSetTerrainShininess Error: shininess must be greater than or equal to 0.0", COLOR_RED);
		return 0;
	}

	if (g_terrain)
		g_terrain->SetShininess(shininess);

	return 0;
}

CInt GetTerrainAmbient(lua_State* L)
{
	if (!g_terrain)
	{
		//PrintInfo("\nGetTerrainAmbient() Error: Couldn't find terrain object", COLOR_RED);
		return 0;
	}

	CFloat x = g_terrain->GetAmbientColor()[0];
	CFloat y = g_terrain->GetAmbientColor()[1];
	CFloat z = g_terrain->GetAmbientColor()[2];

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

CInt GetTerrainDiffuse(lua_State* L)
{
	if (!g_terrain)
	{
		//PrintInfo("\nGetTerrainDiffuse() Error: Couldn't find terrain object", COLOR_RED);
		return 0;
	}

	CFloat x = g_terrain->GetDiffuseColor()[0];
	CFloat y = g_terrain->GetDiffuseColor()[1];
	CFloat z = g_terrain->GetDiffuseColor()[2];

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

CInt GetTerrainSpecular(lua_State* L)
{
	if (!g_terrain)
	{
		//PrintInfo("\nGetTerrainSpecular() Error: Couldn't find terrain object", COLOR_RED);
		return 0;
	}

	CFloat x = g_terrain->GetSpecularColor()[0];
	CFloat y = g_terrain->GetSpecularColor()[1];
	CFloat z = g_terrain->GetSpecularColor()[2];

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

CInt GetTerrainShininess(lua_State* L)
{
	if (!g_terrain)
	{
		//PrintInfo("\nGetTerrainShininess() Error: Couldn't find terrain object", COLOR_RED);
		return 0;
	}

	CFloat shininess = g_terrain->GetShininess();

	lua_pushnumber(L, shininess);

	return 1;
}

//Engine Camera
CInt SetEngineCameraPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetEngineCameraPosition()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat x = (CFloat)lua_tonumber(L, 2);
	CFloat y = (CFloat)lua_tonumber(L, 3);
	CFloat z = (CFloat)lua_tonumber(L, 4);

	CVec3f position;
	position.x = x; position.y = y; position.z = z;

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetPos(position);
			g_currentEngineCamera->MoveTransform2(position.x, position.y, position.z);
		}
		else
		{
			//PrintInfo("\nSetEngineCameraPosition Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->SetPos(position);
			g_engineCameraInstances[i]->MoveTransform2(position.x, position.y, position.z);
			break;
		}
	}

	return 0;
}

CInt SetEngineCameraPan(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetEngineCameraPan()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat pan = (CFloat)lua_tonumber(L, 2);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetPan(pan);
			g_currentEngineCamera->SetPanAndTilt2(pan, g_currentEngineCamera->GetTilt());
		}
		else
		{
			//PrintInfo("\nSetEngineCameraPan Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->SetPan(pan);
			g_engineCameraInstances[i]->SetPanAndTilt2(pan, g_engineCameraInstances[i]->GetTilt());
			break;
		}
	}

	return 0;
}

CInt SetEngineCameraTilt(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetEngineCameraTilt()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat tilt = (CFloat)lua_tonumber(L, 2);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetTilt(tilt);
			g_currentEngineCamera->SetPanAndTilt2(g_currentEngineCamera->GetPan(), tilt);
		}
		else
		{
			//PrintInfo("\nSetEngineCameraTilt Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->SetTilt(tilt);
			g_engineCameraInstances[i]->SetPanAndTilt2(g_engineCameraInstances[i]->GetPan(), tilt);
			break;
		}
	}

	return 0;
}

CInt SetEngineCameraNearClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetEngineCameraNearClipPlane()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat ncp = (CFloat)lua_tonumber(L, 2);

	if (ncp <= 0.0)
	{
		//PrintInfo("\nSetEngineCameraNearClipPlane() Error: near clip plane must be greater than 0.0", COLOR_RED);
		return 0;
	}

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetNCP(ncp);
		}
		else
		{
			//PrintInfo("\nSetEngineCameraNearClipPlane Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->SetNCP(ncp);
			break;
		}
	}

	return 0;
}

CInt SetEngineCameraFarClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetEngineCameraFarClipPlane()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat fcp = (CFloat)lua_tonumber(L, 2);

	if (fcp <= 0.0)
	{
		//PrintInfo("\nSetEngineCameraFarClipPlane() Error: far clip plane must be greater than 0.0", COLOR_RED);
		return 0;
	}

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->SetFCP(fcp);
		}
		else
		{
			//PrintInfo("\nSetEngineCameraFarClipPlane Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->SetFCP(fcp);
			break;
		}
	}

	return 0;
}

CInt SetEngineCameraAngle(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetEngineCameraAngle()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CFloat angle = (CFloat)lua_tonumber(L, 2);

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentEngineCamera)
		{
			g_currentEngineCamera->m_abstractCamera->SetAngle(angle);
		}
		else
		{
			//PrintInfo("\nSetEngineCameraAngle Error: Couldn't find current engine camera", COLOR_RED);
		}

		return 0;
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);

		if (Cmp(luaToString, cameraName))
		{
			g_engineCameraInstances[i]->m_abstractCamera->SetAngle(angle);
			break;
		}
	}

	return 0;
}

CInt GetEngineCameraPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraPosition()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat x = g_currentEngineCamera->GetPos().x;
			CFloat y = g_currentEngineCamera->GetPos().y;
			CFloat z = g_currentEngineCamera->GetPos().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraPosition() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat x = g_engineCameraInstances[i]->GetPos().x;
			CFloat y = g_engineCameraInstances[i]->GetPos().y;
			CFloat z = g_engineCameraInstances[i]->GetPos().z;

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetEngineCameraPan(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraPan()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat val = g_currentEngineCamera->GetPan();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraPan() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat val = g_engineCameraInstances[i]->GetPan();
			lua_pushnumber(L, val);
			return 1;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetEngineCameraTilt(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraTilt()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat val = g_currentEngineCamera->GetTilt();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraTilt() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat val = g_engineCameraInstances[i]->GetTilt();
			lua_pushnumber(L, val);
			return 1;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetEngineCameraNearClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraNearClipPlane()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat val = g_currentEngineCamera->GetNCP();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraNearClipPlane() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat val = g_engineCameraInstances[i]->GetNCP();
			lua_pushnumber(L, val);
			return 1;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetEngineCameraFarClipPlane(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraFarClipPlane()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat val = g_currentEngineCamera->GetFCP();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraFarClipPlane() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat val = g_engineCameraInstances[i]->GetFCP();
			lua_pushnumber(L, val);
			return 1;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetEngineCameraAngle(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetEngineCameraAngle()", COLOR_RED);
		return 0;
	}
	CBool foundCameraInstance = CFalse;

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1)); //Camera Instance Name- First Argument
	StringToUpper(luaToString);

	if (Cmp("THIS", luaToString))
	{
		if (g_currentEngineCamera)
		{
			CFloat val = g_currentEngineCamera->m_abstractCamera->GetAngle();
			lua_pushnumber(L, val);
			return 1;
		}
		else
		{
			//PrintInfo("\nGetEngineCameraAngle() Error: Couldn't find current engine camera", COLOR_RED);
			return 0;
		}
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		Cpy(cameraName, g_engineCameraInstances[i]->m_abstractCamera->GetName());
		StringToUpper(cameraName);
		if (Cmp(cameraName, luaToString))
		{
			foundCameraInstance = CTrue;
			CFloat val = g_engineCameraInstances[i]->m_abstractCamera->GetAngle();
			lua_pushnumber(L, val);
			return 1;
		}
	}
	if (!foundCameraInstance)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find '", luaToString, "' engine camera");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}


//Water
CInt SetWaterPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetWaterPosition()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat x = (CFloat)lua_tonumber(L, 2);
	CFloat y = (CFloat)lua_tonumber(L, 3);
	CFloat z = (CFloat)lua_tonumber(L, 4);

	CFloat position[3];
	position[0] = x;
	position[1] = y;
	position[2] = z;

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetPos(position);
			g_currentWater->SetSideVertexPositions();
		}
		else
		{
			//PrintInfo("\nSetWaterPosition() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetPos(position);
			g_engineWaters[i]->SetSideVertexPositions();

			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterPosition() Error: Couldn't find '", waterName, "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterRotation(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetWaterRotation()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat rotateY = (CFloat)lua_tonumber(L, 2);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetRotateY(rotateY);
			g_currentWater->SetSideVertexPositions();
		}
		else
		{
			//PrintInfo("\nSetWaterRotation() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetRotateY(rotateY);
			g_engineWaters[i]->SetSideVertexPositions();

			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterRotation() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterScale(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetWaterScale()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat scaleX = (CFloat)lua_tonumber(L, 2);
	CFloat scaleZ = (CFloat)lua_tonumber(L, 3);

	if (scaleX < 0.01 || scaleZ < 0.01)
	{
		//PrintInfo("\nSetWaterScale() Error: scaleX and scaleZ must be equal to or greater than 0.01", COLOR_RED);
		return 0;
	}

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetScaleX(scaleX);
			g_currentWater->SetScaleZ(scaleZ);
			g_currentWater->SetSideVertexPositions();
		}
		else
		{
			//PrintInfo("\nSetWaterScale() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetScaleX(scaleX);
			g_engineWaters[i]->SetScaleZ(scaleZ);
			g_engineWaters[i]->SetSideVertexPositions();

			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterScale() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterLightPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetWaterLightPosition()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat x = (CFloat)lua_tonumber(L, 2);
	CFloat y = (CFloat)lua_tonumber(L, 3);
	CFloat z = (CFloat)lua_tonumber(L, 4);

	CFloat position[3];
	position[0] = x;
	position[1] = y;
	position[2] = z;

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetLightPos(position);
		}
		else
		{
			//PrintInfo("\nSetWaterLightPosition() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetLightPos(position);

			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterLightPosition() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterUnderwaterColor(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for SetWaterUnderwaterColor()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CBool foundColorError = CFalse;
	CFloat R = (CFloat)lua_tonumber(L, 2);
	if (R < 0.0f || R > 1.0f)
	{
		//PrintInfo("\nSecond argument of SetWaterUnderwaterColor() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat G = (CFloat)lua_tonumber(L, 3);
	if (G < 0.0f || G > 1.0f)
	{
		//PrintInfo("\nThird argument of SetWaterUnderwaterColor() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	CFloat B = (CFloat)lua_tonumber(L, 4);
	if (B < 0.0f || B > 1.0f)
	{
		//PrintInfo("\nFourth argument of SetWaterUnderwaterColor() must be between 0.0 and 1.0", COLOR_RED);
		foundColorError = CTrue;
	}

	if (foundColorError)
		return 0;

	CFloat Color[3] = { R, G, B };

	if (Cmp(luaToString, "THIS"))
	{
		if (g_currentWater)
		{
			g_currentWater->SetColor(Color);
		}
		else
		{
			//PrintInfo("\nSetWaterUnderwaterColor Error: Couldn't find current water", COLOR_RED);
		}

		return 0;
	}

	CBool foundTarget = CFalse;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar waterName[MAX_NAME_SIZE];
		Cpy(waterName, g_engineWaters[i]->GetName());
		StringToUpper(waterName);

		if (Cmp(luaToString, waterName))
		{
			g_engineWaters[i]->SetColor(Color);
			foundTarget = CTrue;
			break;
		}
	}

	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "%s%s%s", "\nSetWaterUnderwaterColor() Error: Couldn't find water '", lua_tostring(L, 1), "'");
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt SetWaterUnderwaterFogDensity(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetWaterUnderwaterFogDensity()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat fogDensity = (CFloat)lua_tonumber(L, 2);

	if (fogDensity < 0.0)
	{
		//PrintInfo("\nSetWaterUnderwaterFogDensity Error: fog density must be equal or greater than 0.0", COLOR_RED);
		return 0;
	}

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetFogDensity(fogDensity);
		}
		else
		{
			//PrintInfo("\nSetWaterUnderwaterFogDensity() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetFogDensity(fogDensity);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterUnderwaterFogDensity() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterTransparency(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetWaterTransparency()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat transparency = (CFloat)lua_tonumber(L, 2);

	if (transparency < 0.0f || transparency > 1.0f)
	{
		//PrintInfo("\nSetWaterTransparency Error: transparency must be in [0.0, 1.0] range", COLOR_RED);
		return 0;
	}

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetTransparency(transparency);
		}
		else
		{
			//PrintInfo("\nSetWaterTransparency() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetTransparency(transparency);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterTransparency() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterFlowSpeed(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetWaterFlowSpeed()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat flowSpeed = (CFloat)lua_tonumber(L, 2);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetSpeed(flowSpeed);
		}
		else
		{
			//PrintInfo("\nSetWaterFlowSpeed() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetSpeed(flowSpeed);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterFlowSpeed() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterUV(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 2)
	{
		//PrintInfo("\nPlease specify 2 arguments for SetWaterUV()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	CFloat UV = (CFloat)lua_tonumber(L, 2);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetUV(UV);
		}
		else
		{
			//PrintInfo("\nSetWaterUV() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetUV(UV);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterUV() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterVisible(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetWaterVisible()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetVisible(CTrue);
		}
		else
		{
			//PrintInfo("\nSetWaterVisible() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetVisible(CTrue);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterVisible() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt SetWaterInvisible(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for SetWaterInvisible()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetVisible(CFalse);
		}
		else
		{
			//PrintInfo("\nSetWaterInvisible() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetVisible(CFalse);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "SetWaterInvisible() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt EnableWaterShadow(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for EnableWaterShadow()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetShadow(CTrue);
		}
		else
		{
			//PrintInfo("\nEnableWaterShadow() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetShadow(CTrue);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "EnableWaterShadow() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt DisableWaterShadow(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for DisableWaterShadow()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetShadow(CFalse);
		}
		else
		{
			//PrintInfo("\nDisableWaterShadow() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetShadow(CFalse);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "DisableWaterShadow() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt EnableWaterSunReflection(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for EnableWaterSunReflection()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetSunReflection(CTrue);
		}
		else
		{
			//PrintInfo("\nEnableWaterSunReflection() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetSunReflection(CTrue);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "EnableWaterSunReflection() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt DisableWaterSunReflection(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for DisableWaterSunReflection()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;
			g_currentWater->SetSunReflection(CFalse);
		}
		else
		{
			//PrintInfo("\nDisableWaterSunReflection() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			g_engineWaters[i]->SetSunReflection(CFalse);
			break;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "DisableWaterSunReflection() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterPosition()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat x = g_currentWater->GetPos()[0];
			CFloat y = g_currentWater->GetPos()[1];
			CFloat z = g_currentWater->GetPos()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetWaterPosition() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat x = g_engineWaters[i]->GetPos()[0];
			CFloat y = g_engineWaters[i]->GetPos()[1];
			CFloat z = g_engineWaters[i]->GetPos()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterPosition() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterRotation(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterRotation()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat rotationY = g_currentWater->GetRotateY();

			lua_pushnumber(L, rotationY);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetWaterRotation() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat rotationY = g_engineWaters[i]->GetRotateY();

			lua_pushnumber(L, rotationY);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterRotation() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterScale(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterScale()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat scaleX = g_currentWater->GetScaleX();
			CFloat scaleZ = g_currentWater->GetScaleZ();

			lua_pushnumber(L, scaleX);
			lua_pushnumber(L, scaleZ);

			return 2;
		}
		else
		{
			//PrintInfo("\nGetWaterScale() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;

			CFloat scaleX = g_engineWaters[i]->GetScaleX();
			CFloat scaleZ = g_engineWaters[i]->GetScaleZ();

			lua_pushnumber(L, scaleX);
			lua_pushnumber(L, scaleZ);

			return 2;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterScale() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterLightPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterLightPosition()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat x = g_currentWater->GetLightPos()[0];
			CFloat y = g_currentWater->GetLightPos()[1];
			CFloat z = g_currentWater->GetLightPos()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetWaterLightPosition() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat x = g_engineWaters[i]->GetLightPos()[0];
			CFloat y = g_engineWaters[i]->GetLightPos()[1];
			CFloat z = g_engineWaters[i]->GetLightPos()[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterLightPosition() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterUnderwaterColor(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterUnderwaterColor()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat r = g_currentWater->GetColor()[0];
			CFloat g = g_currentWater->GetColor()[1];
			CFloat b = g_currentWater->GetColor()[2];

			lua_pushnumber(L, r);
			lua_pushnumber(L, g);
			lua_pushnumber(L, b);

			return 3;
		}
		else
		{
			//PrintInfo("\nGetWaterUnderwaterColor() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat r = g_engineWaters[i]->GetColor()[0];
			CFloat g = g_engineWaters[i]->GetColor()[1];
			CFloat b = g_engineWaters[i]->GetColor()[2];

			lua_pushnumber(L, r);
			lua_pushnumber(L, g);
			lua_pushnumber(L, b);

			return 3;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterUnderwaterColor() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterUnderwaterFogDensity(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterUnderwaterFogDensity()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat fogDensity = g_currentWater->GetFogDensity();

			lua_pushnumber(L, fogDensity);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetWaterUnderwaterFogDensity() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat fogDensity = g_engineWaters[i]->GetFogDensity();

			lua_pushnumber(L, fogDensity);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterUnderwaterFogDensity() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterTransparency(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterTransparency()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat transparency = g_currentWater->GetTransparency();

			lua_pushnumber(L, transparency);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetWaterTransparency() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat transparency = g_engineWaters[i]->GetTransparency();

			lua_pushnumber(L, transparency);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterTransparency() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterFlowSpeed(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterFlowSpeed()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat flowSpeed = g_currentWater->GetSpeed();

			lua_pushnumber(L, flowSpeed);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetWaterFlowSpeed() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat flowSpeed = g_engineWaters[i]->GetSpeed();

			lua_pushnumber(L, flowSpeed);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterFlowSpeed() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetWaterUV(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for GetWaterUV()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			CFloat UV = g_currentWater->GetUV();

			lua_pushnumber(L, UV);

			return 1;
		}
		else
		{
			//PrintInfo("\nGetWaterUV() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			foundWater = CTrue;
			CFloat UV = g_engineWaters[i]->GetUV();

			lua_pushnumber(L, UV);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "GetWaterUV() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt IsWaterVisible(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for IsWaterVisible()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			if (g_currentWater->GetVisible())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
		else
		{
			//PrintInfo("\nIsWaterVisible() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			if (g_engineWaters[i]->GetVisible())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "IsWaterVisible() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt IsWaterShadowEnabled(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for IsWaterShadowEnabled()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			if (g_currentWater->GetShadow())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
		else
		{
			//PrintInfo("\nIsWaterShadowEnabled() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			if (g_engineWaters[i]->GetShadow())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "IsWaterShadowEnabled() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt IsWaterSunReflectionEnabled(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for IsWaterSunReflectionEnabled()", COLOR_RED);
		return 0;
	}

	CBool foundWater = CFalse;

	CChar waterName[MAX_NAME_SIZE];
	Cpy(waterName, lua_tostring(L, 1)); //Water name- First Argument
	StringToUpper(waterName);

	if (Cmp(waterName, "THIS"))
	{
		if (g_currentWater)
		{
			foundWater = CTrue;

			if (g_currentWater->GetSunReflection())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
		else
		{
			//PrintInfo("\nIsWaterSunReflectionEnabled() Error: Couldn't find current water object", COLOR_RED);
		}
		return 0;
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		CChar currentWaterName[MAX_NAME_SIZE];
		Cpy(currentWaterName, g_engineWaters[i]->GetName());
		StringToUpper(currentWaterName);

		if (Cmp(waterName, currentWaterName))
		{
			if (g_engineWaters[i]->GetSunReflection())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);

			return 1;
		}
	}

	if (!foundWater)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "IsWaterSunReflectionEnabled() Error: Couldn't find '", lua_tostring(L, 1), "' water object");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

//Sky
CInt SetSkyPosition(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for SetSkyPosition()", COLOR_RED);
		return 0;
	}

	CFloat x = (CFloat)lua_tonumber(L, 1);
	CFloat y = (CFloat)lua_tonumber(L, 2);
	CFloat z = (CFloat)lua_tonumber(L, 3);

	if (g_skyDome)
	{
		CFloat position[3];
		position[0] = x; position[1] = y; position[2] = z;

		g_skyDome->SetPosition(position);
	}
	else
	{
		//PrintInfo("\nSetSkyPosition Error: Couldn't find sky object", COLOR_RED);
		return 0;
	}

	return 0;
}

CInt EnableSkyFog(lua_State* L)
{
	if (g_skyDome)
	{
		g_skyDome->SetFog(CTrue);
	}
	else
	{
		//PrintInfo("\nEnableSkyFog Error: Couldn't find sky object", COLOR_RED);
		return 0;
	}

	return 0;
}

CInt DisableSkyFog(lua_State* L)
{
	if (g_skyDome)
	{
		g_skyDome->SetFog(CFalse);
	}
	else
	{
		//PrintInfo("\nDisableSkyFog Error: Couldn't find sky object", COLOR_RED);
		return 0;
	}

	return 0;
}

CInt GetSkyPosition(lua_State* L)
{
	if (g_skyDome)
	{
		CFloat x = g_skyDome->GetPosition()[0];
		CFloat y = g_skyDome->GetPosition()[1];
		CFloat z = g_skyDome->GetPosition()[2];

		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushnumber(L, z);

		return 3;
	}
	else
	{
		//PrintInfo("\nGetSkyPosition Error: Couldn't find sky object", COLOR_RED);
		return 0;
	}

	return 0;
}

CInt IsSkyFogEnabled(lua_State* L)
{
	if (g_skyDome)
	{
		if (g_skyDome->GetFog())
			lua_pushboolean(L, 1); //true
		else
			lua_pushboolean(L, 0); //false

		return 1;
	}
	else
	{
		//PrintInfo("\nIsSkyFogEnabled Error: Couldn't find sky object", COLOR_RED);
		return 0;
	}

	return 0;
}


//manage files/folders/Save/Load

CInt CreateFolder(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for CreateFolder()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	CreateWindowsDirectory(path);

	return 0;
}

CInt RemoveFolder(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for RemoveFolder()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	if (!RemoveAllFilesAndFoldersInDirectory(path))
	{
		//CChar temp[MAX_URI_SIZE];
		//sprintf(temp, "\nRemoveFolder() Error: An error occured while removing the folder '%s'", path);
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	return 0;
}

CInt RemoveFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for RemoveFile()", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	if (!DeleteFileA(path))
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s", "RemoveFile() Error: Couldn't remove the file ", path);
		//PrintInfo(temp, COLOR_RED);
	}

	return 0;
}

CInt OpenFileForReading(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for OpenFileForReading()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile)
	{
		//PrintInfo("\nOpenFileForReading() Error: a file is already open", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	g_main->m_luaFile = fopen(path, "rb");

	if (!g_main->m_luaFile)
	{
		//CChar temp[MAX_URI_SIZE];
		//sprintf(temp, "\nOpenFileForReading() Error: Couldn't open '%s' file", path);
		//PrintInfo(temp, COLOR_RED);

		return 0;
	}

	return 0;
}

CInt OpenFileForWriting(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for OpenFileForWriting()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile)
	{
		//PrintInfo("\nOpenFileForWriting() Error: a file is already open", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	g_main->m_luaFile = fopen(path, "wb");

	return 0;
}

CInt CloseFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for CloseFile()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nCloseFile() Error: file is already closed", COLOR_RED);
		return 0;
	}

	CChar luaToString[MAX_URI_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));

	CChar path[MAX_URI_SIZE];
	Cpy(path, "Assets/Data/");
	Append(path, luaToString);

	fclose(g_main->m_luaFile);
	g_main->m_luaFile = NULL;

	return 0;
}

CInt ReadBoolVariableFromFile(lua_State* L)
{
	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nReadBoolVariableFromFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CBool result;
	fread(&result, sizeof(CBool), 1, g_main->m_luaFile);

	if (result)
		lua_pushboolean(L, 1); //true
	else
		lua_pushboolean(L, 0); //false

	return 1;
}

CInt WriteBoolVariableToFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for WriteBoolVariableToFile()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nWriteBoolVariableToFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CInt iResult = lua_toboolean(L, 1);
	CBool bResult = CFalse;

	if (iResult)
		bResult = CTrue;
	else
		bResult = CFalse;

	fwrite(&bResult, sizeof(CBool), 1, g_main->m_luaFile);

	return 0;
}

CInt ReadFloatVariableFromFile(lua_State* L)
{
	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nReadFloatVariableFromFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CFloat result;
	fread(&result, sizeof(CFloat), 1, g_main->m_luaFile);

	lua_pushnumber(L, result);

	return 1;
}

CInt WriteFloatVariableToFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for WriteFloatVariableToFile()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nWriteFloatVariableToFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CFloat result = (CFloat)lua_tonumber(L, 1);
	fwrite(&result, sizeof(CFloat), 1, g_main->m_luaFile);

	return 0;
}

CInt ReadIntVariableFromFile(lua_State* L)
{
	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nReadIntVariableFromFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CInt result;
	fread(&result, sizeof(CInt), 1, g_main->m_luaFile);

	lua_pushinteger(L, result);

	return 1;
}

CInt WriteIntVariableToFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for WriteIntVariableToFile()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nWriteIntVariableToFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CInt result = (CFloat)lua_tointeger(L, 1);
	fwrite(&result, sizeof(CInt), 1, g_main->m_luaFile);

	return 0;
}

CInt ReadStringVariableFromFile(lua_State* L)
{
	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nReadStringVariableFromFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CChar string[MAX_URI_SIZE];
	fread(string, sizeof(CChar), MAX_URI_SIZE, g_main->m_luaFile);

	lua_pushstring(L, string);

	return 1;
}

CInt WriteStringVariableToFile(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc < 1)
	{
		//PrintInfo("\nPlease specify 1 argument for WriteStringVariableToFile()", COLOR_RED);
		return 0;
	}

	if (g_main->m_luaFile == NULL)
	{
		//PrintInfo("\nWriteStringVariableToFile() Error: Please open a file first!", COLOR_RED);
		return 0;
	}

	CChar string[MAX_URI_SIZE];
	Cpy(string, lua_tostring(L, 1));
	fwrite(string, sizeof(CChar), MAX_URI_SIZE, g_main->m_luaFile);

	return 0;
}


CBool CMain::firstIdle = CTrue;
CChar CMain::currentIdleName[MAX_NAME_SIZE];

CMain::CMain()
{
	m_showHelpInfo = CFalse;
	m_showShadowMap = CFalse;
	m_timerCounter = 0;
	m_totalElapsedTime = 0;
	m_lockInput = CFalse;
	m_lockDefaultFreeCamera = CFalse;
	m_lockEscape = CFalse;
	m_loadScene = CFalse;
	m_soundSystem = NULL;
	m_renderQuery = CFalse;
	Cpy(g_currentZipFileName, "\n");
	m_characterRotationTransition = CFalse;
	m_previousCharacterRotation = 0.0f;
	m_idleCounter = 0.0f;
	m_previousRightButtonDown = CFalse;
	m_previousLeftButtonDown = CFalse;
	m_cursorIcon = CNew(CIcon);
	Cpy(m_previousCharacterAnimationType, "\n");
	m_menuCursorImg = NULL;

	#ifdef _DEBUG
		m_publishDebug = CTrue;

	#else
		m_publishDebug = CFalse;
	#endif
	
	m_exitGame = CFalse;
	m_mousePosition.x = (CFloat)g_width / 2.f;
	m_mousePosition.y = (CFloat)g_height / 2.f;

	m_fSelectionDistance = -1.f;
	m_bSelectionDistance = CFalse;

	m_forceSpeed = 0.0f;
	m_forceDirection.x = 0.0;
	m_forceDirection.y = 0.0;
	m_forceDirection.z = 0.0;
	m_forceDecreaseValue = 0.0f;
	m_pushTransparentGeometry = CFalse;
	m_dx = m_dy = m_prev_dx = m_prev_dy = 0;
	m_padding = 0;
	m_renderVideo = CFalse;

	m_showMenuCursor = CFalse;
	m_pauseMainCharacterAnimations = CFalse;
	m_pauseAllAnimationsOfPrefabInstances = CFalse;
	m_pausePhysics = CFalse;
	m_pauseAllWaterAnimations = CFalse;
    m_pauseAllUpdateEvents = CFalse;
	m_pauseAllSounds = CFalse;
	m_renderVideoEnabled = CTrue;
	m_fixedTiming = CFalse;

	m_luaFile = NULL;
}

CMain::~CMain()
{
	m_dof.DeleteFBOs();
	g_octree->ResetState();
	distance_vector.clear();
	sorted_prefabs.clear();

	CDelete(m_cursorIcon);
	DeleteMenuCursorTexture();
}

void CMain::RemoveSelectedScene(CChar* szBuffer, CChar* sceneId)
{
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CBool condition = CFalse;
		if (sceneId)
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
		else
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
		if (condition)
		{
			//remove unshared images
			for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
			{
				CGeometry* m_geo = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;
				if (m_geo)
				{
					for (CUInt k = 0; k < m_geo->m_groups.size(); k++)
					{
						for (CUInt l = 0; l < 4; l++)
						{
							CImage* currentImage = NULL;
							if (l == 0 && m_geo->m_groups[k]->m_hasDiffuse && m_geo->m_groups[k]->m_diffuseImg)
							{
								currentImage = m_geo->m_groups[k]->m_diffuseImg;
							}
							else if (l == 1 && m_geo->m_groups[k]->m_hasNormalMap && m_geo->m_groups[k]->m_normalMapImg)
							{
								currentImage = m_geo->m_groups[k]->m_normalMapImg;
							}
							else if (l == 2 && m_geo->m_groups[k]->m_hasGlossMap && m_geo->m_groups[k]->m_glossMapImg)
							{
								currentImage = m_geo->m_groups[k]->m_glossMapImg;
							}
							else if (l == 3 && m_geo->m_groups[k]->m_hasDirtMap && m_geo->m_groups[k]->m_dirtMapImg)
							{
								currentImage = m_geo->m_groups[k]->m_dirtMapImg;
							}
							if (currentImage)
							{
								//see if other scenes use this texture
								CBool foundTarget = CFalse;
								for (CUInt m = 0; m < g_scene.size(); m++)
								{
									CBool condition = CFalse;
									condition = Cmp(g_scene[m]->GetName(), g_scene[i]->GetName()) && Cmp(g_scene[m]->GetDocURI(), g_scene[i]->GetDocURI());

									if (!condition)
									{
										for (CUInt n = 0; n < g_scene[m]->m_instanceGeometries.size(); n++)
										{
											CGeometry* m_geo = g_scene[m]->m_instanceGeometries[n]->m_abstractGeometry;
											for (CUInt o = 0; o < m_geo->m_groups.size(); o++)
											{
												if (m_geo->m_groups[o]->m_hasDiffuse && Cmp(GetAfterPath(currentImage->GetFileName()), GetAfterPath(m_geo->m_groups[o]->m_diffuseImg->GetFileName())))
												{
													foundTarget = CTrue;
													break;
												}
												else if (m_geo->m_groups[o]->m_hasNormalMap && Cmp(GetAfterPath(currentImage->GetFileName()), GetAfterPath(m_geo->m_groups[o]->m_normalMapImg->GetFileName())))
												{
													foundTarget = CTrue;
													break;
												}
												else if (m_geo->m_groups[o]->m_hasGlossMap && Cmp(GetAfterPath(currentImage->GetFileName()), GetAfterPath(m_geo->m_groups[o]->m_glossMapImg->GetFileName())))
												{
													foundTarget = CTrue;
													break;
												}
												else if (m_geo->m_groups[o]->m_hasDirtMap && Cmp(GetAfterPath(currentImage->GetFileName()), GetAfterPath(m_geo->m_groups[o]->m_dirtMapImg->GetFileName())))
												{
													foundTarget = CTrue;
													break;
												}
											}
											if (foundTarget)
												break;
										}
									}
									if (foundTarget)
										break;
								}
								if (!foundTarget)
								{
									for (CUInt p = 0; p < g_images.size(); p++)
									{
										if (Cmp(GetAfterPath(g_images[p]->GetFileName()), GetAfterPath(currentImage->GetFileName())))
										{
											CDelete(currentImage);
											g_images.erase(g_images.begin() + p);
											break;
										}
									}
								} //if (!foundTarget )
							} //if( currentImage )
						} //for( CUInt l = 0; l < 6; l++ )
					} //for groups
				}
			} //for other scenes
			//save functions///////////////////////////////////////////////////
		} //if
	} //for
	//remove unshared geometry
	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CBool condition = CFalse;
		if (sceneId)
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
		else
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
		if (condition)
		{
			//delete unshared geometries
			for (CUInt m = 0; m < g_scene[i]->m_instanceGeometries.size(); m++)
			{
				CGeometry* m_geo1 = g_scene[i]->m_instanceGeometries[m]->m_abstractGeometry;
				if (m_geo1)
				{
					for (CUInt j = 0; j < g_scene.size(); j++)
					{
						CBool condition = CFalse;
						if (sceneId)
							condition = Cmp(g_scene[j]->m_pureFileName, szBuffer) && Cmp(g_scene[j]->GetDocURI(), sceneId);
						else
							condition = Cmp(g_scene[j]->m_pureFileName, szBuffer);
						if (!condition)
						{
							//delete unshared geometries
							for (CUInt k = 0; k < g_scene[j]->m_instanceGeometries.size(); k++)
							{
								CGeometry* m_geo2 = g_scene[j]->m_instanceGeometries[k]->m_abstractGeometry;
								if (m_geo2)
								{
									if (ICmp(m_geo1->GetName(), m_geo2->GetName()) &&
										ICmp(m_geo1->GetDocURI(), m_geo2->GetDocURI()))
									{
										foundTarget = CTrue;
										break;
									}
								}
							}
						}
						if (foundTarget)
							break;
					}
				}
				if (foundTarget)
					break;
			}
		}
		if (foundTarget)
			break;
	}
	if (!foundTarget)
	{
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			CBool condition = CFalse;
			if (sceneId)
				condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
			else
				condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
			if (condition)
			{
				//delete unshared geometries
				for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
				{
					CGeometry* m_geo1 = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;

					for (CUInt l = 0; l < g_geometries.size(); l++)
					{
						if (ICmp(g_geometries[l]->GetName(), m_geo1->GetName()) &&
							ICmp(g_geometries[l]->GetDocURI(), m_geo1->GetDocURI()))
						{

							CDelete(g_geometries[l]);
							g_geometries.erase(g_geometries.begin() + l);
							break;
						}
					}
				}
				break;
			}
		}
	}

	//remove selected scene
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CBool condition = CFalse;
		if (sceneId)
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
		else
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
		if (condition)
		{
			//delete the scene

			CDelete(g_scene[i]);
			//delete the vector that holds the scene
			g_scene.erase(g_scene.begin() + i);
			break;
		}
	}
}

CBool CMain::Init()
{
	//initialize lua/////////////////
	glShadeModel(GL_SMOOTH);										// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);							// Black Background
	glEnable(GL_TEXTURE_2D);							
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); 
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
	GLfloat globalAmbientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbientLight );
	glEnable( GL_NORMALIZE );

	CFog fog;
	fog.SetColor( g_fogProperties.m_fogColor );
	fog.SetDensity( g_fogProperties.m_fogDensity );
	//SetTimer( 1, 0 , 0 );
	glDrawBuffer( GL_BACK );
	// I use this variable to load this data once. We may destroy and create the window multiple times,
	// But we don't need to load and unload some data for multiple times
	//initialize audio
	// Position of the listener.
	m_listenerPos[0] = m_listenerPos[1] = m_listenerPos[2] = 0.0f;
	// Velocity of the listener.
	m_listenerVel[0] = m_listenerVel[1] = m_listenerVel[2] =  0.0f;
	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	m_listenerOri[0] = 0.0f;
	m_listenerOri[1] = 0.0f;
	m_listenerOri[2] = 1.0f;
	m_listenerOri[3] = 0.0f;
	m_listenerOri[4] = 1.0f;
	m_listenerOri[5] = 0.0f;

	g_soundSystem = m_soundSystem = CNew(COpenALSystem);
	m_soundSystem->Init();

	alDistanceModel( AL_INVERSE_DISTANCE );

	g_input.AcquireAll();
	if( !g_input.Initialize( g_window.m_windowGL.hWnd, g_instance, true, IS_USEKEYBOARD | IS_USEMOUSE ) )
	{
		MessageBox( NULL, _T("CMain::Init():Couldn't initialize Input Engine"), _T("VandaEngine Error"), MB_OK );
		return false;
	}

	g_timer = CNew( CTimer );
	if( !g_timer->Init() )
	{
		//Report an error message
		MessageBox( NULL, _T("Couldn't initialize the timer"), _T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return CFalse; 
	}
	g_nx = CNew( CNovodex );
	if (!g_nx->initNx(10.0, 5.0, 10.0, g_physXProperties.m_fCapsuleRadius, g_physXProperties.m_fCapsuleHeight, g_physXProperties.m_fCharacterSlopeLimit, g_physXProperties.m_fCharacterSkinWidth, g_physXProperties.m_fCharacterStepOffset))
	{
		MessageBox( NULL, _T("Couldn't initialize physX"),_T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return false;
	}
	g_font = CNew( CFont );
	if( !g_font->Init( _T("Arial"), 13 ) )
	{
		MessageBox( NULL, _T("Couldn't initialize the font"),_T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return false;
	}

	g_render.Init();
	if (!g_render.m_shaderAvailable)
	{
		MessageBoxA(NULL, "Couldn't initialize shaders", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	g_camera = new CUpdateCamera();

	if (!g_render.SupportForFBOs() || !g_render.SupportForVBOs())
	{
		MessageBoxA(NULL, "Your implementation doesn't support FBO or VBO.\nUpdating the driver of your graphics card may solve the issue", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ilInit();
	iluInit();
	if(GLEW_ARB_color_buffer_float)
	{
		glClampColorARB( GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE );
		glClampColorARB( GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE );
	}

	g_render.m_useBloom = CFalse;
	//bloom effect requires NPOT textures, FBO and shaders
	if( GLEW_ARB_texture_non_power_of_two && g_render.UsingFBOs() && g_options.m_enableFBO && g_render.UsingShader() && g_options.m_enableShader )
	{
		g_bloom = CNew( CBloom );
		g_bloom->CreateEmptyTexture( g_width, g_height, GL_RGB, GL_RGB );
		g_render.m_useBloom = CTrue;
	}

	m_dynamicShadowMap = CNew( CShadowMap );
	g_dynamicShadowMap = m_dynamicShadowMap;
	if( m_dynamicShadowMap->Init() )
		g_render.m_useDynamicShadowMap = CTrue;
	else
	{
		g_render.m_useDynamicShadowMap = CFalse;
	}

	if( !g_useOldRenderingStyle )
		InitFBOs( GL_RGBA, GL_RGBA );

	if( g_render.UsingShader() && g_options.m_enableShader )
	{
		g_render.m_useWaterReflection = CTrue;
	}
	else
	{
		g_render.m_useWaterReflection = CFalse;
	}
	g_octree = CNew( COctree );

	m_padding = g_window.GetPadding();

	return CTrue;
}

CVoid CMain::Release()
{
	glDeleteTextures(eGBUFFER_NUM_TEXTURES, &m_textureTarget[0] );								

	glDeleteFramebuffersEXT( 1, &m_mFboID );
	glDeleteFramebuffersEXT( 1, &m_fboID );
	CDelete(g_mainCharacter);

	//remove gui
	for (CUInt i = 0; i < g_guiButtons.size(); i++)
	{
		CDelete(g_guiButtons[i]);
	}
	g_guiButtons.clear();

	for (CUInt i = 0; i < g_guiImages.size(); i++)
	{
		CDelete(g_guiImages[i]);
	}
	g_guiImages.clear();

	for (CUInt i = 0; i < g_guiTexts.size(); i++)
	{
		CDelete(g_guiTexts[i]);
	}
	g_guiTexts.clear();

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CDelete(g_guis[i]);
	}
	g_guis.clear();

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CDelete(g_triggers[i]);
	}
	g_triggers.clear();

	for( std::vector<CScene*>::iterator it = g_scene.begin(); it != g_scene.end(); it++ )
	{
		CDelete( *it );
	}

	//clear prefabs
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		CDelete(g_prefab[i]);
	}
	g_prefab.clear();

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CDelete(g_instancePrefab[i]);
	}
	g_instancePrefab.clear();

	//clear prefab resource
	for (CUInt i = 0; i < g_resourcePrefab.size(); i++)
	{
		CDelete(g_resourcePrefab[i]);
	}
	g_resourcePrefab.clear();

	//Clear the vctor objects
	g_scene.clear();
	g_render.Destroy();

	CDelete( m_dynamicShadowMap );
	CDelete( g_skyDome );
	CDelete(g_terrain);
	CDelete(g_VSceneScript);

	for( std::vector<CInstanceLight*>::iterator it = g_engineLights.begin(); it != g_engineLights.end(); it++ )
	{
		CDelete( (*it)->m_abstractLight );
		CDelete( *it );
	}
	g_engineLights.clear();

	for (std::vector<CInstanceCamera*>::iterator it = g_engineCameraInstances.begin(); it != g_engineCameraInstances.end(); it++)
	{
		CDelete((*it)->m_abstractCamera);
		CDelete(*it);
	}
	g_engineCameraInstances.clear();

	for( std::vector<CWater*>::iterator it = g_engineWaters.begin(); it != g_engineWaters.end(); it++ )
	{
		CDelete( *it );
	}
	g_engineWaters.clear();

	//delete 3d sounds
	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CDelete(g_engine3DSounds[i]);
	}
	g_engine3DSounds.clear();

	//delete ambient sounds
	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CDelete(g_engineAmbientSounds[i]);
	}
	g_engineAmbientSounds.clear();

	//delete videos
	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CDelete(g_engineVideos[i]);
	}
	g_engineVideos.clear();

	for( std::vector<CImage*>::iterator it = g_images.begin(); it != g_images.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_images.size() > 0 )
		g_images.clear();

	CDelete( g_bloom );

	for( std::vector<CImage*>::iterator it = g_waterImages.begin(); it != g_waterImages.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_waterImages.size() > 0 )
		g_waterImages.clear();

	//Delete Resource Files
	for (CUInt j = 0; j < g_resourceFiles.size(); j++)
		CDelete(g_resourceFiles[j]);
	g_resourceFiles.clear();

	if (g_main && g_main->GetCursorIcon())
		g_main->GetCursorIcon()->SetVisible(CFalse);

	//Release Audio
	CDelete(m_soundSystem );

	g_input.Shutdown();
	CDelete( g_timer );
	if( g_nx )
	{
		g_nx->releaseNx();
		CDelete( g_nx )
	}
	if( g_font )
	{
		g_font->Release();
		CDelete( g_font );
	}
	CDelete( g_camera );
}

CVoid CMain::ResetTimer()
{
	elapsedTime = g_timer->GetElapsedSeconds( CTrue ); //start from the beginning
	g_timer->GetFps( 1, CTrue );
	g_elapsedTime = elapsedTime;
	m_timerCounter  = 0;
	m_totalElapsedTime = 0;
}

CVoid CMain::CalculateDistnces(CBool force)
{
	//calculate the distance of instance prefabs from camera
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!force)
		{
			if (!m_calculateDistance && !g_instancePrefab[i]->GetIsAnimated() && !g_instancePrefab[i]->GetIsControlledByPhysX()) continue;
			if (!g_instancePrefab[i]->GetVisible2()) continue;
		}
		//CScene* scene = NULL;

		//CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		//for (CUInt j = 0; j < 3; j++)
		//{
		//	if (prefab && prefab->GetHasLod(j))
		//	{
		//		scene = g_instancePrefab[i]->GetScene(j);
		//		if (scene)
		//			scene->CalculateDistances();
		//	}
		//}
		g_instancePrefab[i]->CalculateDistance();
	}
	if (m_calculateDistance || force)
	{
		for (CUInt i = 0; i < g_engineLights.size(); i++)
		{
			g_engineLights[i]->CalculateDistance();
		}
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			g_engineWaters[i]->CalculateDistance();
		}
	}
}

CBool CMain::Render()
{
	if (g_clickedOpen)
		return CTrue;
	g_numVerts = 0; //debug info

	if (m_fixedTiming)
		elapsedTime = 1.0f / 60.0f;
	else
		elapsedTime = g_timer->GetElapsedSeconds();

	//if (elapsedTime > (1.0f / 60.0f))
	//{
	//	CFloat elapsed;
	//	CInt steps = 1;

	//	steps = (CInt)roundf(elapsedTime / (1.0f / 60.0f));

	//	elapsed = (CFloat)steps * (1.0f / 60.0f);
	//	gPhysXscene->setTiming(1.0f / 60.0f, steps, NX_TIMESTEP_FIXED);

	//	elapsedTime = elapsed;
	//}
	//else
	//{
	gPhysXscene->setTiming(1.0f / 60.0f, 8, NX_TIMESTEP_FIXED);
	//}

	g_elapsedTime = elapsedTime;
	m_totalElapsedTime += elapsedTime;

	//manage videos here
	if (m_renderVideoEnabled)
	{
		CBool renderVideo = CFalse;

		if (g_engineVideos.size() == 0)
			m_renderVideo = CFalse;

		for (CUInt i = 0; i < g_engineVideos.size(); i++)
		{
			if (g_engineVideos[i]->Render() && g_engineVideos[i]->GetHasScript())
				g_engineVideos[i]->UpdateScript();

			//force exit
			if (!m_loadScene && g_input.KeyDown(DIK_F5))
				m_exitGame = CTrue;

			if (GetExitGame()) return CFalse; //exit game 

			g_engineVideos[i]->Update();

			CBool result = g_engineVideos[i]->Render();
			if (result)
			{
				m_soundSystem->SetListenerGain(g_currentVSceneProperties.m_globalSoundVolume);

				if (!m_loadScene)
					ProcessInputs();

				if (!m_loadScene && g_engineVideos[i]->GetExitWithEscKey() && g_input.KeyDown(DIK_ESCAPE))
				{
					m_lockEscape = CTrue;
					g_engineVideos[i]->SetPlay(CFalse);
				}

				renderVideo = CTrue;
			}
		}
		if (renderVideo)
		{
			m_renderVideo = CTrue;
			glViewport(0, m_padding, g_width, g_height);

			DrawGUI();

			glFlush();
			return CTrue;
		}
		else
		{
			m_renderVideo = CFalse;
		}

		if (!m_loadScene && g_input.KeyUp(DIK_ESCAPE))
			m_lockEscape = CFalse;
	}
	///////////////////////

	if (!g_currentVSceneProperties.m_pauseGame)
	{
		UpdatePrefabInstanceTransformations();

		//use multithreading for animationsr
		/*std::thread t1(&CMain::*/UpdateAnimations();/*, this, false)*/;
		//t1.join();
	}

	if (g_databaseVariables.m_insertAndShowTerrain && g_terrain->GetTerrain())
	{
		g_terrain->GetTerrain()->ManagePhysics();
	}

	if (!m_loadScene)
	{
		if (!ProcessInputs())
			return CFalse;
	}

	if (!g_currentVSceneProperties.m_pauseGame && !m_pauseMainCharacterAnimations)
	{
		UpdateCharacterTransformations();
	}

	if (GetExitGame()) return CFalse; //exit game 

	if (!g_currentVSceneProperties.m_pauseGame && !m_pausePhysics)
	{
		UpdateDynamicPhysicsObjects();
	}

	if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_mFboID);
	else if (!g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_fboID);

	if (!g_useOldRenderingStyle)
	{
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT };
		glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);
	}
	else
		glDrawBuffer(GL_BACK);
	m_cameraTypeOfPreviousFrame = m_cameraType;


	if (g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX)
	{
		CInstanceCamera* instanceCamera = g_render.GetDefaultInstanceCamera();
		if (instanceCamera)
		{
			m_cameraType = eCAMERA_DEFAULT_FREE_NO_PHYSX;
			SetInstanceCamera(instanceCamera, g_width / 2.f, g_height / 2.f, instanceCamera->m_abstractCamera->GetAngle(), g_cameraProperties.m_freePerspectiveNCP, g_cameraProperties.m_freePerspectiveFCP);
			m_lockInput = CFalse;
		}
		else
		{
			g_currentCameraType = eCAMERA_PHYSX;
		}
	}
	CInstanceCamera* instanceCamera = g_render.GetActiveInstanceCamera();
	if (g_currentCameraType == eCAMERA_COLLADA)
	{
		if (instanceCamera)
		{
			CBool enableColladaCamera = CTrue;
			if (instanceCamera->IsTimerEnabled())
			{
				if (!instanceCamera->IncreaseElapsedSeconds(elapsedTime))
					enableColladaCamera = CFalse;
			}
			if (enableColladaCamera)
			{
				m_cameraType = eCAMERA_COLLADA;

				m_lockInput = CTrue;
				CFloat m_fov, m_zNear, m_zFar;

				if (g_cameraProperties.m_readDAECameraFOVFromFile)
					m_fov = instanceCamera->m_abstractCamera->GetYFov();
				else
					m_fov = g_cameraProperties.m_daeCameraFOV;

				if (g_cameraProperties.m_readDAECameraNCPFromFile)
					m_zNear = instanceCamera->m_abstractCamera->GetZNear();
				else
					m_zNear = g_cameraProperties.m_daeCameraNCP;

				if (g_cameraProperties.m_readDAECameraFCPFromFile)
					m_zFar = instanceCamera->m_abstractCamera->GetZFar();
				else
					m_zFar = g_cameraProperties.m_daeCameraFCP;


				SetInstanceCamera(instanceCamera, g_width / 2.f, g_height / 2.f, m_fov, m_zNear, m_zFar);
			}
			else
			{
				g_currentCameraType = eCAMERA_PHYSX;
				g_render.SetActiveInstanceCamera(NULL);
			}
		}
		else
		{
			g_currentCameraType = eCAMERA_PHYSX;
		}
	}
	if (g_currentCameraType == eCAMERA_ENGINE)
	{
		CBool foundTarget = CFalse;
		for (CUInt c = 0; c < g_engineCameraInstances.size(); c++)
		{
			if (g_engineCameraInstances[c]->IsActive())
			{
				if (g_engineCameraInstances[c]->IsTimerEnabled())
				{
					if (g_engineCameraInstances[c]->IncreaseElapsedSeconds(elapsedTime))
					{
						SetInstanceCamera(g_engineCameraInstances[c], g_width / 2.f, g_height / 2.f, g_engineCameraInstances[c]->m_abstractCamera->GetAngle(), g_engineCameraInstances[c]->GetNCP(), g_engineCameraInstances[c]->GetFCP());
						g_render.SetActiveInstanceCamera(g_engineCameraInstances[c]);
						foundTarget = CTrue;
						break;
					}
				}
				else
				{
					SetInstanceCamera(g_engineCameraInstances[c], g_width / 2.f, g_height / 2.f, g_engineCameraInstances[c]->m_abstractCamera->GetAngle(), g_engineCameraInstances[c]->GetNCP(), g_engineCameraInstances[c]->GetFCP());
					g_render.SetActiveInstanceCamera(g_engineCameraInstances[c]);
					foundTarget = CTrue;
					break;
				}
				if (!foundTarget)
				{
					g_engineCameraInstances[c]->SetActive(CFalse);
					g_render.SetActiveInstanceCamera(NULL);
					g_currentCameraType = eCAMERA_PHYSX;
					break;
				}
			}
		}
		if (!foundTarget)
			g_currentCameraType = eCAMERA_PHYSX;
	}
	if (g_currentCameraType == eCAMERA_PHYSX)
	{
		m_cameraType = eCAMERA_PHYSX;

		m_lockInput = CFalse;
		g_camera->m_cameraManager->SetPerspective(g_camera->m_cameraManager->GetAngle(), g_width, g_height, g_cameraProperties.m_playModePerspectiveNCP, g_cameraProperties.m_playModePerspectiveFCP);
		if (g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader)
		{
			//shadow
			glGetFloatv(GL_PROJECTION_MATRIX, cam_proj);
		}

		//previous position of character controller
		g_camera->m_perspectiveCharacterPosOfPreviousFrame = g_camera->m_perspectiveCharacterPos;
		g_nx->GetCameraAndCharacterPositions(g_camera->m_perspectiveCameraPitch,
			g_camera->m_perspectiveCameraYaw,
			g_camera->m_perspectiveCameraPos,
			g_camera->m_perspectiveCharacterPos,
			g_camera->m_perspectiveCameraDir);

		g_camera->m_cameraManager->UpdateCameraPosDir(g_camera->m_perspectiveCameraPos,
			g_camera->m_perspectiveCharacterPos,
			g_camera->m_perspectiveCameraDir,
			g_camera->m_perspectiveCurrentCameraTilt);
	}

	//if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	//{
		//shadow
		// store the inverse of the resulting modelview matrix for the shadow computation
	glGetFloatv(GL_MODELVIEW_MATRIX, cam_modelview);
	g_camera->m_cameraManager->GetInverseMatrix(cam_inverse_modelview);
	//////////////
//}
	g_camera->m_cameraManager->UpdateFrustum();
	g_octree->ResetOctreeGeoCount();
	g_octree->Render(CTrue);
	g_octree->ResetOctreeGeoCount();

	m_calculateDistance = CFalse;
	if (m_cameraType != m_cameraTypeOfPreviousFrame)
	{
		m_calculateDistance = CTrue;
	}
	//calculate distance between geometries and main camera
	else if (!(g_camera->m_perspectiveCharacterPosOfPreviousFrame.x - g_camera->m_perspectiveCharacterPos.x == 0.0f &&
		g_camera->m_perspectiveCharacterPosOfPreviousFrame.y - g_camera->m_perspectiveCharacterPos.y == 0.0f &&
		g_camera->m_perspectiveCharacterPosOfPreviousFrame.z - g_camera->m_perspectiveCharacterPos.z == 0.0f))
	{
		m_calculateDistance = CTrue;
	}
	else if (instanceCamera)
	{
		if (!(g_camera->m_perspectiveCameraPosOfPreviousFrame.x - g_camera->m_perspectiveCameraPos.x == 0.0f &&
			g_camera->m_perspectiveCameraPosOfPreviousFrame.y - g_camera->m_perspectiveCameraPos.y == 0.0f &&
			g_camera->m_perspectiveCameraPosOfPreviousFrame.z - g_camera->m_perspectiveCameraPos.z == 0.0f))
		{
			m_calculateDistance = CTrue;
		}
	}

	UpdatePrefabInstanceBB();

	CalculateDistnces();

	//shadow
	const float bias[16] = { 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f };

	if (!g_useOldRenderingStyle && g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader)
	{
		float light_dir[4] = { g_defaultDirectionalLight.x, g_defaultDirectionalLight.y, g_defaultDirectionalLight.z, 1.0f };
		if (!instanceCamera)
		{
			cam_pos[0] = g_camera->m_perspectiveCameraPos.x;
			cam_pos[1] = g_camera->m_perspectiveCameraPos.y;
			cam_pos[2] = g_camera->m_perspectiveCameraPos.z;

			cam_dir[0] = g_camera->m_perspectiveCharacterPos.x - cam_pos[0];
			cam_dir[1] = g_camera->m_perspectiveCharacterPos.y + (g_physXProperties.m_fCapsuleHeight / 2.f) + g_camera->m_perspectiveCurrentCameraTilt - cam_pos[1];
			cam_dir[2] = g_camera->m_perspectiveCharacterPos.z - cam_pos[2];
		}
		if (!Cmp(g_shadowProperties.m_directionalLightName, "\n") && g_shadowProperties.m_enable && g_render.UsingShadowShader())
			m_dynamicShadowMap->MakeShadowMap(cam_pos, cam_dir, light_dir);
	}

	//audio
	m_ambientSourcePos[0] = m_listenerPos[0] = g_camera->m_perspectiveCameraPos.x;
	m_ambientSourcePos[1] = m_listenerPos[1] = g_camera->m_perspectiveCameraPos.y;
	m_ambientSourcePos[2] = m_listenerPos[2] = g_camera->m_perspectiveCameraPos.z;

	m_listenerOri[0] = -g_camera->m_perspectiveCameraDir.x;
	m_listenerOri[1] = g_camera->m_perspectiveCameraDir.y;
	m_listenerOri[2] = -g_camera->m_perspectiveCameraDir.z;

	m_soundSystem->SetListenerPosition(m_listenerPos);
	//m_soundSystem->SetListenerVelocity( m_listenerVel );
	m_soundSystem->SetListenerOrientation(m_listenerOri);
	m_soundSystem->SetListenerGain(g_currentVSceneProperties.m_globalSoundVolume);

	//set ambient sound
	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
		g_engineAmbientSounds[i]->GetSoundSource()->SetSoundPosition(m_ambientSourcePos);

	g_render.m_useShader = CTrue;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushAttrib(GL_LIGHTING_BIT);
	//create all the waters textures here
	if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	{
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			if (g_engineWaters[i]->GetOutsideFrustom()) continue;

			if (CTrue/*g_currentCameraType == eCAMERA_PHYSX*/)
			{
				if (g_engineWaters[i]->GetVisible() && g_engineWaters[i]->GetQueryVisible())
				{
					g_engineWaters[i]->CreateReflectionTexture(g_waterTextureSize);
					//g_engineWaters[i]->CreateRefractionDepthTexture(g_waterTextureSize );
				}
			}
			else
			{
				g_engineWaters[i]->CreateReflectionTexture(g_waterTextureSize);
			}
		}
	}


	if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_mFboID);
	else if (!g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_fboID);

	if (g_useOldRenderingStyle || !g_options.m_enableFBO)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, g_width, g_height);// resets the viewport to new dimensions.
	if (g_window.m_windowGL.multiSampling &&/* g_options.m_numSamples*/g_window.m_numSamples)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);

	//render sky
	if (g_databaseVariables.m_insertAndShowSky)
	{
		g_skyDome->RenderDome();
	}

	//render waters here
	CVec3f cameraPos(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z);

	if (g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader)
	{
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_dynamicShadowMap->depth_tex_ar);
		if (g_shadowProperties.m_shadowType > 3 || g_shadowProperties.m_shadowType == 0 /* 0 is debug state*/)
			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		else
			glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		//glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		for (int i = m_dynamicShadowMap->cur_num_splits; i < MAX_SPLITS; i++)
			far_bound[i] = 0;

		// for every active split
		for (int i = 0; i < m_dynamicShadowMap->cur_num_splits; i++)
		{
			float light_m[16];

			// f[i].fard is originally in eye space - tell's us how far we can see.
			// Here we compute it in camera homogeneous coordinates. Basically, we calculate
			// cam_proj * (0, 0, f[i].fard, 1)^t and then normalize to [0; 1]
			far_bound[i] = 0.5f * (-m_dynamicShadowMap->f[i].fard * cam_proj[10] + cam_proj[14]) / m_dynamicShadowMap->f[i].fard + 0.5f;

			// compute a matrix that transforms from camera eye space to light clip space
			// and pass it to the shader through the OpenGL texture matrices, since we
			// don't use them now
			glActiveTexture(GL_TEXTURE0 + (GLenum)i);
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(bias);
			glMultMatrixf(m_dynamicShadowMap->shad_cpm[i]);
			// multiply the light's (bias*crop*proj*modelview) by the inverse camera modelview
			// so that we can transform a pixel as seen from the camera
			glMultMatrixf(cam_inverse_modelview);

			// compute a normal matrix for the same thing (to transform the normals)
			// Basically, N = ((L)^-1)^-t
			glGetFloatv(GL_TEXTURE_MATRIX, light_m);
			matrix4<float> nm;
			nm.set_value(light_m);
			nm = inverse(nm);
			nm = transpose(nm);

			glActiveTexture(GL_TEXTURE0 + (GLenum)(i + 4));
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(nm._array);
		}
		glMatrixMode(GL_MODELVIEW);
	}

	g_numLights = 0;
	if (!g_render.UsingShader())
		glEnable(GL_LIGHTING);

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_instancePrefab[i]->InitScript();
		if (g_instancePrefab[i]->GetUpdateEvent())
			g_instancePrefab[i]->UpdateScript();
	}

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		if (g_engineWaters[i]->GetHasScript() && g_engineWaters[i]->GetUpdateEvent())
			g_engineWaters[i]->UpdateScript();
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		if (g_engineLights[i]->m_abstractLight->GetHasScript() && g_engineLights[i]->GetUpdateEvent())
			g_engineLights[i]->m_abstractLight->UpdateScript();
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		if (g_engineCameraInstances[i]->GetHasScript() && g_engineCameraInstances[i]->GetUpdateEvent())
			g_engineCameraInstances[i]->UpdateScript();
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		if (g_engineAmbientSounds[i]->GetHasScript() && g_engineAmbientSounds[i]->GetUpdateEvent())
			g_engineAmbientSounds[i]->UpdateScript();
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		if (g_engine3DSounds[i]->GetHasScript() && g_engine3DSounds[i]->GetUpdateEvent())
			g_engine3DSounds[i]->UpdateScript();
	}

	if (g_VSceneScript && g_VSceneScript->GetUpdateEvent())
		g_VSceneScript->UpdateScript();

	if (g_skyDome && g_skyDome->GetUpdateEvent())
		g_skyDome->UpdateScript();

	if (g_terrain && g_terrain->GetUpdateEvent())
		g_terrain->UpdateScript();

	if (g_mainCharacter && g_mainCharacter->GetUpdateEvent())
		g_mainCharacter->UpdateScript();

	DeletePrefabInstancesAtRuntime();

	if (g_updateOctree && g_scene.size() > 0)
	{
		//Force Update
		//for( CUInt i = 0 ; i < g_scene.size(); i++ )
		//{
		//	g_render.SetScene( g_scene[i] );
		//	g_render.GetScene()->m_update = CTrue;
		//}

		//update octree
		Render3DModels(CTrue, NULL);
		g_octree->Init();
		g_octree->GetWorldDimensions();
		g_octree->SetName("octree_root");
		g_octree->SetLevel(0);
		g_octree->AttachGeometriesToNode();
		g_octree->AttachLightsToGeometries();
		g_updateOctree = CFalse;

		//update shadow max radius
		g_maxInstancePrefabRadius = -1.f;
		for (CUInt j = 0; j < g_instancePrefab.size(); j++)
		{
			if (g_instancePrefab[j]->GetScene(0) && g_instancePrefab[j]->GetScene(0)->CastShadow())
			{
				if (g_instancePrefab[j]->GetRadius() > g_maxInstancePrefabRadius)
					g_maxInstancePrefabRadius = g_instancePrefab[j]->GetRadius();
			}
		}

	}

	RenderQueries();

	ManageLODs();

	if (g_databaseVariables.m_insertAndShowTerrain)
		RenderTerrain();

	Draw3DObjects();

	g_currentInstancePrefab = NULL;


	if (g_databaseVariables.m_showPrefabBoundingBox)
	{
		COpenGLUtility g_glUtil;
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			CVector lineColor(1.0, 1.0, 1.0);
			CVector min(g_instancePrefab[i]->GetMinAABB().x, g_instancePrefab[i]->GetMinAABB().y, g_instancePrefab[i]->GetMinAABB().z);
			CVector max(g_instancePrefab[i]->GetMaxAABB().x, g_instancePrefab[i]->GetMaxAABB().y, g_instancePrefab[i]->GetMaxAABB().z);
			g_glUtil.DrawCWBoxWithLines(min, max, lineColor);
		}
	}

	if (g_databaseVariables.m_showTerrainBoundingBox)
	{
		if (g_databaseVariables.m_insertAndShowTerrain)
			if (g_terrain->GetTerrain())
				g_terrain->GetTerrain()->DrawBoundingBox();
	}


	if (g_databaseVariables.m_showOctree)
	{
		g_octree->Render(CFalse, CFalse);
	}

	if (!g_useOldRenderingStyle &&  g_window.m_windowGL.multiSampling && /*g_options.m_numSamples*/g_window.m_numSamples && g_options.m_enableFBO)
		g_render.BindForWriting( m_mFboID );
	else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
		g_render.BindForWriting( m_fboID );

	g_currentInstanceLight = NULL;
	g_totalLights = g_numLights;

	if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		//reset matrice
		for( CUInt i = 0; i < 8; i++ )
		{
			glActiveTexture(GL_TEXTURE0 + i );
			glMatrixMode( GL_TEXTURE );
			glLoadIdentity();
		}
		glActiveTexture(GL_TEXTURE0);
	}

	glDisable( GL_LIGHT0 );

	for( CInt i = 0; i <g_numLights ; i++ )
	{
		glDisable( GL_LIGHT0 + i );
	}
	glDisable( GL_LIGHTING ); //just for per vertex lighting
	glMatrixMode( GL_MODELVIEW );

	glPopAttrib();

	
	for( CUInt i = 0 ; i < g_scene.size(); i++ )
	{
		g_render.SetScene( g_scene[i] );
		if( !g_render.GetScene()->m_isTrigger )
		{
			for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			{
				g_scene[i]->m_instanceGeometries[j]->m_renderCount = 0;
			}
		}
		if( g_render.GetScene()->m_hasAnimation )
		{
			g_render.GetScene()->m_updateAnimation = CTrue;
		}
	}

	if( g_physXProperties.m_bDebugMode )
		g_nx->debugMode = CTrue;
	else
		g_nx->debugMode = CFalse;

	if( g_nx->debugMode )
		g_nx->debugRenderer();

	if( g_databaseVariables.m_showPerspectiveGrids )
		DrawGrid();
	if( g_databaseVariables.m_showSoundPositions )
	{
		CFloat pos[3];
		for( CUInt i = 0; i < g_engine3DSounds.size(); i++ )
		{
			 g_engine3DSounds[i]->GetPosition(pos);
			 g_render.DrawCube( 150.0f, pos );
		}
	}
	glFlush();
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
	{
		g_render.BindForReading(m_mFboID);
		glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
		g_render.BindForWriting( m_fboID);
		glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	if( !g_useOldRenderingStyle && g_options.m_enableFBO)
	{
		if( g_dofProperties.m_enable )
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			/* Second pass: downsampling */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[0]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
			glUseProgram(g_render.m_dofProgram[0]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[0], "Tex0"), 0 );
			glViewport(0, 0, g_width/2, g_height/2);
			m_dof.DrawQuad();

			/* Third pass: Gaussian filtering along the X axis */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[0]);
			glUseProgram(g_render.m_dofProgram[1]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[1], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[1], "Width"), g_width );
			m_dof.DrawQuad();

			/* Fourth pass: Gaussian filtering along the Y axis */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[2]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(g_render.m_dofProgram[2]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[2], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[2], "Height"), g_height );
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[1]);
			m_dof.DrawQuad();

			/* Fifth pass: final compositing */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[3]);
			////glDrawBuffer(GL_BACK);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[2]);
			glUseProgram(g_render.m_dofProgram[3]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[3], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[3], "Tex1"), 1 );

			glViewport(0, 0, g_width, g_height);

			glBegin(GL_QUADS);
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
				glVertex2d(-1, -1);
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
				glVertex2d(1, -1);
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
				glVertex2d(1, 1);
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
				glVertex2d(-1, 1);
			glEnd();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

		glViewport(0, m_padding, g_width, g_height);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glUseProgram(0);
		glDrawBuffer( GL_BACK );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		glPushAttrib( GL_ENABLE_BIT );
		glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW); glPushMatrix();	glLoadIdentity();

		glDisable( GL_LIGHTING );

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if( g_dofProperties.m_enable )
			glBindTexture( GL_TEXTURE_2D, m_dof.m_texid[3] );
		else
			glBindTexture( GL_TEXTURE_2D, m_textureTarget[0] );

		glBegin(GL_QUADS); 
		glTexCoord2d(0,	0);	glVertex2d(0, 0);
		glTexCoord2d(1, 0);	glVertex2d(1, 0);
		glTexCoord2d(1, 1);	glVertex2d(1, 1);
		glTexCoord2d(0, 1);	glVertex2d(0, 1);
		glEnd();

		if( g_render.m_useBloom && g_bloomProperties.m_enable)
		{
			//if( g_dofProperties.m_enable )
			//	g_bloom->CreateRuntimeTexture( g_width, g_height, m_dof.m_texid[3]  );
			//else
			glPushAttrib( GL_CURRENT_BIT );
			g_bloom->CreateRuntimeTexture( g_width, g_height, m_textureTarget[0]  );

			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glColor4f( g_bloomProperties.m_bloomColor[0], g_bloomProperties.m_bloomColor[1], g_bloomProperties.m_bloomColor[2], g_bloomProperties.m_bloomIntensity );
			glBindTexture( GL_TEXTURE_2D, g_bloom->m_bloomTexture );
			glBegin(GL_QUADS); 
			glTexCoord2d(0,	0);	glVertex2d(0, 0); 
			glTexCoord2d(1, 0);	glVertex2d(1, 0);
			glTexCoord2d(1, 1);	glVertex2d(1, 1);
			glTexCoord2d(0, 1);	glVertex2d(0, 1);
			glEnd();
			glDisable( GL_BLEND );
			glPopAttrib();
		}

		if(m_showShadowMap)
 			m_dynamicShadowMap->ShowDepthTex();

		DrawGUI();
		glPopAttrib();
		glMatrixMode(GL_PROJECTION); glPopMatrix();
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
	else
	{
		DrawGUI();
	}

	ResetData();

	return CTrue;
}

CVoid CMain::ResetData()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetRenderForQuery(CFalse);
		g_instancePrefab[i]->SetRealTimeSceneCheckIsInFrustom(CTrue);
		for (CUInt j = 0; j < 4; j++)
		{
			CBool condition = CFalse;
			if (j < 3)
			{
				if (g_instancePrefab[i]->GetPrefab() && g_instancePrefab[i]->GetPrefab()->GetHasLod(j))
					condition = CTrue;
			}
			else
			{
				if (g_instancePrefab[i]->GetPrefab() && g_instancePrefab[i]->GetHasCollider())
					condition = CTrue;
			}

			if (condition)
			{
				CScene* scene = g_instancePrefab[i]->GetScene(j);
				if (!scene) continue;
				scene->ResetSkinData();
			}
		}
	}

	RemoveTransparentGeometries();
	g_currentInstancePrefab = NULL;

}

CVoid CMain::DrawGrid(CVoid)
{
	glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	//Draw the grid here
	glLineWidth( 1.0f );
	glColor3f( 0.7f, 0.7f, 0.7f );
	glBegin( GL_LINES );
	for( CInt index = -100; index <= 100; index += 1 )
	{
		glVertex3f( -100.0f, 0.0f, (CFloat)index );
		glVertex3f(  100.0f, 0.0f, (CFloat)index );

		glVertex3f( (CFloat)index, 0.0f, -100.0f );
		glVertex3f( (CFloat)index, 0.0f, 100.0f );
	}
	glEnd();
	glLineWidth( 3.0f );
	glBegin( GL_LINES );
	//Draw the axes here
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 1.0f );

	glEnd();

	glPopAttrib();
}

CBool CMain::GetJumpCurrentEndDuration(CFloat& duration)
{
	if (g_mainCharacter && g_mainCharacter->GetCameraType() == ePHYSX_CAMERA_FIRST_PERSON /*&& g_currentCameraType == eCAMERA_PHYSX*/) return CFalse;

	if (!g_mainCharacter)
		return CFalse;
	g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();
	if (!g_currentInstancePrefab)
		return CFalse;
	if (!g_currentInstancePrefab->GetVisible()) return CFalse;

	std::vector<std::string> jumpName = g_mainCharacter->GetJumpName();

	CScene* scene = NULL;

	CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
	for (CUInt j = 0; j < 3; j++)
	{
		if (prefab && prefab->GetHasLod(j) && g_currentInstancePrefab->GetSceneVisible(j))
		{
			scene = g_currentInstancePrefab->GetScene(j);
			break;
		}
	}
	if (!scene) return CFalse;

	CBool foundTarget = CFalse;
	CUInt index = -1;

	for (CInt k = 0; k < scene->GetNumClips(); k++)
	{
		CChar animationName[MAX_NAME_SIZE];
		Cpy(animationName, scene->m_animationClips[k]->GetName());
		StringToUpper(animationName);
		StringToUpper((CChar*)jumpName[0].c_str());
		if (Cmp(jumpName[0].c_str(), animationName))
		{
			index = k;
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		return CFalse;
	}
	duration = scene->m_animationClips[index]->GetEnd() - scene->m_animationClips[index]->GetCurrentAnimationTime();
	return CTrue;
}

CBool CMain::ManageCharacterBlends(CChar* animationType, CChar* IdleAnimationName)
{
	if (Cmp(m_previousCharacterAnimationType, animationType))
		return CFalse;
	else
		Cpy(m_previousCharacterAnimationType, animationType);

	if (g_mainCharacter && g_mainCharacter->GetCameraType() == ePHYSX_CAMERA_FIRST_PERSON) return CFalse;

	if (!g_mainCharacter)
		return CFalse;
	g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();
	if (!g_currentInstancePrefab)
		return CFalse;
	if (!g_currentInstancePrefab->GetVisible()) return CFalse;

	std::vector<std::string> idleName;
	std::vector<std::string> walkName;
	std::vector<std::string> jumpName;
	std::vector<std::string> runName;

	for (CUInt i = 0; i < g_mainCharacter->GetIdleName().size(); i++)
		idleName.push_back(g_mainCharacter->GetIdleName()[i]);

	for (CUInt i = 0; i < g_mainCharacter->GetWalkName().size(); i++)
		walkName.push_back(g_mainCharacter->GetWalkName()[i]);

	for (CUInt i = 0; i < g_mainCharacter->GetJumpName().size(); i++)
		jumpName.push_back(g_mainCharacter->GetJumpName()[i]);

	for (CUInt i = 0; i < g_mainCharacter->GetRunName().size(); i++)
		runName.push_back(g_mainCharacter->GetRunName()[i]);

	CScene* scene = NULL;

	CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
	for (CUInt j = 0; j < 3; j++)
	{
		if (prefab && prefab->GetHasLod(j) && g_currentInstancePrefab->GetSceneVisible(j))
		{
			scene = g_currentInstancePrefab->GetScene(j);
			break;
		}
	}
	if (!scene) return CFalse;

	CBool foundTarget = CFalse;
	CUInt index = -1;

	if (Cmp(animationType, "idle") || Cmp(animationType, "more_idle"))
	{
		for (CInt i = 0; i < scene->GetNumClips(); i++)
		{
			if (IdleAnimationName)
			{
				if (Cmp(IdleAnimationName, scene->m_animationClips[i]->GetName()))
				{
					index = i;
					foundTarget = CTrue;
					break;
				}
			}
			else if (Cmp(idleName[0].c_str(), scene->m_animationClips[i]->GetName()))
			{
				index = i;
				foundTarget = CTrue;
				break;
			}
		}
		if (!foundTarget)
		{
			return CFalse;
		}

		if (index != -1)
		{
			scene->SetClipIndex(index);
			scene->ExecuteCyclicAnimation(index, 1.0f, g_characterBlendingProperties.m_idleDelayIn);

			for (CInt i = 0; i < scene->GetNumClips(); i++)
			{
				if (i != index)
				{
					//make sure we clear animations
					scene->RemoveCyclicAnimation(i, g_characterBlendingProperties.m_idleDelayIn);
				}
			}
		}
	}
	else if (Cmp(animationType, "walk"))
	{
		for (CInt i = 0; i < scene->GetNumClips(); i++)
		{
			if (Cmp(walkName[0].c_str(), scene->m_animationClips[i]->GetName()))
			{
				index = i;
				foundTarget = CTrue;
				break;
			}
		}
		if (!foundTarget)
		{
			return CFalse;
		}

		if (index != -1)
		{
			scene->SetClipIndex(index);
			scene->ExecuteCyclicAnimation(index, 1.0f, g_characterBlendingProperties.m_walkDelayIn);

			for (CInt i = 0; i < scene->GetNumClips(); i++)
			{
				if (i != index)
				{
					//make sure we clear animations
					scene->RemoveCyclicAnimation(i, g_characterBlendingProperties.m_walkDelayIn);
				}
			}
		}
	}
	else if (Cmp(animationType, "run"))
	{
		for (CInt i = 0; i < scene->GetNumClips(); i++)
		{
			if (Cmp(runName[0].c_str(), scene->m_animationClips[i]->GetName()))
			{
				index = i;
				foundTarget = CTrue;
				break;
			}
		}
		if (!foundTarget)
		{
			return CFalse;
		}

		if (index != -1)
		{
			scene->SetClipIndex(index);
			scene->ExecuteCyclicAnimation(index, 1.0f, g_characterBlendingProperties.m_runDelayIn);

			for (CInt i = 0; i < scene->GetNumClips(); i++)
			{
				if (i != index)
				{
					//make sure we clear animations
					scene->RemoveCyclicAnimation(i, g_characterBlendingProperties.m_runDelayIn);
				}
			}
		}

	}
	else if (Cmp(animationType, "jump"))
	{
		for (CInt i = 0; i < scene->GetNumClips(); i++)
		{
			if (Cmp(jumpName[0].c_str(), scene->m_animationClips[i]->GetName()))
			{
				index = i;
				foundTarget = CTrue;
				break;
			}
		}
		if (!foundTarget)
		{
			return CFalse;
		}

		if (index != -1)
		{
			scene->SetClipIndex(index);
			scene->ExecuteNonCyclicAnimation(index, g_characterBlendingProperties.m_jumpDelayIn, g_characterBlendingProperties.m_jumpDelayOut, 1.0, CFalse);

			for (CInt i = 0; i < scene->GetNumClips(); i++)
			{
				if (i != index)
				{
					//make sure we clear animations
					scene->RemoveCyclicAnimation(i, g_characterBlendingProperties.m_jumpDelayIn);
				}
			}
		}

	}
	else if (Cmp(animationType, ""))
	//reset
	{
		for (CInt i = 0; i < scene->GetNumClips(); i++)
		{
			//make sure we clear animations
			scene->RemoveCyclicAnimation(i, 0.0f);
			scene->m_animationClips[i]->SetCurrentTime(0.0f);
		}
	}
	idleName.clear();
	walkName.clear();
	jumpName.clear();
	runName.clear();

	return CTrue;

}

CBool CMain::IsJumping(CBool &isInList)
{
	if (!g_mainCharacter)
		return CFalse;
	g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();
	if (!g_currentInstancePrefab)
		return CFalse;

	if (g_mainCharacter->GetCameraType() != ePHYSX_CAMERA_FIRST_PERSON)
		if (!g_currentInstancePrefab->GetVisible())
			return CFalse;

	std::vector<std::string> jumpName = g_mainCharacter->GetJumpName();

	CScene* scene = NULL;

	CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
	for (CUInt j = 0; j < 3; j++)
	{
		if (prefab && prefab->GetHasLod(j) && g_currentInstancePrefab->GetSceneVisible(j))
		{
			scene = g_currentInstancePrefab->GetScene(j);
			break;
		}
	}
	if (!scene) return CFalse;
	CBool foundTarget = CFalse;
	for (CUInt j = 0; j < jumpName.size(); j++)
	{
		for (CUInt i = 0; i < scene->m_executeNonCyclicAnimationList.size(); i++)
		{
			if (Cmp(scene->m_executeNonCyclicAnimationList[i].c_str(), jumpName[j].c_str()))
			{
				foundTarget = CTrue;
				break;
			}
		}
	}
	if (foundTarget)
		isInList = CTrue;
	else
		isInList = CFalse;
	return CTrue;
}

CBool CMain::ProcessInputs()
{
	if (m_loadScene) return CTrue;

	static CUInt cameraIndex = 0;
	static CBool pKeyDown = CFalse;
	static CBool oKeyDown = CFalse;
	CBool inputUpdated = CFalse;

	GetMouseMovement();

	if (m_publishDebug)
	{
		g_input.Update();
		inputUpdated = CTrue;
		if (g_input.KeyDown(DIK_P) && !pKeyDown && g_importedCameraInstances.size() > 0)
		{
			pKeyDown = CTrue;
			cameraIndex++;
			if (cameraIndex > g_importedCameraInstances.size() - 1)
				cameraIndex = 0;
			g_render.SetActiveInstanceCamera(g_importedCameraInstances[cameraIndex]);
			g_currentCameraType = eCAMERA_COLLADA;
		}
		if (g_input.KeyUp(DIK_P))
			pKeyDown = CFalse;

		if (g_input.KeyDown(DIK_O) && !oKeyDown && g_importedCameraInstances.size() > 0)
		{
			oKeyDown = CTrue;
			if (cameraIndex > 0)
				cameraIndex--;
			else
				cameraIndex = g_importedCameraInstances.size() - 1;

			g_render.SetActiveInstanceCamera(g_importedCameraInstances[cameraIndex]);
			g_currentCameraType = eCAMERA_COLLADA;

		}
		if (g_input.KeyUp(DIK_O))
			oKeyDown = CFalse;

		if (g_input.KeyDown(DIK_I))
		{
			g_render.SetActiveInstanceCamera(NULL);
			g_currentCameraType = eCAMERA_PHYSX;
		}
		if (g_input.KeyDown(DIK_U))
		{
			g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());
			g_currentCameraType = eCAMERA_DEFAULT_FREE_NO_PHYSX;
		}

		static CBool f1KeyDown = CFalse;
		if (g_input.KeyDown(DIK_F1) && !f1KeyDown)
		{
			f1KeyDown = CTrue;
			m_showHelpInfo = !m_showHelpInfo;
		}
		if (g_input.KeyUp(DIK_F1))
			f1KeyDown = CFalse;

		static CBool mKeyDown = CFalse;
		if (g_input.KeyDown(DIK_M) && !mKeyDown)
		{
			mKeyDown = CTrue;
			m_showShadowMap = !m_showShadowMap;
		}
		if (g_input.KeyUp(DIK_M))
			mKeyDown = CFalse;

		static CBool gKeyDown = CFalse;
		static CBool vKeyDown = CFalse;

		//physics
		if (g_input.KeyDown(DIK_V) && !vKeyDown)
		{
			vKeyDown = CTrue;
			g_physXProperties.m_bDebugMode = !g_physXProperties.m_bDebugMode;
		}
		if (g_input.KeyUp(DIK_V))
			vKeyDown = CFalse;

		//Prefab bounding box
		static CBool bKeyDown = CFalse;

		if (g_input.KeyDown(DIK_B) && !bKeyDown)
		{
			bKeyDown = CTrue;
			g_databaseVariables.m_showPrefabBoundingBox = !g_databaseVariables.m_showPrefabBoundingBox;
		}
		if (g_input.KeyUp(DIK_B))
			bKeyDown = CFalse;

		//Terrain bounding box
		static CBool tKeyDown = CFalse;

		if (g_input.KeyDown(DIK_T) && !tKeyDown)
		{
			tKeyDown = CTrue;
			g_databaseVariables.m_showTerrainBoundingBox = !g_databaseVariables.m_showTerrainBoundingBox;
		}
		if (g_input.KeyUp(DIK_T))
			tKeyDown = CFalse;

		//octree
		static CBool nKeyDown = CFalse;

		if (g_input.KeyDown(DIK_N) && !nKeyDown)
		{
			nKeyDown = CTrue;
			g_databaseVariables.m_showOctree = !g_databaseVariables.m_showOctree;
		}
		if (g_input.KeyUp(DIK_N))
			nKeyDown = CFalse;

		//gravity
		if (g_input.KeyDown(DIK_G) && !gKeyDown)
		{
			gKeyDown = CTrue;
			g_physXProperties.m_bApplyGravity = !g_physXProperties.m_bApplyGravity;
			if (g_physXProperties.m_bApplyGravity)
				g_nx->m_defaultGravity = NxVec3(g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ);
			else
				g_nx->m_defaultGravity = NxVec3(0.0f);

		}
		if (g_input.KeyUp(DIK_G))
			gKeyDown = CFalse;

	}

	//force exit
	if (g_input.KeyDown(DIK_F5))
		m_exitGame = CTrue;

	if (m_lockInput)
	{
		ApplyForce(IDLE, elapsedTime);
		EnableIdleAnimations();
		return CTrue;
	}

	if (!inputUpdated)
		g_input.Update();

	//static CBool bEscapeDown = CFalse;

	//if (g_input.KeyDown(DIK_ESCAPE) && !bEscapeDown)
	//{
	//	bEscapeDown = CTrue;
	//	return CFalse;
	//}
	//if (g_input.KeyUp(DIK_ESCAPE))
	//	bEscapeDown = CFalse;

	if (m_showMenuCursor)
	{
		//deprecate
		//CInt dx, dy;
		//g_input.GetMouseMovement(dx, dy);
		if (g_main->m_prevLoadScene)
		{
			m_dx = m_dy = 0;
			g_main->m_prevLoadScene = CFalse;
		}
		if (g_input.ButtonDown(0))
		{
			if (!m_previousLeftButtonDown)
			{
				CUInt index = GetSelectedGUI();
				for (CUInt i = 0; i < g_guis.size(); i++)
				{
					for (CUInt k = 0; k < g_guis[i]->m_guiButtons.size(); k++)
					{
						if (g_guis[i]->m_guiButtons[k]->GetIndex() == index)
						{
							if (g_guis[i]->m_guiButtons[k]->GetCurrentImageType() != eBUTTON_IMAGE_DISABLE)
							{
								if (g_guis[i]->m_guiButtons[k]->GetHasLeftClickImage())
								{
									g_timer->GetElapsedSeconds2(CTrue);
									g_guis[i]->m_guiButtons[k]->SetCurrentImageType(eBUTTON_IMAGE_LEFT_CLICK);
								}
							}
						}
					}
				}
			}
			m_previousLeftButtonDown = CTrue;
		}
		else
		{
			m_previousLeftButtonDown = CFalse;
		}

		if (g_input.ButtonDown(1))
		{
			if (!m_previousRightButtonDown)
			{
				CUInt index = GetSelectedGUI();
				for (CUInt i = 0; i < g_guis.size(); i++)
				{
					for (CUInt k = 0; k < g_guis[i]->m_guiButtons.size(); k++)
					{
						if (g_guis[i]->m_guiButtons[k]->GetIndex() == index)
						{
							if (g_guis[i]->m_guiButtons[k]->GetCurrentImageType() != eBUTTON_IMAGE_DISABLE)
							{
								if (g_guis[i]->m_guiButtons[k]->GetHasRightClickImage())
								{
									g_timer->GetElapsedSeconds2(CTrue);
									g_guis[i]->m_guiButtons[k]->SetCurrentImageType(eBUTTON_IMAGE_RIGHT_CLICK);
								}
							}
						}
					}
				}
			}
			m_previousRightButtonDown = CTrue;
		}
		else
		{
			m_previousRightButtonDown = CFalse;
		}
		if (m_dx != 0 || m_dy != 0)
		{
			//mouse moved
			m_selectedGUIIndex = GetSelectedGUI();
			for (CUInt i = 0; i < g_guis.size(); i++)
			{
				for (CUInt k = 0; k < g_guis[i]->m_guiButtons.size(); k++)
				{
					if (g_guis[i]->m_guiButtons[k]->GetIndex() == m_selectedGUIIndex)
					{
						if (g_guis[i]->m_guiButtons[k]->GetCurrentImageType() == eBUTTON_IMAGE_MAIN && g_guis[i]->m_guiButtons[k]->GetHasHoverImage())
						{
							if (g_guis[i]->m_guiButtons[k]->GetCurrentImageType() != eBUTTON_IMAGE_HOVER)
							{
								if (m_previuosSelectedGUIIndex != m_selectedGUIIndex)
									g_guis[i]->m_guiButtons[k]->OnSelectMouseEnterScript();
							}

							g_guis[i]->m_guiButtons[k]->SetCurrentImageType(eBUTTON_IMAGE_HOVER);
						}
					}
					else
					{
						g_guis[i]->m_guiButtons[k]->SetCurrentImageType(eBUTTON_IMAGE_MAIN);
					}
				}
			}
			m_previuosSelectedGUIIndex = m_selectedGUIIndex;
		}

		g_main->m_mousePosition.x += m_dx;
		if (g_main->m_mousePosition.x < 1.0f) g_main->m_mousePosition.x = 1.0f;
		else if (g_main->m_mousePosition.x > g_width) g_main->m_mousePosition.x = g_width;

		g_main->m_mousePosition.y += m_dy;
		if (g_main->m_mousePosition.y < 0.0f) g_main->m_mousePosition.y = 0.0f;
		else if (g_main->m_mousePosition.y > g_height) g_main->m_mousePosition.y = g_height;

	}

	if (g_currentVSceneProperties.m_lockCharacterController)
	{
		ApplyForce(IDLE, elapsedTime);
		EnableIdleAnimations();
		return CTrue;
	}

	if( g_currentCameraType == eCAMERA_PHYSX )
	{
		CBool forceApplied = false;
		CBool move = CFalse;
		if( (g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP )) && (g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT )) )
		{
			ApplyForce( MOVE_FRONT_RIGHT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else	if( (g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP )) && (g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT )) )
		{
			ApplyForce( MOVE_FRONT_LEFT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else if( (g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN )) && (g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT )) )
		{
			ApplyForce( MOVE_BACK_RIGHT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else if( (g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN )) && (g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT )) )
		{
			ApplyForce( MOVE_BACK_LEFT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}

		else if( g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP ) )
		{
			ApplyForce( MOVE_FRONT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else if( g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN ) )
		{
			ApplyForce( MOVE_BACK, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else if( g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT ) )
		{
			ApplyForce( MOVE_LEFT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		else if( g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT ) )
		{
			ApplyForce( MOVE_RIGHT, elapsedTime );
			forceApplied = true;
			if (!g_nx->gJump)
				move = CTrue;
			if (move)
			{
				if (g_input.KeyDown(DIK_LSHIFT))
				{
					ManageCharacterBlends("run");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
					}
				}
				else
				{
					ManageCharacterBlends("walk");
					if (g_mainCharacter)
					{
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}
				}
			}

		}
		//else if( g_input.KeyDown( DIK_Q ) )
		//{
		//	ApplyForce( MOVE_UP, elapsedTime );
		//	forceApplied = true;
		//}
		//else if( g_input.KeyDown( DIK_E ) )
		//{
		//	ApplyForce( MOVE_DOWN, elapsedTime );
		//	forceApplied = true;
		//}

		if (!forceApplied || !move)
		{
			if (!forceApplied)
				ApplyForce(IDLE, elapsedTime);
			if (!move && g_mainCharacter)
			{
				CBool idle = CFalse;
				if (!g_nx->gJump)
					idle = CTrue;
				CBool jumping = CFalse;
				if (IsJumping(jumping) && jumping)
				{
					CFloat duration = 1.0f;
					if (GetJumpCurrentEndDuration(duration) && duration <= g_characterBlendingProperties.m_jumpDelayOut)
						idle = CTrue;
				}
				//Fix Bug: if we don't have any active animation:
				CInt index = 0;
				for (CUInt i = 0; i < g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips.size(); i++)
				{
					if (g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips[i]->GetAnimationStatus() == eANIM_NONE)
					{
						index++;
					}
				}
				if (index == g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips.size())
				{
					Cpy(m_previousCharacterAnimationType, "");
					idle = CTrue;
				}

				if (idle)
				{
					if (!Cmp(m_previousCharacterAnimationType, "idle") && !Cmp(m_previousCharacterAnimationType, "more_idle"))
					{
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
					}

					std::vector<std::string> idleName = g_mainCharacter->GetIdleName();

					if (!Cmp(m_previousCharacterAnimationType, "idle") && !Cmp(m_previousCharacterAnimationType, "more_idle"))
					{
						if (ManageCharacterBlends("idle"))
						{
							Cpy(currentIdleName, (CChar*)idleName[0].c_str());
							m_idleCounter = 0.0f;
							firstIdle = CTrue;
						}
					}
					if (firstIdle)
						m_idleCounter += elapsedTime;
					else if (idleName.size() > 1)
					{
						g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();
						CScene* scene = NULL;
						CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
						for (CUInt j = 0; j < 3; j++)
						{
							if (prefab && prefab->GetHasLod(j) && g_currentInstancePrefab->GetSceneVisible(j))
							{
								scene = g_currentInstancePrefab->GetScene(j);
								break;
							}
						}

						for (CInt j = 0; j < scene->GetNumClips(); j++)
						{
							if (Cmp(currentIdleName, scene->m_animationClips[j]->GetName()))
							{
								if (m_idleCounter < scene->m_animationClips[j]->GetEnd())
								{
									m_idleCounter += elapsedTime;
								}
								else
								{
									if (ManageCharacterBlends("idle"))
									{
										Cpy(currentIdleName, (CChar*)idleName[0].c_str());
										m_idleCounter = 0.0f;
										firstIdle = CTrue;
									}
								}
							}
						}
					}
					if (idleName.size() > 1 && m_idleCounter >= g_mainCharacter->GetIdleDelayForRandomPlay() && firstIdle)
					{
						CInt index = rand() % idleName.size();
						if (index == 0 && idleName.size() > 1)
							index = 1;
						if (ManageCharacterBlends("more_idle", (CChar*)idleName[index].c_str()))
						{
							Cpy(currentIdleName, (CChar*)idleName[index].c_str());
							m_idleCounter = 0.0f;
							firstIdle = CFalse;
						}
					}
				}
			}
		}
		if (forceApplied)
			m_idleCounter = 0.0f;

		if (/*g_physXProperties.m_bApplyGravity &&*/ g_physXProperties.m_bJumping)
			if( g_input.KeyDown( DIK_SPACE ) )
				if (!g_nx->gJump)
				{
					CBool jumping = CFalse;
					if (IsJumping(jumping) && !jumping)
					{
						ManageCharacterBlends("jump");
						g_nx->StartJump(g_physXProperties.m_fJumpPower);
						g_main->m_soundSystem->PlayALSound(*(g_mainCharacter->m_jumpSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
						g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));


					}
					else if (!g_mainCharacter)
					{
						g_nx->StartJump(g_physXProperties.m_fJumpPower);
					}
				}

		//deprecate
		//CInt dx, dy;
		//g_input.GetMouseMovement( dx, dy );
		if (g_main->m_prevLoadScene)
		{
			m_dx = m_dy = 0;
			g_main->m_prevLoadScene = CFalse;
		}

		if (m_dx != 0 || m_dy != 0)
			m_calculateDistance = CTrue;

		if (g_input.ButtonDown(0))
		{
			m_previousLeftButtonDown = CTrue;
		}
		else
		{
			m_previousLeftButtonDown = CFalse;
		}

		if (g_input.ButtonDown(1))
		{
			m_previousRightButtonDown = CTrue;
		}
		else
		{
			m_previousRightButtonDown = CFalse;
		}

		g_main->m_mousePosition.x += m_dx;
		if (g_main->m_mousePosition.x < 1.0f) g_main->m_mousePosition.x = 1.0f;
		else if (g_main->m_mousePosition.x > g_width) g_main->m_mousePosition.x = g_width;

		g_main->m_mousePosition.y += m_dy;
		if (g_main->m_mousePosition.y < 0.0f) g_main->m_mousePosition.y = 0.0f;
		else if (g_main->m_mousePosition.y > g_height) g_main->m_mousePosition.y = g_height;

		if(m_dx > 0 )
		{
			g_camera->m_perspectiveCameraYaw -= (CFloat)m_dx * 0.002f;
		}
		else if(m_dx < 0 )
		{
			g_camera->m_perspectiveCameraYaw -= (CFloat)m_dx * 0.002f;
		}
		//if( g_input.ButtonDown(1) )
		//{
		//	if( dy > 0 ) 
		//		g_camera->m_cameraManager->SetZoomOut( elapsedTime * 50.0f );
		//	else if( dy < 0 )
		//		g_camera->m_cameraManager->SetZoomIn( elapsedTime * 50.0f );
		//}
		//else
		//{
			if(m_dy > 0 )
			{
				g_camera->m_perspectiveCurrentCameraTilt -= (CFloat)m_dy * 0.002f;
				if (g_camera->m_perspectiveCurrentCameraTilt > g_camera->m_perspectiveCameraMaxTilt)
					g_camera->m_perspectiveCurrentCameraTilt = g_camera->m_perspectiveCameraMaxTilt;
				else if (g_camera->m_perspectiveCurrentCameraTilt < g_camera->m_perspectiveCameraMinTilt)
					g_camera->m_perspectiveCurrentCameraTilt = g_camera->m_perspectiveCameraMinTilt;

			}
			else if(m_dy < 0 )
			{
				g_camera->m_perspectiveCurrentCameraTilt -= (CFloat)m_dy * 0.002f;
				if (g_camera->m_perspectiveCurrentCameraTilt > g_camera->m_perspectiveCameraMaxTilt)
					g_camera->m_perspectiveCurrentCameraTilt = g_camera->m_perspectiveCameraMaxTilt;
				else if (g_camera->m_perspectiveCurrentCameraTilt < g_camera->m_perspectiveCameraMinTilt)
					g_camera->m_perspectiveCurrentCameraTilt = g_camera->m_perspectiveCameraMinTilt;

			}

		//}
	}
	else if( g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX && !m_lockDefaultFreeCamera)
	{
		ApplyForce(IDLE, elapsedTime);

		//deprecate
		//CInt dx, dy;
		//g_input.GetMouseMovement( dx, dy );
		if (g_main->m_prevLoadScene)
		{
			m_dx = m_dy = 0;
			g_main->m_prevLoadScene = CFalse;
		}

		if (m_dx != 0 || m_dy != 0)
			m_calculateDistance = CTrue;

		if( g_input.ButtonDown(1) )
		{
			if (m_dy > 0)
				g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetZoomOut(elapsedTime * 50.0f);
			else if (m_dy < 0)
				g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetZoomIn(elapsedTime * 50.0f);
		}
		else
		{
			g_render.GetDefaultInstanceCamera()->SetPanAndTilt( -(CFloat)m_dx * 0.2f, -(CFloat)m_dy * 0.2f );
		}

		//default speed
		if ((g_input.KeyDown(DIK_LCONTROL) || g_input.KeyDown(DIK_RCONTROL)) &&
			(g_input.KeyDown(DIK_LSHIFT) || g_input.KeyDown(DIK_RSHIFT)))
		{
			if (fabs(g_camera->m_cameraSpeed - DEFAULT_CAMERA_SPEED) > EPSILON)
			{
				g_camera->m_cameraSpeed = DEFAULT_CAMERA_SPEED;
			}
		}
		//default zoom
		if ((g_input.KeyDown(DIK_LCONTROL) || g_input.KeyDown(DIK_RCONTROL)) &&
			g_input.KeyDown(DIK_Z))
		{
			if (fabs(g_render.GetDefaultInstanceCamera()->m_abstractCamera->GetAngle() - DEFAULT_CAMERA_ANGLE) > EPSILON)
			{
				g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetAngle(DEFAULT_CAMERA_ANGLE);
			}
		}

		if (g_input.KeyDown( DIK_S ))
		{
			//Decrease Speed?
			if (g_input.KeyDown(DIK_LCONTROL) || g_input.KeyDown(DIK_RCONTROL))
			{
				g_camera->m_cameraSpeed -= 5.0f;
				if (g_camera->m_cameraSpeed < EPSILON)
				{
					g_camera->m_cameraSpeed += 5.0f;
				}
			}
			else
			{
				// UI code to move the camera closer
				g_render.GetDefaultInstanceCamera()->MoveTransform(elapsedTime * g_camera->m_cameraSpeed, 0.0f, 0.0f);
			}
		}

		if (g_input.KeyDown( DIK_W ))
		{
			if (g_input.KeyDown(DIK_LCONTROL) || g_input.KeyDown(DIK_RCONTROL))
			{
				g_camera->m_cameraSpeed += 5.0f;
			}
			else
			{
				// UI code to move the camera farther away
				g_render.GetDefaultInstanceCamera()->MoveTransform(-elapsedTime* g_camera->m_cameraSpeed, 0.0f, 0.0f);
			}
		}

		if (g_input.KeyDown( DIK_Q ))
		{
			// UI code to move the camera farther up
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, 0.0f, elapsedTime* g_camera->m_cameraSpeed);
		}

		if (g_input.KeyDown( DIK_E ))
		{
			// UI code to move the camera farther down
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, 0.0f, -elapsedTime* g_camera->m_cameraSpeed);
		}

		if (g_input.KeyDown( DIK_D ))
		{
			// UI code to move the camera farther right
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, -elapsedTime* g_camera->m_cameraSpeed, 0.0f);
		}

		if (g_input.KeyDown( DIK_A ))
		{
			// UI code to move the camera farther left
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, elapsedTime* g_camera->m_cameraSpeed, 0.0f);
		}
	}
	else if (g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX || g_currentCameraType == eCAMERA_COLLADA || g_currentCameraType == eCAMERA_ENGINE)
	{
		ApplyForce(IDLE, elapsedTime);
		EnableIdleAnimations(); //for now I enable idle animations for main character everytime PhysX camera is not active
	}
	return CTrue;
}

CVoid CMain::EnableIdleAnimations()
{
	if (g_mainCharacter)
	{
		CBool idle = CFalse;
		if (!g_nx->gJump)
			idle = CTrue;
		CBool jumping = CFalse;
		if (IsJumping(jumping) && jumping)
		{
			CFloat duration = 1.0f;
			if (GetJumpCurrentEndDuration(duration) && duration <= g_characterBlendingProperties.m_jumpDelayOut)
				idle = CTrue;
		}
		//Fix Bug: if we don't have any active animation:
		CInt index = 0;
		for (CUInt i = 0; i < g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips.size(); i++)
		{
			if (g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips[i]->GetAnimationStatus() == eANIM_NONE)
			{
				index++;
			}
		}
		if (index == g_mainCharacter->GetInstancePrefab()->GetScene(0)->m_animationClips.size())
		{
			Cpy(m_previousCharacterAnimationType, "");
			idle = CTrue;
		}

		if (idle)
		{
			if (!Cmp(m_previousCharacterAnimationType, "idle") && !Cmp(m_previousCharacterAnimationType, "more_idle"))
			{
				g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_walkSound->GetSoundSource()));
				g_main->m_soundSystem->StopALSound(*(g_mainCharacter->m_runSound->GetSoundSource()));
			}

			std::vector<std::string> idleName = g_mainCharacter->GetIdleName();

			if (!Cmp(m_previousCharacterAnimationType, "idle") && !Cmp(m_previousCharacterAnimationType, "more_idle"))
			{
				if (ManageCharacterBlends("idle"))
				{
					Cpy(currentIdleName, (CChar*)idleName[0].c_str());
					m_idleCounter = 0.0f;
					firstIdle = CTrue;
				}
			}
			if (firstIdle)
				m_idleCounter += elapsedTime;
			else if (idleName.size() > 1)
			{
				g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();
				CScene* scene = NULL;
				CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
				for (CUInt j = 0; j < 3; j++)
				{
					if (prefab && prefab->GetHasLod(j) && g_currentInstancePrefab->GetSceneVisible(j))
					{
						scene = g_currentInstancePrefab->GetScene(j);
						break;
					}
				}

				for (CInt j = 0; j < scene->GetNumClips(); j++)
				{
					if (Cmp(currentIdleName, scene->m_animationClips[j]->GetName()))
					{
						if (m_idleCounter < scene->m_animationClips[j]->GetEnd())
						{
							m_idleCounter += elapsedTime;
						}
						else
						{
							if (ManageCharacterBlends("idle"))
							{
								Cpy(currentIdleName, (CChar*)idleName[0].c_str());
								m_idleCounter = 0.0f;
								firstIdle = CTrue;
							}
						}
					}
				}
			}
			if (idleName.size() > 1 && m_idleCounter >= g_mainCharacter->GetIdleDelayForRandomPlay() && firstIdle)
			{
				CInt index = rand() % idleName.size();
				if (index == 0 && idleName.size() > 1)
					index = 1;
				if (ManageCharacterBlends("more_idle", (CChar*)idleName[index].c_str()))
				{
					Cpy(currentIdleName, (CChar*)idleName[index].c_str());
					m_idleCounter = 0.0f;
					firstIdle = CFalse;
				}
			}
		}
	}

}

CVoid CMain::ApplyForce( CInt direction, CFloat elapsedTime )
{
	if (g_nx->bPushCharacter)
	{
		if (m_forceSpeed >= 0.0f)
		{
			g_nx->runPhysics(m_forceDirection, m_forceSpeed, direction, elapsedTime);
			m_forceSpeed -= elapsedTime * m_forceDecreaseValue;
		}
		else
		{
			g_nx->bPushCharacter = false;
			m_forceDirection.x = m_forceDirection.y = m_forceDirection.z = 0.0f;
			m_forceSpeed = 0.0f;
			m_forceDecreaseValue = 0.0f;

			g_nx->runPhysics(NxVec3(0.0f, 0.0f, 0.0f), 0.0f, direction, elapsedTime);
		}

	}
	else
		g_nx->runPhysics(NxVec3(0.0f, 0.0f, 0.0f), 0.0f, direction, elapsedTime);
}

CBool CMain::Reset()
{
	g_clickedNew = CTrue;
	g_camera->Reset();
	g_dofProperties.Reset();
	g_fogProperties.Reset();
	g_bloomProperties.Reset();
	g_lightProperties.Reset();
	g_currentVSceneProperties.Reset();
	g_instancePrefabLODPercent.Reset();
	g_prefabProperties.Reset();
	g_cameraProperties.Reset();
	g_characterBlendingProperties.Reset();
	g_shadowProperties.Reset();
	g_physXProperties.Reset();
	g_physXCollisionFlags.Reset();
	g_shadowProperties.m_enable = CTrue;
	g_pathProperties.Reset();
	g_extraTexturesNamingConventions.Reset();
	g_sceneManagerObjectsPerSplit = 15;
	//g_options.Reset();
	g_useGlobalAmbientColor = CFalse;
	g_globalAmbientColor.r = g_globalAmbientColor.g = g_globalAmbientColor.b = 0.5f;
	g_globalAmbientColor.a = 1.0f;

	if (g_mainCharacter)
	{
		g_mainCharacter->Destroy();
		g_mainCharacter->Reset();
		g_mainCharacter = NULL;
	}

	//remove gui
	for (CUInt i = 0; i < g_guiButtons.size(); i++)
	{
		CDelete(g_guiButtons[i]);
	}
	g_guiButtons.clear();

	for (CUInt i = 0; i < g_guiImages.size(); i++)
	{
		CDelete(g_guiImages[i]);
	}
	g_guiImages.clear();

	for (CUInt i = 0; i < g_guiTexts.size(); i++)
	{
		CDelete(g_guiTexts[i]);
	}
	g_guiTexts.clear();

	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		CDelete(g_guis[i]);
	}
	g_guis.clear();

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CDelete(g_triggers[i]);
	}
	g_triggers.clear();

	//clear geometries
	for (CUInt i = 0; i < g_geometries.size(); i++)
		CDelete(g_geometries[i]);
	g_geometries.clear();

	//clear prefabs
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		CDelete(g_prefab[i]);
	}
	g_prefab.clear();

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CDelete(g_instancePrefab[i]);
	}
	g_instancePrefab.clear();

	//clear prefab resource
	for (CUInt i = 0; i < g_resourcePrefab.size(); i++)
	{
		CDelete(g_resourcePrefab[i]);
	}
	g_resourcePrefab.clear();

	g_octree->ResetState();
	g_render.SetScene( NULL );

	if( g_currentCameraType != eCAMERA_PHYSX )
	{
		g_currentCameraType = eCAMERA_PHYSX;
		g_render.SetActiveInstanceCamera(NULL);
		g_main->m_lockInput = CFalse;
	}
	g_main->distance_vector.clear();
	g_main->sorted_prefabs.clear();

	Cpy(g_shadowProperties.m_directionalLightName, "\n" );
	if(gPhysXscene)
	{
		if(!g_clickedOpen)
			ResetPhysX();
	}
	for( std::vector<CWater*>::iterator it = g_engineWaters.begin(); it != g_engineWaters.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_engineWaters.size() > 0 )
		g_engineWaters.clear();

	//delete 3d sounds
	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		CDelete(g_engine3DSounds[i]);
	}
	if (g_engine3DSounds.size() > 0)
		g_engine3DSounds.clear();

	//delete ambient sounds
	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		CDelete(g_engineAmbientSounds[i]);
	}
	g_engineAmbientSounds.clear();

	//delete videos
	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		CDelete(g_engineVideos[i]);
	}
	g_engineVideos.clear();

	//clear the scene
	for (std::vector<CScene*>::iterator it = g_scene.begin(); it != g_scene.end(); it++)
	{
		CDelete(*it);
	}
	//Clear the vctor objects
	if( g_scene.size() > 0 ) 
		g_scene.clear();
	
	//clear all the default engines here
	for( std::vector<CInstanceLight*>::iterator it = g_engineLights.begin(); it != g_engineLights.end(); it++ )
	{
		CDelete( (*it)->m_abstractLight ) ;
		CDelete( *it );
	}
	//Clear the vctor objects
	if( g_engineLights.size() > 0 )
		g_engineLights.clear();

	for (std::vector<CInstanceCamera*>::iterator it = g_engineCameraInstances.begin(); it != g_engineCameraInstances.end(); it++)
	{
		CDelete((*it)->m_abstractCamera);
		CDelete(*it);
	}
	if (g_engineCameraInstances.size() > 0)
		g_engineCameraInstances.clear();

	for( std::vector<CImage*>::iterator it = g_images.begin(); it != g_images.end(); it++ )
	{
		CDelete( *it );
	}

	if( g_images.size() > 0 )
		g_images.clear();

	for( std::vector<CImage*>::iterator it = g_waterImages.begin(); it != g_waterImages.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_waterImages.size() > 0 )
		g_waterImages.clear();

	//Delete Resource Files
	for (CUInt j = 0; j < g_resourceFiles.size(); j++)
		CDelete(g_resourceFiles[j]);
	g_resourceFiles.clear();

	if (g_main && g_main->GetCursorIcon())
		g_main->GetCursorIcon()->SetVisible(CFalse);

	CDelete( g_skyDome );

	CDelete(g_terrain);

	CDelete(g_VSceneScript);

	//Reset name indexes which are used for selection
	g_nameIndex = 1;

	g_selectedName = -1; 		//no object has been selected

	Cpy( g_currentVSceneName, "\n" ); //save functions
	Cpy(g_currentVSceneName, "\n"); //save functions
	Cpy(g_currentPrefabName, "\n");
	Cpy(g_currentPrefabPackagePath, "\n");
	Cpy(g_currentPrefabAndSceneName, "\n");
	Cpy(g_currentInstancePrefabName, "\n");

	g_currentInstancePrefab = NULL;
	g_currentWater = NULL;
	g_currentLight = NULL;
	g_current3DSound = NULL;
	g_currentAmbientSound = NULL;
	g_currentTrigger = NULL;
	g_currentEngineCamera = NULL;
	g_currentVideo = NULL;
	g_currentGUI = NULL;

	m_showMenuCursor = CFalse;
	m_pauseMainCharacterAnimations = CFalse;
	m_pauseAllAnimationsOfPrefabInstances = CFalse;
	m_pausePhysics = CFalse;
	m_pauseAllWaterAnimations = CFalse;
	m_pauseAllUpdateEvents = CFalse;
	m_pauseAllSounds = CFalse;

	g_maxInstancePrefabRadius = -1.0f;
	g_clickedNew = CFalse;


	return CTrue;
}

CBool CMain::InsertPrefab(CPrefab* prefab)
{
	CChar packagePath[MAX_NAME_SIZE];
	CChar prefabPath[MAX_NAME_SIZE];
	CChar prefabAndPackageName[MAX_NAME_SIZE];

	Cpy(prefabAndPackageName, prefab->GetName());

	Cpy(packagePath, "Assets/Packages/");
	Append(packagePath, prefab->GetPackageName());
	Append(packagePath, "/");

	Cpy(prefabPath, packagePath);
	Append(prefabPath, prefab->GetPrefabName());
	Append(prefabPath, "/");
	Append(prefabPath, prefabAndPackageName);
	Append(prefabPath, ".vpf");

	Cpy(g_currentPrefabPackagePath, packagePath);
	Cpy(g_currentPrefabName, prefab->GetPrefabName());

	CPrefab* m_newPrefab = prefab;

	FILE *filePtr = fopen(prefabPath, "rb");
	if (filePtr)
	{

		CChar engineName[MAX_NAME_SIZE];
		fread(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		if (!CmpIn(engineName, "VandaEngine"))
		{
			fclose(filePtr);
			MessageBoxA(NULL, "Invalid VPF file!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}

		CInt engine_version;
		fread(&g_edition, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&engine_version, 1, sizeof(CInt), filePtr);
		if (engine_version > g_version)
		{
			fclose(filePtr);
			MessageBoxA(NULL, "Prefab file has been saved with newer version of Vanda Engine. Please Update!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}
		fread(&g_currentPassword, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		//read engine options
		CShadowProperties shadowProperties;
		fread(&shadowProperties, sizeof(CShadowProperties), 1, filePtr);

		CPhysXProperties physXProperties;
		CPhysXCollisionFlags physXCollisionFlags;
		CDOFProperties dofProperties;
		CFogProperties fogProperties;
		CBloomProperties bloomProperties;
		CLightProperties lightProperties;
		CLODProperties lodProperties;
		CCameraProperties cameraProperties;
		CPathProperties pathProperties;
		CCharacterBlendingProperties characterBlendingProperties;
		fread(&physXProperties, sizeof(CPhysXProperties), 1, filePtr);
		fread(&physXCollisionFlags, sizeof(CPhysXCollisionFlags), 1, filePtr);
		fread(&dofProperties, sizeof(CDOFProperties), 1, filePtr);
		fread(&fogProperties, sizeof(CFogProperties), 1, filePtr);
		fread(&bloomProperties, sizeof(CBloomProperties), 1, filePtr);
		fread(&lightProperties, sizeof(CLightProperties), 1, filePtr);
		fread(&lodProperties, sizeof(CLODProperties), 1, filePtr);
		fread(&cameraProperties, sizeof(CCameraProperties), 1, filePtr);

		//fread(&characterBlendingProperties, sizeof(CCharacterBlendingProperties), 1, filePtr);
		fread(&pathProperties, sizeof(CPathProperties), 1, filePtr);
		CBool demo;
		fread(&demo, sizeof(CBool), 1, filePtr);

		CChar banner[MAX_NAME_SIZE];
		fread(&banner, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CExtraTexturesNamingConventions namingConventions;
		CBool bGlobalAmbientColor;
		CColor4f globalAmbientColor;
		CInt sceneManagersObjectsPerSplit;
		fread(&namingConventions, sizeof(CExtraTexturesNamingConventions), 1, filePtr);
		fread(&bGlobalAmbientColor, sizeof(CBool), 1, filePtr);
		fread(&globalAmbientColor, sizeof(CColor4f), 1, filePtr);
		fread(&sceneManagersObjectsPerSplit, sizeof(CInt), 1, filePtr);

		//read physX 
		NxExtendedVec3 characterPos;
		CVec3f cameraInstancePos;
		CVec2f cameraInstancePanTilt;
		CFloat cameraInstanceZoom;
		fread(&characterPos, sizeof(NxExtendedVec3), 1, filePtr);

		fread(&cameraInstancePos, sizeof(CVec3f), 1, filePtr);
		fread(&cameraInstancePanTilt, sizeof(CVec2f), 1, filePtr);
		fread(&cameraInstanceZoom, sizeof(CFloat), 1, filePtr);

		CBool hasScript;
		CChar script[MAX_NAME_SIZE];
		fread(&hasScript, 1, sizeof(CBool), filePtr);
		fread(script, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);

		CChar scriptPath[MAX_NAME_SIZE];
		CChar* tempPath = GetAfterPath(script);
		if (hasScript)
			sprintf(scriptPath, "%s%s%s%s", packagePath, prefab->GetPrefabName(), "/Scripts/", tempPath);
		else
			Cpy(scriptPath, "\n");

		prefab->SetHasScript(hasScript);
		prefab->SetScript(scriptPath);

		CChar tempSceneName[MAX_NAME_SIZE];

		CInt tempSceneSize, tempGeoSize;
		fread(&tempSceneSize, sizeof(CInt), 1, filePtr);

		for (CInt i = 0; i < tempSceneSize; i++)
		{
			CInt clipIndex;
			CBool playAnimation, loopAnimation, isVisible, isAlwaysVisible, castShadow, transformable, selectable;
			CInt tempSceneAnimationListSize;

			fread(tempSceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fread(&clipIndex, 1, sizeof(CInt), filePtr);
			fread(&playAnimation, 1, sizeof(CBool), filePtr);
			fread(&loopAnimation, 1, sizeof(CBool), filePtr);
			fread(&isVisible, 1, sizeof(CBool), filePtr);
			fread(&isAlwaysVisible, 1, sizeof(CBool), filePtr);
			fread(&castShadow, 1, sizeof(CBool), filePtr);
			fread(&transformable, 1, sizeof(CBool), filePtr);
			fread(&selectable, 1, sizeof(CBool), filePtr);

			fread(&tempSceneAnimationListSize, 1, sizeof(CInt), filePtr);

			CScene * tempScene = new CScene();

			for (CInt j = 0; j < tempSceneAnimationListSize; j++)
			{
				CChar name[MAX_NAME_SIZE];
				fread(name, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				tempScene->m_animationSceneNames.push_back(name);
			}

			fread(&tempGeoSize, sizeof(CInt), 1, filePtr);


			CBool sceneLoaded = CFalse;

			CChar * nameOnly = GetAfterPath(tempSceneName);

			//save functions. it should be copies in WIN32 Project as well
			CChar name[MAX_NAME_SIZE];

			sprintf(name, "%s%s%s%s", packagePath, prefab->GetPrefabName(), "/External Scenes/", nameOnly);

			CChar prefabAndSceneName[MAX_NAME_SIZE];
			sprintf(prefabAndSceneName, "%s%s%s", prefabAndPackageName, "_", nameOnly);
			Cpy(g_currentPrefabAndSceneName, prefabAndSceneName);

			g_scene.push_back(tempScene);

			for (CUInt j = 0; j < tempScene->m_animationSceneNames.size(); j++)
			{
				CChar sceneName[MAX_NAME_SIZE];
				Cpy(sceneName, tempScene->m_animationSceneNames[j].c_str());
				CChar * nameOnly = GetAfterPath(sceneName);
				GetWithoutDot(nameOnly);
				Append(nameOnly, ".vac");
				//save functions. it should be copied in WIN32 Project as well
				CChar name[MAX_NAME_SIZE];
				sprintf(name, "%s%s%s%s", packagePath, g_currentPrefabName, "/External Scenes/", nameOnly);

				CChar clipName[MAX_NAME_SIZE];
				Cpy(clipName, GetAfterPath(sceneName));
				GetWithoutDot(clipName);

				//Load 
				std::ifstream ifs(name, ios_base::binary);
				boost::archive::binary_iarchive ia(ifs);
				CAnimationClip* p;
				ia >> p;
				for (CUInt l = 0; l < p->m_animations.size(); l++)
				{
					if (!tempScene->GetAnimation(p->m_animations[l]->GetName(), p->m_animations[l]->GetDocURI()))
					{
						tempScene->m_animations.push_back(p->m_animations[l]);

						if (p->m_animations[l]->GetChannelSize() > 0)
						{
							p->m_animations[l]->GenerateKeys();
						}
						if (p->m_animations[l]->GetEndTime() > tempScene->GetLastKeyTime())
							tempScene->SetLastKeyTime(p->m_animations[l]->GetEndTime());

					}
				}
				p->SetAnimationStatus(eANIM_NONE);
				p->SetCurrentTime(0.0f);
				p->SetCurrentTime2(0.0f);
				p->SetCurrentWeight(0.0f);
				tempScene->m_animationClips.push_back(p);

				tempScene->m_hasAnimation = CTrue;
			}
			if (tempScene->m_animationSceneNames.size())
				tempScene->SetNumClips(tempScene->m_animationSceneNames.size());

			if (tempScene->m_animationSceneNames.size())
				tempScene->SetLoadAnimation(CFalse);
			if (tempScene->Load(name))
			{
				tempScene->SetLoadAnimation(CTrue);

				tempScene->SetClipIndexForStartup(clipIndex);
				tempScene->m_playAnimation = playAnimation;
				tempScene->m_isVisible = isVisible;
				tempScene->m_alwaysVisible = isAlwaysVisible;
				tempScene->m_loopAnimationAtStartup = loopAnimation;
				tempScene->m_castShadow = castShadow;
				tempScene->m_isTransformable = transformable;
				tempScene->m_isSelectable = selectable;

				if (tempScene->m_isTransformable)
				{
					for (CUInt l = 0; l < tempScene->m_instanceGeometries.size(); l++)
					{
						CGeometry* m_abstractGeometry = tempScene->m_instanceGeometries[l]->m_abstractGeometry;
						m_abstractGeometry->m_hasAnimation = CTrue;
					}
					tempScene->m_hasAnimation = CTrue;
				}

				if (tempScene->m_playAnimation)
				{
					tempScene->SetAnimationStatus(eANIM_PLAY);
					tempScene->SetClipIndex(clipIndex, loopAnimation);
					if (tempScene->m_loopAnimationAtStartup)
						tempScene->ExecuteCyclicAnimation(tempScene->GetCurrentClipIndex(), 1.0f, 0.0f);
					else
						tempScene->ExecuteNonCyclicAnimation(tempScene->GetCurrentClipIndex(), 0.0f, 0.0f, 1.0f, CFalse);
				}
				else
				{
					tempScene->SetAnimationStatus(eANIM_PAUSE);
				}
				//save functions/////////////////////////////////
				g_currentScene = tempScene; //mark the current scene. Save functions

				//save functions/////////////////////////////////
				g_render.SetScene(tempScene);
				tempScene->Update(0.0f, CTrue);

				if (CmpIn(tempScene->GetName(), "_LOD1"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(0, tempScene);
					m_newPrefab->SetHasLod(0);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				else if (CmpIn(tempScene->GetName(), "_LOD2"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(1, tempScene);
					m_newPrefab->SetHasLod(1);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				else if (CmpIn(tempScene->GetName(), "_LOD3"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(2, tempScene);
					m_newPrefab->SetHasLod(2);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				else if (CmpIn(tempScene->GetName(), "_COL"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(3, tempScene);
					m_newPrefab->GetCurrentInstance()->SetHasCollider(CTrue);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}

				sceneLoaded = CTrue;

			}
			else
			{
				delete tempScene;
				tempScene = NULL;
				g_scene.erase(g_scene.end());
				fclose(filePtr);
				MessageBox(NULL, _T("Couldn't load the scene"), _T("VandaEngine Error"), MB_OK);
				sceneLoaded = CFalse;
			}
			if (!sceneLoaded)
				return CFalse;
			if (CmpIn(tempScene->GetName(), "trigger")) //triggers
			{
				tempScene->m_isTrigger = CTrue;
				tempScene->Update();
				tempScene->CreateTrigger(g_nx);
			}

			for (CUInt j = 0; j < (CUInt)tempGeoSize; j++)
			{
				CChar m_strNormalMap[MAX_NAME_SIZE];
				CChar m_strDirtMap[MAX_NAME_SIZE];
				CChar m_strGlossMap[MAX_NAME_SIZE];
				CChar m_strHeightMap[MAX_NAME_SIZE];
				CChar m_strDuDvMap[MAX_NAME_SIZE];
				CChar m_strDiffuse[MAX_NAME_SIZE];
				CBool m_hasNormalMap, m_hasGlossMap, m_hasHeightMap, m_hasDuDvMap, m_hasDirtMap, m_hasDiffuse;
				CBool m_cullFaces;
				CUInt m_groupSize;
				CFloat m_bias, m_scale;

				fread(&m_hasDirtMap, sizeof(CBool), 1, filePtr);
				fread(m_strDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasNormalMap, sizeof(CBool), 1, filePtr);
				fread(m_strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(&m_bias, sizeof(CFloat), 1, filePtr);
				fread(&m_scale, sizeof(CFloat), 1, filePtr);

				fread(&m_hasGlossMap, sizeof(CBool), 1, filePtr);
				fread(m_strGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasHeightMap, sizeof(CBool), 1, filePtr);
				fread(m_strHeightMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDuDvMap, sizeof(CBool), 1, filePtr);
				fread(m_strDuDvMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDiffuse, sizeof(CBool), 1, filePtr);
				fread(m_strDiffuse, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				//load cull face( enabled or disabled data) for the current geo
				fread(&m_cullFaces, sizeof(CBool), 1, filePtr);

				//read PhysX data
				CUInt physx_point_size;
				fread(&physx_point_size, sizeof(CUInt), 1, filePtr);
				for (CUInt m = 0; m < physx_point_size; m++)
				{
					CVec3f* point = CNew(CVec3f);
					fread(point, sizeof(CVec3f), 1, filePtr);
					if( tempScene->m_geometries.size() >= j + 1 )
						tempScene->m_geometries[j]->m_physx_points.push_back(point);
				}
				CUInt physx_triangles_size;
				fread(&physx_triangles_size, sizeof(CUInt), 1, filePtr);

				for (CUInt m = 0; m < physx_triangles_size; m++)
				{
					CTriangles* tri = CNew(CTriangles);
					CUInt count;
					fread(&count, sizeof(CUInt), 1, filePtr);
					tri->m_count = count;
					tri->m_indexes = CNewData(CUInt, tri->m_count * 3);

					for (CUInt n = 0; n < tri->m_count * 3; n++)
					{
						CUInt index;
						fread(&index, sizeof(CUInt), 1, filePtr);
						tri->m_indexes[n] = index;
					}
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_physx_triangles.push_back(tri);
				}

				CInt cm_size = 0;
				fread(&cm_size, sizeof(CInt), 1, filePtr);
				if(tempScene->m_geometries.size() >= j + 1)
					tempScene->m_geometries[j]->m_collapseMap.SetSize(cm_size);
				for (CInt m = 0; m < cm_size; m++)
				{
					CInt cm_value;
					fread(&cm_value, sizeof(CInt), 1, filePtr);
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_collapseMap[m] = cm_value;
				}

				///////////////////

				fread(&m_groupSize, sizeof(CUInt), 1, filePtr);
				//store group info

				if (sceneLoaded)
				{
					if (m_hasDirtMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDirtMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDirtMap, m_strDirtMap);
					}
					if (m_hasNormalMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasNormalMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strNormalMap, m_strNormalMap);
					}
					if ( tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_parallaxMapBias = m_bias;
						tempScene->m_geometries[j]->m_parallaxMapScale = m_scale;
					}
					if (m_hasGlossMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasGlossMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strGlossMap, m_strGlossMap);
					}
					if (m_hasDiffuse && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDiffuse = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDiffuse, m_strDiffuse);
					}
					//if( m_hasHeightMap )
					//	tempScene->m_geometries[j]->SetHeightMap( m_strHeightMap );
					//if( m_hasDuDvMap )
					//	tempScene->m_geometries[j]->SetDuDvMap(); //under construction!
					if (m_cullFaces && tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CTrue);
					else if ( tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CFalse);

					for (CUInt k = 0; k < m_groupSize; k++)
					{
						CChar m_strGroupGlossMap[MAX_NAME_SIZE];
						CBool m_hasGroupGlossMap;
						fread(&m_hasGroupGlossMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupGlossMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetGlossMap(m_strGroupGlossMap);

						CChar m_strGroupDirtMap[MAX_NAME_SIZE];
						CBool m_hasGroupDirtMap;
						fread(&m_hasGroupDirtMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDirtMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDirtMap(m_strGroupDirtMap);

						CChar m_strGroupNormalMap[MAX_NAME_SIZE];
						CFloat m_bias, m_scale;
						CBool m_hasGroupNormalMap;
						fread(&m_hasGroupNormalMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						fread(&m_bias, sizeof(CFloat), 1, filePtr);
						fread(&m_scale, sizeof(CFloat), 1, filePtr);
						if (m_hasGroupNormalMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetNormalMap(m_strGroupNormalMap, m_bias, m_scale);

						CChar m_strGroupDiffuseMap[MAX_NAME_SIZE];
						CBool m_hasGroupDiffuse;
						fread(&m_hasGroupDiffuse, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDiffuseMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDiffuse && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDiffuse(m_strGroupDiffuseMap);

						//Material Colors
						CFloat ambient[4]; CFloat diffuse[4]; CFloat specular[4]; CFloat emission[4];
						CFloat shininess, transparency;

						fread(ambient, sizeof(CFloat), 4, filePtr);
						fread(diffuse, sizeof(CFloat), 4, filePtr);
						fread(specular, sizeof(CFloat), 4, filePtr);
						fread(emission, sizeof(CFloat), 4, filePtr);
						fread(&shininess, sizeof(CFloat), 1, filePtr);
						fread(&transparency, sizeof(CFloat), 1, filePtr);

						if (tempScene->m_geometries.size() >= j + 1)
						{
							tempScene->m_geometries[j]->m_groups[k]->SetReadMaterialColorFromGameEngine();
							tempScene->m_geometries[j]->m_groups[k]->SetAmbient(ambient);
							tempScene->m_geometries[j]->m_groups[k]->SetDiffuse(diffuse);
							tempScene->m_geometries[j]->m_groups[k]->SetSpecular(specular);
							tempScene->m_geometries[j]->m_groups[k]->SetEmission(emission);
							tempScene->m_geometries[j]->m_groups[k]->SetShininess(shininess);
							tempScene->m_geometries[j]->m_groups[k]->SetTransparency(transparency);
						}
						//////////////////

					}
				}
			} //for all of the geos

			CUInt instanceGeoSize;
			fread(&instanceGeoSize, 1, sizeof(CUInt), filePtr);
			for (CUInt j = 0; j < instanceGeoSize; j++)
			{
				CChar geoName[MAX_NAME_SIZE];
				CMatrix instanceLocalToWorldMatrix;
				CBool hasPhysX;
				CChar PhysXName[MAX_NAME_SIZE];
				CPhysXAlgorithm physXAlgorithm;
				CFloat physXDensity;
				CInt physXPercentage;
				CBool isTrigger;
				CBool isInvisible;
				CBool renderWithPhysics;

				fread(geoName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(instanceLocalToWorldMatrix, 1, sizeof(CMatrix), filePtr);
				fread(&hasPhysX, 1, sizeof(CBool), filePtr);
				fread(PhysXName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&physXAlgorithm, 1, sizeof(CPhysXAlgorithm), filePtr);
				fread(&physXDensity, 1, sizeof(CFloat), filePtr);
				fread(&physXPercentage, 1, sizeof(CInt), filePtr);
				fread(&isTrigger, 1, sizeof(CBool), filePtr);
				fread(&isInvisible, 1, sizeof(CBool), filePtr);
				fread(&renderWithPhysics, 1, sizeof(CBool), filePtr);

				for (CUInt k = 0; k < tempScene->m_instanceGeometries.size(); k++)
				{
					if (Cmp(geoName, tempScene->m_instanceGeometries[k]->m_abstractGeometry->GetName()))
					{
						tempScene->m_instanceGeometries[k]->m_isInvisible = isInvisible;
						tempScene->m_instanceGeometries[k]->m_renderWithPhysX = renderWithPhysics;
					}
				}

				//physics material
				CBool hasPhysicsMaterial;
				CFloat PhysicsRestitution;
				CFloat SkinWidth;
				CFloat StaticFriction;
				CFloat DynamicFriction;

				fread(&hasPhysicsMaterial, 1, sizeof(CBool), filePtr);
				fread(&PhysicsRestitution, 1, sizeof(CFloat), filePtr);
				fread(&SkinWidth, 1, sizeof(CFloat), filePtr);
				fread(&StaticFriction, 1, sizeof(CFloat), filePtr);
				fread(&DynamicFriction, 1, sizeof(CFloat), filePtr);
				///////////////////

				if (hasPhysX)
				{
					for (CUInt k = 0; k < tempScene->m_instanceGeometries.size(); k++)
					{
						if (Cmp(geoName, tempScene->m_instanceGeometries[k]->m_abstractGeometry->GetName()))
						{
							CBool equal = CTrue;
							for (CUInt l = 0; l < 16; l++)
							{
								if (instanceLocalToWorldMatrix[l] != tempScene->m_instanceGeometries[k]->m_firstLocalToWorldMatrix[l])
								{
									equal = CFalse;
									break;
								}
							}
							if (equal)
							{
								//if (tempScene->GeneratePhysX(physXAlgorithm, physXDensity, physXPercentage, isTrigger, tempScene->m_instanceGeometries[k], CTrue))
								//{
									tempScene->m_instanceGeometries[k]->m_lodAlgorithm = physXAlgorithm;
									tempScene->m_instanceGeometries[k]->m_hasPhysX = CTrue;
									tempScene->m_instanceGeometries[k]->m_physXDensity = physXDensity;
									tempScene->m_instanceGeometries[k]->m_physXPercentage = physXPercentage;
									tempScene->m_instanceGeometries[k]->m_isTrigger = isTrigger;

									tempScene->m_instanceGeometries[k]->EnablePhysicsMaterial(hasPhysicsMaterial);
									tempScene->m_instanceGeometries[k]->SetPhysicsRestitution(PhysicsRestitution);
									tempScene->m_instanceGeometries[k]->SetPhysicsSkinWidth(SkinWidth);
									tempScene->m_instanceGeometries[k]->SetPhysicsStaticFriction(StaticFriction);
									tempScene->m_instanceGeometries[k]->SetPhysicsDynamicFriction(DynamicFriction);

									//if (physXDensity > 0 || tempScene->m_instanceGeometries[k]->m_abstractGeometry->m_hasAnimation)
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_DYNAMIC_RIGIDBODY);
									//else if (isTrigger)
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_TRIGGER);
									//else
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_STATIC_RIGIDBODY);
								//}
							}
						}
					} //for
				} //if has PhysX
			}
		} // for all of the scenes

		//if (g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX)
		//{
			//g_nx->gControllers->reportSceneChanged();
			//gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
			//gPhysXscene->flushStream();
			//gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		//}

		fclose(filePtr);
	}
	return CTrue;
}

CBool CMain::Load(CChar* pathName)
{
	g_clickedOpen = CTrue;
	Cpy(g_currentVSceneName, GetAfterPath(pathName));

	FILE *filePtr;
	filePtr = fopen(pathName, "rb");
	if (!filePtr)
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "%s%s%s", "Couldn't open the file: '", pathName, "' to load data");
		//fclose( filePtr );
		MessageBoxA(NULL, temp, "Error", MB_OK);
		return CFalse;
	}

	CChar engineName[MAX_NAME_SIZE];
	fread(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
	if (!CmpIn(engineName, "VandaEngine"))
	{
		fclose(filePtr);
		MessageBoxA(NULL, "Invalid Vin file!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return CFalse;
	}
	CInt engine_version;
	fread(&g_edition, sizeof(CChar), MAX_NAME_SIZE, filePtr);
	fread(&engine_version, 1, sizeof(CInt), filePtr);
	if (engine_version > g_version)
	{
		fclose(filePtr);
		MessageBoxA(NULL, "VIN file has been saved with newer version of Vanda Engine. Please Update!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return CFalse;
	}
	fread(&g_currentPassword, sizeof(CChar), MAX_NAME_SIZE, filePtr);

	//read engine options
	fread(&g_shadowProperties, sizeof(CShadowProperties), 1, filePtr);

	switch (g_shadowProperties.m_shadowResolution)
	{
	case eSHADOW_1024:
		g_dynamicShadowMap->depth_size = 1024;
		break;
	case eSHADOW_2048:
		g_dynamicShadowMap->depth_size = 2048;
		break;
	case eSHADOW_4096:
		g_dynamicShadowMap->depth_size = 4096;
		break;
	default:
		break;
	}
	g_dynamicShadowMap->RegenerateDepthTex(g_dynamicShadowMap->depth_size);

	switch (g_shadowProperties.m_shadowSplits)
	{
	case eSHADOW_1_SPLIT:
		g_dynamicShadowMap->cur_num_splits = 1;
		break;
	case eSHADOW_2_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 2;
		break;
	case eSHADOW_3_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 3;
		break;
	case eSHADOW_4_SPLITS:
		g_dynamicShadowMap->cur_num_splits = 4;
		break;
	default:
		break;
	}

	g_dynamicShadowMap->split_weight = g_shadowProperties.m_shadowSplitWeight;

	fread(&g_physXProperties, sizeof(CPhysXProperties), 1, filePtr);
	fread(&g_physXCollisionFlags, sizeof(CPhysXCollisionFlags), 1, filePtr);
	ResetPhysX(); //reset the physX based on the g_physXProperties information
	//temprary disable gravity to avoid falling objects while laoding the scene:
	gPhysXscene->setGravity(NxVec3(0.0f, 0.0f, 0.0f));

	fread(&g_dofProperties, sizeof(CDOFProperties), 1, filePtr);
	fread(&g_fogProperties, sizeof(CFogProperties), 1, filePtr);
	fread(&g_bloomProperties, sizeof(CBloomProperties), 1, filePtr);
	fread(&g_lightProperties, sizeof(CLightProperties), 1, filePtr);
	fread(&g_instancePrefabLODPercent, sizeof(CLODProperties), 1, filePtr);
	fread(&g_cameraProperties, sizeof(CCameraProperties), 1, filePtr);
	fread(&g_currentVSceneProperties, sizeof(CCurrentVSceneProperties), 1, filePtr);
	if (engine_version >= 230)
	{
		fread(&g_main->m_showMenuCursor, sizeof(CBool), 1, filePtr);
		fread(&g_main->m_pauseMainCharacterAnimations, sizeof(CBool), 1, filePtr);
		fread(&g_main->m_pauseAllAnimationsOfPrefabInstances, sizeof(CBool), 1, filePtr);
		fread(&g_main->m_pausePhysics, sizeof(CBool), 1, filePtr);
		fread(&g_main->m_pauseAllWaterAnimations, sizeof(CBool), 1, filePtr);
	}
	if (engine_version >= 248)
	{
		fread(&g_main->m_pauseAllUpdateEvents, sizeof(CBool), 1, filePtr);
		fread(&g_main->m_pauseAllSounds, sizeof(CBool), 1, filePtr);
	}

	//fread(&g_characterBlendingProperties, sizeof(CCharacterBlendingProperties), 1, filePtr);
	fread(&g_pathProperties, sizeof(CPathProperties), 1, filePtr);
	CBool demo;
	fread(&demo, sizeof(CBool), 1, filePtr);
	if (demo)
	{
		MessageBoxA(NULL, "Thank you for your interest in Vanda Engine RTI.\nUnfortunatelly this feature is not supported in demo version. Please order Vanda Engine to run the final output.", "Warning", MB_OK | MB_ICONERROR);
		return CFalse;
	}

	CChar bannerName[MAX_NAME_SIZE];
	fread(&bannerName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

	CChar cursorName[MAX_NAME_SIZE];
	fread(&cursorName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

	fread(&g_extraTexturesNamingConventions, sizeof(CExtraTexturesNamingConventions), 1, filePtr);
	fread(&g_useGlobalAmbientColor, sizeof(CBool), 1, filePtr);
	fread(&g_globalAmbientColor, sizeof(CColor4f), 1, filePtr);
	fread(&g_sceneManagerObjectsPerSplit, sizeof(CInt), 1, filePtr);
	CFog fog;
	fog.SetColor(g_fogProperties.m_fogColor);
	fog.SetDensity(g_fogProperties.m_fogDensity);

	//read physX 
	NxExtendedVec3 characterPos;
	CVec3f cameraInstancePos;
	CVec2f cameraInstancePanTilt;
	CFloat cameraInstanceZoom;
	fread(&characterPos, sizeof(NxExtendedVec3), 1, filePtr);

	g_nx->gCharacterPos.x = characterPos.x;
	g_nx->gCharacterPos.y = characterPos.y;
	g_nx->gCharacterPos.z = characterPos.z;

	fread(&cameraInstancePos, sizeof(CVec3f), 1, filePtr);
	fread(&cameraInstancePanTilt, sizeof(CVec2f), 1, filePtr);
	fread(&cameraInstanceZoom, sizeof(CFloat), 1, filePtr);

	g_render.GetDefaultInstanceCamera()->MoveTransform2(cameraInstancePos.x, cameraInstancePos.y, cameraInstancePos.z);
	g_render.GetDefaultInstanceCamera()->SetPanAndTilt2(cameraInstancePanTilt.x, cameraInstancePanTilt.y);
	g_render.GetDefaultInstanceCamera()->ZoomTransform2(0.0f);
	g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetAngle(cameraInstanceZoom);
	g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetMinAngle(MIN_CAMERA_ANGLE);
	g_render.GetDefaultInstanceCamera()->m_abstractCamera->SetMaxAngle(MAX_CAMERA_ANGLE);

	///////////////////////////////////

	g_nx->ResetCharacterPos(characterPos);

	//load prefab resources here
	CUInt prefab_resource_size;
	fread(&prefab_resource_size, sizeof(CUInt), 1, filePtr);
	for (CUInt i = 0; i < prefab_resource_size; i++)
	{
		//read prefab data
		CChar name[MAX_NAME_SIZE];
		CChar package_name[MAX_NAME_SIZE];
		CChar prefab_name[MAX_NAME_SIZE];
		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(package_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CPrefab* new_prefab = CNew(CPrefab);
		new_prefab->SetName(name);
		new_prefab->SetPackageName(package_name);
		new_prefab->SetPrefabName(prefab_name);

		g_resourcePrefab.push_back(new_prefab);
	}

	//load prefabs here
	CUInt prefabSize;
	fread(&prefabSize, sizeof(CUInt), 1, filePtr);
	for (CUInt i = 0; i < prefabSize; i++)
	{
		//read prefab data
		CChar name[MAX_NAME_SIZE];
		CChar package_name[MAX_NAME_SIZE];
		CChar prefab_name[MAX_NAME_SIZE];
		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(package_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CPrefab* new_prefab = CNew(CPrefab);
		new_prefab->SetName(name);
		new_prefab->SetPackageName(package_name);
		new_prefab->SetPrefabName(prefab_name);

		g_prefab.push_back(new_prefab);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading Prefab: ", name);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CUInt prefabInstanceSize;
		fread(&prefabInstanceSize, sizeof(CUInt), 1, filePtr);

		for (CUInt j = 0; j < prefabInstanceSize; j++)
		{
			//read instance data
			CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
			g_currentInstancePrefab = new_instance_prefab;
			//fread(new_instance_prefab, sizeof(CInstancePrefab), 1, filePtr);

			CChar name[MAX_NAME_SIZE];
			CVec3f translate;
			CVec4f rotate;
			CVec3f scale;
			CBool isVisible;
			CChar script[MAX_NAME_SIZE];
			CBool isTrigger;
			CBool isControlledByPhysx;
			CBool isAnimated;
			CBool isStatic;
			CBool totalLights;
			CBool castShadow;
			CBool transformable;
			CBool selectable;

			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			new_instance_prefab->SetName(name);

			fread(&translate, sizeof(CVec3f), 1, filePtr);
			new_instance_prefab->SetTranslate(translate);

			fread(&rotate, sizeof(CVec4f), 1, filePtr);
			new_instance_prefab->SetRotate(rotate);

			fread(&scale, sizeof(CVec3f), 1, filePtr);
			new_instance_prefab->SetScale(scale);

			//Materials
			CBool enableMaterial;
			fread(&enableMaterial, sizeof(CBool), 1, filePtr);
			if (enableMaterial)
				new_instance_prefab->EnableMaterial();
			else
				new_instance_prefab->DisableMaterial();

			CFloat ambient[4];
			fread(ambient, sizeof(CFloat), 4, filePtr);
			new_instance_prefab->SetAmbient(ambient);

			CFloat diffuse[4];
			fread(diffuse, sizeof(CFloat), 4, filePtr);
			new_instance_prefab->SetDiffuse(diffuse);

			CFloat specular[4];
			fread(specular, sizeof(CFloat), 4, filePtr);
			new_instance_prefab->SetSpecular(specular);

			CFloat emission[4];
			fread(emission, sizeof(CFloat), 4, filePtr);
			new_instance_prefab->SetEmission(emission);

			CFloat shininess;
			fread(&shininess, sizeof(CFloat), 1, filePtr);
			new_instance_prefab->SetShininess(shininess);

			CFloat transparency;
			fread(&transparency, sizeof(CFloat), 1, filePtr);
			new_instance_prefab->SetTransparency(transparency);
			//////////

			fread(&isVisible, sizeof(CBool), 1, filePtr);

			fread(script, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			new_instance_prefab->SetScript(script);
			new_instance_prefab->LoadLuaFile();

			fread(&isTrigger, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetIsTrigger(isTrigger);

			fread(&isControlledByPhysx, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetIsControlledByPhysX(isControlledByPhysx);

			fread(&isAnimated, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetIsAnimated(isAnimated);

			fread(&isStatic, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetIsStatic(isStatic);

			fread(&totalLights, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetTotalLights(totalLights);

			fread(&castShadow, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetCastShadow(castShadow);

			fread(&transformable, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetTransformable(transformable);

			fread(&selectable, sizeof(CBool), 1, filePtr);
			new_instance_prefab->SetSelectable(selectable);

			new_prefab->AddInstance(new_instance_prefab);
			new_prefab->SetCurrentInstance(new_instance_prefab);
			new_instance_prefab->SetPrefab(new_prefab);
			new_instance_prefab->SetNameIndex(); //for selection only
			new_instance_prefab->GenQueryIndex();
			new_instance_prefab->SetWater(NULL);

			if (g_main->m_pauseAllAnimationsOfPrefabInstances)
				new_instance_prefab->SetUpdateAnimation(CFalse);
			else
				new_instance_prefab->SetUpdateAnimation(CTrue);

			g_instancePrefab.push_back(new_instance_prefab);
			Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
			InsertPrefab(new_prefab);
			//new_instance_prefab->UpdateBoundingBox();
			//new_instance_prefab->CalculateDistance();
			//new_instance_prefab->UpdateIsStaticOrAnimated();
			new_instance_prefab->SetVisible(isVisible);
			new_instance_prefab->SetHasScript(new_prefab->GetHasScript());
			new_instance_prefab->SetScript(new_prefab->GetScript());
			new_instance_prefab->LoadLuaFile();

			CChar tempInstanceName[MAX_NAME_SIZE];
			sprintf(tempInstanceName, "%s%s%s", "\nPrefab Instance ' ", new_instance_prefab->GetName(), " ' created successfully");
		}
	}

	g_updateOctree = CTrue;

	CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
	Cpy(g_currentVSceneNameWithoutDot, g_currentVSceneName);
	GetWithoutDot(g_currentVSceneNameWithoutDot);

	//load GUIs//////////////////
	CUInt guiSize;
	fread(&guiSize, sizeof(CUInt), 1, filePtr);

	for (CUInt i = 0; i < guiSize; i++)
	{
		CGUI* new_gui = CNew(CGUI);
		//save gui name
		CChar name[MAX_NAME_SIZE];
		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		new_gui->SetName(name);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading GUI: ", name);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CChar packageName[MAX_NAME_SIZE];
		fread(packageName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		new_gui->SetPackageName(packageName);

		CChar guiName[MAX_NAME_SIZE];
		fread(guiName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		new_gui->SetGUIName(guiName);

		//is gui visible?
		CBool isVisible;
		fread(&isVisible, sizeof(CBool), 1, filePtr);

		new_gui->SetVisible(isVisible);

		CVec2f pos;
		fread(&pos, sizeof(CVec2f), 1, filePtr);
		new_gui->SetPosition(pos);

		CFloat rot;
		fread(&rot, sizeof(CFloat), 1, filePtr);
		new_gui->SetRotation(rot);

		CChar guiPath[MAX_NAME_SIZE];
		CChar guiAndPackageName[MAX_NAME_SIZE];

		Cpy(guiAndPackageName, packageName);
		Append(guiAndPackageName, "_");
		Append(guiAndPackageName, guiName);

		Cpy(guiPath, "Assets/GUIs/");
		Append(guiPath, packageName);
		Append(guiPath, "/");
		Append(guiPath, guiName);
		Append(guiPath, "/");
		Append(guiPath, guiAndPackageName);
		Append(guiPath, ".gui");

		FILE *filePtr;
		filePtr = fopen(guiPath, "rb");
		if (!filePtr)
		{
			MessageBoxA(NULL, "Couldn't open the GUI file to load data", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}

		CInt engine_version;
		fread(&engine_version, 1, sizeof(CInt), filePtr);
		if (engine_version > g_version)
		{
			fclose(filePtr);
			MessageBoxA(NULL, "GUI file has been saved with newer version of Vanda Engine. Please Update!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}

		CUInt numButtons;
		fread(&numButtons, sizeof(CUInt), 1, filePtr);

		for (CUInt j = 0; j < numButtons; j++)
		{
			//load button information

			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar packageName[MAX_NAME_SIZE];
			fread(packageName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar guiName[MAX_NAME_SIZE];
			fread(guiName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CVec2f pos;
			fread(&pos, sizeof(CVec2f), 1, filePtr);
			pos.x *= (CFloat)g_width;
			pos.y *= (CFloat)g_height;

			CInt size;
			fread(&size, sizeof(CInt), 1, filePtr);

			CBool isVisible;
			fread(&isVisible, sizeof(CBool), 1, filePtr);

			CChar mainImagePath[MAX_NAME_SIZE];
			fread(mainImagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			//disable image
			CBool hasDisableImage;
			fread(&hasDisableImage, sizeof(CBool), 1, filePtr);

			CChar disableImagePath[MAX_NAME_SIZE];
			if (hasDisableImage)
				fread(disableImagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			//hover image
			CBool hasHoverImage;
			fread(&hasHoverImage, sizeof(CBool), 1, filePtr);

			CChar hoverImagePath[MAX_NAME_SIZE];
			if (hasHoverImage)
				fread(hoverImagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			//left click image
			CBool hasLeftClickImage;
			fread(&hasLeftClickImage, sizeof(CBool), 1, filePtr);

			CChar leftClickImagePath[MAX_NAME_SIZE];
			if (hasLeftClickImage)
				fread(leftClickImagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			//right click image
			CBool hasRightClickImage;
			fread(&hasRightClickImage, sizeof(CBool), 1, filePtr);

			CChar rightClickImagePath[MAX_NAME_SIZE];
			if (hasRightClickImage)
				fread(rightClickImagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			//left click script
			CBool hasScript;
			fread(&hasScript, sizeof(CBool), 1, filePtr);

			CChar ScriptPath[MAX_NAME_SIZE];
			if (hasScript)
				fread(ScriptPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar finalMainImagePath[MAX_NAME_SIZE];
			CChar* tempMainImagePath = GetAfterPath(mainImagePath);

			sprintf(finalMainImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Buttons/", tempMainImagePath);

			CGUIButton* guiButton = CNew(CGUIButton);

			guiButton->SetName(name);
			guiButton->SetPackageName(packageName);
			guiButton->SetGUIName(guiName);
			guiButton->SetPosition(pos);
			guiButton->SetSize(size);
			guiButton->SetVisible(isVisible);
			guiButton->SetMainImagePath(finalMainImagePath);
			guiButton->LoadMainImage();

			if (hasDisableImage)
			{
				CChar finalImagePath[MAX_NAME_SIZE];
				CChar* tempImagePath = GetAfterPath(disableImagePath);
				sprintf(finalImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Buttons/", tempImagePath);

				guiButton->SetDisableImagePath(finalImagePath);
				guiButton->SetHasDisableImage(CTrue);
				guiButton->LoadDisableImage();
			}
			else
			{
				guiButton->SetHasDisableImage(CFalse);
			}
			if (hasHoverImage)
			{
				CChar finalImagePath[MAX_NAME_SIZE];
				CChar* tempImagePath = GetAfterPath(hoverImagePath);
				sprintf(finalImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Buttons/", tempImagePath);

				guiButton->SetHoverImagePath(finalImagePath);
				guiButton->SetHasHoverImage(CTrue);
				guiButton->LoadHoverImage();
			}
			else
			{
				guiButton->SetHasHoverImage(CFalse);
			}
			if (hasLeftClickImage)
			{
				CChar finalImagePath[MAX_NAME_SIZE];
				CChar* tempImagePath = GetAfterPath(leftClickImagePath);
				sprintf(finalImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Buttons/", tempImagePath);

				guiButton->SetLeftClickImagePath(finalImagePath);
				guiButton->SetHasLeftClickImage(CTrue);
				guiButton->LoadLeftClickImage();
			}
			else
			{
				guiButton->SetHasLeftClickImage(CFalse);
			}
			if (hasRightClickImage)
			{
				CChar finalImagePath[MAX_NAME_SIZE];
				CChar* tempImagePath = GetAfterPath(rightClickImagePath);
				sprintf(finalImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Buttons/", tempImagePath);

				guiButton->SetRightClickImagePath(finalImagePath);
				guiButton->SetHasRightClickImage(CTrue);
				guiButton->LoadRightClickImage();
			}
			else
			{
				guiButton->SetHasRightClickImage(CFalse);
			}
			if (hasScript)
			{
				CChar finalScriptPath[MAX_NAME_SIZE];
				CChar* tempScriptPath = GetAfterPath(ScriptPath);
				sprintf(finalScriptPath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Scripts/", tempScriptPath);

				guiButton->SetScriptPath(finalScriptPath);
				guiButton->SetHasScript(CTrue);
				guiButton->LoadLuaFile();
			}
			else
			{
				guiButton->SetHasScript(CFalse);
			}
			new_gui->AddGUIButton(guiButton);

		}

		CUInt numImages;
		fread(&numImages, sizeof(CUInt), 1, filePtr);

		for (CUInt j = 0; j < numImages; j++)
		{
			//load image information

			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar packageName[MAX_NAME_SIZE];
			fread(packageName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar guiName[MAX_NAME_SIZE];
			fread(guiName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CVec2f pos;
			fread(&pos, sizeof(CVec2f), 1, filePtr);
			pos.x *= (CFloat)g_width;
			pos.y *= (CFloat)g_height;

			CInt size;
			fread(&size, sizeof(CInt), 1, filePtr);

			CBool isVisible;
			fread(&isVisible, sizeof(CBool), 1, filePtr);

			CChar imagePath[MAX_NAME_SIZE];
			fread(imagePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar finalImagePath[MAX_NAME_SIZE];
			CChar* tempPath = GetAfterPath(imagePath);

			sprintf(finalImagePath, "%s%s%s%s%s%s", "assets/GUIs/",packageName, "/", guiName, "/Textures/Images/", tempPath);

			CGUIImage* guiImage = CNew(CGUIImage);
			guiImage->SetName(name);
			guiImage->SetPackageName(packageName);
			guiImage->SetGUIName(guiName);
			guiImage->SetPosition(pos);
			guiImage->SetSize(size);
			guiImage->SetVisible(isVisible);
			guiImage->SetImagePath(finalImagePath);
			guiImage->LoadGUIImage();

			new_gui->AddGUIImage(guiImage);

		}

		CUInt numTexts;
		fread(&numTexts, sizeof(CUInt), 1, filePtr);

		for (CUInt j = 0; j < numTexts; j++)
		{
			//load text information

			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar packageName[MAX_NAME_SIZE];
			fread(packageName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CChar guiName[MAX_NAME_SIZE];
			fread(guiName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CVec2f pos;
			fread(&pos, sizeof(CVec2f), 1, filePtr);
			pos.x *= (CFloat)g_width;
			pos.y *= (CFloat)g_height;

			CInt size;
			fread(&size, sizeof(CInt), 1, filePtr);

			CBool isVisible;
			fread(&isVisible, sizeof(CBool), 1, filePtr);

			CChar text[MAX_URI_SIZE];
			fread(text, sizeof(CChar), MAX_URI_SIZE, filePtr);

			CVec3f color;
			fread(&color, sizeof(CVec3f), 1, filePtr);

			CFontType font;
			fread(&font, sizeof(CFontType), 1, filePtr);

			CGUIText* guiText = CNew(CGUIText);
			guiText->SetName(name);
			guiText->SetPackageName(packageName);
			guiText->SetGUIName(guiName);
			guiText->SetPosition(pos);
			guiText->SetSize(size);
			guiText->SetVisible(isVisible);
			guiText->SetColor(color);
			guiText->SetText(text);
			guiText->SetType(font);
			guiText->SetFont();

			new_gui->AddGUIText(guiText);

		}
		fclose(filePtr);
		g_guis.push_back(new_gui);

	}
	//End of GUIs///////////////////

	//sky object

	CBool showSky;
	fread(&showSky, sizeof(CBool), 1, filePtr);

	if (showSky)
	{
		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s", "Loading sky dome");
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CChar name[MAX_NAME_SIZE];
		CChar path[MAX_NAME_SIZE];
		CInt slices, sides;
		CFloat dampening, radius, position[3];
		CBool exponential, fog;
		CBool hasScript;
		CChar scriptPath[MAX_NAME_SIZE];

		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(path, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&slices, sizeof(CInt), 1, filePtr);
		fread(&sides, sizeof(CInt), 1, filePtr);
		fread(&radius, sizeof(CFloat), 1, filePtr);
		fread(position, sizeof(CFloat), 3, filePtr);
		fread(&dampening, sizeof(CFloat), 1, filePtr);
		fread(&exponential, sizeof(CBool), 1, filePtr);
		fread(&fog, sizeof(CBool), 1, filePtr);
		fread(&hasScript, sizeof(CBool), 1, filePtr);
		fread(&scriptPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		g_skyDome = CNew(CSkyDome);
		CChar skyPath[MAX_NAME_SIZE];
		CChar* tempPath = GetAfterPath(path);

		sprintf(skyPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sky/", tempPath);

		CChar skyScriptPath[MAX_NAME_SIZE];
		CChar* tempScriptPath = GetAfterPath(scriptPath);
		sprintf(skyScriptPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sky/", tempScriptPath);

		g_skyDome->SetName(name);
		g_skyDome->SetPath(skyPath);
		g_skyDome->SetSlices(slices);
		g_skyDome->SetSides(sides);
		g_skyDome->SetRadius(radius);
		g_skyDome->SetPosition(position);
		g_skyDome->SetDampening(dampening);
		g_skyDome->SetExponential(exponential);
		g_skyDome->SetFog(fog);
		g_skyDome->SetHasScript(hasScript);
		g_skyDome->SetScript(skyScriptPath);
		g_skyDome->Initialize();
		g_skyDome->LoadLuaFile();
		g_databaseVariables.m_insertAndShowSky = CTrue;
	}
	else
		g_databaseVariables.m_insertAndShowSky = CFalse;

	//Load terrain here
	CBool showTerrain;

	fread(&showTerrain, sizeof(CBool), 1, filePtr);
	if (showTerrain)
	{
		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s", "Loading terrain");
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CChar name[MAX_NAME_SIZE], heightMapPath[MAX_NAME_SIZE], bottomTexturePath[MAX_NAME_SIZE], bottomNormalMapPath[MAX_NAME_SIZE],
			slopeTexturePath[MAX_NAME_SIZE], slopeNormalMapPath[MAX_NAME_SIZE], topTexturePath[MAX_NAME_SIZE], topNormalPath[MAX_NAME_SIZE];

		CChar* temp_heightMapPath; CChar* temp_bottomTexturePath; CChar* temp_bottomNormalMapPath;
		CChar* temp_slopeTexturePath; CChar* temp_slopeNormalMapPath; CChar* temp_topTexturePath; CChar* temp_topNormalPath;

		CChar final_heightMapPath[MAX_NAME_SIZE], final_bottomTexturePath[MAX_NAME_SIZE], final_bottomNormalMapPath[MAX_NAME_SIZE],
			final_slopeTexturePath[MAX_NAME_SIZE], final_slopeNormalMapPath[MAX_NAME_SIZE], final_topTexturePath[MAX_NAME_SIZE], final_topNormalPath[MAX_NAME_SIZE];

		CFloat shininess, scaleHeight, scaleWidth, slopeFactor, startHeight;
		CInt smooth;
		CBool flatten;
		CFloat ambientColor[4], diffuseColor[4], specularColor[4];
		CBool hasScript;
		CChar scriptPath[MAX_NAME_SIZE];

		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(heightMapPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(bottomTexturePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(bottomNormalMapPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(slopeTexturePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(slopeNormalMapPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(topTexturePath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(topNormalPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&shininess, sizeof(CFloat), 1, filePtr);
		fread(&smooth, sizeof(CInt), 1, filePtr);
		fread(&scaleHeight, sizeof(CFloat), 1, filePtr);
		fread(&scaleWidth, sizeof(CFloat), 1, filePtr);
		fread(&slopeFactor, sizeof(CFloat), 1, filePtr);
		fread(&startHeight, sizeof(CFloat), 1, filePtr);
		fread(&flatten, sizeof(CBool), 1, filePtr);
		fread(ambientColor, sizeof(CFloat), 4, filePtr);
		fread(diffuseColor, sizeof(CFloat), 4, filePtr);
		fread(specularColor, sizeof(CFloat), 4, filePtr);
		fread(&hasScript, sizeof(CBool), 1, filePtr);
		fread(&scriptPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		temp_heightMapPath = GetAfterPath(heightMapPath);
		sprintf(final_heightMapPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_heightMapPath);

		temp_bottomTexturePath = GetAfterPath(bottomTexturePath);
		sprintf(final_bottomTexturePath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_bottomTexturePath);

		temp_bottomNormalMapPath = GetAfterPath(bottomNormalMapPath);
		sprintf(final_bottomNormalMapPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_bottomNormalMapPath);

		temp_slopeTexturePath = GetAfterPath(slopeTexturePath);
		sprintf(final_slopeTexturePath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_slopeTexturePath);

		temp_slopeNormalMapPath = GetAfterPath(slopeNormalMapPath);
		sprintf(final_slopeNormalMapPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_slopeNormalMapPath);

		temp_topTexturePath = GetAfterPath(topTexturePath);
		sprintf(final_topTexturePath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_topTexturePath);

		temp_topNormalPath = GetAfterPath(topNormalPath);
		sprintf(final_topNormalPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", temp_topNormalPath);

		CChar physicsPath[MAX_NAME_SIZE];
		sprintf(physicsPath, "%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/");

		CChar terrainScriptPath[MAX_NAME_SIZE];
		CChar* tempScriptPath = GetAfterPath(scriptPath);
		sprintf(terrainScriptPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Terrain/", tempScriptPath);

		g_terrain = CNew(CTerrain);
		g_terrain->SetCookPhysicsTriangles(CFalse);
		g_terrain->SetPhysicsPath(physicsPath);
		g_terrain->SetName(name);

		g_terrain->SetHeightMapPath(final_heightMapPath);

		g_terrain->SetBottomTexturePath(final_bottomTexturePath);
		g_terrain->SetBottomNormalMapPath(final_bottomNormalMapPath);

		g_terrain->SetSlopeTexturePath(final_slopeTexturePath);
		g_terrain->SetSlopeNormalMapPath(final_slopeNormalMapPath);

		g_terrain->SetTopTexturePath(final_topTexturePath);
		g_terrain->SetTopNormalMapPath(final_topNormalPath);

		g_terrain->SetShininess(shininess);
		g_terrain->SetSmooth(smooth);
		g_terrain->SetScaleHeight(scaleHeight);
		g_terrain->SetScaleWidth(scaleWidth);
		g_terrain->SetSlopeFactor(slopeFactor);
		g_terrain->SetTopStartHeight(startHeight);
		g_terrain->SetFlatten(flatten);

		g_terrain->SetAmbientColor(ambientColor);
		g_terrain->SetDiffuseColor(diffuseColor);
		g_terrain->SetSpecularColor(specularColor);

		g_terrain->SetHasScript(hasScript);
		g_terrain->SetScript(terrainScriptPath);

		g_terrain->Initialize();
		g_terrain->LoadLuaFile();

		g_databaseVariables.m_insertAndShowTerrain = CTrue;
	}
	else
		g_databaseVariables.m_insertAndShowTerrain = CFalse;

	//Load water here
	CInt tempWaterCount, tempInstancePrefabWaterCount;
	CChar strNormalMap[MAX_NAME_SIZE];
	CChar strDuDvMap[MAX_NAME_SIZE];
	CChar strWaterName[MAX_NAME_SIZE];
	CFloat waterPos[3];
	CFloat waterLightPos[3];
	CFloat waterHeight, waterSpeed, waterScaleX, waterScaleZ, waterRotateY, waterUV, waterTransparency, waterFogDensity;
	CFloat waterColor[3];
	CBool waterVisible;
	CBool waterShadow;
	CBool waterSunReflection;
	CBool waterHasScript;
	CChar waterScriptPath[MAX_NAME_SIZE];

	fread(&tempWaterCount, sizeof(CInt), 1, filePtr);
	for (CInt i = 0; i < tempWaterCount; i++)
	{
		CWater* water = new CWater;

		fread(strWaterName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strDuDvMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading water: ", strWaterName);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		fread(waterPos, sizeof(CFloat), 3, filePtr);
		fread(waterLightPos, sizeof(CFloat), 3, filePtr);
		fread(&waterHeight, sizeof(CFloat), 1, filePtr);
		fread(&waterSpeed, sizeof(CFloat), 1, filePtr);
		fread(&waterScaleX, sizeof(CFloat), 1, filePtr);
		fread(&waterScaleZ, sizeof(CFloat), 1, filePtr);
		fread(&waterRotateY, sizeof(CFloat), 1, filePtr);
		fread(&waterTransparency, sizeof(CFloat), 1, filePtr);
		fread(&waterFogDensity, sizeof(CFloat), 1, filePtr);
		fread(waterColor, sizeof(CFloat), 3, filePtr);
		fread(&waterUV, sizeof(CFloat), 1, filePtr);
		fread(&waterVisible, sizeof(CBool), 1, filePtr);
		fread(&waterShadow, sizeof(CBool), 1, filePtr);
		fread(&waterSunReflection, sizeof(CBool), 1, filePtr);
		fread(&waterHasScript, sizeof(CBool), 1, filePtr);
		fread(&waterScriptPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		fread(&tempInstancePrefabWaterCount, sizeof(CInt), 1, filePtr);
		for (CInt j = 0; j < tempInstancePrefabWaterCount; j++)
		{
			CChar instanceName[MAX_NAME_SIZE];
			fread(instanceName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			for (CUInt k = 0; k < g_instancePrefab.size(); k++)
			{
				if (Cmp(g_instancePrefab[k]->GetName(), instanceName))
				{
					water->AddPrefabInstance(g_instancePrefab[k]);
					g_instancePrefab[k]->SetWater(water);
					g_instancePrefab[k]->UpdateBoundingBoxForWater(waterHeight);
				}
			}
		}
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy(g_currentVSceneNameWithoutDot, g_currentVSceneName);
		GetWithoutDot(g_currentVSceneNameWithoutDot);

		CChar dudvPath[MAX_NAME_SIZE];
		CChar* DuDvAfterPath = GetAfterPath(strDuDvMap);


		sprintf(dudvPath, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Waters/", strWaterName, "/", DuDvAfterPath);

		CChar normalPath[MAX_NAME_SIZE];
		CChar* normalAfterPath = GetAfterPath(strNormalMap);


		sprintf(normalPath, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Waters/", strWaterName, "/", normalAfterPath);

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(waterScriptPath);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Waters/", strWaterName, "/", scriptAfterPath);

		water->SetName(strWaterName);
		water->SetDuDvMap(dudvPath);
		water->SetNormalMap(normalPath);
		water->SetHeight(waterHeight);
		water->SetSpeed(waterSpeed);
		water->SetScaleX(waterScaleX);
		water->SetScaleZ(waterScaleZ);
		water->SetRotateY(waterRotateY);
		water->SetHasScript(waterHasScript);
		water->SetScript(script);
		water->SetTransparency(waterTransparency);
		water->SetFogDensity(waterFogDensity);
		water->SetColor(waterColor);
		water->SetUV(waterUV);
		water->SetPos(waterPos);
		water->SetVisible(waterVisible);
		water->SetShadow(waterShadow);
		water->SetSunReflection(waterSunReflection);
		water->SetLightPos(waterLightPos);
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFLECTION_ID);
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFRACTION_ID);
		water->CreateRenderTexture(g_waterTextureSize, 1, GL_DEPTH_COMPONENT, WATER_DEPTH_ID);
		water->SetSideVertexPositions();

		if (m_pauseAllWaterAnimations)
			water->SetUpdateAnimation(CFalse);
		else
			water->SetUpdateAnimation(CTrue);

		water->LoadLuaFile();
		g_engineWaters.push_back(water);

	}
	//Engine Lights
	CInt tempLightCount;
	fread(&tempLightCount, sizeof(CInt), 1, filePtr);
	CFloat m_fLightPos[4]; 	CFloat m_fConstantAttenuation; CFloat m_fLinearAttenuation; CFloat m_fQuadAttenuation; CFloat m_fSpotCuttoff;
	CFloat m_fSpotDirection[4]; CFloat m_fSpotExponent; CFloat m_fShininess; CFloat m_fAmbientColor[4]; CFloat m_fDiffuseColor[4];
	CFloat m_fSpecularColor[4]; CLightType m_lightType;
	CChar lightName[MAX_NAME_SIZE];

	CBool lightHasScript;
	CChar lightScript[MAX_NAME_SIZE];

	for (CInt i = 0; i < tempLightCount; i++)
	{
		fread(lightName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(m_fLightPos, sizeof(CFloat), 4, filePtr);
		fread(&m_fConstantAttenuation, sizeof(CFloat), 1, filePtr);
		fread(&m_fLinearAttenuation, sizeof(CFloat), 1, filePtr);
		fread(&m_fQuadAttenuation, sizeof(CFloat), 1, filePtr);
		fread(&m_fShininess, sizeof(CFloat), 1, filePtr);
		fread(m_fAmbientColor, sizeof(CFloat), 4, filePtr);
		fread(m_fDiffuseColor, sizeof(CFloat), 4, filePtr);
		fread(m_fSpecularColor, sizeof(CFloat), 4, filePtr);
		fread(&m_lightType, sizeof(CLightType), 1, filePtr);
		fread(&lightHasScript, sizeof(CBool), 1, filePtr);
		fread(&lightScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		if (m_lightType == eLIGHTTYPE_SPOT)
		{
			fread(&m_fSpotCuttoff, sizeof(CFloat), 1, filePtr);
			fread(m_fSpotDirection, sizeof(CFloat), 4, filePtr);
			fread(&m_fSpotExponent, sizeof(CFloat), 1, filePtr);
		}

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(lightScript);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Lights/", lightName, "/", scriptAfterPath);

		CInstanceLight* instance_light = new CInstanceLight();
		CLight* abstract_light = new CLight();

		instance_light->m_parent = NULL; //Not using COLLADA parent
		instance_light->m_abstractLight = abstract_light;
		abstract_light->m_instanceLight = instance_light;
		abstract_light->SetName(lightName);
		abstract_light->SetType(m_lightType);
		switch (m_lightType)
		{
		case eLIGHTTYPE_SPOT:
			instance_light->m_abstractLight->SetSpotCutoff(m_fSpotCuttoff);
			instance_light->m_abstractLight->SetSpotExponent(m_fSpotExponent);
			instance_light->m_abstractLight->SetSpotDirection(m_fSpotDirection);
			break;
		}

		abstract_light->SetAmbient(m_fAmbientColor);
		abstract_light->SetDiffuse(m_fDiffuseColor);
		abstract_light->SetSpecular(m_fSpecularColor);
		abstract_light->SetPosition(m_fLightPos);
		abstract_light->SetShininess(m_fShininess);
		abstract_light->SetConstantAttenuation(m_fConstantAttenuation);
		abstract_light->SetLinearAttenuation(m_fLinearAttenuation);
		abstract_light->SetQuadraticAttenuation(m_fQuadAttenuation);
		abstract_light->SetHasScript(lightHasScript);
		abstract_light->SetScript(script);
		abstract_light->LoadLuaFile();
		instance_light->SetIndex();

		g_engineLights.push_back(instance_light);

	}

	//Engine Cameras
	CInt tempCameraCount;
	fread(&tempCameraCount, sizeof(CInt), 1, filePtr);

	for (CInt i = 0; i < tempCameraCount; i++)
	{
		CChar cameraName[MAX_NAME_SIZE];
		CFloat pos[3], pan, tilt, fov, ncp, fcp;
		CBool cameraHasScript;
		CChar cameraScript[MAX_NAME_SIZE];

		fread(cameraName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(pos, sizeof(CFloat), 3, filePtr);
		fread(&pan, sizeof(CFloat), 1, filePtr);
		fread(&tilt, sizeof(CFloat), 1, filePtr);
		fread(&fov, sizeof(CFloat), 1, filePtr);
		fread(&ncp, sizeof(CFloat), 1, filePtr);
		fread(&fcp, sizeof(CFloat), 1, filePtr);
		fread(&cameraHasScript, sizeof(CBool), 1, filePtr);
		fread(&cameraScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(cameraScript);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Cameras/", cameraName, "/", scriptAfterPath);

		CInstanceCamera* instance_camera = new CInstanceCamera();
		CCamera* abstract_camera = new CCamera();
		CNode* parent = CNew(CNode);
		instance_camera->m_parent = parent;
		instance_camera->m_abstractCamera = abstract_camera;
		abstract_camera->SetName(cameraName);

		CVec3f vec_pos(pos[0], pos[1], pos[2]);
		instance_camera->SetPos(vec_pos);
		instance_camera->SetPan(pan);
		instance_camera->SetTilt(tilt);
		instance_camera->m_abstractCamera->SetAngle(fov);
		instance_camera->m_abstractCamera->SetMinAngle(MIN_CAMERA_ANGLE);
		instance_camera->m_abstractCamera->SetMaxAngle(MAX_CAMERA_ANGLE);
		instance_camera->SetNCP(ncp);
		instance_camera->SetFCP(fcp);
		instance_camera->SetHasScript(cameraHasScript);
		instance_camera->SetScript(script);

		instance_camera->SetIndex();

		instance_camera->MoveTransform2(vec_pos.x, vec_pos.y, vec_pos.z);
		instance_camera->SetPanAndTilt2(pan, tilt);
		instance_camera->ZoomTransform2(0.0f);
		instance_camera->LoadLuaFile();

		g_engineCameraInstances.push_back(instance_camera);
	}

	alGetError(); //clear possible sound errors

	//3D sounds
	CInt temp3DSoundCount;
	fread(&temp3DSoundCount, sizeof(CInt), 1, filePtr);
	CFloat ThreeDSoundMaxDistance, ThreeDSoundPitch, ThreeDSoundReferenceDistance, ThreeDSoundRolloff, ThreeDSoundVolume;
	CFloat ThreeDSoundPos[3];
	CBool play, loop;
	CBool soundHasScript;
	CChar soundScript[MAX_NAME_SIZE];

	CChar name[MAX_NAME_SIZE], path[MAX_NAME_SIZE];
	for (CInt i = 0; i < temp3DSoundCount; i++)
	{
		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading sound: ", name);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		fread(path, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&loop, sizeof(CBool), 1, filePtr);
		fread(&ThreeDSoundMaxDistance, sizeof(CFloat), 1, filePtr);
		fread(&ThreeDSoundPitch, sizeof(CFloat), 1, filePtr);
		fread(&play, sizeof(CBool), 1, filePtr);
		fread(ThreeDSoundPos, sizeof(CFloat), 3, filePtr);
		fread(&ThreeDSoundReferenceDistance, sizeof(CFloat), 1, filePtr);
		fread(&ThreeDSoundRolloff, sizeof(CFloat), 1, filePtr);
		fread(&ThreeDSoundVolume, sizeof(CFloat), 1, filePtr);
		fread(&soundHasScript, sizeof(CBool), 1, filePtr);
		fread(&soundScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar ThreeDSoundPath[MAX_NAME_SIZE];
		CChar* ThreeDSoundFileName = GetAfterPath(path);

		sprintf(ThreeDSoundPath, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/3D/", name, "/", ThreeDSoundFileName);

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(soundScript);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/3D/", name, "/", scriptAfterPath);

		COpenALSoundBuffer* m_3DSoundBuffer = CNew(COpenALSoundBuffer);

		if (!m_3DSoundBuffer->LoadOggVorbisFromFile(ThreeDSoundPath))
			MessageBoxA(NULL, "Couldn't load the 3D sound", "Report", MB_OK);

		// all of the buffers are loaded from one location and may be shared.
		//however, I continue to create the sound if it fails to load

		m_3DSoundBuffer->SetName(ThreeDSoundPath);

		COpenALSoundSource* m_3DSoundSource = CNew(COpenALSoundSource);
		C3DSound* m_3DSound = CNew(C3DSound);

		m_3DSoundSource->BindSoundBuffer(*m_3DSoundBuffer);
		m_3DSoundSource->SetLooping(loop);
		m_3DSoundSource->SetPitch(ThreeDSoundPitch);

		m_3DSoundSource->SetVolume( ThreeDSoundVolume );
		m_3DSoundSource->SetReferenceDistance(ThreeDSoundReferenceDistance);
		m_3DSoundSource->SetMaxDistance(ThreeDSoundMaxDistance);
		m_3DSoundSource->SetRolloff(ThreeDSoundRolloff);
		m_3DSoundSource->SetSoundPosition(ThreeDSoundPos);

		m_3DSound->SetName(name);
		m_3DSound->SetPath(ThreeDSoundPath);
		m_3DSound->SetPosition(ThreeDSoundPos);
		m_3DSound->SetLoop(loop);
		m_3DSound->SetMaxDistance(ThreeDSoundMaxDistance);
		m_3DSound->SetPitch(ThreeDSoundPitch);
		m_3DSound->SetPlay(play);
		m_3DSound->SetReferenceDistance(ThreeDSoundReferenceDistance);
		m_3DSound->SetRolloff(ThreeDSoundRolloff);
		m_3DSound->SetVolume(ThreeDSoundVolume);
		m_3DSound->SetSoundSource(m_3DSoundSource);
		m_3DSound->SetSoundBuffer(m_3DSoundBuffer);
		m_3DSound->SetHasScript(soundHasScript);
		m_3DSound->SetScript(script);
		m_3DSound->SetIndex();
		m_3DSound->LoadLuaFile();

		g_engine3DSounds.push_back(m_3DSound);
	}

	//Ambient Sound
	CUInt ambientSoundSize;
	fread(&ambientSoundSize, sizeof(CUInt), 1, filePtr);
	for (CUInt i = 0; i < ambientSoundSize; i++)
	{
		CChar strAmbientSoundName[MAX_NAME_SIZE];
		CChar strAmbientSoundPath[MAX_NAME_SIZE];
		CChar strAmbientSoundFileName[MAX_NAME_SIZE];
		CFloat volume, pitch;
		CBool soundHasScript;
		CChar soundScript[MAX_NAME_SIZE];

		fread(strAmbientSoundName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strAmbientSoundPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strAmbientSoundFileName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&volume, sizeof(CFloat), 1, filePtr);
		fread(&pitch, sizeof(CFloat), 1, filePtr);
		fread(&play, sizeof(CBool), 1, filePtr);
		fread(&loop, sizeof(CBool), 1, filePtr);
		fread(&soundHasScript, sizeof(CBool), 1, filePtr);
		fread(&soundScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar AmbientSoundPath[MAX_NAME_SIZE];

		sprintf(AmbientSoundPath, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/Ambient/", strAmbientSoundName, "/", strAmbientSoundFileName);

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(soundScript);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/Ambient/", strAmbientSoundName, "/", scriptAfterPath);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading ambient sound: ", strAmbientSoundName);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		COpenALSoundSource* m_ambientSoundSource = CNew(COpenALSoundSource);
		COpenALSoundBuffer* m_ambientSoundBuffer = CNew(COpenALSoundBuffer);

		//Initialize ambient sound here
		// Velocity of the source sound.
		m_ambientSoundBuffer->LoadOggVorbisFromFile(AmbientSoundPath);
		m_ambientSoundBuffer->SetName(strAmbientSoundPath);

		m_ambientSoundSource->BindSoundBuffer(*m_ambientSoundBuffer);

		m_ambientSoundSource->SetLooping(loop);
		m_ambientSoundSource->SetPitch(pitch);
		m_ambientSoundSource->SetVolume(volume);

		CAmbientSound* ambientSound = CNew(CAmbientSound);
		ambientSound->SetSoundSource(m_ambientSoundSource);
		ambientSound->SetSoundBuffer(m_ambientSoundBuffer);
		ambientSound->SetName(strAmbientSoundName);
		ambientSound->SetPath(AmbientSoundPath);
		ambientSound->SetVolume(volume);
		ambientSound->SetPitch(pitch);
		ambientSound->SetLoop(loop);
		ambientSound->SetPlay(play);
		ambientSound->SetSoundFileName(strAmbientSoundFileName);
		ambientSound->SetHasScript(soundHasScript);
		ambientSound->SetScript(script);

		ambientSound->LoadLuaFile();

		g_engineAmbientSounds.push_back(ambientSound);
		//CChar temp[MAX_NAME_SIZE];
		//sprintf( temp, "ambient sound '%s' loaded successfully\n", strAmbientSoundPath );
		//PrintInfo2( temp );
	}


	//Video
	CUInt videoSize;
	fread(&videoSize, sizeof(CUInt), 1, filePtr);
	for (CUInt i = 0; i < videoSize; i++)
	{
		CChar strVideoName[MAX_NAME_SIZE];
		CChar strVideoPath[MAX_NAME_SIZE];
		CChar strVideoFileName[MAX_NAME_SIZE];
		CFloat volume;
		CBool playVideo, loopVideo;
		CBool videoHasScript;
		CChar videoScript[MAX_NAME_SIZE];
		CBool ExitEscKey = CTrue;
		CBool pauseGameSoundsWhenStarting = CTrue;
		CBool resumeGameSoundsWhenFinished = CTrue;
		CBool PlayAudio = CTrue;

		fread(strVideoName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strVideoPath, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(strVideoFileName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&volume, sizeof(CFloat), 1, filePtr);
		fread(&playVideo, sizeof(CBool), 1, filePtr);
		fread(&loopVideo, sizeof(CBool), 1, filePtr);
		if (engine_version >= 211)
		{
			fread(&ExitEscKey, sizeof(CBool), 1, filePtr);
			fread(&PlayAudio, sizeof(CBool), 1, filePtr);
		}
		if (engine_version >= 230)
		{
			fread(&pauseGameSoundsWhenStarting, sizeof(CBool), 1, filePtr);
			fread(&resumeGameSoundsWhenFinished, sizeof(CBool), 1, filePtr);
		}
		fread(&videoHasScript, sizeof(CBool), 1, filePtr);
		fread(&videoScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		CChar VideoPath[MAX_NAME_SIZE];

		sprintf(VideoPath, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Videos/", strVideoName, "/", strVideoFileName);

		CChar script[MAX_NAME_SIZE];
		CChar* scriptAfterPath = GetAfterPath(videoScript);
		sprintf(script, "%s%s%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Videos/", strVideoName, "/", scriptAfterPath);

		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s%s", "Loading video: ", strVideoName);
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CVideo* video = CNew(CVideo);
		video->SetName(strVideoName);
		video->SetPath(VideoPath);
		video->SetVolume(volume);
		video->SetLoop(loopVideo);
		video->SetPlay2(playVideo);
		video->SetExitWithEscKey(ExitEscKey);
		video->SetPauseGameSoundsWhenStarting(pauseGameSoundsWhenStarting);
		video->SetResumeGameSoundsWhenFinished(resumeGameSoundsWhenFinished);
		video->SetPlayAudio(PlayAudio);
		video->SetVideoFileName(strVideoFileName);
		video->SetHasScript(videoHasScript);
		video->SetScript(script);
		
		video->LoadLuaFile();

		g_engineVideos.push_back(video);

		if (video->Load())
		{
			//PrintInfo("\nVideo '", COLOR_GREEN);
			//PrintInfo(video->GetName(), COLOR_RED_GREEN);
			//PrintInfo("' initialized successfully", COLOR_GREEN);
		}
		else
		{
			//PrintInfo("\nCouldn't initialize the video '", COLOR_RED);
			//PrintInfo(video->GetName(), COLOR_RED_GREEN);
			//PrintInfo("'", COLOR_RED);
		}
	}

	//Triggers
	CPrefab* box = CNew(CPrefab);
	CChar pr_name[MAX_NAME_SIZE];
	CChar package_name[MAX_NAME_SIZE];
	CChar prefab_name[MAX_NAME_SIZE];
	Cpy(package_name, "Vanda_Basics");
	Cpy(prefab_name, "Box_Trigger");
	sprintf(pr_name, "%s%s%s", package_name, "_", prefab_name);

	box->SetName(pr_name);
	box->SetPackageName(package_name);
	box->SetPrefabName(prefab_name);

	g_prefab.push_back(box);

	CPrefab* sphere = CNew(CPrefab);
	CChar pr_name2[MAX_NAME_SIZE];
	CChar package_name2[MAX_NAME_SIZE];
	CChar prefab_name2[MAX_NAME_SIZE];
	Cpy(package_name2, "Vanda_Basics");
	Cpy(prefab_name2, "Sphere_Trigger");
	sprintf(pr_name2, "%s%s%s", package_name2, "_", prefab_name2);

	sphere->SetName(pr_name2);
	sphere->SetPackageName(package_name2);
	sphere->SetPrefabName(prefab_name2);

	g_prefab.push_back(sphere);

	CInt triggerSize;
	fread(&triggerSize, sizeof(CInt), 1, filePtr);
	for (CInt i = 0; i < triggerSize; i++)
	{
		CChar trigger_name[MAX_NAME_SIZE];
		CChar instance_name[MAX_NAME_SIZE];
		CTriggerType type;
		CVec3f translation;
		CVec4f rotation;
		CVec3f scaling;
		CChar m_script[MAX_NAME_SIZE];
		CBool m_hasScript;
		fread(trigger_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&type, sizeof(CTriggerType), 1, filePtr);

		fread(instance_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&translation, sizeof(CVec3f), 1, filePtr);
		fread(&rotation, sizeof(CVec4f), 1, filePtr);
		fread(&scaling, sizeof(CVec3f), 1, filePtr);
		fread(m_script, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&m_hasScript, sizeof(CBool), 1, filePtr);

		CChar trimmed_script[MAX_NAME_SIZE];

		//read prefab data

		CTrigger* new_trigger = CNew(CTrigger);
		CPrefab* new_prefab;
		if (type == eTRIGGER_BOX)
		{
			new_trigger->SetTriggerType(eTRIGGER_BOX);
			new_prefab = box;
		}
		else
		{
			new_trigger->SetTriggerType(eTRIGGER_SPHERE);
			new_prefab = sphere;
		}

		new_trigger->SetName(trigger_name);

		CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
		g_currentInstancePrefab = new_instance_prefab;
		new_prefab->AddInstance(new_instance_prefab);
		new_prefab->SetCurrentInstance(new_instance_prefab);
		new_instance_prefab->SetPrefab(new_prefab);
		new_instance_prefab->SetName(instance_name);
		new_instance_prefab->SetNameIndex(); //for selection only
		new_instance_prefab->GenQueryIndex();
		new_instance_prefab->SetWater(NULL);
		g_instancePrefab.push_back(new_instance_prefab);
		Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
		InsertPrefab(new_prefab);
		new_instance_prefab->SetHasScript(new_prefab->GetHasScript());
		new_instance_prefab->SetScript(new_prefab->GetScript());
		new_instance_prefab->LoadLuaFile();

		new_trigger->SetInstancePrefab(g_instancePrefab[g_instancePrefab.size() - 1]); //last element
		new_trigger->GetInstancePrefab()->SetIsTrigger(CTrue);

		new_trigger->GetInstancePrefab()->SetTranslate(translation);
		new_trigger->GetInstancePrefab()->SetRotate(rotation);
		new_trigger->GetInstancePrefab()->SetScale(scaling);
		new_trigger->GetInstancePrefab()->UpdateBoundingBox();
		new_trigger->GetInstancePrefab()->CalculateDistance();
		//new_trigger->GetInstancePrefab()->UpdateIsStaticOrAnimated();

		CChar* scriptAfterPath = GetAfterPath(m_script);
		sprintf(trimmed_script, "%s%s%s%s/%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Triggers/", trigger_name, scriptAfterPath);

		new_trigger->SetScript(trimmed_script);
		new_trigger->SetHasScript(m_hasScript);
		new_trigger->LoadLuaFile();

		g_triggers.push_back(new_trigger);
	}
	CBool hasCharacter;
	fread(&hasCharacter, sizeof(CBool), 1, filePtr);
	g_databaseVariables.m_insertCharacter = hasCharacter;

	if (hasCharacter)
	{
		CChar Message[MAX_NAME_SIZE];
		sprintf(Message, "%s", "Loading main character");
		MSG lpMsg;
		HWND hwnd = NULL;
		while (PeekMessage(&lpMsg, hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&lpMsg);		// translate and dispatch to event queue
			DispatchMessage(&lpMsg);
		}
		ShowLoadingScene(Message);

		if (!g_mainCharacter)
			g_mainCharacter = CNew(CMainCharacter);

		CPhysXCameraType type;
		CChar name[MAX_NAME_SIZE];
		CChar packageName[MAX_NAME_SIZE];
		CChar prefabName[MAX_NAME_SIZE];
		CChar m_script[MAX_NAME_SIZE];
		CBool m_hasScript;

		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(packageName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(prefabName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&type, sizeof(CTriggerType), 1, filePtr);
		fread(m_script, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&m_hasScript, sizeof(CBool), 1, filePtr);

		CChar trimmed_script[MAX_NAME_SIZE];

		g_mainCharacter->SetName(name);
		g_mainCharacter->SetPackageName(packageName);
		g_mainCharacter->SetPrefabName(prefabName);
		g_mainCharacter->SetCameraType(type);

		CChar instance_name[MAX_NAME_SIZE];
		CVec3f translation;
		CVec4f rotation;
		CVec3f scaling;
		CFloat delayIdle;
		//save instance data
		fread(instance_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&translation, sizeof(CVec3f), 1, filePtr);
		fread(&rotation, sizeof(CVec4f), 1, filePtr);
		fread(&scaling, sizeof(CVec3f), 1, filePtr);
		fread(&delayIdle, sizeof(CFloat), 1, filePtr);
		g_mainCharacter->SetIdleDelayForRandomPlay(delayIdle);

		CChar strWalkSoundPath[MAX_URI_SIZE];
		CChar strRunSoundPath[MAX_URI_SIZE];
		CChar strJumpSoundPath[MAX_URI_SIZE];

		fread(strWalkSoundPath, sizeof(CChar), MAX_URI_SIZE, filePtr);
		fread(strRunSoundPath, sizeof(CChar), MAX_URI_SIZE, filePtr);
		fread(strJumpSoundPath, sizeof(CChar), MAX_URI_SIZE, filePtr);

		//walk sound
		CChar walkSoundPath[MAX_NAME_SIZE];
		CChar* walkSoundName = GetAfterPath(strWalkSoundPath);

		sprintf(walkSoundPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Character/", walkSoundName);
		g_mainCharacter->SetWalkSoundPath(walkSoundPath);

		CChar runSoundPath[MAX_NAME_SIZE];
		CChar* runSoundName = GetAfterPath(strRunSoundPath);

		sprintf(runSoundPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Character/", runSoundName);
		g_mainCharacter->SetRunSoundPath(runSoundPath);

		CChar jumpSoundPath[MAX_NAME_SIZE];
		CChar* jumpSoundName = GetAfterPath(strJumpSoundPath);

		sprintf(jumpSoundPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Character/", jumpSoundName);
		g_mainCharacter->SetJumpSoundPath(jumpSoundPath);

		g_mainCharacter->SetWalkSound();
		g_mainCharacter->SetRunSound();
		g_mainCharacter->SetJumpSound();

		CPrefab* new_prefab = CNew(CPrefab);

		CChar pr_name[MAX_NAME_SIZE];
		sprintf(pr_name, "%s%s%s", packageName, "_", prefabName);

		new_prefab->SetName(pr_name);
		new_prefab->SetPackageName(packageName);
		new_prefab->SetPrefabName(prefabName);

		g_prefab.push_back(new_prefab);

		CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
		g_currentInstancePrefab = new_instance_prefab;
		new_prefab->AddInstance(new_instance_prefab);
		new_prefab->SetCurrentInstance(new_instance_prefab);
		new_instance_prefab->SetPrefab(new_prefab);
		new_instance_prefab->SetName(instance_name);
		new_instance_prefab->SetNameIndex(); //for selection only
		new_instance_prefab->GenQueryIndex();
		new_instance_prefab->SetWater(NULL);
		g_instancePrefab.push_back(new_instance_prefab);
		Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
		InsertPrefab(new_prefab);

		CChar* scriptAfterPath = GetAfterPath(m_script);
		sprintf(trimmed_script, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Character/", scriptAfterPath);

		g_mainCharacter->SetScriptPath(trimmed_script);
		g_mainCharacter->SetHasScript(m_hasScript);

		if(m_hasScript)
			g_mainCharacter->LoadLuaFile();

		new_instance_prefab->SetHasScript(new_prefab->GetHasScript());
		new_instance_prefab->SetScript(new_prefab->GetScript());
		new_instance_prefab->LoadLuaFile();

		g_instancePrefab[g_instancePrefab.size() - 1]->SetName("VANDA_MAIN_CHARACTER");

		g_mainCharacter->SetInstancePrefab(g_instancePrefab[g_instancePrefab.size() - 1]); //last element

		g_mainCharacter->GetInstancePrefab()->SetTranslate(translation);
		g_mainCharacter->GetInstancePrefab()->SetRotate(rotation);
		g_mainCharacter->GetInstancePrefab()->SetScale(scaling);
		g_mainCharacter->GetInstancePrefab()->UpdateBoundingBox();
		g_mainCharacter->GetInstancePrefab()->CalculateDistance();
		//g_mainCharacter->GetInstancePrefab()->UpdateIsStaticOrAnimated();
		g_mainCharacter->SetCurrentRotation(rotation.y);
		g_mainCharacter->SetPosition(translation);
		g_camera->m_perspectiveCameraYaw = NxMath::degToRad(rotation.y) + NxMath::degToRad(180.f);


		g_main->RenderCharacter(CFalse);
		//save main actions
		CUInt idleSize;
		CUInt walkSize;
		CUInt runSize;
		CUInt jumpSize;
		std::vector<std::string> idleName;
		std::vector<std::string> walkName;
		std::vector<std::string> jumpName;
		std::vector<std::string> runName;

		fread(&idleSize, sizeof(CUInt), 1, filePtr);
		for (CUInt i = 0; i < idleSize; i++)
		{
			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			idleName.push_back(name);
		}
		g_mainCharacter->SetIdleName(idleName);

		fread(&walkSize, sizeof(CUInt), 1, filePtr);
		for (CUInt i = 0; i < walkSize; i++)
		{
			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			walkName.push_back(name);
		}
		g_mainCharacter->SetWalkName(walkName);

		fread(&jumpSize, sizeof(CUInt), 1, filePtr);
		for (CUInt i = 0; i < jumpSize; i++)
		{
			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			jumpName.push_back(name);
		}
		g_mainCharacter->SetJumpName(jumpName);

		fread(&runSize, sizeof(CUInt), 1, filePtr);
		for (CUInt i = 0; i < runSize; i++)
		{
			CChar name[MAX_NAME_SIZE];
			fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			runName.push_back(name);
		}
		g_mainCharacter->SetRunName(runName);
		std::vector<std::string> idleName1 = g_mainCharacter->GetIdleName();
		std::vector<std::string> walkName1 = g_mainCharacter->GetWalkName();
		std::vector<std::string> jumpName1 = g_mainCharacter->GetJumpName();
		std::vector<std::string> runName1 = g_mainCharacter->GetRunName();

		CBool isSceneVisible = g_mainCharacter->GetInstancePrefab()->GetSceneVisible(0);
		g_mainCharacter->GetInstancePrefab()->SetSceneVisible(0, CTrue);
		CFloat idleDelayIn = g_characterBlendingProperties.m_idleDelayIn;
		g_characterBlendingProperties.m_idleDelayIn = 0.0f;
		g_main->ManageCharacterBlends("idle", (CChar*)idleName1[0].c_str());
		g_main->m_idleCounter = 0.0f;
		g_characterBlendingProperties.m_idleDelayIn = idleDelayIn;
		g_mainCharacter->GetInstancePrefab()->SetSceneVisible(0, isSceneVisible);

		g_main->m_characterRotationTransition = CTrue;
		g_mainCharacter->SetCurrentRotation(g_mainCharacter->GetInstancePrefab()->GetRotate().y);

	}
	GLdouble angle;
	GLdouble minAngle;
	GLdouble maxAngle;

	fread(&angle, sizeof(GLdouble), 1, filePtr);
	fread(&minAngle, sizeof(GLdouble), 1, filePtr);
	fread(&maxAngle, sizeof(GLdouble), 1, filePtr);

	g_camera->m_cameraManager->SetAngle(angle);
	g_camera->m_cameraManager->SetDefaultAngle(angle);
	g_camera->m_cameraManager->SetMinAngle(minAngle);
	g_camera->m_cameraManager->SetMaxAngle(maxAngle);

	fread(&g_camera->m_perspectiveCameraTilt, sizeof(CFloat), 1, filePtr);
	fread(&g_camera->m_perspectiveCameraMinTilt, sizeof(CFloat), 1, filePtr);
	fread(&g_camera->m_perspectiveCameraMaxTilt, sizeof(CFloat), 1, filePtr);

	g_camera->m_perspectiveCurrentCameraTilt = g_camera->m_perspectiveCameraTilt;

	//VScene Script object
	CBool insertVSceneScript;
	fread(&insertVSceneScript, sizeof(CBool), 1, filePtr);
	if (insertVSceneScript)
	{
		CChar message[MAX_NAME_SIZE];
		sprintf(message, "%s", "Loading VScene scripts");
		MSG msg;
		HWND m_hwnd = NULL;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate and dispatch to event queue
			DispatchMessage(&msg);
		}
		ShowLoadingScene(message);

		CChar name[MAX_NAME_SIZE];
		CChar path[MAX_URI_SIZE];

		fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(path, sizeof(CChar), MAX_URI_SIZE, filePtr);

		CChar* tempPath = GetAfterPath(path);

		CChar newPath[MAX_URI_SIZE];
		sprintf(newPath, "%s%s%s%s", "Assets/VScenes/", g_currentVSceneNameWithoutDot, "/VSceneScript/", tempPath);
		if (!g_VSceneScript)
			g_VSceneScript = CNew(CVSceneScript);
		g_VSceneScript->SetName(name);
		g_VSceneScript->SetScriptPath(newPath);
		g_VSceneScript->SetUpdateScript(CFalse);
		g_VSceneScript->LoadLuaFile();

		g_databaseVariables.m_insertVSceneScript = CTrue;

	}
	else
	{
		g_databaseVariables.m_insertVSceneScript = CFalse;
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		CScene* scene = NULL;
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				if (scene)
				{
					g_render.SetScene(scene);
					g_render.GetScene()->Update(0.001f, CTrue);
					g_instancePrefab[i]->SetSceneVisible(j, CTrue);
				}
			}
		}

		if (prefab && g_instancePrefab[i]->GetHasCollider())
		{
			scene = g_instancePrefab[i]->GetScene(3);
			if (scene)
			{
				g_render.SetScene(scene);
				g_render.GetScene()->Update(0.001f, CTrue);
			}
		}

		g_instancePrefab[i]->UpdateBoundingBox(CTrue);//init
		g_instancePrefab[i]->UpdateIsStaticOrAnimated();//init
	}

	CChar message[MAX_NAME_SIZE];
	sprintf(message, "%s", "Generating physics");
	MSG msg;
	HWND m_hwnd = NULL;
	while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);		// translate and dispatch to event queue
		DispatchMessage(&msg);
	}
	ShowLoadingScene(message);

	//generate physX colliders
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_currentInstancePrefab = g_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				if (scene)
				{
					for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
					{
						CPhysXMaterial physicsMaterial;
						physicsMaterial.HasMaterial = scene->m_instanceGeometries[k]->HasPhysicsMaterial();
						physicsMaterial.Restitution = scene->m_instanceGeometries[k]->GetPhysicsRestitution();
						physicsMaterial.SkinWidth = scene->m_instanceGeometries[k]->GetPhysicsSkinWidth();
						physicsMaterial.StaticFriction = scene->m_instanceGeometries[k]->GetPhysicsStaticFriction();
						physicsMaterial.DynamicFriction = scene->m_instanceGeometries[k]->GetPhysicsDynamicFriction();

						if (scene->m_instanceGeometries[k]->m_hasPhysX && scene->m_controllers.size())
						{
							scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, g_currentInstancePrefab);
						}
						else if (scene->m_instanceGeometries[k]->m_hasPhysX)
						{
							scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, NULL);
						}
					}
					scene->m_update = CFalse;
				}
			}
		}
		if (g_currentInstancePrefab->GetHasCollider())
		{
			scene = g_instancePrefab[i]->GetScene(3);
			if (scene)
			{
				for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
				{
					CPhysXMaterial physicsMaterial;
					physicsMaterial.HasMaterial = scene->m_instanceGeometries[k]->HasPhysicsMaterial();
					physicsMaterial.Restitution = scene->m_instanceGeometries[k]->GetPhysicsRestitution();
					physicsMaterial.SkinWidth = scene->m_instanceGeometries[k]->GetPhysicsSkinWidth();
					physicsMaterial.StaticFriction = scene->m_instanceGeometries[k]->GetPhysicsStaticFriction();
					physicsMaterial.DynamicFriction = scene->m_instanceGeometries[k]->GetPhysicsDynamicFriction();

					if (scene->m_instanceGeometries[k]->m_hasPhysX && scene->m_controllers.size())
					{
						scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, g_currentInstancePrefab);
					}
					else if (scene->m_instanceGeometries[k]->m_hasPhysX)
					{
						scene->GeneratePhysX(scene->m_instanceGeometries[k]->m_lodAlgorithm, scene->m_instanceGeometries[k]->m_physXDensity, scene->m_instanceGeometries[k]->m_physXPercentage, scene->m_instanceGeometries[k]->m_isTrigger, scene->m_instanceGeometries[k]->m_isInvisible, physicsMaterial, scene->m_instanceGeometries[k], CFalse, NULL);
					}
				}
				scene->m_update = CFalse;
			}
		}
	}

	if (g_physXProperties.m_bGroundPlane)
	{
		NxVec3 rot0(0, 0, 0);
		NxVec3 rot1(0, 0, 0);
		NxVec3 rot2(0, 0, 0);
		NxMat33 rot(rot0, rot1, rot2);

		CPhysXMaterial physicsMaterial;

		g_nx->m_groundBox = g_nx->CreateBox(NxVec3(0.0f, g_physXProperties.m_fGroundHeight - 0.5, 0.0f), NxVec3(100.0f, 0.01f, 100.0f), 0, rot, NULL, CFalse, CFalse, physicsMaterial);
		g_nx->SetActorCollisionGroup(g_nx->m_groundBox, GROUP_GROUND);
	}
	
	m_soundSystem->SetListenerGain(0); //mute listener for now

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		if (g_engine3DSounds[i]->GetPlay())
			m_soundSystem->PlayALSound(*(g_engine3DSounds[i]->GetSoundSource()));
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		if (g_engineAmbientSounds[i]->GetPlay())
			m_soundSystem->PlayALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		if (g_engineVideos[i]->GetPlay())
			g_engineVideos[i]->SetPlay(CTrue); //initialize video
	}

	if (m_pauseAllSounds)
	{
		PauseSounds();
	}

	if (m_pauseAllUpdateEvents)
	{
		PauseAllScriptUpdateEvents();
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->InitScript();
	}

	if (g_VSceneScript)
		g_VSceneScript->InitScript();

	if (g_skyDome)
		g_skyDome->InitScript();

	if (g_terrain)
		g_terrain->InitScript();

	if (g_mainCharacter)
		g_mainCharacter->InitScript();

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		if (g_engineWaters[i]->GetHasScript())
			g_engineWaters[i]->InitScript();
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		if (g_engineLights[i]->m_abstractLight->GetHasScript())
			g_engineLights[i]->m_abstractLight->InitScript();
	}

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		if (g_engineCameraInstances[i]->GetHasScript())
			g_engineCameraInstances[i]->InitScript();
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		if (g_engineAmbientSounds[i]->GetHasScript())
			g_engineAmbientSounds[i]->InitScript();
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		if (g_engine3DSounds[i]->GetHasScript())
			g_engine3DSounds[i]->InitScript();
	}

	for (CUInt i = 0; i < g_engineVideos.size(); i++)
	{
		if (g_engineVideos[i]->GetHasScript())
			g_engineVideos[i]->InitScript();
	}

	//if( g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX )
	//{

	//g_nx->gControllers->reportSceneChanged();
	//gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
	//gPhysXscene->flushStream();
	//gPhysXscene->fetchResults(NX_ALL_FINISHED, true);

	//}
	g_selectedName = -1;

	fclose( filePtr );

	//if (m_showMenuCursor)
	//{
		//Load Cursor Icon
		CChar cursorPath[MAX_NAME_SIZE];
		CChar* tempCursorPath = GetAfterPath(cursorName);

		sprintf(cursorPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Cursor/", tempCursorPath);
		GenerateMenuCursorTexture(cursorPath);
	//}

	m_dof.InitFBOs( g_width, g_height );

	RenderQueries(CTrue);
	CalculateDistnces();
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetLightCooked(CFalse);
	}
	ResetTimer();

	g_clickedOpen = CFalse;
	return CTrue;

}

CVoid CMain::InitPrefabSelection(CDouble mouseXPos, CDouble mouseYPos, CDouble selectionWidth, CDouble selectionHeight)
{
	GLint viewport[4];
	glSelectBuffer(MAX_NAME_SIZE, Buffer);
	(void)glRenderMode(GL_SELECT);
	g_render.ProjectionMatrix();
	g_render.PushMatrix();
	g_render.IdentityMatrix();
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluPickMatrix(mouseXPos - (selectionWidth / 2.0), mouseYPos - (selectionHeight / 2.0), selectionWidth, selectionHeight, viewport);

	if (g_currentCameraType == eCAMERA_PHYSX)
	{
		gluPerspective(g_camera->m_cameraManager->GetAngle(), CDouble(g_width) / CDouble(g_height), g_cameraProperties.m_playModePerspectiveNCP, g_cameraProperties.m_playModePerspectiveFCP);
	}
	g_render.ModelViewMatrix();
	glInitNames();
}

CVoid CMain::FinishPrefabSelection()
{
	g_render.ProjectionMatrix();
	g_render.PopMatrix();
	g_render.ModelViewMatrix();
	glFlush();
	GLint hits = glRenderMode(GL_RENDER);

	if (hits > 0)
	{
		ptr = Buffer;
		for (CInt j = 0; j < hits; j++)
		{
			ptr++; //skip the first info ( number of names which is one in my engine )and move to the min depth info

			if (*ptr < minZ)
			{
				minZ = *ptr;

				m_selectedPrefabName = *(ptr + 2); //name of the object
			}
			ptr += 3; //move to the next hit
		}
	}
}

CChar* CMain::SelectPrefabInstances(CDouble mouseXPos, CDouble mouseYPos, CDouble selectionWidth, CDouble selectionHeight)
{
	minZ = 0xffffffff;
	m_selectedPrefabName = -1;
	glPushAttrib(GL_VIEWPORT_BIT);
	
	glViewport(0, 0, g_width, g_height);// resets the viewport to new dimensions.

	g_render.m_useShader = CFalse;
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;

		g_currentInstancePrefab = g_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		scene = g_instancePrefab[i]->GetScene(0);

		if (!scene) continue;

		if (!scene->m_isSelectable) continue;

		if (m_bSelectionDistance)
		{
			if (g_currentInstancePrefab->GetDistanceFromCamera() > m_fSelectionDistance)
				continue;
		}

		InitPrefabSelection(mouseXPos, mouseYPos, selectionWidth, selectionHeight);
		glPushName(g_instancePrefab[i]->GetNameIndex());
		//g_scene[i]->RenderAABBWithQuads(); //Here's the code to "draw" the objects in selection mode
		g_render.SetScene(scene);
		//g_render.GetScene()->Update();
		if (!g_render.GetScene()->m_isTrigger /*&& g_render.GetScene()->m_hasAnimation*/)
		{
			g_render.GetScene()->Render(CTrue, NULL, CFalse);
			if (g_currentInstancePrefab)
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();
				g_render.MultMatrix(*(g_currentInstancePrefab->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(CTrue, CTrue);
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(CTrue, CFalse);

			g_render.GetScene()->RenderModelsControlledByPhysX(CTrue);
		}

		glPopName();
		FinishPrefabSelection();
	}
	if (g_render.UsingShader())
		g_render.m_useShader = CTrue;


	glPopAttrib();

	Cpy(m_selectedPrefabInstanceName, ""); //no name by default

	//run OnSelect function
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (m_selectedPrefabName != -1 && g_instancePrefab[i]->GetNameIndex() == m_selectedPrefabName)
		{
			g_instancePrefab[i]->OnSelectScript();
			Cpy(m_selectedPrefabInstanceName, g_instancePrefab[i]->GetName());
			break;
		}
	}
	return m_selectedPrefabInstanceName;
}


CBool CMain::InitFBOs( CInt channels, CInt type )
{
	//glGetIntegerv( GL_MAX_DRAW_BUFFERS, &g_numBuffers );

	CUInt numSamples;
	GLenum status;
	if( g_window.m_windowGL.multiSampling ) //initialize 2 separate FBOs
	{
		switch ( /*g_options.m_numSamples*/g_window.m_numSamples)
		{
		case 2:
			numSamples = 2;
			break;
		case 4:
			numSamples = 4;
			break;
		case 8:
			numSamples = 8;
			break;
		case 16:
			numSamples = 16;
			break;
		default:
			numSamples = 0;
		}
		GLint samples;
		glGetIntegerv(GL_MAX_SAMPLES_EXT, &samples);
		if( numSamples > (CUInt)samples )
			numSamples = samples;
		m_mFboID = g_render.GenerateFBO();
		g_render.BindFBO( m_mFboID );
		//g_render.BindForWriting( m_mFboID );
		//g_render.Attach2DTextureToFBOColor( m_bloomTexture );
		m_rbDepthID = g_render.GenerateRenderBuffer();
		g_render.BindRenderBuffer( m_rbDepthID );
		g_render.RenderbufferDepthStorageMultisample( numSamples, g_width, g_height );
		g_render.AttachDepthToFBO( m_rbDepthID );

		for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
		{
			m_rbColorID[i] = g_render.GenerateRenderBuffer();
			g_render.BindRenderBuffer( m_rbColorID[i] );
			g_render.RenderbufferColorStorageMultisample( numSamples, type, g_width, g_height );
			g_render.AttachRenderBufferToFBOColor( m_rbColorID[i], i );
		}
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
		glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);

		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				break;
			default:
				MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine Error",  MB_OK | MB_ICONERROR);
				break;
		}

		g_render.BindFBO(0);
		g_render.BindRenderBuffer(0);

	}

	for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
	{
		glGenTextures(1, &m_textureTarget[i] );								
		glBindTexture(GL_TEXTURE_2D, m_textureTarget[i] );					
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if( i == eGBUFFER_TEXTURE_TYPE_POSITION || i == eGBUFFER_TEXTURE_TYPE_NORMAL ) //vertex or normal
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_width, g_height, 0, GL_RGBA, GL_FLOAT, NULL);	
		else
    		glTexImage2D(GL_TEXTURE_2D, 0, channels, g_width, g_height, 0, type, GL_UNSIGNED_BYTE, NULL );
	}

	m_fboID = g_render.GenerateFBO();
	g_render.BindFBO( m_fboID );
	//g_render.BindForWriting( m_fboID );


	for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
	{
		g_render.Attach2DTextureToFBOColor( m_textureTarget[i], i );
	}

	m_rbDepthID2 = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbDepthID2 );
	g_render.RenderbufferStorage( g_width, g_height );
	g_render.AttachDepthToFBO( m_rbDepthID2 );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
	glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine Error",  MB_OK | MB_ICONERROR);
			break;
	}

	g_render.BindFBO( 0 );
	g_render.BindRenderBuffer(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return CTrue;

}

CVoid CMain::SetInstanceCamera(CInstanceCamera * inst, CFloat sWidth, CFloat sHeight, CFloat fov, CFloat zNear, CFloat zFar)
{
	//previous position of the perspective camera (used while instanced camera is enabled)
	g_camera->m_perspectiveCameraPosOfPreviousFrame = g_camera->m_perspectiveCameraPos;

	// Get the camera from the instance and set the projection matrix from it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,
					( sWidth / sHeight ),
					zNear,
					zFar);

	if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		//shadow
		glGetFloatv(GL_PROJECTION_MATRIX, cam_proj);
	}

	// Now get the node from the instance, and set the modelview matrix to the inverse of 
	// the node's matrix.
	glMatrixMode(GL_MODELVIEW);
	CMatrix mat;
	const CMatrix *ltow;
	ltow = inst->m_parent->GetLocalToWorldMatrix();
	CMatrixCopy(*ltow, mat); 
	//CMatrix3x4Invert( mat, mat);
	//LoadMatrix( mat );
	CVec3f at(0,0,-1),pos( 0,0,0);
	CMatrixTransform( mat, at, free_dae_cam_at );
	CMatrixTransform( mat, pos, free_dae_cam_pos );
	g_camera->m_perspectiveCameraPos.x = free_dae_cam_pos.x;
	g_camera->m_perspectiveCameraPos.y = free_dae_cam_pos.y;
	g_camera->m_perspectiveCameraPos.z = free_dae_cam_pos.z;

	//it's used for OpenAL
	g_camera->m_perspectiveCameraDir.x = -(free_dae_cam_at.x - free_dae_cam_pos.x);
	g_camera->m_perspectiveCameraDir.y = free_dae_cam_at.y - free_dae_cam_pos.y;
	g_camera->m_perspectiveCameraDir.z = -(free_dae_cam_at.z - free_dae_cam_pos.z);

	//shadow 
	cam_pos[0] = g_camera->m_perspectiveCameraPos.x;
	cam_pos[1] = g_camera->m_perspectiveCameraPos.y;
	cam_pos[2] = g_camera->m_perspectiveCameraPos.z;

	cam_dir[0] = -g_camera->m_perspectiveCameraDir.x;
	cam_dir[1] = g_camera->m_perspectiveCameraDir.y;
	cam_dir[2] = -g_camera->m_perspectiveCameraDir.z;

	g_render.ModelViewMatrix();
	g_render.IdentityMatrix();
	gluLookAt(free_dae_cam_pos.x, free_dae_cam_pos.y, free_dae_cam_pos.z, free_dae_cam_at.x, free_dae_cam_at.y, free_dae_cam_at.z, 0.0, 1.0, 0.0);
	//if( g_shadowProperties.m_enable && g_render.UsingShadowShader()  && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	//{
		// store the inverse of the resulting modelview matrix for the shadow computation
		glGetFloatv(GL_MODELVIEW_MATRIX, cam_modelview);
		g_camera->m_cameraManager->GetInverseMatrix( cam_inverse_modelview );
	//}
}

CVoid CMain::UpdateDynamicPhysicsObjects()
{
	//3D Model data
	CBool foundDynamicPhysX = CFalse;
	if (gPhysXscene)
	{
		for (CUInt j = 0; j < gPhysXscene->getNbActors(); j++)
		{
			if (!gPhysXscene->getActors()[j]->isSleeping())
				foundDynamicPhysX = CTrue;
		}
	}

	if (foundDynamicPhysX)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (!g_instancePrefab[i]->GetIsControlledByPhysX()) continue;

			g_currentInstancePrefab = g_instancePrefab[i];

			CScene* scene = NULL;

			CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
			for (CUInt j = 0; j < 3; j++)
			{
				if (prefab && prefab->GetHasLod(j))
				{
					scene = g_instancePrefab[i]->GetScene(j);
					if (!scene) continue;
					g_render.SetScene(scene);

					if (g_render.GetScene()->m_update)
					{
						g_render.GetScene()->Update();
						g_render.GetScene()->m_update = CFalse;
					}
					g_render.GetScene()->UpdateDynamicPhysicsObjects();
				}
			}
			//update collider
			if (g_instancePrefab[i]->GetHasCollider())
			{
				scene = g_instancePrefab[i]->GetScene(3);
				if (!scene) continue;
				g_render.SetScene(scene);

				if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				}
				g_render.GetScene()->UpdateDynamicPhysicsObjects();
			}
		}
	}
}

CVoid CMain::UpdateAnimations(CBool init)
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		if (!g_instancePrefab[i]->GetIsAnimated() && !g_instancePrefab[i]->GetIsControlledByPhysX()) continue;
		if (!g_instancePrefab[i]->GetUpdateAnimation()) continue;

		g_currentInstancePrefab = g_instancePrefab[i];

		if (Cmp(g_currentInstancePrefab->GetName(), "VANDA_MAIN_CHARACTER")) continue;

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 4; j++)
		{
			CBool update = CFalse;
			if (j < 3)
			{
				if (prefab && prefab->GetHasLod(j))
					update = CTrue;
			}
			else if (j == 3)
			{
				if (prefab && g_instancePrefab[i]->GetHasCollider())
					update = CTrue;
			}
			if (update)
			{
				scene = g_instancePrefab[i]->GetScene(j);
				if (!scene) continue;
				g_render.SetScene(scene);
				if (!g_render.GetScene()->m_isTrigger)
				{
					if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
					{
						if ((g_render.GetScene()->GetAnimationStatus() == eANIM_PLAY && g_render.GetScene()->UpdateAnimationLists()) || g_render.GetScene()->IsTransformable())
						{
							g_render.GetScene()->Update(elapsedTime);
							g_render.GetScene()->m_updateAnimation = CFalse;
							g_render.GetScene()->m_update = CFalse;
							g_render.GetScene()->SetUpdateBB(CTrue);
						} //if
						else if (init)
						{
							g_render.GetScene()->Update(0.0f);
							g_render.GetScene()->m_updateAnimation = CFalse;
							g_render.GetScene()->m_update = CFalse;
						}
						else if (g_render.GetScene()->m_update)
						{
							g_render.GetScene()->Update();
							g_render.GetScene()->m_update = CFalse;
						} //else

					} //if
					else if (g_render.GetScene()->m_update)
					{
						g_render.GetScene()->Update();
						g_render.GetScene()->m_update = CFalse;
					} //else
				} //if
			}
		}
	} //for
}
CVoid CMain::Render3DModelsControlledByPhysX(CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		if (!g_instancePrefab[i]->GetIsControlledByPhysX()) continue;

		g_currentInstancePrefab = g_instancePrefab[i];

		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() >(g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_currentInstancePrefab->SetLight();
			g_render.GetScene()->RenderModelsControlledByPhysX(sceneManager);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}

}

CVoid CMain::Render3DModelsControlledByPhysXForWater(CWater* water, CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < water->GetNumPrefabInstances(); i++)
	{
		if (!water->GetPrefabInstance(i)->GetVisible()) continue;
		if (!water->GetPrefabInstance(i)->GetIsControlledByPhysX()) continue;
		g_currentInstancePrefab = water->GetPrefabInstance(i);

		CVec3f src[8];
		CVec3f maxAABB = water->GetPrefabInstance(i)->GetInverseMaxAABB();
		CVec3f minAABB = water->GetPrefabInstance(i)->GetInverseMinAABB();
		src[0].x = minAABB.x; src[0].y = minAABB.y; src[0].z = minAABB.z;
		src[1].x = maxAABB.x; src[1].y = minAABB.y; src[1].z = maxAABB.z;
		src[2].x = maxAABB.x; src[2].y = minAABB.y; src[2].z = minAABB.z;
		src[3].x = minAABB.x; src[3].y = minAABB.y; src[3].z = maxAABB.z;
		src[4].x = maxAABB.x; src[4].y = maxAABB.y; src[4].z = minAABB.z;
		src[5].x = minAABB.x; src[5].y = maxAABB.y; src[5].z = minAABB.z;
		src[6].x = minAABB.x; src[6].y = maxAABB.y; src[6].z = maxAABB.z;
		src[7].x = maxAABB.x; src[7].y = maxAABB.y; src[7].z = maxAABB.z;

		if (!g_camera->m_cameraManager->IsBoxInFrustum(&src[0], 8))
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CFalse);
			continue;
		}
		else
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CTrue);
		}

		g_currentInstancePrefab = water->GetPrefabInstance(i);

		CScene* scene = NULL;

		CPrefab* prefab = water->GetPrefabInstance(i)->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->GetPrefabInstance(i)->GetSceneVisible(j))
			{
				scene = water->GetPrefabInstance(i)->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_currentInstancePrefab->SetLight();
			g_render.GetScene()->RenderModelsControlledByPhysX(sceneManager);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}

}

CVoid CMain::Render3DTransparentModels(CBool renderTop)
{
	if (!renderTop)
	{
		if (g_engineWaters.size() == 0)
			return;

		for (CUInt i = 0; i < m_transparentGeometries.size(); i++)
		{
			CBool foundTarget = CFalse;
			int index = -1;
			for (CUInt j = 0; j < g_engineWaters.size(); j++)
			{
				if (g_engineWaters[j]->GetVisible() && g_engineWaters[j]->GetQueryVisible())
				{
					if (DoesGeometryInstanceIntersectsWater(m_transparentGeometries[i].m_instanceGeometry, g_engineWaters[j]))
					{
						index = j;
						foundTarget = CTrue;
						break;
					}
				}
			}
			if (!foundTarget)
				continue;

			CDouble plane[4] = { 0.0, -1.0, 0.0, g_engineWaters[index]->m_fWaterCPos[1] };
			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, plane);

			g_currentInstancePrefab = m_transparentGeometries[i].m_instancePrefab;
			CScene* scene = m_transparentGeometries[i].m_scene;
			g_render.SetScene(scene);
			g_currentInstancePrefab->SetLight();

			CGeometry* geometry = m_transparentGeometries[i].m_instanceGeometry->m_abstractGeometry;

			if (m_transparentGeometries[i].m_instanceGeometry->m_hasPhysX && m_transparentGeometries[i].m_instanceGeometry->m_physXDensity > 0.0f) //render with physics
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();

				g_render.MultMatrix(m_transparentGeometries[i].m_instanceGeometry->m_localToWorldMatrixControlledByPhysX);

				geometry->m_currentInstanceGeometry = m_transparentGeometries[i].m_instanceGeometry;
				geometry->Draw(NULL, m_transparentGeometries[i].m_instanceGeometry);

				g_render.PopMatrix();
			}
			else
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();

				g_render.MultMatrix(m_transparentGeometries[i].m_instanceGeometry->m_localToWorldMatrix);
				geometry->m_currentInstanceGeometry = m_transparentGeometries[i].m_instanceGeometry;
				geometry->Draw(NULL, m_transparentGeometries[i].m_instanceGeometry);

				g_render.PopMatrix();
			}

			glDisable(GL_CLIP_PLANE0);
		}
	}
	else
	{
		for (CUInt i = 0; i < m_transparentGeometries.size(); i++)
		{
			CBool foundTarget = CFalse;
			int index = -1;
			for (CUInt j = 0; j < g_engineWaters.size(); j++)
			{
				if (g_engineWaters[j]->GetVisible() && g_engineWaters[j]->GetQueryVisible())
				{
					if (DoesGeometryInstanceIntersectsWater(m_transparentGeometries[i].m_instanceGeometry, g_engineWaters[j]))
					{
						index = j;
						foundTarget = CTrue;
						break;
					}
				}
			}

			if (foundTarget)
			{
				CDouble plane[4] = { 0.0, 1.0, 0.0, -g_engineWaters[index]->m_fWaterCPos[1] };
				glEnable(GL_CLIP_PLANE0);
				glClipPlane(GL_CLIP_PLANE0, plane);
			}

			g_currentInstancePrefab = m_transparentGeometries[i].m_instancePrefab;
			CScene* scene = m_transparentGeometries[i].m_scene;
			g_render.SetScene(scene);
			g_currentInstancePrefab->SetLight();

			CGeometry* geometry = m_transparentGeometries[i].m_instanceGeometry->m_abstractGeometry;

			if (m_transparentGeometries[i].m_instanceGeometry->m_hasPhysX && m_transparentGeometries[i].m_instanceGeometry->m_physXDensity > 0.0f) //render with physics
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();

				g_render.MultMatrix(m_transparentGeometries[i].m_instanceGeometry->m_localToWorldMatrixControlledByPhysX);

				geometry->m_currentInstanceGeometry = m_transparentGeometries[i].m_instanceGeometry;
				geometry->Draw(NULL, m_transparentGeometries[i].m_instanceGeometry);

				g_render.PopMatrix();
			}
			else
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();

				g_render.MultMatrix(m_transparentGeometries[i].m_instanceGeometry->m_localToWorldMatrix);
				geometry->m_currentInstanceGeometry = m_transparentGeometries[i].m_instanceGeometry;
				geometry->Draw(NULL, m_transparentGeometries[i].m_instanceGeometry);

				g_render.PopMatrix();
			}

			if (foundTarget)
			{
				glDisable(GL_CLIP_PLANE0);
			}
		}
	}
}

CVoid CMain::Render3DTransparentModelsForWater(CWater* water)
{
	for (CUInt i = 0; i < m_transparentGeometries.size(); i++)
	{
		CBool foundTarget = CFalse;
		CInt index = -1;
		for (CUInt j = 0; j < water->GetNumPrefabInstances(); j++)
		{
			g_currentInstancePrefab = water->GetPrefabInstance(j);

			if (g_currentInstancePrefab == m_transparentGeometries[i].m_instancePrefab)
			{
				foundTarget = CTrue;
				index = i;

				CScene* scene = NULL;

				CPrefab* prefab = water->GetPrefabInstance(j)->GetPrefab();
				for (CUInt k = 0; k < 3; k++)
				{
					if (prefab && prefab->GetHasLod(k) && water->GetPrefabInstance(j)->GetSceneVisible(k))
					{
						scene = water->GetPrefabInstance(j)->GetScene(k);
						break;
					}
				}
				if (!scene) continue;
				g_render.SetScene(scene);

				break;
			}
		}
		if (!foundTarget)
			continue;

		g_currentInstancePrefab->SetLight();

		for (CUInt j = 0; j < g_render.GetScene()->m_instanceGeometries.size(); j++)
		{
			CGeometry* geometry = g_render.GetScene()->m_instanceGeometries[j]->m_abstractGeometry;

			if (g_render.GetScene()->m_instanceGeometries[j] == m_transparentGeometries[index].m_instanceGeometry)
			{
				if (g_render.GetScene()->m_instanceGeometries[j]->m_hasPhysX && g_render.GetScene()->m_instanceGeometries[j]->m_physXDensity > 0.0f) //render with physics
				{
					g_render.ModelViewMatrix();
					g_render.PushMatrix();

					g_render.MultMatrix(g_render.GetScene()->m_instanceGeometries[j]->m_localToWorldMatrixControlledByPhysX);

					geometry->Draw(NULL, g_render.GetScene()->m_instanceGeometries[j]);

					g_render.PopMatrix();
				}
				else
				{
					g_render.ModelViewMatrix();
					g_render.PushMatrix();

					g_render.MultMatrix(g_render.GetScene()->m_instanceGeometries[j]->m_localToWorldMatrix);

					geometry->Draw(NULL, g_render.GetScene()->m_instanceGeometries[j]);

					g_render.PopMatrix();
				}
			}
		}
	}
}

CVoid CMain::RenderCharacter(CBool sceneManager)
{
	if (g_mainCharacter && g_mainCharacter->GetCameraType() == ePHYSX_CAMERA_FIRST_PERSON) return;

	if (!g_databaseVariables.m_insertCharacter)
		return;
	//3D Model data
	if (!g_mainCharacter)
		return;

	g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();

	if (!g_currentInstancePrefab->GetVisible()) return;
	g_currentInstancePrefab->SetLight();

	//if (m_bQuery)
	//{
	//	if (g_instancePrefab[i]->GetResult() == 0)
	//	{
	//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() >(g_instancePrefab[i]->GetRadius() + 20.0f))
	//			continue;
	//	}
	//}

	CScene* scene = NULL;

	CPrefab* prefab = g_currentInstancePrefab->GetPrefab();
	for (CUInt j = 0; j < 3; j++)
	{
		if (prefab && prefab->GetHasLod(j) )
		{
			scene = g_currentInstancePrefab->GetScene(j);
			break;
		}
	}
	if (!scene) return;

	g_render.SetScene(scene);
	if (!g_render.GetScene()->m_isTrigger)
	{
		//if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
		//{
		//	if (g_render.GetScene()->GetAnimationStatus() == eANIM_PLAY  && g_render.GetScene()->UpdateAnimationLists())
		//	{
		//		g_render.GetScene()->Update(elapsedTime);
		//		g_render.GetScene()->m_updateAnimation = CFalse;
		//		g_render.GetScene()->m_update = CFalse;
		//		g_render.GetScene()->SetUpdateBB(CTrue);
		//	}
		//	else if (g_render.GetScene()->m_update)
		//	{
		//		g_render.GetScene()->Update();
		//		g_render.GetScene()->m_update = CFalse;
		//	}

		//}
		//else if (g_render.GetScene()->m_update)
		//{
		//	g_render.GetScene()->Update();
		//	g_render.GetScene()->m_update = CFalse;
		//}
		if (g_currentInstancePrefab)
		{
			g_render.ModelViewMatrix();
			g_render.PushMatrix();
			g_render.MultMatrix(*(g_currentInstancePrefab->GetInstanceMatrix()));
		}
		//disable self shadow to prevent artifacts
		CBool useShadow = g_render.m_useDynamicShadowMap;
		//g_render.m_useDynamicShadowMap = CFalse;
		g_render.GetScene()->RenderAnimatedModels(sceneManager, CTrue); //render controller
		g_render.m_useDynamicShadowMap = useShadow;

		if (g_currentInstancePrefab)
		{
			g_render.PopMatrix();
		}
		if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
		{
			glUseProgram(0);
		}

	}
	
}


CVoid CMain::Render3DAnimatedModels(CBool sceneManager)
{
	//3D Model data

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		if (!g_instancePrefab[i]->GetIsAnimated()) continue;

		g_currentInstancePrefab = g_instancePrefab[i];
		if (Cmp(g_currentInstancePrefab->GetName(), "VANDA_MAIN_CHARACTER"))
			continue;
		if (!g_currentInstancePrefab->GetRenderForQuery())
			continue;

		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() >(g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;

		g_render.SetScene(scene);
		if (!g_render.GetScene()->m_isTrigger)
		{
			g_currentInstancePrefab->SetLight();

			if (g_currentInstancePrefab)
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();
				g_render.MultMatrix(*(g_instancePrefab[i]->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CTrue); //render controller
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CFalse); //render animated models
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}
}

CVoid CMain::Render3DAnimatedModelsForWater(CWater* water, CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < water->GetNumPrefabInstances(); i++)
	{
		if (!water->GetPrefabInstance(i)->GetVisible()) continue;
		if (!water->GetPrefabInstance(i)->GetIsAnimated()) continue;
		g_currentInstancePrefab = water->GetPrefabInstance(i);

		CVec3f src[8];
		CVec3f maxAABB = water->GetPrefabInstance(i)->GetInverseMaxAABB();
		CVec3f minAABB = water->GetPrefabInstance(i)->GetInverseMinAABB();
		src[0].x = minAABB.x; src[0].y = minAABB.y; src[0].z = minAABB.z;
		src[1].x = maxAABB.x; src[1].y = minAABB.y; src[1].z = maxAABB.z;
		src[2].x = maxAABB.x; src[2].y = minAABB.y; src[2].z = minAABB.z;
		src[3].x = minAABB.x; src[3].y = minAABB.y; src[3].z = maxAABB.z;
		src[4].x = maxAABB.x; src[4].y = maxAABB.y; src[4].z = minAABB.z;
		src[5].x = minAABB.x; src[5].y = maxAABB.y; src[5].z = minAABB.z;
		src[6].x = minAABB.x; src[6].y = maxAABB.y; src[6].z = maxAABB.z;
		src[7].x = maxAABB.x; src[7].y = maxAABB.y; src[7].z = maxAABB.z;

		if (!g_camera->m_cameraManager->IsBoxInFrustum(&src[0], 8))
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CFalse);
			continue;
		}
		else
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CTrue);
		}


		CScene* scene = NULL;

		CPrefab* prefab = water->GetPrefabInstance(i)->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->GetPrefabInstance(i)->GetSceneVisible(j))
			{
				scene = water->GetPrefabInstance(i)->GetScene(j);
				break;
			}
		}
		if (!scene) continue;

		g_render.SetScene(scene);
		if (!g_render.GetScene()->m_isTrigger)
		{
			CBool update = CTrue;
			if (g_currentVSceneProperties.m_pauseGame || !g_currentInstancePrefab->GetUpdateAnimation())
				update = CFalse;
			if (update)
			{
				if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
				{
					if (g_render.GetScene()->GetAnimationStatus() == eANIM_PLAY  && g_render.GetScene()->UpdateAnimationLists())
					{
						g_render.GetScene()->Update(elapsedTime);
						g_render.GetScene()->m_updateAnimation = CFalse;
						g_render.GetScene()->m_update = CFalse;
						g_render.GetScene()->SetUpdateBB(CTrue);
					}
					else if (g_render.GetScene()->m_update)
					{
						g_render.GetScene()->Update();
						g_render.GetScene()->m_update = CFalse;
					}

				}
				else if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				}
			}
			g_currentInstancePrefab->SetLight();
			if (g_currentInstancePrefab)
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();
				g_render.MultMatrix(*(water->GetPrefabInstance(i)->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CTrue); //render controller
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CFalse); //render animated models
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}
}

CVoid CMain::RenderTerrain(CBool useFBO)
{
	if (useFBO && !g_useOldRenderingStyle && g_options.m_enableFBO && g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	{
		if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && /*g_options.m_numSamples*/g_window.m_numSamples && g_options.m_enableFBO)
			g_render.BindForWriting(m_mFboID);
		else if (!g_useOldRenderingStyle && g_options.m_enableFBO)
			g_render.BindForWriting(m_fboID);
	}
	if (!g_terrain) return;
	if (g_engineLights.size() == 0)
		SetDefaultLight();

	// Display the terrain mesh.
	if (g_terrain->GetTerrainTexture())
		g_terrain->GetTerrainTexture()->enableTextures();

	if (g_options.m_enableShader && g_render.UsingShader())
	{
		if (g_renderForWater)
		{
			g_shaderType = g_render.m_terrainProgram;
		}
		else if (g_shadowProperties.m_enable && g_render.UsingShadowShader() && !Cmp(g_shadowProperties.m_directionalLightName, "\n"))
		{
			if (false/*m_hasNormalMap*/) //currently no normal map
			{
				//switch (g_shadowProperties.m_shadowType)
				//{
				//case eSHADOW_SINGLE_HL:
				//	g_shaderType = g_render.m_terrain_shad_single_hl_normal_prog;
				//	break;
				//case eSHADOW_SINGLE:
				//	g_shaderType = g_render.m_terrain_shad_single_normal_prog;
				//	break;
				//case eSHADOW_MULTI_LEAK:
				//	g_shaderType = g_render.m_terrain_shad_multi_normal_prog;
				//	break;
				//case eSHADOW_MULTI_NOLEAK:
				//	g_shaderType = g_render.m_terrain_shad_multi_noleak_normal_prog;
				//	break;
				//case eSHADOW_PCF:
				//	g_shaderType = g_render.m_terrain_shad_pcf_normal_prog;
				//	break;
				//case eSHADOW_PCF_TRILIN:
				//	g_shaderType = g_render.m_terrain_shad_pcf_trilin_normal_prog;
				//	break;
				//case eSHADOW_PCF_4TAP:
				//	g_shaderType = g_render.m_terrain_shad_pcf_4tap_normal_prog;
				//	break;
				//case eSHADOW_PCF_8TAP:
				//	g_shaderType = g_render.m_terrain_shad_pcf_8tap_normal_prog;
				//	break;
				//case eSHADOW_PCF_GAUSSIAN:
				//	g_shaderType = g_render.m_terrain_shad_pcf_gaussian_normal_prog;
				//	break;
				//}
			}
			else
			{
				switch (g_shadowProperties.m_shadowType)
				{
				case eSHADOW_SINGLE_HL:
					g_shaderType = g_render.m_shad_single_hl_prog;
					break;
				case eSHADOW_SINGLE:
					g_shaderType = g_render.m_terrain_shad_single_prog;
					break;
				case eSHADOW_MULTI_LEAK:
					g_shaderType = g_render.m_terrain_shad_multi_prog;
					break;
				case eSHADOW_MULTI_NOLEAK:
					g_shaderType = g_render.m_terrain_shad_multi_noleak_prog;
					break;
				case eSHADOW_PCF:
					g_shaderType = g_render.m_terrain_shad_pcf_prog;
					break;
				case eSHADOW_PCF_TRILIN:
					g_shaderType = g_render.m_terrain_shad_pcf_trilin_prog;
					break;
				case eSHADOW_PCF_4TAP:
					g_shaderType = g_render.m_terrain_shad_pcf_4tap_prog;
					break;
				case eSHADOW_PCF_8TAP:
					g_shaderType = g_render.m_terrain_shad_pcf_8tap_prog;
					break;
				case eSHADOW_PCF_GAUSSIAN:
					g_shaderType = g_render.m_terrain_shad_pcf_gaussian_prog;
					break;
				}
			}
		}
		else
		{
			g_shaderType = g_render.m_terrainProgram;
		}

		glUseProgram(g_shaderType);
		glUniform1f(glGetUniformLocation(g_shaderType, "focalDistance"), g_main->m_dof.m_focalDistance);
		glUniform1f(glGetUniformLocation(g_shaderType, "focalRange"), g_main->m_dof.m_focalRange);
		CBool useFog;
		if ((g_dofProperties.m_enable && g_dofProperties.m_debug) || (g_shadowProperties.m_shadowType == eSHADOW_SINGLE_HL && g_shadowProperties.m_enable && g_render.UsingShadowShader()))
			useFog = CFalse;
		else
			useFog = CTrue;

		if ((g_fogProperties.m_enable && useFog) || (g_waterFogProperties.m_enable && useFog))
			glUniform1i(glGetUniformLocation(g_shaderType, "enableFog"), CTrue);
		else
			glUniform1i(glGetUniformLocation(g_shaderType, "enableFog"), CFalse);

		glUniform1i(glGetUniformLocation(g_shaderType, "image1"), 0);
		glUniform1i(glGetUniformLocation(g_shaderType, "image2"), 1);
		glUniform1i(glGetUniformLocation(g_shaderType, "image3"), 2);
		glUniform1i(glGetUniformLocation(g_shaderType, "shadowMap"), 3);
		glUniform1i(glGetUniformLocation(g_shaderType, "image1Normal"), 4);
		glUniform1i(glGetUniformLocation(g_shaderType, "image2Normal"), 5);
		glUniform1i(glGetUniformLocation(g_shaderType, "image3Normal"), 6);

		glUniform1i(glGetUniformLocation(g_shaderType, "stex"), 7); // depth-maps
		glUniform4fv(glGetUniformLocation(g_shaderType, "far_d"), 1, g_main->far_bound);
		glUniform1f(glGetUniformLocation(g_shaderType, "shadow_intensity"), g_shadowProperties.m_intensity);
	}
	else
	{
		glUseProgram(0);
	}

	if (g_terrain->GetTerrain())
		g_terrain->GetTerrain()->draw();
	if (g_terrain->GetTerrainTexture())
		g_terrain->GetTerrainTexture()->disableTextures();
}

CVoid CMain::RenderQueries(CBool init)
{
	if (m_bQuery || init)
	{
		COpenGLUtility m_glUtil;
		m_renderQuery = CTrue;
		glDepthFunc(GL_LEQUAL);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
			g_render.BindFBO(m_mFboID);
		else if (!g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
			g_render.BindFBO(m_fboID);
		glClear(GL_DEPTH_BUFFER_BIT);

		g_render.m_useShader = CFalse;
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		for (CUInt i = 0; i < 8; i++)
		{
			glActiveTexture(GL_TEXTURE0 + (GLenum)i);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		g_render.ModelViewMatrix();

		//render Bounding Boxes for 3d objects here
		//Results are used in ManageLOD() functions to determine the LOD level
		g_render.ModelViewMatrix();
		glDisable(GL_CULL_FACE);
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (g_instancePrefab[i]->GetIsTrigger() || Cmp(g_instancePrefab[i]->GetName(), "VANDA_MAIN_CHARACTER") || !g_instancePrefab[i]->GetRenderForQuery() || !g_instancePrefab[i]->GetVisible())
				continue;

			g_currentInstancePrefab = g_instancePrefab[i];
			glBeginQuery(GL_SAMPLES_PASSED, g_instancePrefab[i]->GetQueryIndex());

			CVec3f ObjectCenter = (g_currentInstancePrefab->GetMinAABB() + g_currentInstancePrefab->GetMaxAABB()) * 0.5f;
			g_render.ModelViewMatrix();
			g_render.PushMatrix();
			g_render.IdentityMatrix();
			if (g_currentCameraType == eCAMERA_PHYSX)
			{
				gluLookAt(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z,
				ObjectCenter.x, ObjectCenter.y, ObjectCenter.z, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				gluLookAt(free_dae_cam_pos.x, free_dae_cam_pos.y, free_dae_cam_pos.z,
					ObjectCenter.x, ObjectCenter.y, ObjectCenter.z, 0.0f, 1.0f, 0.0f);
			}
			m_glUtil.DrawSimpleBox(g_currentInstancePrefab->GetMinAABB(), g_currentInstancePrefab->GetMaxAABB());
			g_render.PopMatrix();
			glEndQuery(GL_SAMPLES_PASSED);
		}

		g_render.m_useShader = CTrue;
		//end of rendering Object's bounding boxes 
		if (g_engineWaters.size() > 0)
		{
			//set all the lights here
			for (CUInt j = 0; j < g_engineWaters.size(); j++)
			{
				CVec3f WaterCenter(g_engineWaters[j]->m_fWaterCPos[0], g_engineWaters[j]->m_fWaterCPos[1], g_engineWaters[j]->m_fWaterCPos[2]);

				CVec3f waterPoints[4];
				waterPoints[0].x = g_engineWaters[j]->m_sidePoint[0].x; waterPoints[0].y = g_engineWaters[j]->m_sidePoint[0].y; waterPoints[0].z = g_engineWaters[j]->m_sidePoint[0].z;
				waterPoints[1].x = g_engineWaters[j]->m_sidePoint[1].x; waterPoints[1].y = g_engineWaters[j]->m_sidePoint[1].y; waterPoints[1].z = g_engineWaters[j]->m_sidePoint[1].z;
				waterPoints[2].x = g_engineWaters[j]->m_sidePoint[2].x; waterPoints[2].y = g_engineWaters[j]->m_sidePoint[2].y; waterPoints[2].z = g_engineWaters[j]->m_sidePoint[2].z;
				waterPoints[3].x = g_engineWaters[j]->m_sidePoint[3].x; waterPoints[3].y = g_engineWaters[j]->m_sidePoint[3].y; waterPoints[3].z = g_engineWaters[j]->m_sidePoint[3].z;

				if (g_camera->m_cameraManager->IsBoxInFrustum(waterPoints, 4))
				{
					g_engineWaters[j]->SetOutsideFrustom(CFalse);
					glBeginQuery(GL_SAMPLES_PASSED, g_engineWaters[j]->GetQueryIndex());

					g_render.ModelViewMatrix();
					g_render.PushMatrix();
					g_render.IdentityMatrix();
					if (g_currentCameraType == eCAMERA_PHYSX)
					{
						gluLookAt(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z,
						WaterCenter.x, WaterCenter.y, WaterCenter.z, 0.0f, 1.0f, 0.0f);
					}
					else
					{
						gluLookAt(free_dae_cam_pos.x, free_dae_cam_pos.y, free_dae_cam_pos.z,
							WaterCenter.x, WaterCenter.y, WaterCenter.z, 0.0f, 1.0f, 0.0f);
					}

					COpenGLUtility glUtil;
					glUtil.DrawSquare(g_engineWaters[j]->m_sidePoint[0], g_engineWaters[j]->m_sidePoint[1], g_engineWaters[j]->m_sidePoint[2], g_engineWaters[j]->m_sidePoint[3]);
					g_render.PopMatrix();
					glEndQuery(GL_SAMPLES_PASSED);

					for (CUInt k = 0; k < g_engineWaters[j]->GetNumPrefabInstances(); k++)
					{
						g_currentInstancePrefab = g_engineWaters[j]->GetPrefabInstance(k);

						//to determine the LOD level of water, look at object, even if it's not inside the camera.
						//if we have already rendered the object query, skip it
						if (!g_currentInstancePrefab->GetRenderForWaterQuery() || g_currentInstancePrefab->GetRenderForQuery() || !g_currentInstancePrefab->GetVisible() || g_currentInstancePrefab->GetIsTrigger() || Cmp(g_currentInstancePrefab->GetName(), "VANDA_MAIN_CHARACTER"))
							continue;
						glBeginQuery(GL_SAMPLES_PASSED, g_currentInstancePrefab->GetQueryIndex());

						CVec3f ObjectCenter = (g_currentInstancePrefab->GetMinAABB() + g_currentInstancePrefab->GetMaxAABB()) * 0.5f;
						g_render.ModelViewMatrix();
						g_render.PushMatrix();
						g_render.IdentityMatrix();
						if (g_currentCameraType == eCAMERA_PHYSX)
						{
							gluLookAt(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z,
							ObjectCenter.x, ObjectCenter.y, ObjectCenter.z, 0.0f, 1.0f, 0.0f);
						}
						else
						{
							gluLookAt(free_dae_cam_pos.x, free_dae_cam_pos.y, free_dae_cam_pos.z,
								ObjectCenter.x, ObjectCenter.y, ObjectCenter.z, 0.0f, 1.0f, 0.0f);

						}
						m_glUtil.DrawSimpleBox(g_currentInstancePrefab->GetMinAABB(), g_currentInstancePrefab->GetMaxAABB());
						g_render.PopMatrix();
						glEndQuery(GL_SAMPLES_PASSED);
					}

				}
				else
				{
					g_engineWaters[j]->SetOutsideFrustom(CTrue);
				}

			}
		}

		glDepthMask(GL_FALSE);

		if ( g_engineLights.size() > 0)
		{
			//set all the lights here
			for (CUInt j = 0; j < g_engineLights.size(); j++)
			{
				CInstanceLight *instanceLight = g_engineLights[j];

				if (g_engineLights[j]->m_abstractLight->GetType() == eLIGHTTYPE_DIRECTIONAL)
				{
					//Cpy(g_shadowProperties.m_directionalLightName, instanceLight->m_abstractLight->GetName());
					g_engineLights[j]->SetRunTimeVisible(CTrue);
				}

				CVec3f  Position;
				if (instanceLight->m_parent)
				{
					float *matrix = (float *)instanceLight->m_parent->GetLocalToWorldMatrix();
					Position.x = matrix[12]; Position.y = matrix[13]; Position.z = matrix[14];
				}
				else
				{
					Position.x = instanceLight->m_abstractLight->GetPosition()[0];
					Position.y = instanceLight->m_abstractLight->GetPosition()[1];
					Position.z = instanceLight->m_abstractLight->GetPosition()[2];
				}

				if (g_camera->m_cameraManager->IsSphereInFrustum(Position.x, Position.y, Position.z, (CFloat)instanceLight->GetRadius()))
				{
					g_engineLights[j]->SetRunTimeVisible(CTrue);
				}
				else
				{
					g_engineLights[j]->SetRunTimeVisible(CFalse);
				}
				//glBeginQuery(GL_SAMPLES_PASSED, instanceLight->GetQueryIndex());
				//CFloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				//CFloat pos[3]; pos[0] = Position.x; pos[1] = Position.y; pos[2] = Position.z;
				//g_render.DrawSolidSphere(instanceLight->GetRadius(), 10, 10, pos, color);
				//glEndQuery(GL_SAMPLES_PASSED);
			}
		}
		glDepthMask(CTrue);
		glEnable(GL_CULL_FACE);
		glPopAttrib();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//glDepthMask(GL_TRUE);
		m_renderQuery = CFalse;
		g_render.BindFBO(0);
	}
}

CVoid CMain::RenderBakedOctree3DModels()
{
	//3D Model data
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		//if it's not visible, skip
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];

		//if it's not in view frustom, skip
		if (!g_instancePrefab[i]->GetRenderForQuery())
			continue;

		//if it's 3rd person character, skip
		if (Cmp(g_currentInstancePrefab->GetName(), "VANDA_MAIN_CHARACTER"))
			continue;


		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j))
			{
				if (g_instancePrefab[i]->GetSceneVisible(j))
				{
					scene = g_instancePrefab[i]->GetScene(j);
					break;
				}
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_currentInstancePrefab->SetLight();
			g_render.GetScene()->Render(g_currentInstancePrefab->GetRealTimeSceneCheckIsInFrustom());

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}

	}
}

CVoid CMain::Render3DModels(CBool sceneManager, CChar* parentTreeNameOfGeometries, CBool checkVisibility, CBool drawGeometry)
{
	//3D Model data

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];

		if (!checkVisibility)
			if (!g_instancePrefab[i]->GetRenderForQuery())
				continue;

		if (Cmp(g_currentInstancePrefab->GetName(), "VANDA_MAIN_CHARACTER"))
			continue;

		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() > (g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) )
			{
				if (checkVisibility)
				{
					scene = g_instancePrefab[i]->GetScene(j);
					break;
				}
				else
				{
					if (g_instancePrefab[i]->GetSceneVisible(j))
					{
						scene = g_instancePrefab[i]->GetScene(j);
						break;
					}
				}
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);
		if (checkVisibility && g_currentInstancePrefab)
		{
			g_currentInstancePrefab->SetRealTimeSceneCheckIsInFrustom(sceneManager);
		}

		if (!g_render.GetScene()->m_isTrigger)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_currentInstancePrefab->SetLight();
			g_render.GetScene()->Render(sceneManager, parentTreeNameOfGeometries, checkVisibility, drawGeometry);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}
		}
	}

}
CVoid CMain::Render3DModelsForWater(CWater* water, CBool sceneManager, CChar* parentTreeNameOfGeometries)
{
	//3D Model data
	for (CUInt i = 0; i < water->GetNumPrefabInstances(); i++)
	{
		if (!water->GetPrefabInstance(i)->GetVisible()) continue;
		g_currentInstancePrefab = water->GetPrefabInstance(i);

		CVec3f src[8];
		CVec3f maxAABB = water->GetPrefabInstance(i)->GetInverseMaxAABB();
		CVec3f minAABB = water->GetPrefabInstance(i)->GetInverseMinAABB();
		src[0].x = minAABB.x; src[0].y = minAABB.y; src[0].z = minAABB.z;
		src[1].x = maxAABB.x; src[1].y = minAABB.y; src[1].z = maxAABB.z;
		src[2].x = maxAABB.x; src[2].y = minAABB.y; src[2].z = minAABB.z;
		src[3].x = minAABB.x; src[3].y = minAABB.y; src[3].z = maxAABB.z;
		src[4].x = maxAABB.x; src[4].y = maxAABB.y; src[4].z = minAABB.z;
		src[5].x = minAABB.x; src[5].y = maxAABB.y; src[5].z = minAABB.z;
		src[6].x = minAABB.x; src[6].y = maxAABB.y; src[6].z = maxAABB.z;
		src[7].x = maxAABB.x; src[7].y = maxAABB.y; src[7].z = maxAABB.z;

		if (!g_camera->m_cameraManager->IsBoxInFrustum(&src[0], 8))
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CFalse);
			continue;
		}
		else
		{
			g_currentInstancePrefab->SetRenderForWaterQuery(CTrue);
		}

		CScene* scene = NULL;

		CPrefab* prefab = water->GetPrefabInstance(i)->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->GetPrefabInstance(i)->GetSceneVisible(j))
			{
				scene = water->GetPrefabInstance(i)->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_currentInstancePrefab->SetLight();
			g_render.GetScene()->Render(sceneManager, parentTreeNameOfGeometries);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}
			//if (g_databaseVariables.m_showPrefabBoundingBox)
			//{
			//	g_render.GetScene()->RenderAABBWithLines();
			//}
		}
	}
}

CVoid CMain::DrawGUI()
{
	if (!m_renderVideo)
	{
		for (CUInt i = 0; i < g_guis.size(); i++)
		{
			if (g_guis[i]->GetVisible())
			{
				for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
				{
					g_guis[i]->m_guiImages[j]->Render(g_guis[i]->GetPosition(CTrue));
				}
				for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
				{
					g_guis[i]->m_guiButtons[j]->Render(g_guis[i]->GetPosition(CTrue));
				}
				for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
				{
					g_guis[i]->m_guiTexts[j]->Render(g_guis[i]->GetPosition(CTrue));
				}
			}
		}

		//Draw Cursor
		if (m_showMenuCursor && !g_main->GetCursorIcon()->GetVisible())
		{
			CFloat cursorSize = ((CFloat)g_currentVSceneProperties.m_menuCursorSize * (CFloat)g_width) / 100.0f;
			CFloat halfCursorSize = cursorSize / 2.0f;
			glUseProgram(0);
			glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5f);
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glViewport(0, g_main->GetPadding(), g_width, g_height);
			g_render.ProjectionMatrix();
			g_render.PushMatrix();
			g_render.IdentityMatrix();
			gluOrtho2D(0.0, g_width, 0.0, g_height);
			g_render.ModelViewMatrix();
			g_render.PushMatrix();
			g_render.IdentityMatrix();
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glEnable(GL_TEXTURE_2D);
			if (m_menuCursorImg)
			{
				glBindTexture(GL_TEXTURE_2D, m_menuCursorImg->GetId());
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(g_main->m_mousePosition.x - halfCursorSize, g_height - g_main->m_mousePosition.y + halfCursorSize - cursorSize);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(g_main->m_mousePosition.x - halfCursorSize + cursorSize, g_height - g_main->m_mousePosition.y + halfCursorSize - cursorSize);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(g_main->m_mousePosition.x - halfCursorSize + cursorSize, g_height - g_main->m_mousePosition.y + halfCursorSize);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(g_main->m_mousePosition.x - halfCursorSize, g_height - g_main->m_mousePosition.y + halfCursorSize);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			g_render.ProjectionMatrix();
			g_render.PopMatrix();

			g_render.ModelViewMatrix();
			g_render.PopMatrix();
			glPopAttrib(); //viewport
		}

		CVec2f pos(g_main->m_mousePosition.x, g_height - g_main->m_mousePosition.y);
		m_cursorIcon->Render(pos);
	}

	m_timerCounter++;
	g_font->StartRendering();
	//g_font->Print( "Vanda Engine", 10.0f, 980.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "Copyright (C) 2024 Ehsan Kamrani ", 10.0f, 950.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "http://www.vandaengine.org", 10.0f, 920.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "Press F4 for help...", 10.0f, 890.0f, 0.0f, 1.0f, 1.0f, 1.0f );

	if( m_showHelpInfo )
	{
		//draw a transparent quad
		glUseProgram(0);
		glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glViewport(0, 0, g_width, g_height);
		g_render.ProjectionMatrix();
		g_render.PushMatrix();
		g_render.IdentityMatrix();
		gluOrtho2D(0.0, g_width, 0.0, g_height);
		g_render.ModelViewMatrix();
		g_render.PushMatrix();
		g_render.IdentityMatrix();

		CDouble window_width = (CDouble)g_width;
		CDouble window_height = (CDouble)g_height;

		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glBegin(GL_QUADS);
		glVertex2d(0.0, 0.0);
		glVertex2d(window_width / 3.0, 0.0);
		glVertex2d(window_width / 3.0, window_height);
		glVertex2d(0.0, window_height);
		glEnd();
		glDisable(GL_BLEND);

		g_render.ProjectionMatrix();
		g_render.PopMatrix();

		g_render.ModelViewMatrix();
		g_render.PopMatrix();
		glPopAttrib();
		//End of drawing a transparent quad

		//g_font->Print( "F1 key: Enable / Disable help.\n", 10.0f, 830.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Right Arrow key or D key: Move Right.\n", 10.0f, 800.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Up Arrow key or W key: Move Front.\n", 10.0f, 770.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Down Arrow key or S key: Move Back.\n", 10.0f, 740.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Left Arrow key or A key: Move Left.\n", 10.0f, 710.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "E key: Move Down.\n", 10.0f, 680.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Q key: Move Up (If gravity is disabled).\n", 10.0f, 650.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "G key: Enable / Disable gravity.\n", 10.0f, 620.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Spacebar key: Jump (If gravity is enabled).\n", 10.0f, 590.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Mouse movement: Camera rotation.\n", 10.0f, 560.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Right mouse button: Zoom in / Zoom out.\n", 10.0f, 530.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "P key: Enable / Disable physics debug mode.\n", 10.0f, 500.0f, 0.0f, 1.0f, 1.0f, 0.5f );

		if (/*g_databaseVariables.m_showStatistics*/true)
		{
			CInt m_numSamples;
			if (g_window.m_windowGL.multiSampling)
				m_numSamples = g_width * g_height *  /*g_options.m_numSamples*/g_window.m_numSamples;
			else
				m_numSamples = g_width * g_height;
			CFloat percentage;

			static int m_numVerts = 0;
			static int m_fps = 0;

			CChar temp[MAX_NAME_SIZE];
			static CFloat fps_timer = 0.0f;
			static CInt fps = (CInt)g_timer->GetFps(m_timerCounter);

			fps_timer += g_elapsedTime;
			if (fps_timer >= 1.0f)
			{
				fps = (CInt)g_timer->GetFps(m_timerCounter);
				fps_timer = 0.0f;
			}

			if( m_totalElapsedTime >= 0.5 ) //Every 0.5 seconds update the FPS
			{
				ResetTimer();
				m_numVerts = g_numVerts;
				m_fps = fps;
			}
						
			GetCursorPos(&m_point);

			sprintf(temp, "FPS : %i", m_fps);
			g_font->Print("---Statistics---", 10.0f, 990.0, 0.0f, 0.85f, 0.67f, 0.0f);
			g_font->Print(temp, 10.0f, 960.0, 0.0f, 0.85f, 0.67f, 0.0f);
			sprintf(temp, "Rendered Vertexes : %i", m_numVerts);
			g_font->Print(temp, 10.0f, 930.0, 0.0f, 0.85f, 0.67f, 0.0f);
			sprintf(temp, "Rendered Triangles : %i", m_numVerts / 3);
			g_font->Print(temp, 10.0f, 900, 0.0f, 0.85f, 0.67f, 0.0f);
			g_font->Print("Object -------------------- Samples", 10.0f, 870, 0.0f, 0.85f, 0.67f, 0.0f);
			CInt index = 0;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetIsTrigger() || Cmp(g_instancePrefab[i]->GetName(), "VANDA_MAIN_CHARACTER") || !g_instancePrefab[i]->GetVisible())
					continue;

				CBool renderForWater = CFalse;
				if (g_instancePrefab[i]->GetWater())
				{
					if (!g_instancePrefab[i]->GetWater()->GetOutsideFrustom())
					{
						if (g_instancePrefab[i]->GetRenderForWaterQuery())
						{
							renderForWater = CTrue;
						}
					}
				}
				if (!g_instancePrefab[i]->GetRenderForQuery() && !renderForWater)
					continue;

				percentage = ((CFloat)g_instancePrefab[i]->GetResult() / (CFloat)m_numSamples) * 100.0f;

				if (index <= 20)
				{
					char temp[MAX_NAME_SIZE];
					sprintf(temp, "%s LOD Percentage: %.2f", g_instancePrefab[i]->GetName(), percentage);
					g_font->Print(temp, 10.0f, 870.0f - (index + 1) * 30, 0.0f, 0.85f, 0.67f, 0.0f);
				}
				else
				{
					break;
				}
				index++;
			}
			for (CUInt i = 0; i < g_engineLights.size(); i++)
			{
				char temp[200];
				if (g_engineLights[i]->GetRunTimeVisible())
					sprintf(temp, "%s : %s", g_engineLights[i]->m_abstractLight->GetName(), "Visible"/*g_engineLights[i]->GetLODPercent()*/);
				else
					sprintf(temp, "%s : %s", g_engineLights[i]->m_abstractLight->GetName(), "Invisible"/*g_engineLights[i]->GetLODPercent()*/);
				g_font->Print(temp, 10.0f, 870.0f - (index + i + 1) * 30, 0.0f, g_engineLights[i]->m_abstractLight->GetDiffuse()[0], g_engineLights[i]->m_abstractLight->GetDiffuse()[1], g_engineLights[i]->m_abstractLight->GetDiffuse()[2]);
			}

			for (CUInt i = 0; i < g_engineWaters.size(); i++)
			{
				if (g_engineWaters[i]->GetOutsideFrustom()) continue;
				if (CTrue/*g_currentCameraType == eCAMERA_PHYSX*/)
					if (!g_engineWaters[i]->GetVisible())
						continue;
				percentage = ((CFloat)g_engineWaters[i]->GetResult() / (CFloat)m_numSamples) * 100.0f;

				char temp[200];
				sprintf(temp, "%s LOD Percentage: %.2f", g_engineWaters[i]->GetName(), percentage);
				g_font->Print(temp, 10.0f, 870.0f - (index + g_engineLights.size() + i + 1) * 30, 0.0f, 0.65f, 0.5f, 0.65f);
			}

			//for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			//{
			//	if (i <= 25)
			//	{
			//		char temp[200];
			//		sprintf(temp, "%s Distance: %.2f", g_instancePrefab[i]->GetName(), g_instancePrefab[i]->GetDistanceFromCamera());
			//		g_font->Print(temp, 140.0, 1020.0 - (i + 1) * 30, 0.0f, 0.85f, 0.85f, 0.0f);
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}

		}
	}

	g_font->EndRendering();

}

void CMain::ResetPhysX(CBool releaseActors)
{
	if( releaseActors )
	{
		CUInt actor_counts = gPhysXscene->getNbActors();
		for( CUInt i = 0; i < actor_counts; i++ )
		{
			if( !gPhysXscene->getActors()[i]->getName() ) continue;
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
		}
	}
	//always release ground plane
	if (!releaseActors)
		g_nx->ReleaseGroundPlane();

	g_nx->gControllers->reportSceneChanged();

	//release character controller
	for( CUInt i = 0; i < gPhysXscene->getNbActors(); i++ )
	{
		if( !gPhysXscene->getActors()[i]->getName() )
		{
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
			break;
		}
	}

	g_nx->ReleaseCharacterControllers();

	g_nx->m_defaultGravity.x = g_physXProperties.m_fGravityX;
	g_nx->m_defaultGravity.y = g_physXProperties.m_fGravityY;
	g_nx->m_defaultGravity.z = g_physXProperties.m_fGravityZ;
	gPhysicsSDK->setParameter( NX_SKIN_WIDTH, g_physXProperties.m_fDefaultSkinWidth );

	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(g_physXProperties.m_fDefaultRestitution);
	defaultMaterial->setStaticFriction(g_physXProperties.m_fDefaultStaticFriction);
	defaultMaterial->setDynamicFriction(g_physXProperties.m_fDefaultDynamicFriction);
	if (g_physXProperties.m_bGroundPlane)
		g_nx->CreateGroundPlane(g_physXProperties.m_fGroundHeight);
	g_nx->InitCharacterControllers(g_nx->gCharacterPos.x, g_nx->gCharacterPos.y, g_nx->gCharacterPos.z, g_physXProperties.m_fCapsuleRadius, g_physXProperties.m_fCapsuleHeight, g_physXProperties.m_fCharacterSkinWidth, g_physXProperties.m_fCharacterSlopeLimit, g_physXProperties.m_fCharacterStepOffset);
	
	if( g_physXProperties.m_bApplyGravity )
	{
		g_nx->m_defaultGravity = NxVec3( g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ );
		gPhysXscene->setGravity(NxVec3(g_nx->m_defaultGravity.x, g_nx->m_defaultGravity.y, g_nx->m_defaultGravity.z));
	}
	else
	{
		g_nx->m_defaultGravity = NxVec3(0.0f);
		gPhysXscene->setGravity(NxVec3(0.0f));
	}
	g_nx->gDesiredDistance = g_physXProperties.m_fCameraCharacterDistance;
	g_nx->debugMode = g_physXProperties.m_bDebugMode;
	g_nx->gCharacterWalkSpeed = g_physXProperties.m_fCharacterWalkSpeed;
	g_nx->gCharacterRunSpeed = g_physXProperties.m_fCharacterRunSpeed;

	if( g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX || g_currentCameraType == eCAMERA_COLLADA || g_currentCameraType == eCAMERA_ENGINE)
	{
		if( gPhysXscene )
		{
		  // Run collision and dynamics for delta time since the last frame
			g_nx->gControllers->reportSceneChanged();
			gPhysXscene->simulate(1.0f / 60.0f /*elapsedTime*/);
			gPhysXscene->flushStream();
			gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		}
	}

	if (gPhysXscene)
	{
		//collision flags
		if (g_physXCollisionFlags.m_kinematicDynamic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_NON_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CTrue);
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT, GROUP_COLLIDABLE_PUSHABLE, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_NON_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CFalse);
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT, GROUP_COLLIDABLE_PUSHABLE, CFalse);
		}

		if (g_physXCollisionFlags.m_dynamicDynamic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CFalse);
		}

		if (g_physXCollisionFlags.m_dynamicStatic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_STATIC, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_STATIC, CFalse);
		}

		if (g_physXCollisionFlags.m_dynamicGround)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_GROUND, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_COLLIDABLE_PUSHABLE, GROUP_GROUND, CFalse);
		}

		if (g_physXCollisionFlags.m_triggerTrigger)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_TRIGGER, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_TRIGGER, CFalse);
		}

		if (g_physXCollisionFlags.m_triggerKinematic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_NON_PUSHABLE, CTrue);
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_NON_PUSHABLE, CFalse);
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT, CFalse);
		}

		if (g_physXCollisionFlags.m_triggerStatic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_STATIC, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_STATIC, CFalse);
		}

		if (g_physXCollisionFlags.m_triggerDynamic)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_PUSHABLE, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_COLLIDABLE_PUSHABLE, CFalse);
		}

		if (g_physXCollisionFlags.m_triggerGround)
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_GROUND, CTrue);
		}
		else
		{
			gPhysXscene->setGroupCollisionFlag(GROUP_TRIGGER, GROUP_GROUND, CFalse);
		}
	}
}

CVoid CMain::ManageLODs()
{
	CInt m_numSamples;
	if (g_window.m_windowGL.multiSampling)
		m_numSamples = g_width * g_height *  /*g_options.m_numSamples*/g_window.m_numSamples;
	else
		m_numSamples = g_width * g_height;

	//CInt index = 0;
	//m_bQuery = CFalse;
	//for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	//{
	//	GLint resultAvailable = GL_FALSE;
	//	glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT_AVAILABLE, &resultAvailable);

	//	if (resultAvailable)
	//		index++;
	//}
	//CInt numLights = 0;
	//for (CUInt i = 0; i < g_engineLights.size(); i++)
	//{
	//	if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName()))
	//		continue;
	//	numLights++;
	//	GLint resultAvailable = GL_FALSE;
	//	glGetQueryObjectiv(g_engineLights[i]->GetQueryIndex(), GL_QUERY_RESULT_AVAILABLE, &resultAvailable);

	//	if (resultAvailabrenle)
	//		index++;
	//}

	//if (index == g_instancePrefab.size() + numLights)
	//{
		//all results are available
		m_bQuery = CTrue;
	//}
	if (m_bQuery)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (g_instancePrefab[i]->GetIsTrigger() || Cmp(g_instancePrefab[i]->GetName(), "VANDA_MAIN_CHARACTER") || !g_instancePrefab[i]->GetVisible())
				continue;

			CBool renderForWater = CFalse;
			if (g_instancePrefab[i]->GetWater())
			{
				if (!g_instancePrefab[i]->GetWater()->GetOutsideFrustom())
				{
					if (g_instancePrefab[i]->GetRenderForWaterQuery())
					{
						renderForWater = CTrue;
					}
				}
			}

			if (!g_instancePrefab[i]->GetRenderForQuery() && !renderForWater)
				continue;

			GLint result;
			glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);
			g_instancePrefab[i]->SetResult(result);
			CFloat percentage;
			percentage = ((CFloat)g_instancePrefab[i]->GetResult() / (CFloat)m_numSamples) * 100.0f;


			if (result == 0 && (g_instancePrefab[i]->GetDistanceFromCamera() > g_instancePrefab[i]->GetRadius() + g_instancePrefabLODPercent.m_lod1) && g_instancePrefab[i]->GetNameIndex() != g_selectedName)
			{
				g_instancePrefab[i]->SetSceneVisible(0, CFalse);
				g_instancePrefab[i]->SetSceneVisible(1, CFalse);
				g_instancePrefab[i]->SetSceneVisible(2, CFalse);

				continue;
			}

			if (percentage >= g_instancePrefabLODPercent.m_lod1 || (g_instancePrefab[i]->GetDistanceFromCamera() <= (g_instancePrefab[i]->GetRadius() + g_instancePrefabLODPercent.m_lod1MinObjectCameraDistance)))
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (g_instancePrefab[i]->GetSceneVisible(1) && g_instancePrefab[i]->GetScene(1) && g_instancePrefab[i]->GetScene(1)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(1)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(0))
							{
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentAnimationTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(2) && g_instancePrefab[i]->GetScene(2) && g_instancePrefab[i]->GetScene(2)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(2)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(0))
							{
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentAnimationTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}

					g_instancePrefab[i]->SetSceneVisible(0, CTrue);
					g_instancePrefab[i]->SetSceneVisible(1, CFalse);
					g_instancePrefab[i]->SetSceneVisible(2, CFalse);
					g_instancePrefab[i]->ResetElapsedTime();
				}
			}
			else if (percentage >= g_instancePrefabLODPercent.m_lod2.x && percentage < g_instancePrefabLODPercent.m_lod2.y)
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (g_instancePrefab[i]->GetSceneVisible(0) && g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(0)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(1))
							{
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentAnimationTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(2) && g_instancePrefab[i]->GetScene(2) && g_instancePrefab[i]->GetScene(2)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(2)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(1))
							{
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentAnimationTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}

					if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
					{
						g_instancePrefab[i]->SetSceneVisible(0, CFalse);
						g_instancePrefab[i]->SetSceneVisible(1, CTrue);
						g_instancePrefab[i]->SetSceneVisible(2, CFalse);
						g_instancePrefab[i]->ResetElapsedTime();
					}
				}
			}
			else if (percentage > g_instancePrefabLODPercent.m_lod3.x && percentage < g_instancePrefabLODPercent.m_lod3.y)
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (!g_instancePrefab[i]->GetSceneVisible(0) && !g_instancePrefab[i]->GetSceneVisible(1) && !g_instancePrefab[i]->GetSceneVisible(2))
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(2))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CTrue);
							g_instancePrefab[i]->ResetElapsedTime();
						}
						else if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CTrue);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
						else if (g_instancePrefab[i]->GetPrefab()->GetHasLod(0))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CTrue);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(1))
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(2))
						{
							if (g_instancePrefab[i]->GetScene(1) && g_instancePrefab[i]->GetScene(1)->m_hasAnimation)
							{
								for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(1)->m_animationClips.size(); j++)
								{
									if (g_instancePrefab[i]->GetScene(2))
									{
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentAnimationTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayInTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayOutTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentWeight());
									}
								}
							}

							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CTrue);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(0))
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(2))
						{
							if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->m_hasAnimation)
							{
								for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(0)->m_animationClips.size(); j++)
								{
									if (g_instancePrefab[i]->GetScene(2))
									{
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentAnimationTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayInTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayOutTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentWeight());
									}
								}
							}

							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CTrue);
							g_instancePrefab[i]->ResetElapsedTime();
						}
						else if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
						{
							if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->m_hasAnimation)
							{
								for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(0)->m_animationClips.size(); j++)
								{
									if (g_instancePrefab[i]->GetScene(1))
									{
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentAnimationTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayInTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayOutTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentWeight());
									}
								}
							}

							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CTrue);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}

				}
			}

			g_instancePrefab[i]->SetElapsedTime(g_elapsedTime);

			//Make sure objects are always visible in free camera
			if (g_currentCameraType == eCAMERA_DEFAULT_FREE_NO_PHYSX)
			{
				if (!g_instancePrefab[i]->GetSceneVisible(0) && !g_instancePrefab[i]->GetSceneVisible(1) && !g_instancePrefab[i]->GetSceneVisible(2))
				{
					for (CUInt j = 2; j >= 0; j--)
					{
						if (g_instancePrefab[i]->GetPrefab() && g_instancePrefab[i]->GetPrefab()->GetHasLod(j))
						{
							CScene* scene = g_instancePrefab[i]->GetScene(j);
							if (!scene) continue;

							g_instancePrefab[i]->SetSceneVisible(j, CTrue);
							break;
						}
					}
				}
			}

		}

		//for (CUInt i = 0; i < g_engineLights.size(); i++)
		//{
		//	if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName()))
		//		continue;

		//	if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName()))
		//		continue;

		//	if (g_engineLights[i]->GetRunTimeVisible())
		//	{
		//		GLint result;
		//		glGetQueryObjectiv(g_engineLights[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);
		//		CFloat percentage;
		//		percentage = ((CFloat)result / (CFloat)m_numSamples) * 100.0f;
		//		g_engineLights[i]->SetLODPercent(percentage);
		//	}
		//	else
		//	{
		//		g_engineLights[i]->SetLODPercent(0.0f);
		//	}
		//}

		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			if (g_engineWaters[i]->GetOutsideFrustom()) continue;

			GLint result;
			glGetQueryObjectiv(g_engineWaters[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);
			g_engineWaters[i]->SetResult(result);

			if (result == 0)
				g_engineWaters[i]->SetQueryVisible(CFalse);
			else
				g_engineWaters[i]->SetQueryVisible(CTrue);
		}


	}

}

CVoid CMain::Draw3DObjects()
{
	if (!g_useOldRenderingStyle && g_options.m_enableFBO && g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	{
		if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && /*g_options.m_numSamples*/g_window.m_numSamples && g_options.m_enableFBO)
			g_render.BindForWriting(m_mFboID);
		else if (!g_useOldRenderingStyle && g_options.m_enableFBO)
			g_render.BindForWriting(m_fboID);

		if (g_engineLights.size() == 0)
			SetDefaultLight();

		m_checkBlending = CTrue;
		m_renderBlending = CFalse;
		m_pushTransparentGeometry = CTrue;

		RenderBakedOctree3DModels();
		Render3DAnimatedModels(CTrue);
		Render3DModelsControlledByPhysX();
		RenderCharacter(CFalse);

		m_renderBlending = CTrue;
		m_pushTransparentGeometry = CFalse;

		CalculateAndSort3DTransparentModelDistances();

		CBool condition = CFalse;

		CVec3f cameraPos(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y + 0.01f, g_camera->m_perspectiveCameraPos.z);
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			if (g_engineWaters[i]->IsPointInWater(cameraPos))
			{
				if (!g_engineWaters[i]->IsPointAboveWater(cameraPos))
				{
					condition = CTrue;
					g_waterFogProperties.m_enable = CTrue;
					CFog fog;
					fog.SetColor(g_engineWaters[i]->GetColor());
					fog.SetDensity(g_engineWaters[i]->GetFogDensity());
					break;
				}
			}
		}

		if (!condition) //we didn't find any active cameras under water
		{
			g_waterFogProperties.m_enable = CFalse;
			if (g_fogProperties.m_enable)
			{
				CFog fog;
				fog.SetColor(g_fogProperties.m_fogColor);
				fog.SetDensity(g_fogProperties.m_fogDensity);
			}
		}

		Render3DTransparentModels(condition); //render bottom part of transparent models

		RenderWaters();

		Render3DTransparentModels(!condition); //render top part of transparent models
		m_checkBlending = CFalse;

		++g_numLights;
		//g_octree->ResetOctreeGeoCount();
	} //if !g_useOldRenderingStyle
}

CVoid CMain::SetDefaultLight()
{
	glEnable(GL_LIGHT0);	 //just for per vertex lighting 	

	//This is the properties of the camera light
	GLfloat light_pos0[4] = { g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z, 1.0f };

	GLfloat light_ambient0[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	GLfloat light_diffuse0[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat light_specular0[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_shininess0 = 100.0f;

	glLightfv( GL_LIGHT0, GL_POSITION, light_pos0  );
	glLightfv( GL_LIGHT0, GL_AMBIENT , light_ambient0 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE , light_diffuse0 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular0 );
	glLightf ( GL_LIGHT0, GL_SHININESS, light_shininess0 );
	glLightf ( GL_LIGHT0, GL_SPOT_CUTOFF,(GLfloat)180.0f );
	glLightf ( GL_LIGHT0, GL_CONSTANT_ATTENUATION , (GLfloat)1.0f );
	glLightf ( GL_LIGHT0, GL_LINEAR_ATTENUATION,	(GLfloat)0.0f );
	glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, (GLfloat)0.0f );
		
	g_numLights++;
}

CVoid CMain::ResetPhysXCounts()
{
	for( CUInt i = 0 ; i < g_scene.size(); i++ )
	{
		for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			g_scene[i]->m_instanceGeometries[j]->m_physXCount = 0;
	}
}

CImage *CWater::GetWaterImage( const CChar * name )
{
	if (name == NULL) return NULL;
	for(CUInt i=0; i<g_waterImages.size(); i++)
	{
		if ( ICmp(GetAfterPath(g_waterImages[i]->GetName()), name ) )
			return g_waterImages[i];
	}
	return NULL;
}

CVoid CMain::UpdatePrefabInstanceBB()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CBool foundDynamicActorTarget = CFalse;

		if (!g_instancePrefab[i]->GetVisible())
		{
			if (g_instancePrefab[i]->GetIsControlledByPhysX())
			{
				for (CUInt s = 0; s < 4; s++)
				{
					CBool condition = CFalse;
					CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
					if (s < 3)
					{
						if (prefab && g_instancePrefab[i]->GetPrefab()->GetHasLod(s))
							condition = CTrue;
					}
					else
					{
						if (prefab && g_instancePrefab[i]->GetHasCollider())
							condition = CTrue;
					}

					if (condition)
					{
						CScene* scene = g_instancePrefab[i]->GetScene(s);
						if (!scene) continue;
						if (!scene->m_isTrigger)
						{
							for (CUInt j = 0; j < scene->m_instanceGeometries.size(); j++)
							{
								if (scene->m_instanceGeometries[j]->GetHasPhysXActor() && scene->m_instanceGeometries[j]->GetPhysXActorDensity() > 0.0f)
								{
									for (CUInt k = 0; k < gPhysXscene->getNbActors(); k++)
									{
										//Start Of Load/Unload PhysX Actors
										if (gPhysXscene->getActors()[k]->isDynamic() && !gPhysXscene->getActors()[k]->isSleeping())
										{
											CChar actorName[MAX_NAME_SIZE];
											if (!gPhysXscene->getActors()[k]->getName()) continue; //main character
											Cpy(actorName, gPhysXscene->getActors()[k]->getName());
											if (Cmp(scene->m_instanceGeometries[j]->GetPhysXActorName(), actorName))
											{
												foundDynamicActorTarget = CTrue;
											}
										}
										if (foundDynamicActorTarget)
											break;
									}
								}
								if (foundDynamicActorTarget)
									break;
							}
						}
					}
					if (foundDynamicActorTarget)
						break;
				}
			}
		}

		if (!foundDynamicActorTarget && !g_instancePrefab[i]->GetVisible()) continue;

		if (g_instancePrefab[i]->GetIsAnimated() || g_instancePrefab[i]->GetIsControlledByPhysX())
		{
			if (g_camera->m_cameraManager->IsBoxInFrustum(&g_instancePrefab[i]->m_boundingBox[0], 8))
			{
				g_instancePrefab[i]->SetRenderForQuery(CTrue);
			}
			else
			{
				g_instancePrefab[i]->SetRenderForQuery(CFalse);
			}
		}

		g_currentInstancePrefab = g_instancePrefab[i];
		CBool foundTarget = CFalse;
		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 4; j++)
		{
			CBool condition = CFalse;
			if (j < 3)
			{
				if (prefab && g_instancePrefab[i]->GetPrefab()->GetHasLod(j))
					condition = CTrue;
			}
			else
			{
				if (prefab && g_instancePrefab[i]->GetHasCollider())
					condition = CTrue;
			}

			if (condition)
			{
				scene = g_instancePrefab[i]->GetScene(j);

				if (!scene) continue;
				if (scene->GetUpdateBB())
				{
					scene->SetUpdateBB(CFalse);
					foundTarget = CTrue;
				}
			}
		}

		if (foundTarget)
			g_instancePrefab[i]->UpdateBoundingBox(CFalse);
	}
}

CVoid CMain::InitGUISelection()
{
	GLint viewport[4];
	glSelectBuffer(MAX_NAME_SIZE, m_guiBuffer);
	(void)glRenderMode(GL_SELECT);
	g_render.ProjectionMatrix();
	g_render.PushMatrix();
	g_render.IdentityMatrix();
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluPickMatrix((CDouble)m_mousePosition.x - 3.0, (CDouble)g_height - (CDouble)m_mousePosition.y - 3.0, 6.0, 6.0, viewport);

	glOrtho(0.0, g_width, 0.0, g_height, -100.0, 100.0);

	g_render.ModelViewMatrix();
	g_render.PushMatrix();
	g_render.IdentityMatrix();

	glInitNames();
}

CVoid CMain::FinishGUISelection()
{
	g_render.ProjectionMatrix();
	g_render.PopMatrix();
	g_render.ModelViewMatrix();
	g_render.PopMatrix();

	glFlush();
	GLint hits = glRenderMode(GL_RENDER);

	if (hits > 0)
	{
		m_guiPtr = m_guiBuffer;
		for (CInt j = 0; j < hits; j++)
		{
			m_guiPtr++; //skip the first info ( number of names which is one in my engine )and move to the min depth info

			if (*m_guiPtr < m_guiMinZ)
			{
				m_guiMinZ = *m_guiPtr;

				m_guiSelectedName = *(m_guiPtr + 2); //name of the object
			}
			m_guiPtr += 3; //move to the next hit
		}
	}
}
//Returns -1 if no selection occurs
CUInt CMain::GetSelectedGUI()
{
	m_guiMinZ = 0xffffffff;
	m_guiSelectedName = -1;
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, g_width, g_height);// resets the viewport to new dimensions.

	InitGUISelection();
	//render here
	for (CUInt i = 0; i < g_guis.size(); i++)
	{
		if (g_guis[i]->GetVisible())
		{
			for (CUInt j = 0; j < g_guis[i]->m_guiImages.size(); j++)
			{
				g_guis[i]->m_guiImages[j]->Render(g_guis[i]->GetPosition(CTrue), CTrue);
			}
			for (CUInt j = 0; j < g_guis[i]->m_guiButtons.size(); j++)
			{
				g_guis[i]->m_guiButtons[j]->Render(g_guis[i]->GetPosition(CTrue), CTrue);
			}
			for (CUInt j = 0; j < g_guis[i]->m_guiTexts.size(); j++)
			{
				g_guis[i]->m_guiTexts[j]->Render(g_guis[i]->GetPosition(CTrue), CTrue);
			}
		}
	}
	FinishGUISelection();

	glPopAttrib();

	return m_guiSelectedName;
}

void CMain::GenerateMenuCursorTexture(char* fileName)
{
	DeleteMenuCursorTexture();

	m_menuCursorImg = CNew(CImage);

	Cpy(m_strMenuCursorImg, fileName);

	if (!CTexture::LoadDDSTexture(m_menuCursorImg, m_strMenuCursorImg, NULL))
		MessageBox(NULL, _T("GenerateMenuCursorTexture>Couldn't load the cursor image"), _T("VandaEngine Error"), MB_OK);

	m_menuCursorImg->SetFileName(GetAfterPath(m_strMenuCursorImg));
}

void CMain::DeleteMenuCursorTexture()
{
	CDelete(m_menuCursorImg);
}

CFloat CMain::GetCursorX()
{
	CFloat posX = (CFloat)g_main->m_mousePosition.x;
	return posX;
}

CFloat CMain::GetCursorY()
{
	CFloat posY = CFloat(g_height - g_main->m_mousePosition.y);
	return posY;
}

CVoid CMain::UpdateCharacterTransformations()
{
	if (g_mainCharacter && g_mainCharacter->GetCameraType() == ePHYSX_CAMERA_FIRST_PERSON) return;

	if (!g_databaseVariables.m_insertCharacter)
		return;

	if (!g_mainCharacter)
		return;

	static CFloat lerpFactor = 0.0f;

	g_nx->gCharacterPos = g_nx->GetRawCharacterPos();
	CFloat y = (CFloat)g_nx->gCharacterPos.y - (g_physXProperties.m_fCapsuleHeight * 0.5f) - g_physXProperties.m_fCapsuleRadius - g_physXProperties.m_fCharacterSkinWidth;
	CVec3f translate(g_nx->gCharacterPos.x, y, g_nx->gCharacterPos.z);

	g_mainCharacter->GetInstancePrefab()->SetTranslate(translate);
	if (g_nx->m_pushCharacter)
	{
		CFloat y = NxMath::radToDeg(atan2(g_nx->m_horizontalDisp.x, g_nx->m_horizontalDisp.z));
		if (y < 0.0f)
			y = 360.0f + y;
		if (y == 360.0f)
			y = 0.0f;
		if (g_nx->m_previousMoveDirection != g_nx->m_currentMoveDirection)
		{
			m_characterRotationTransition = CTrue;
			lerpFactor = 0.0f;
		}
		else if (!m_characterRotationTransition)
		{
			g_mainCharacter->SetCurrentRotation(y);
		}
		if (m_characterRotationTransition)
		{
			//find the shortest path to rotate
			if (g_mainCharacter->GetCurrentRotation() > y)
			{
				if (g_mainCharacter->GetCurrentRotation() - y > 360 + y - g_mainCharacter->GetCurrentRotation())
					y = y + 360.0f;
			}
			else if (g_mainCharacter->GetCurrentRotation() < y)
			{
				if (y - g_mainCharacter->GetCurrentRotation() > 360 + g_mainCharacter->GetCurrentRotation() - y)
					g_mainCharacter->SetCurrentRotation(g_mainCharacter->GetCurrentRotation() + 360.0f);
			}

			g_mainCharacter->SetCurrentRotation(Lerp(g_mainCharacter->GetCurrentRotation(), y, lerpFactor));
			lerpFactor += 2.0 * elapsedTime; //total: 0.5
			//if it's equal to y, stop the transition
			if (fabs(g_mainCharacter->GetCurrentRotation() - y) <= elapsedTime)
			{
				m_characterRotationTransition = CFalse;
				lerpFactor = 0.0f;
			}
		}

		CVec4f rot(0.0f, g_mainCharacter->GetCurrentRotation(), 0.0f, 0.0f);

		m_previousCharacterRotation = y;
		g_mainCharacter->GetInstancePrefab()->SetRotate(rot);
	}

	CVec4f trans(g_mainCharacter->GetInstancePrefab()->GetTranslate().x, g_mainCharacter->GetInstancePrefab()->GetTranslate().y, g_mainCharacter->GetInstancePrefab()->GetTranslate().z, 1.0f);
	CVec4f scale(g_mainCharacter->GetInstancePrefab()->GetScale().x, g_mainCharacter->GetInstancePrefab()->GetScale().y, g_mainCharacter->GetInstancePrefab()->GetScale().z, 1.0f);
	CVec4f rotx(1.0f, 0.0f, 0.0f, g_mainCharacter->GetInstancePrefab()->GetRotate().x);
	CVec4f roty(0.0f, 1.0f, 0.0f, g_mainCharacter->GetInstancePrefab()->GetRotate().y);
	CVec4f rotz(0.0f, 0.0f, 1.0f, g_mainCharacter->GetInstancePrefab()->GetRotate().z);
	CMatrixLoadIdentity((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix());
	CMatrix4x4Translate((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix(), trans);
	CMatrix4x4RotateAngleAxis((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix(), rotx);
	CMatrix4x4RotateAngleAxis((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix(), roty);
	CMatrix4x4RotateAngleAxis((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix(), rotz);
	CMatrix4x4Scale((CFloat*)g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix(), scale);

	g_currentInstancePrefab = g_mainCharacter->GetInstancePrefab();

	if (!g_mainCharacter->GetInstancePrefab()->GetVisible()) return;
	if (!g_mainCharacter->GetInstancePrefab()->GetIsAnimated()) return;

	CScene* scene = NULL;

	CPrefab* prefab = g_mainCharacter->GetInstancePrefab()->GetPrefab();

	if (g_mainCharacter->GetInstancePrefab()->GetHasCollider())
	{
		CScene* scene = g_mainCharacter->GetInstancePrefab()->GetScene(3);
		if (scene && scene->m_isTransformable)
		{
			if (scene->m_controllers.size() == 0)
			{
				scene->m_sceneRoot->SetLocalMatrix(g_mainCharacter->GetInstancePrefab()->GetInstanceMatrix());
				scene->m_update = CTrue;
			}
		}
	}

	for (CUInt j = 0; j < 4; j++)
	{
		CBool update = CFalse;

		if (j < 3)
		{
			if (prefab && prefab->GetHasLod(j))
				update = CTrue;
		}
		else
		{
			if (g_mainCharacter->GetInstancePrefab()->GetHasCollider())
				update = CTrue;
		}

		if (update)
		{
			scene = g_mainCharacter->GetInstancePrefab()->GetScene(j);
			if (!scene) continue;

			g_render.SetScene(scene);

			if (!g_render.GetScene()->m_isTrigger)
			{
				if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
				{
					if ((g_render.GetScene()->GetAnimationStatus() == eANIM_PLAY && g_render.GetScene()->UpdateAnimationLists()))
					{
						g_render.GetScene()->Update(elapsedTime);
						g_render.GetScene()->m_updateAnimation = CFalse;
						g_render.GetScene()->m_update = CFalse;
						g_render.GetScene()->SetUpdateBB(CTrue);
					} //if
					else if (g_render.GetScene()->m_update)
					{
						g_render.GetScene()->Update();
						g_render.GetScene()->m_update = CFalse;
					} //else

				} //if
				else if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				} //else
			} //if
		}
	}
}

CVoid CMain::UpdatePrefabInstanceTransformations()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CInstancePrefab* currentInstance = g_instancePrefab[i];

		if (Cmp(currentInstance->GetName(), "VANDA_MAIN_CHARACTER")) continue;

		if (!currentInstance->GetUpdateAnimation()) continue;

		CVec4f trans(currentInstance->GetTranslate().x, currentInstance->GetTranslate().y, currentInstance->GetTranslate().z, 1.0f);
		CVec4f scale(currentInstance->GetScale().x, currentInstance->GetScale().y, currentInstance->GetScale().z, 1.0f);
		CVec4f rotx(1.0f, 0.0f, 0.0f, currentInstance->GetRotate().x);
		CVec4f roty(0.0f, 1.0f, 0.0f, currentInstance->GetRotate().y);
		CVec4f rotz(0.0f, 0.0f, 1.0f, currentInstance->GetRotate().z);
		CMatrixLoadIdentity((CFloat*)currentInstance->GetInstanceMatrix());
		CMatrix4x4Translate((CFloat*)currentInstance->GetInstanceMatrix(), trans);
		CMatrix4x4RotateAngleAxis((CFloat*)currentInstance->GetInstanceMatrix(), rotx);
		CMatrix4x4RotateAngleAxis((CFloat*)currentInstance->GetInstanceMatrix(), roty);
		CMatrix4x4RotateAngleAxis((CFloat*)currentInstance->GetInstanceMatrix(), rotz);
		CMatrix4x4Scale((CFloat*)currentInstance->GetInstanceMatrix(), scale);

		for (CUInt i = 0; i < 4; i++)
		{
			CBool condition = CFalse;
			if (i < 3)
			{
				if (currentInstance->GetPrefab()->GetHasLod(i))
					condition = CTrue;
			}
			else
			{
				if (currentInstance->GetHasCollider())
					condition = CTrue;
			}
			if (condition)
			{
				CScene* scene = currentInstance->GetScene(i);
				if (scene && scene->m_isTransformable)
				{
					if (scene->m_controllers.size() == 0)
					{
						scene->m_sceneRoot->SetLocalMatrix(currentInstance->GetInstanceMatrix());
					}
				}
			}
		}
	}
}

CVoid CMain::CalculateAndSort3DTransparentModelDistances()
{
	for (CUInt i = 0; i < m_transparentGeometries.size(); i++)
	{
		m_transparentGeometries[i].m_instanceGeometry->CalculateDistance();
	}
	std::sort(m_transparentGeometries.begin(), m_transparentGeometries.end(), SortTransparentGeometry());
}

CVoid CMain::RenderWaters()
{
	//render waters here
	CVec3f cameraPos(g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z);

	if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	{
		for (CUInt i = 0; i < g_engineWaters.size(); i++)
		{
			if (g_engineWaters[i]->GetOutsideFrustom()) continue;

			if (CTrue/*g_currentCameraType == eCAMERA_PHYSX*/)
			{
				if (g_engineWaters[i]->GetVisible() && g_engineWaters[i]->GetQueryVisible())
				{
					glUseProgram(g_render.m_waterProgram);
					g_engineWaters[i]->RenderWater(cameraPos, elapsedTime);
					glUseProgram(0);
				}
			}
			else
			{
				glUseProgram(g_render.m_waterProgram);
				g_engineWaters[i]->RenderWater(cameraPos, elapsedTime);
				glUseProgram(0);
			}
		}
	}
}

CBool CMain::DoesGeometryInstanceIntersectsWater(CInstanceGeometry* geometryInstance, CWater* water)
{
	CFloat xmin, xmax, ymin, ymax;
	xmin = water->m_sidePoint[0].x; ymin = water->m_sidePoint[0].z;
	xmax = water->m_sidePoint[2].x; ymax = water->m_sidePoint[2].z;

	CInstanceGeometry* current_geometry = geometryInstance;
	CVec2f v1, v2, v3, v4;
	if (current_geometry->m_hasPhysX && current_geometry->m_physXDensity > 0.0f)
	{
		v1.x = current_geometry->m_minLocalToWorldAABBControlledByPhysX.x; v1.y = current_geometry->m_minLocalToWorldAABBControlledByPhysX.z;
		v2.x = current_geometry->m_minLocalToWorldAABBControlledByPhysX.x; v2.y = current_geometry->m_maxLocalToWorldAABBControlledByPhysX.z;
		v3.x = current_geometry->m_maxLocalToWorldAABBControlledByPhysX.x; v3.y = current_geometry->m_maxLocalToWorldAABBControlledByPhysX.z;
		v4.x = current_geometry->m_maxLocalToWorldAABBControlledByPhysX.x; v4.y = current_geometry->m_minLocalToWorldAABBControlledByPhysX.z;
	}
	else
	{
		v1.x = current_geometry->m_minLocalToWorldAABB.x; v1.y = current_geometry->m_minLocalToWorldAABB.z;
		v2.x = current_geometry->m_minLocalToWorldAABB.x; v2.y = current_geometry->m_maxLocalToWorldAABB.z;
		v3.x = current_geometry->m_maxLocalToWorldAABB.x; v3.y = current_geometry->m_maxLocalToWorldAABB.z;
		v4.x = current_geometry->m_maxLocalToWorldAABB.x; v4.y = current_geometry->m_minLocalToWorldAABB.z;
	}

	if ((v1.x > xmin && v1.x < xmax && v1.y > ymin && v1.y < ymax) ||
		(v2.x > xmin && v2.x < xmax && v2.y > ymin && v2.y < ymax) ||
		(v3.x > xmin && v3.x < xmax && v3.y > ymin && v3.y < ymax) ||
		(v4.x > xmin && v4.x < xmax && v4.y > ymin && v4.y < ymax))
	{
		return CTrue;
	}
	return CFalse;
}

CVoid CMain::GetMouseMovement()
{
	if (GetActiveWindow() == g_window.m_windowGL.hWnd)
	{
		ShowCursor(FALSE);

		GetCursorPos(&m_point);

		m_dx = m_point.x - m_prev_dx;
		m_dy = m_point.y - m_prev_dy;

		//reset cursor position
		m_point.x = g_width / 2;
		m_point.y = (g_height / 2) + m_padding;

		SetCursorPos(m_point.x, m_point.y);

		m_prev_dx = m_point.x;
		m_prev_dy = m_point.y;
	}
}

CVoid CMain::PauseSounds()
{
	//pause all sounds as well
	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}
	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}
	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PauseALSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}
}

CVoid CMain::ResumeSounds()
{
	//pause all sounds as well
	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		if (g_engineAmbientSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engineAmbientSounds[i]->GetSoundSource()));
		}
	}
	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		if (g_engine3DSounds[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_engine3DSounds[i]->GetSoundSource()));
		}
	}
	for (CUInt i = 0; i < g_resourceFiles.size(); i++)
	{
		if (g_resourceFiles[i]->GetSoundSource())
		{
			g_soundSystem->PlayALPausedSound(*(g_resourceFiles[i]->GetSoundSource()->GetSoundSource()));
		}
	}
}

CVoid CMain::PauseGame()
{
	g_currentVSceneProperties.m_lockCharacterController = CTrue;

	m_pauseMainCharacterAnimations = CTrue;

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetUpdateAnimation(CFalse);
	}

	m_pausePhysics = CTrue;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		g_engineWaters[i]->SetUpdateAnimation(CFalse);
	}

	PauseAllScriptUpdateEvents();
}

CVoid CMain::ResumeGame()
{
	g_currentVSceneProperties.m_lockCharacterController = CFalse;

	m_pauseMainCharacterAnimations = CFalse;

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetUpdateAnimation(CTrue);
	}

	m_pausePhysics = CFalse;

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		g_engineWaters[i]->SetUpdateAnimation(CTrue);
	}

	ResumeAllScriptUpdateEvents();
}

CVoid CMain::PauseAllScriptUpdateEvents()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetUpdateEvent(CFalse);
	}

	if (g_VSceneScript)
		g_VSceneScript->SetUpdateEvent(CFalse);

	if (g_skyDome)
		g_skyDome->SetUpdateEvent(CFalse);

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		g_engineWaters[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		g_engineLights[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		g_engine3DSounds[i]->SetUpdateEvent(CFalse);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		g_engineAmbientSounds[i]->SetUpdateEvent(CFalse);
	}

	if (g_mainCharacter)
		g_mainCharacter->SetUpdateEvent(CFalse);

	if (g_terrain)
		g_terrain->SetUpdateEvent(CFalse);

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		g_engineCameraInstances[i]->SetUpdateEvent(CFalse);
	}
}

CVoid CMain::ResumeAllScriptUpdateEvents()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		g_instancePrefab[i]->SetUpdateEvent(CTrue);
	}

	if (g_VSceneScript)
		g_VSceneScript->SetUpdateEvent(CTrue);

	if (g_skyDome)
		g_skyDome->SetUpdateEvent(CTrue);

	for (CUInt i = 0; i < g_engineWaters.size(); i++)
	{
		g_engineWaters[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineLights.size(); i++)
	{
		g_engineLights[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engine3DSounds.size(); i++)
	{
		g_engine3DSounds[i]->SetUpdateEvent(CTrue);
	}

	for (CUInt i = 0; i < g_engineAmbientSounds.size(); i++)
	{
		g_engineAmbientSounds[i]->SetUpdateEvent(CTrue);
	}

	if (g_mainCharacter)
		g_mainCharacter->SetUpdateEvent(CTrue);

	if (g_terrain)
		g_terrain->SetUpdateEvent(CTrue);

	for (CUInt i = 0; i < g_engineCameraInstances.size(); i++)
	{
		g_engineCameraInstances[i]->SetUpdateEvent(CTrue);
	}

}

CInt CMain::DeletePrefabInstancesAtRuntime()
{
	if (m_prefabInstanceNamesToBeDeleted.size() == 0)
		return 0;

	for (CUInt index = 0; index < m_prefabInstanceNamesToBeDeleted.size(); index++)
	{
		CChar name[MAX_NAME_SIZE];
		Cpy(name, m_prefabInstanceNamesToBeDeleted[index].c_str());

		CBool foundTarget = CFalse;
		CPrefab* dstPrefab = NULL;
		CBool isStatic = CFalse;

		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			CChar currentInstanceName[MAX_NAME_SIZE];
			Cpy(currentInstanceName, g_instancePrefab[i]->GetName());
			StringToUpper(currentInstanceName);

			if (Cmp(currentInstanceName, name))
			{
				dstPrefab = g_instancePrefab[i]->GetPrefab();
				isStatic = g_instancePrefab[i]->GetIsStatic();
				//remove instance from prefab
				for (CUInt j = 0; j < dstPrefab->GetNumInstances(); j++)
				{
					CBool foundTarget = CFalse;
					if (Cmp(dstPrefab->GetInstance(j)->GetName(), g_instancePrefab[i]->GetName()))
					{
						dstPrefab->RemoveInstance(j);
						foundTarget = CTrue;
					}
					if (foundTarget)
						break;
				}
				for (CUInt k = 0; k < 4; k++)
				{
					CBool condition = CFalse;
					if (k < 3)
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(k))
							condition = CTrue;
					}
					else
					{
						if (g_instancePrefab[i]->GetHasCollider())
							condition = CTrue;
					}
					if (condition)
					{
						CScene* scene = g_instancePrefab[i]->GetScene(k);
						g_main->RemoveSelectedScene(scene->GetName(), scene->GetDocURI());
					}
				}

				//remove it from water as well
				for (CUInt k = 0; k < g_engineWaters.size(); k++)
				{
					for (CUInt l = 0; l < g_engineWaters[k]->GetNumPrefabInstances(); l++)
					{
						if (Cmp(g_engineWaters[k]->GetPrefabInstance(l)->GetName(), g_instancePrefab[i]->GetName()))
							g_engineWaters[k]->RemovePrefabInstance(l);
					}
				}

				CDelete(g_instancePrefab[i]);
				g_instancePrefab.erase(g_instancePrefab.begin() + i);

				foundTarget = CTrue;
			}
			if (foundTarget)
				break;
		}

		if (foundTarget)
		{
			if (dstPrefab && dstPrefab->GetNumInstances() == 0)
			{
				//now remove the prefab
				for (CUInt k = 0; k < g_prefab.size(); k++)
				{
					if (Cmp(dstPrefab->GetName(), g_prefab[k]->GetName()))
					{
						CDelete(g_prefab[k]);
						g_prefab.erase(g_prefab.begin() + k);
						break;
					}
				}
			}
			if (isStatic)
				g_octree->ResetState();
		}

		//update max radius
		g_maxInstancePrefabRadius = -1.f;
		for (CUInt j = 0; j < g_instancePrefab.size(); j++)
		{
			if (g_instancePrefab[j]->GetScene(0) && g_instancePrefab[j]->GetScene(0)->CastShadow())
			{
				if (g_instancePrefab[j]->GetRadius() > g_maxInstancePrefabRadius)
					g_maxInstancePrefabRadius = g_instancePrefab[j]->GetRadius();
			}
		}
	}

	m_prefabInstanceNamesToBeDeleted.clear();

	return 0;
}