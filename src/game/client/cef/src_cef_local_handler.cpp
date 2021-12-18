﻿

#include "cbase.h"
#include "src_cef_local_handler.h"

#include <filesystem.h>

// CefStreamResourceHandler is part of the libcef_dll_wrapper project.
#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/cef_parser.h"

// NOTE: This has to be the last file included!
#include "tier0/memdbgon.h"

ConVar cef_scheme_debug_local_handler("cef_scheme_debug_local_handler", "0");

LocalSchemeHandlerFactory::LocalSchemeHandlerFactory() {

}

CefRefPtr <CefResourceHandler> LocalSchemeHandlerFactory::Create(CefRefPtr <CefBrowser> browser,
                                                                 CefRefPtr <CefFrame> frame,
                                                                 const CefString &scheme_name,
                                                                 CefRefPtr <CefRequest> request) {
    CefRefPtr <CefResourceHandler> pResourceHandler = NULL;

    CefURLParts parts;
    CefParseURL(request->GetURL(), parts);

    if (CefString(&parts.path).size() < 2) {
        return NULL;
    }

    char path[MAX_PATH];
    V_strncpy(path, CefString(&parts.path).ToString().c_str() + 1, sizeof(path));
    V_FixupPathName(path, sizeof(path), path);

    if (filesystem->IsDirectory(path)) {
        V_AppendSlash(path, sizeof(path));
        V_strcat(path, "index.html", sizeof(path));
    }

    if (filesystem->FileExists(path, NULL)) {
        const char *pExtension = V_GetFileExtension(path);

        if (cef_scheme_debug_local_handler.GetBool()) {
            Msg("Local scheme request => Path: %s, Extension: %s, Mime Type: %s, modified path: %s, exists: %d, resource type: %d\n",
                CefString(&parts.path).ToString().c_str(), pExtension, CefGetMimeType(pExtension).ToString().c_str(),
                path, filesystem->FileExists(path),
                request->GetResourceType());
        }

        CUtlBuffer buf(0, filesystem->Size(path, NULL));
        if (filesystem->ReadFile(path, NULL, buf)) {
            CefRefPtr <CefStreamReader> stream =
                    CefStreamReader::CreateForData(buf.Base(), buf.TellPut());
            if (stream != NULL) {
                pResourceHandler = new CefStreamResourceHandler(CefGetMimeType(pExtension), stream);
            }
        }
    }

    return pResourceHandler;
}