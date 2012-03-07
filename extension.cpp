/**
 * vim: set ts=4 :
 * ======================================================
 * Metamod:Source Stub Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This stub plugin is public domain.
 *
 * Version: $Id$
 */

#include <stdio.h>
#include "extension.h"
#include "extension_util.h"
#include "sm_ext.h"

SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);

SM_Extension g_SM_Extension;
IVEngineServer *engine = NULL;
IServerGameDLL *server = NULL;
ISmmPlugin *mmsplugin = &g_SM_Extension;

PLUGIN_EXPOSE(SM_Extension, g_SM_Extension);
bool SM_Extension::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

#if defined METAMOD_PLAPI_VERSION
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
#else
	GET_V_IFACE_ANY(serverFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(engineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
#endif

	SH_ADD_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, server, Hook_ServerActivate, true);

	ismm->AddListener(this, this);

	return true;
}

bool SM_Extension::Unload(char *error, size_t maxlen)
{
	SM_UnloadExtension();

	SH_REMOVE_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, server, Hook_ServerActivate, true);

	return true;
}

void Hook_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	META_LOG(g_PLAPI, "ServerActivate() called: edictCount = %d, clientMax = %d", edictCount, clientMax);
}

void *SM_Extension::OnMetamodQuery(const char *iface, int *ret)
{
	if (strcmp(iface, SOURCEMOD_NOTICE_EXTENSIONS) == 0)
	{
		BindToSourcemod();
	}

	if (ret != NULL)
	{
		*ret = IFACE_OK;
	}

	return NULL;
}

void SM_Extension::AllPluginsLoaded()
{
	BindToSourcemod();
}

void SM_Extension::BindToSourcemod()
{
	char error[256];

	if (!SM_LoadExtension(error, sizeof(error)))
	{
		char message[512];
		UTIL_Format(message, sizeof(message), "Could not load as a SourceMod extension: %s\n", error);
		engine->LogPrint(message);
	}
}

bool SM_Extension::Pause(char *error, size_t maxlen)
{
	return true;
}

bool SM_Extension::Unpause(char *error, size_t maxlen)
{
	return true;
}

bool RegisterConCommandBase(ConCommandBase *pVar) {

#if defined METAMOD_PLAPI_VERSION
	return g_SMAPI->RegisterConCommandBase(g_PLAPI, pVar);
#else
	return g_SMAPI->RegisterConCmdBase(g_PLAPI, pVar);
#endif
}

const char *SM_Extension::GetLicense()
{
	return "Public Domain";
}

const char *SM_Extension::GetVersion()
{
	return "0.5";
}

const char *SM_Extension::GetDate()
{
	return __DATE__;
}

const char *SM_Extension::GetLogTag()
{
	return "SMPM";
}

const char *SM_Extension::GetAuthor()
{
	return "Berni";
}

const char *SM_Extension::GetDescription()
{
	return "Allows managed updating, adding, removing of plugins & extensions";
}

const char *SM_Extension::GetName()
{
	return "Sourcemod Packet Mamanger";
}

const char *SM_Extension::GetURL()
{
	return "http://www.mannisfunhouse.eu";
}

