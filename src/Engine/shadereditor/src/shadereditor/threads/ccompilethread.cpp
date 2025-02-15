﻿
#include "cbase.h"
#include "editorCommon.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <direct.h>

#define COMPILE_FRENZY 0
#if COMPILE_FRENZY
static ConVar sedit_cf( "shaderEditor_compilefrenzy", "0" );
#endif


char *compilerSMConstants[4][2] =
        {
                "DSHADER_MODEL_PS_2_B", "Tps_2_b",
                "DSHADER_MODEL_PS_3_0", "Tps_3_0",
                "DSHADER_MODEL_VS_2_0", "Tvs_2_0",
                "DSHADER_MODEL_VS_3_0", "Tvs_3_0",
        };


#ifdef SHADER_EDITOR_DLL_2006

void BuildComboStrings( CUtlVector< SimpleCombo* > &combos, int curDepth,
    int index_static, int index_dynamic,
    int multiplier_static, int multiplier_dynamic,
    CUtlBufferEditor &buf, const char *szPrefixFormat, const char *szSuffix,
    const char *szCurComboList )
{
    if ( curDepth >= combos.Count() )
        return;

    char localComboList[4096];
    localComboList[0] = '\0';

    if ( szCurComboList )
        Q_strcat( localComboList, szCurComboList, sizeof( localComboList ) );

    SimpleCombo *c = combos[ curDepth ];
    const bool bEnd = curDepth == ( combos.Count() - 1 );
    const bool bDynamic = !c->bStatic;
    const int maxPaths = c->GetAmt();
    int nextMultiplier_Dynamic = multiplier_dynamic;
    int nextMultiplier_Static = multiplier_static;

    if ( bDynamic )
        nextMultiplier_Dynamic *= maxPaths;
    else
        nextMultiplier_Static *= maxPaths;

    for ( int i = 0; i < maxPaths; i++ )
    {
        const int curMultiplier = i;
        const int curCombo = (bDynamic ? multiplier_dynamic : multiplier_static) * curMultiplier;
        int curIndex_Static = index_static;
        int curIndex_Dynamic = index_dynamic;

        if ( bDynamic )
            curIndex_Dynamic += curCombo;
        else
            curIndex_Static += curCombo;

        char curComboEntry[MAX_PATH];
        Q_snprintf( curComboEntry, sizeof( curComboEntry ), "/D%s=%i ", c->name, curCombo );
        Q_strupr( curComboEntry );
        Q_strcat( localComboList, curComboEntry, sizeof( localComboList ) );

        if ( !bEnd )
            BuildComboStrings( combos, curDepth + 1, curIndex_Static, curIndex_Dynamic,
                nextMultiplier_Static, nextMultiplier_Dynamic,
                buf, szPrefixFormat, szSuffix, localComboList );
        else
        {
            int shaderCombo = curIndex_Dynamic + curIndex_Static;
            char prefix[2048];
            Q_snprintf( prefix, sizeof( prefix ), szPrefixFormat, shaderCombo );

            char cmd[2048];
            Q_snprintf( cmd, sizeof( cmd ), "%s %s%s", prefix, localComboList, szSuffix );
        }
    }
}

#endif

void BuildFilelistEntry(CUtlBufferEditor &buf, GenericShaderData *data, bool bPS, bool bPosLock) {
    //IdentifierLists_t *pIdentCopy = new IdentifierLists_t( *( bPS ? data->shader->pPS_Identifiers : data->shader->pVS_Identifiers ) );
    //CUtlVector< SimpleCombo* > *_combos = &pIdentCopy->hList_Combos;

    CUtlVector<SimpleCombo *> *_combos = bPS ? &data->shader->pPS_Identifiers->hList_Combos
                                             : &data->shader->pVS_Identifiers->hList_Combos;

    if (bPosLock)
        _combos = NULL;
    //else if ( data->bPreview )
    //	RemoveAllNonPreviewCombos( *_combos );

    char File[MAX_PATH];
    char FileExt[MAX_PATH];
    ComposeShaderName(data, bPS, false, File, sizeof(File), bPosLock);
    ComposeShaderName(data, bPS, true, FileExt, sizeof(FileExt), bPosLock);

    char FXCCmd[MAX_PATH * 4];
    char CmdEnd[MAXTARGC];

    int numDCombos = 1;
    int numSCombos = 1;
    for (int i = 0; _combos && i < _combos->Count(); i++) {
        SimpleCombo *c = (*_combos)[i];
        int delta = c->max - c->min + 1;
        if (c->bStatic)
            numDCombos *= delta;
        else
            numSCombos *= delta;
    }
    int numCombos_All = numSCombos * numDCombos;
    int SMszIdx = ((data->shader->iShaderModel == SM_30) ? 1 : 0) + (bPS ? 0 : 2);

#ifdef SHADER_EDITOR_DLL_2006
    Q_snprintf( FXCCmd, sizeof(FXCCmd), "%s/bin/fxc.exe", GetShaderSourceDirectory() );
#else
    Q_snprintf(FXCCmd, sizeof(FXCCmd), "fxc.exe");
#endif
    Q_snprintf(CmdEnd, sizeof(CmdEnd), "/Dmain=main /Emain /%s /%s=1 /nologo /Foshader.o %s>output.txt 2>&1\n",
               compilerSMConstants[SMszIdx][1],
               compilerSMConstants[SMszIdx][0],
               FileExt);
    Q_FixSlashes(FXCCmd, '/');

#ifdef SHADER_EDITOR_DLL_2006

    char prefix[MAXTARGC];
    Q_snprintf( prefix, sizeof(prefix),
        "%s /DSHADERCOMBO=%%i /DTOTALSHADERCOMBOS=%i /DCENTROIDMASK=0 /DNUMDYNAMICCOMBOS=%i /DFLAGS=0x0",
        FXCCmd, numCombos_All, numDCombos );

    if ( _combos != NULL && _combos->Count() > 0 )
        BuildComboStrings( *_combos, 0, 0, 0, numDCombos, 1, buf,
        prefix, CmdEnd, NULL );
    else
    {
        char prefix_default[MAXTARGC];
        Q_snprintf( prefix_default, sizeof(prefix_default), prefix, 0 );

        buf.PutString( prefix_default );
        buf.PutString( " " );
        buf.PutString( CmdEnd );
    }

#else

    char tmp[MAXTARGC];

    buf.PutString("*** generated by shadereditor.dll ***\n");
    Q_snprintf(tmp, sizeof(tmp), "#BEGIN %s\n", File);
    buf.PutString(tmp);
    Q_snprintf(tmp, sizeof(tmp), "%s\n", FileExt);
    buf.PutString(tmp);
    buf.PutString("#DEFINES-D:\n");
    for (int i = 0; _combos && i < _combos->Count(); i++)
        if (!(*_combos)[i]->bStatic &&
            ((*_combos)[i]->bInPreviewMode || !data->IsPreview())) {
            Q_snprintf(tmp, sizeof(tmp), "%s=%i..%i\n", (*_combos)[i]->name, (*_combos)[i]->min, (*_combos)[i]->max);
            buf.PutString(tmp);
        }
    buf.PutString("#DEFINES-S:\n");
    for (int i = 0; _combos && i < _combos->Count(); i++)
        if ((*_combos)[i]->bStatic &&
            ((*_combos)[i]->bInPreviewMode || !data->IsPreview())) {
            Q_snprintf(tmp, sizeof(tmp), "%s=%i..%i\n", (*_combos)[i]->name, (*_combos)[i]->min, (*_combos)[i]->max);
            buf.PutString(tmp);
        }
    buf.PutString("#SKIP:\n");
    buf.PutString(
            "(defined $HDRTYPE && defined $HDRENABLED && !$HDRTYPE && $HDRENABLED)||(defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA)||(defined $LIGHTING_PREVIEW && defined $HDRTYPE && $LIGHTING_PREVIEW && $HDRTYPE != 0)||(defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT)||(defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST)||(defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH)||(($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW)||0\n");
    buf.PutString("#COMMAND:\n");
    Q_snprintf(tmp, sizeof(tmp), "%s /DTOTALSHADERCOMBOS=%i /DCENTROIDMASK=0 /DNUMDYNAMICCOMBOS=%i /DFLAGS=0x0 ",
               FXCCmd, numCombos_All, numDCombos);
    buf.PutString(tmp);
    buf.PutString(CmdEnd);
    buf.PutString("#END\n");
    buf.PutString("*************************************\n");

#endif
}

static CCompileThread t;

CCompileThread *CCompileThread::CreateWriteFXCThread() {
    //CCompileThread *t = new CCompileThread();
    //return t;
    return &t;
}

CCompileThread::CCompileThread() {
    _solvers_vs = NULL;
    _solvers_ps = NULL;
    _shaderData = NULL;
    _combos_vs.PurgeAndDeleteElements();
    _combos_ps.PurgeAndDeleteElements();
    _target = NULL;
    curFlags = 0;
}

CCompileThread::~CCompileThread() {
    delete _shaderData;
    if (_solvers_vs) {
        DestroySolverStack(*_solvers_vs);
        delete _solvers_vs;
    }
    if (_solvers_ps) {
        DestroySolverStack(*_solvers_ps);
        delete _solvers_ps;
    }

    for (int i = 0; i < m_hCurrentMessages.Count(); i++)
        DestroyCompilerMessage(m_hCurrentMessages[i]);
    m_hCurrentMessages.Purge();
}

void CCompileThread::DestroyCompilerMessage(__threadcmds_CompileCommand *msg) {
    if (msg->__data)
        delete msg->__data;
    msg->__data = NULL;

    if (msg->__ps)
        DestroySolverStack(*((CUtlVector<CHLSL_SolverBase *> *) msg->__ps));
    if (msg->__vs)
        DestroySolverStack(*((CUtlVector<CHLSL_SolverBase *> *) msg->__vs));
    if (msg->__undef)
        DestroySolverStack(*((CUtlVector<CHLSL_SolverBase *> *) msg->__undef));

    delete msg;
}

void CCompileThread::UniquifyCommandList() {
    if (!m_hCurrentMessages.Count())
        return;

    CUtlVector<HAUTOSOLVER *> hKnownIndices;

    for (int i = m_hCurrentMessages.Count() - 1; i >= 0; i--) {
        HAUTOSOLVER cur = m_hCurrentMessages[i]->_Requester;
        bool bKill = false;
        for (int a = 0; a < hKnownIndices.Count(); a++)
            if (*hKnownIndices[a] == cur)
                bKill = true;

        if (bKill) {
            for (int x = m_hCurrentMessages.Count() - 1; x > i; x--) {
                if (m_hCurrentMessages[i]->_Requester == cur) {
                    __threadcmds_CompileCommand *cached = m_hCurrentMessages[x];
                    __threadcmds_CompileCommand *older = m_hCurrentMessages[i];

                    if (!cached->__ps) {
                        cached->__ps = older->__ps;
                        older->__ps = NULL;

                        delete cached->__data->shader->pPS_Identifiers;
                        cached->__data->shader->pPS_Identifiers = older->__data->shader->pPS_Identifiers;
                        older->__data->shader->pPS_Identifiers = NULL;
                    }
                    break;
                }
            }
            DestroyCompilerMessage(m_hCurrentMessages[i]);
            m_hCurrentMessages.Remove(i);
            continue;
        }

        hKnownIndices.AddToTail(new HAUTOSOLVER(cur));
    }

    hKnownIndices.PurgeAndDeleteElements();
}

int CCompileThread::Run() {
    for (;;) {

#if COMPILE_FRENZY
        const bool bCompileFrenzy = sedit_cf.GetBool();
#endif

        //switch (CheckCommon())
        //{
        //case TCMD_COMMON_EXIT:
        //	delete this;
        //	return 0;
        //}

        while (m_Queue_CompileCommand.MessageWaiting()) {
            __threadcmds_CompileCommand *msg;
            m_Queue_CompileCommand.WaitMessage(&msg);
            //if (msg->bHaltOnError)
            //	Sleep( 100000 );
            m_hCurrentMessages.AddToTail(msg);
        }

        UniquifyCommandList();

        //while ( m_Queue_CompileCommand.MessageWaiting() )
        for (int i = 0; i < m_hCurrentMessages.Count(); i++) {
            //__threadcmds_CompileCommand msg_solvers;
            //m_Queue_CompileCommand.WaitMessage( &msg_solvers );
            __threadcmds_CompileCommand *msg_solvers = m_hCurrentMessages[i];
            _target = &msg_solvers->_Requester;
            //if ( m_Queue_CompileCommand.MessageWaiting() )
            //{
            //	continue;
            //}

            if (_solvers_vs) {
                DestroySolverStack(*_solvers_vs);
                _solvers_vs = NULL;
            }
            if (_solvers_ps) {
                DestroySolverStack(*_solvers_ps);
                _solvers_ps = NULL;
            }
            if (msg_solvers->__undef)
                DestroySolverStack(*((CUtlVector<CHLSL_SolverBase *> *) msg_solvers->__undef));
            if (_shaderData) {
                delete _shaderData;
                _shaderData = NULL;
            }

            //_solvers_vs = new CUtlVector< CHLSL_SolverBase* >;
            //CopySolvers( *((CUtlVector< CHLSL_SolverBase* >*)msg_solvers.__vs), *_solvers_vs );

            _solvers_vs = ((CUtlVector<CHLSL_SolverBase *> *) msg_solvers->__vs);
            _solvers_ps = ((CUtlVector<CHLSL_SolverBase *> *) msg_solvers->__ps);
#if (COMPILE_FRENZY)
            if (bCompileFrenzy)
                _shaderData = new GenericShaderData( *msg_solvers->__data );
            else
#endif
            _shaderData = msg_solvers->__data;

            static unsigned int _HackyIndexIncrement = 0;
            if (_shaderData->IsPreview()) {
                int len = Q_strlen(_shaderData->name) + 2 + 7;
                char *uniquifyShader = new char[len];
                Q_snprintf(uniquifyShader, len, "%06X_%s", _HackyIndexIncrement, _shaderData->name);
                delete[] _shaderData->name;
                _shaderData->name = uniquifyShader;
            }


            curFlags = 0;

            if (_shaderData) {

                _combos_vs.PurgeAndDeleteElements();
                _combos_ps.PurgeAndDeleteElements();

                CUtlBufferEditor globalCodeBuffer;

                if (_solvers_vs) {
                    if (_shaderData->IsPreview()) {
                        ResetVariables(*_solvers_vs);

                        WriteContext_FXC Context;
                        Context.m_pActive_Identifiers = _shaderData->shader->pVS_Identifiers;
                        InitializeVariableNames(*_solvers_vs, Context);
                        WriteFXCFile_VS(true);
                        curFlags |= ACTIVEFLAG_VS_POS;
                    }

                    ResetVariables(*_solvers_vs);

                    WriteContext_FXC Context;
                    Context.m_pActive_Identifiers = _shaderData->shader->pVS_Identifiers;
                    InitializeVariableNames(*_solvers_vs, Context);
                    WriteFXCFile_VS(false, &globalCodeBuffer);
                    curFlags |= ACTIVEFLAG_VS;
                }
                if (_solvers_vs && _solvers_ps) {

#if SHOW_SEDIT_ERRORS
                    bool bHasLookupSolver = false;
                    bool bHasTextureIdentifier = _shaderData->shader->pPS_Identifiers->hList_Textures.Count() > 0;
                    {
                        CUtlVector< CHLSL_SolverBase* > &solvers = *_solvers_ps;
                        for ( int i = 0; i < solvers.Count(); i++ )
                            if ( dynamic_cast< CHLSL_Solver_TextureSample* >( solvers[i] ) )
                                bHasLookupSolver = true;
                    }
                    Assert( !bHasLookupSolver || bHasLookupSolver == bHasTextureIdentifier );
#endif

                    ResetVariables(*_solvers_ps);

                    WriteContext_FXC Context;
                    Context.m_pActive_Identifiers = _shaderData->shader->pPS_Identifiers;
                    InitializeVariableNames(*_solvers_ps, Context);
                    WriteFXCFile_PS(&globalCodeBuffer);
                    curFlags |= ACTIVEFLAG_PS;
                }

                if (curFlags != 0) {
                    _HackyIndexIncrement++;
                    WriteFileLists(curFlags, &globalCodeBuffer);

                    StartCompiler();
                    gShaderEditorSystem->SetCompilerState(false);
                }
                //else
                {
                    delete _shaderData;
                    _shaderData = NULL;
                }

                globalCodeBuffer.Purge();

#if (COMPILE_FRENZY)
                if (!bCompileFrenzy)
#endif
                msg_solvers->__data = NULL;
            }

#if (COMPILE_FRENZY)
            //Assert( curFlags != 0 || _shaderData == NULL );
#endif

            _solvers_vs = NULL;
            _solvers_ps = NULL;
            _shaderData = NULL;
#if (COMPILE_FRENZY)
            if (!bCompileFrenzy)
#endif
            {
                DestroyCompilerMessage(msg_solvers);
                m_hCurrentMessages.Remove(i);
                i--;
            }
        }
        Sleep(1);
    }
}

void CCompileThread::WriteFXCFile_VS(bool bPosOverride, CUtlBufferEditor *codeBuff) {
    char targetFile[MAX_PATH];
    ComposeShaderPath(_shaderData, false, true, targetFile, MAX_PATH, bPosOverride);
    //Msg( "writing VS: %s\n", targetFile );

    CUtlBufferEditor buf;
    buf.SetBufferType(true, true);
    buf.EnableDirectives(true);

    buf.PutString("// *********************************\n");
    buf.PutString("// ** auto generated vertexshader **\n");
    buf.PutString("// *********************************\n");
    buf.PutString("\n");

    //WriteIncludes( false, buf, *_solvers_vs );
    WriteCommon(false, buf, *_solvers_vs, bPosOverride, codeBuff);

    CUtlBuffer bufTmp;
    buf.WriteToBuffer(bufTmp);
    g_pFullFileSystem->WriteFile(targetFile, "MOD", bufTmp);
    buf.Purge();
}

void CCompileThread::WriteFXCFile_PS(CUtlBufferEditor *codeBuff) {
    char targetFile[MAX_PATH];
    ComposeShaderPath(_shaderData, true, true, targetFile, MAX_PATH);
    //Msg( "writing PS: %s\n", targetFile );

    CUtlBufferEditor buf;
    buf.SetBufferType(true, true);
    buf.EnableDirectives(true);

    buf.PutString("// ********************************\n");
    buf.PutString("// ** auto generated pixelshader **\n");
    buf.PutString("// ********************************\n");
    buf.PutString("\n");

    //WriteIncludes( true, buf, *_solvers_ps );
    WriteCommon(true, buf, *_solvers_ps, false, codeBuff);

    CUtlBuffer bufTmp;
    buf.WriteToBuffer(bufTmp);
    g_pFullFileSystem->WriteFile(targetFile, "MOD", bufTmp);
    buf.Purge();
}

void CCompileThread::WriteIncludes(bool bPS, CUtlBufferEditor &buf, CUtlVector<CHLSL_SolverBase *> &m_hSolvers) {
    buf.PutString("// Includes\n");
    if (bPS) {
        buf.PutString("#include \"common_ps_fxc.h\"\n");
        buf.PutString("#include \"common_vertexlitgeneric_dx9.h\"\n");
        buf.PutString("#include \"common_lightmappedgeneric_fxc.h\"\n");
        buf.PutString("#include \"common_flashlight_fxc.h\"\n");
        buf.PutString("#include \"common_parallax.h\"\n");
    } else {
        buf.PutString("#include \"common_vs_fxc.h\"\n");
    }
    buf.PutString("\n");
}

void BufferAppend(CUtlBufferEditor &dst, CUtlBufferEditor &src, bool bPurge = true) {
    int size = src.TellPut();
    if (!size)
        return;

    char *__buffer = new char[size + 1];
    src.SeekGet(CUtlBufferEditor::SEEK_HEAD, 0);
    src.Get(__buffer, size);
    __buffer[size] = '\0';
    dst.PutString(__buffer);
    if (bPurge)
        src.Purge();
    delete[] __buffer;
}

void CCompileThread::WriteCommon(bool bPS, CUtlBufferEditor &buf, CUtlVector<CHLSL_SolverBase *> &m_hSolvers,
                                 bool bPosOverride, CUtlBufferEditor *codeBuff) {
    WriteContext_FXC Context;
    Context.bPreview = _shaderData->IsPreview();
    Context.m_hActive_Solvers = &m_hSolvers;
    if (bPS)
        Context.m_pActive_Identifiers = _shaderData->shader->pPS_Identifiers;
    else
        Context.m_pActive_Identifiers = _shaderData->shader->pVS_Identifiers;

    bool bHACK_AddVertexID = false;

    // COMBOS
    for (int t = 0; t < Context.m_pActive_Identifiers->hList_Combos.Count(); t++) {
        SimpleCombo *combo = Context.m_pActive_Identifiers->hList_Combos[t];
        //if ( Context.bPreview && ( combo->iComboType != HLSLCOMBO_VERTEXCOMPRESSION || bPosOverride ) )
        if (Context.bPreview && (!combo->bInPreviewMode || bPosOverride)) {
            int defaultValue = 0;
            if (combo->iComboType == HLSLCOMBO_SKINNING
                #ifndef SHADER_EDITOR_DLL_2006
                || combo->iComboType == HLSLCOMBO_NUM_LIGHTS ||
                combo->iComboType == HLSLCOMBO_LIGHT_DYNAMIC
#endif
                    )
                //combo->iComboType == HLSLCOMBO_MORPHING )
                defaultValue = 1;

            char comb[MAXTARGC];
            Q_snprintf(comb, MAXTARGC, "#define %s %i\n", combo->name, defaultValue);
            Context.buf_combos.PutString(comb);
        } else {
            Assert(combo->min <= combo->max);
            char comb[MAXTARGC];
            Q_snprintf(comb, MAXTARGC, "// %s: \"%s\"\t\t\"%i..%i\"\n",
                       (combo->bStatic ? "STATIC" : "DYNAMIC"),
                       combo->name,
                       combo->min,
                       combo->max);
            Context.buf_combos.PutString(comb);
        }
    }

    // SAMPLERS
    for (int t = 0; t < Context.m_pActive_Identifiers->hList_Textures.Count(); t++) {
        SimpleTexture *tex = Context.m_pActive_Identifiers->hList_Textures[t];
        char samp[MAXTARGC];
        switch (tex->iTextureMode) {
            default: {
                if (bPS)
                    Q_snprintf(samp, MAXTARGC, "sampler _Sampler_%02i\t\t: register( s%i );\n", tex->iSamplerIndex,
                               tex->iSamplerIndex);
                else
                    //Q_snprintf( samp, MAXTARGC, "sampler _Sampler_%02i\t\t: register( D3DVERTEXTEXTURESAMPLER%i );\n", tex->iSamplerIndex, tex->iSamplerIndex );
                    Q_snprintf(samp, MAXTARGC,
                               "sampler2D _Sampler_%02i\t\t: register( D3DVERTEXTEXTURESAMPLER%i, s%i );\n",
                               tex->iSamplerIndex, tex->iSamplerIndex, tex->iSamplerIndex);
            }
                break;
            case HLSLTEX_FLASHLIGHT_COOKIE:
                Q_snprintf(samp, MAXTARGC, "sampler _gSampler_Flashlight_Cookie\t\t: register( s%i );\n",
                           tex->iSamplerIndex);
                break;
            case HLSLTEX_FLASHLIGHT_DEPTH:
                Q_snprintf(samp, MAXTARGC, "sampler _gSampler_Flashlight_Depth\t\t: register( s%i );\n",
                           tex->iSamplerIndex);
                break;
            case HLSLTEX_FLASHLIGHT_RANDOM:
                Q_snprintf(samp, MAXTARGC, "sampler _gSampler_Flashlight_Random\t\t: register( s%i );\n",
                           tex->iSamplerIndex);
                break;
            case HLSLTEX_MORPH:
                Q_snprintf(samp, MAXTARGC, "sampler2D morphSampler\t\t: register( D3DVERTEXTEXTURESAMPLER%i, s%i );\n",
                           tex->iSamplerIndex, tex->iSamplerIndex);
                break;
        }
        Context.buf_samplers.PutString(samp);
    }

    // ENVIRONMENT CONSTANTS
    for (int i = 0; i < Context.m_pActive_Identifiers->hList_EConstants.Count(); i++) {
        SimpleEnvConstant *pConst = Context.m_pActive_Identifiers->hList_EConstants[i];
        char szConst[MAXTARGC];
        const int id = pConst->iEnvC_ID;

        if (HLSLENV_IS_MANUAL_CONST(id)) {
            switch (id) {
                default:
                    Assert(0);
                    break;
                case HLSLENV_STUDIO_LIGHTING_VS:
                    continue;
                case HLSLENV_STUDIO_LIGHTING_PS:
                    Assert(bPS);
                    Q_snprintf(szConst, MAXTARGC, "const float3 g_cAmbientCube[6]\t\t: register( c%i );\n",
                               SSEREG_AMBIENT_CUBE);
                    Context.buf_constants.PutString(szConst);
                    Q_snprintf(szConst, MAXTARGC, "PixelShaderLightInfo g_cLightInfo[3]\t\t: register( c%i );\n",
                               SSEREG_LIGHT_INFO_ARRAY);
                    break;
                case HLSLENV_STUDIO_MORPHING:
                    Assert(!bPS);
                    Context.buf_constants.PutString("#ifdef SHADER_MODEL_VS_3_0\n");
                    Context.buf_constants.PutString(
                            "const float3 g_cMorphTargetTextureDim\t\t: register( SHADER_SPECIFIC_CONST_10 );\n");
                    Context.buf_constants.PutString(
                            "const float4 g_cMorphSubrect\t\t\t\t: register( SHADER_SPECIFIC_CONST_11 );\n");
                    //Context.buf_constants.PutString( "sampler2D morphSampler : register( D3DVERTEXTEXTURESAMPLER0, s0 );\n" );
                    Context.buf_constants.PutString("#endif\n");
                    bHACK_AddVertexID = true;
                    continue;
                case HLSLENV_FLASHLIGHT_VPMATRIX:
                    Assert(bPS || pConst->iHLSLRegister >= 0);
                    Q_snprintf(szConst, MAXTARGC, "const float4x4 g_cFlashlightWorldToTexture\t\t: register( c%i );\n",
                               (bPS ? SSEREG_FLASHLIGHT_TO_WORLD_TEXTURE : RemapEnvironmentConstant(bPS,
                                                                                                    pConst->iHLSLRegister)));
                    break;
                case HLSLENV_FLASHLIGHT_DATA:
                    Q_snprintf(szConst, MAXTARGC,
                               "const float4 g_cFlashlightAttenuationFactors\t\t: register( c%i );\n",
                               SSEREG_LIGHT_INFO_ARRAY + 2);
                    Context.buf_constants.PutString(szConst);
                    Q_snprintf(szConst, MAXTARGC, "const float4 g_cFlashlightPos\t\t: register( c%i );\n",
                               SSEREG_LIGHT_INFO_ARRAY + 3);
                    Context.buf_constants.PutString(szConst);
                    Q_snprintf(szConst, MAXTARGC, "const float4 g_cShadowTweaks\t\t: register( c%i );\n",
                               SSEREG_LIGHT_INFO_ARRAY + 1);
                    Context.buf_constants.PutString(szConst);
                    continue;
                case HLSLENV_CUSTOM_MATRIX: {
                    Assert(pConst->iHLSLRegister >= 0);
                    const customMatrix_t *pM = GetCMatrixInfo(pConst->iSmartNumComps);
                    Q_snprintf(szConst, MAXTARGC, "const %s %s\t\t: register( c%i );\n",
                               pM->szdataTypeName, pM->szIdentifierName,
                               (RemapEnvironmentConstant(bPS, pConst->iHLSLRegister)));
                }
                    break;
                case HLSLENV_SMART_CALLBACK:
                case HLSLENV_SMART_VMT_MUTABLE:
                case HLSLENV_SMART_VMT_STATIC:
                case HLSLENV_SMART_RANDOM_FLOAT: {
                    const char *__info = "Callback";
                    if (id == HLSLENV_SMART_VMT_MUTABLE)
                        __info = "Mutable";
                    else if (id == HLSLENV_SMART_VMT_STATIC)
                        __info = "Static";
                    else if (id == HLSLENV_SMART_RANDOM_FLOAT)
                        __info = "Random gen";

                    Assert(pConst->szSmartHelper);
                    Q_snprintf(szConst, MAXTARGC, "const %s g_cData_%s\t\t: register( c%i ); \t\t// %s\n",
                               ::GetVarTypeNameCode(pConst->iSmartNumComps), pConst->szSmartHelper,
                               RemapEnvironmentConstant(bPS, pConst->iHLSLRegister), __info);
                }
                    break;
                case HLSLENV_LIGHTMAP_RGB:
                    Q_snprintf(szConst, MAXTARGC, "const float %s\t\t: register( c%i );\n",
                               GetLightscaleCodeString(LSCALE_LIGHTMAP_RGB),
                               RemapEnvironmentConstant(bPS, pConst->iHLSLRegister));
                    break;
            }
        } else {
            econstdata *cData = EConstant_GetData(id);
            int _register = RemapEnvironmentConstant(bPS, pConst->iHLSLRegister);
            Q_snprintf(szConst, MAXTARGC, "const %s %s\t\t: register( c%i );\n",
                       ::GetVarCodeNameFromFlag(cData->varflag),
                       cData->varname,
                       _register);
        }
        Context.buf_constants.PutString(szConst);
    }

    // ARRAYS
    for (int i = 0; i < Context.m_pActive_Identifiers->hList_Arrays.Count(); i++) {
        char tmp[MAXTARGC];
        char szAppend[MAXTARGC];
        SimpleArray *pArray = Context.m_pActive_Identifiers->hList_Arrays[i];
        const bool bIs2D = pArray->iSize_Y > 1;
        const char *szDataType = ::GetVarTypeNameCode(pArray->iNumComps);
        Q_snprintf(tmp, sizeof(tmp), "static const %s g_cArray_%i", szDataType, pArray->iIndex);
        if (bIs2D)
            Q_snprintf(szAppend, sizeof(szAppend), "[%i][%i] =\n", pArray->iSize_X, pArray->iSize_Y);
        else
            Q_snprintf(szAppend, sizeof(szAppend), "[%i] =\n", pArray->iSize_X);
        Q_strcat(tmp, szAppend, sizeof(tmp));
        Context.buf_arrays.PutString(tmp);
        Context.buf_arrays.PutString("{\n");

        Context.buf_arrays.PushTab();
        for (int x = 0; x < pArray->iSize_X; x++) {
            for (int y = 0; y < pArray->iSize_Y; y++) {
                int element = pArray->iSize_Y * x + y;
                char szCurElement[MAX_PATH];
                const Vector4D &vec = pArray->vecData[element];
                Q_snprintf(szCurElement, sizeof(szCurElement), "%s( %ff", szDataType, vec[0]);
                for (int c = 1; c <= pArray->iNumComps && c < 4; c++) {
                    Q_snprintf(szAppend, sizeof(szAppend), ", %ff", vec[c]);
                    Q_strcat(szCurElement, szAppend, sizeof(szCurElement));
                }
                Q_strcat(szCurElement, " ),", sizeof(szCurElement));
                Context.buf_arrays.PutString(szCurElement);
            }
            Context.buf_arrays.PutString("\n");
        }
        Context.buf_arrays.PopTab();

        Context.buf_arrays.PutString("};\n");
    }

    // CODE
    for (int i = 0; i < m_hSolvers.Count(); i++) {
        CHLSL_SolverBase *solver = m_hSolvers[i];
        solver->Invoke_WriteFXC(bPS, Context);
    }

    // PRINT START
#ifdef SHADER_EDITOR_DLL_SWARM
    buf.PutString( "#define SHADER_EDITOR_SWARM_COMPILE\n\n" );
#elif SHADER_EDITOR_DLL_SER
    buf.PutString("#define SHADER_EDITOR_2013_COMPILE\n\n");
#endif

    if (Context.buf_combos.TellPut()) {
        buf.PutString("// Combos\n");
        BufferAppend(buf, Context.buf_combos);
    }

    buf.PutString("\n");
    WriteIncludes(bPS, buf, m_hSolvers);

    if (Context.buf_samplers.TellPut()) {
        buf.PutString("\n// Samplers\n");
        BufferAppend(buf, Context.buf_samplers);
    }

    if (Context.buf_constants.TellPut()) {
        buf.PutString("\n// Constants\n");
        BufferAppend(buf, Context.buf_constants);
    }

    if (Context.buf_arrays.TellPut()) {
        buf.PutString("\n// Arrays\n");
        BufferAppend(buf, Context.buf_arrays);
    }

    if (Context.buf_functions_globals.TellPut()) {
        buf.PutString("\n// User code - globals\n");
        if (codeBuff != NULL)
            BufferAppend(*codeBuff, Context.buf_functions_globals, false);

        BufferAppend(buf, Context.buf_functions_globals);
    }

    if (Context.buf_functions_bodies.TellPut()) {
        buf.PutString("\n// User code - function bodies\n");
        BufferAppend(buf, Context.buf_functions_bodies);
    }


    buf.PutString("\n// Semantic structures\n");
    if (bPS)
        buf.PutString("struct PS_INPUT\n{");
    else
        buf.PutString("struct VS_INPUT\n{");
    buf.PushTab();
    BufferAppend(buf, Context.buf_semantics_In);
    if (bHACK_AddVertexID) {
        buf.PutString("\n#ifdef SHADER_MODEL_VS_3_0");
        buf.PutString("\nfloat vVertexID\t\t\t: POSITION2;");
        buf.PutString("\n#endif");
    }
    buf.PopTab();
    buf.PutString("\n};\n\n");

    if (bPS)
        buf.PutString("struct PS_OUTPUT\n{");
    else
        buf.PutString("struct VS_OUTPUT\n{");
    buf.PushTab();
    BufferAppend(buf, Context.buf_semantics_Out);
    buf.PopTab();
    buf.PutString("\n};\n\n");

    buf.PutString("// Entry point\n");
    if (bPS) {
        buf.PutString("PS_OUTPUT main( const PS_INPUT In )\n");
        buf.PutString("{\n\tPS_OUTPUT Out;\n");
    } else {
        buf.PutString("VS_OUTPUT main( const VS_INPUT In )\n");
        buf.PutString("{\n\tVS_OUTPUT Out;\n");
    }
    buf.PushTab();
    BufferAppend(buf, Context.buf_code);
    if (bPosOverride)
        buf.PutString("Out.vProjPos = mul( float4( In.vPos.xyz, 1 ), cViewProj );\n");
    buf.PutString("return Out;");
    buf.PopTab();
    buf.PutString("\n}");
}

void CCompileThread::WriteFileLists(int activefiles, CUtlBufferEditor *codeBuff) {
    CUtlBufferEditor buf_1;
    buf_1.SetBufferType(true, true);

    if (activefiles & ACTIVEFLAG_VS_POS) {
        BuildFilelistEntry(buf_1, _shaderData, false, true);
        BuildFilelistEntry(buf_1, _shaderData, false, true);
        buf_1.PutString("\n");
    }
    if (activefiles & ACTIVEFLAG_VS) {
        BuildFilelistEntry(buf_1, _shaderData, false, false);
        BuildFilelistEntry(buf_1, _shaderData, false, false);
        buf_1.PutString("\n");
    }
    if (activefiles & ACTIVEFLAG_PS) {
        BuildFilelistEntry(buf_1, _shaderData, true, false);
        BuildFilelistEntry(buf_1, _shaderData, true, false);
    }
    char tmp[MAXTARGC];

    Q_snprintf(tmp, MAXTARGC, "%s/filelist.txt", GetShaderSourceDirectory());
    CUtlBuffer bufTmp;
    buf_1.WriteToBuffer(bufTmp);
    g_pFullFileSystem->WriteFile(tmp, "MOD", bufTmp);
    buf_1.Purge();

    buf_1.PutString("bin\\dx_proxy.dll\n");
    //buf_1.PutString( "bin\\mysql_wrapper.dll\n" );
    //buf_1.PutString( "bin\\fxc.exe\n" );
    //buf_1.PutString( "bin\\d3dx9_33.dll\n" );
    Q_snprintf(tmp, MAXTARGC, "%s\\tier0.dll\n", ::GetBinaryPath_Local());
    buf_1.PutString(tmp);
    Q_snprintf(tmp, MAXTARGC, "%s\\vstdlib.dll\n", ::GetBinaryPath_Local());
    buf_1.PutString(tmp);
    //buf_1.PutString( "\\bin\\tier0.dll\n" );
    //buf_1.PutString( "\\bin\\vstdlib.dll\n" );
    buf_1.PutString("common_fxc.h\n");
    buf_1.PutString("common_hlsl_cpp_consts.h\n");
    buf_1.PutString("common_pragmas.h\n");
    buf_1.PutString("common_ps_fxc.h\n");
    buf_1.PutString("common_vs_fxc.h\n");
    buf_1.PutString("common_vertexlitgeneric_dx9.h\n");
    buf_1.PutString("common_lightmappedgeneric_fxc.h\n");
    buf_1.PutString("common_flashlight_fxc.h\n");
    buf_1.PutString("common_parallax.h\n");
    Q_snprintf(tmp, MAXTARGC, "%s\\shadercompile.exe\n", ::GetBinaryPath_Local());
    buf_1.PutString(tmp);
#ifdef SHADER_EDITOR_DLL_2006
    Q_snprintf( tmp, MAXTARGC, "%s\\shadercompile.dll\n", ::GetBinaryPath_Local() );
#else
    Q_snprintf(tmp, MAXTARGC, "%s\\shadercompile_dll.dll\n", ::GetBinaryPath_Local());
#endif
    buf_1.PutString(tmp);
    //buf_1.PutString( "bin\\shadercompile.exe\n" );
    //buf_1.PutString( "bin\\shadercompile_dll.dll\n" );

    if (codeBuff != NULL) {
        int size = codeBuff->TellPut();
        if (size > 0) {
            char *__buffer = new char[size + 1];
            codeBuff->SeekGet(CUtlBufferEditor::SEEK_HEAD, 0);
            codeBuff->Get(__buffer, size);
            __buffer[size] = '\0';

            char *read = __buffer;
            while (read && *read) {
                while (*read && (*read == '\t' || *read == ' ' || *read == '\n'))
                    read++;

                if (*read == '#') {
                    read++;

                    while (*read && (*read == '\t' || *read == ' '))
                        read++;

                    if (Q_stristr(read, "include") == read) {
                        read += 7;
                        while (*read && (*read == ' ' || *read == '\t' || *read == '"'))
                            read++;

                        if (*read >= 'a' && *read <= 'z' || *read >= 'A' && *read <= 'Z' || *read == '_') {
                            char *findWordEnd = read;
                            while (*findWordEnd >= 'a' && *findWordEnd <= 'z' ||
                                   *findWordEnd >= 'A' && *findWordEnd <= 'Z' ||
                                   *findWordEnd >= '0' && *findWordEnd <= '9' ||
                                   *findWordEnd == '_' || *findWordEnd == '.')
                                findWordEnd++;

                            AssertMsg(findWordEnd > read, "include string with no length?");

                            buf_1.Put(read, findWordEnd - read);
                            buf_1.PutString("\n");
                        }
                    }
                }

                while (*read && *read != '\n')
                    read++;
            }

            delete[] __buffer;
        }
    }

    char File_VS_POSLOCK[MAX_PATH];
    char File_VS[MAX_PATH];
    char File_PS[MAX_PATH];
    ComposeShaderName(_shaderData, false, true, File_VS_POSLOCK, sizeof(File_VS_POSLOCK), true);
    ComposeShaderName(_shaderData, false, true, File_VS, sizeof(File_VS));
    ComposeShaderName(_shaderData, true, true, File_PS, sizeof(File_PS));
    if (activefiles & ACTIVEFLAG_VS_POS) {
        Q_snprintf(tmp, MAXTARGC, "%s\n", File_VS_POSLOCK);
        buf_1.PutString(tmp);
    }
    if (activefiles & ACTIVEFLAG_VS) {
        Q_snprintf(tmp, MAXTARGC, "%s\n", File_VS);
        buf_1.PutString(tmp);
    }
    if (activefiles & ACTIVEFLAG_PS) {
        Q_snprintf(tmp, MAXTARGC, "%s\n", File_PS);
        buf_1.PutString(tmp);
    }

    Q_snprintf(tmp, MAXTARGC, "%s/uniquefilestocopy.txt", GetShaderSourceDirectory());
    CUtlBuffer bufTmp2;
    buf_1.WriteToBuffer(bufTmp2);
    g_pFullFileSystem->WriteFile(tmp, "MOD", bufTmp2);
}

HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

// creation flag: CREATE_NO_WINDOW

bool CCompileThread::StartCompiler() {
    ForceTerminateCompilers();

    char old_wd[MAX_PATH];
    getcwd(old_wd, MAX_PATH);
    chdir(GetBinaryPath());

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
        gShaderEditorSystem->QueueLog("Can't create compile pipe!\n");
        return false;
    }
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
        gShaderEditorSystem->QueueLog("Can't update compile pipe!\n");
        return false;
    }

    char szCmdline[1024];
    Q_snprintf(szCmdline, 1024,
               "\"%s\\shadercompile.exe\" -nompi -nop4 -game \"%s\" -shaderpath \"%s\" -allowdebug",
               GetBinaryPath(), GetGamePath(), GetShaderSourceDirectory());

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
    siStartInfo.wShowWindow = SW_HIDE;
    //Msg("commandline: %s\n", szCmdline);
    bSuccess = CreateProcess(NULL,
                             szCmdline,     // command line
                             NULL,          // process security attributes
                             NULL,          // primary thread security attributes
                             TRUE,          // handles are inherited
                             CREATE_NO_WINDOW,             // creation flags
                             NULL,          // use parent's environment
                             NULL,          // use parent's current directory
                             &siStartInfo,  // STARTUPINFO pointer
                             &piProcInfo);  // receives PROCESS_INFORMATION

    if (!bSuccess) {
        gShaderEditorSystem->QueueLog("Can't create compile process!\n");
        return false;
    }

    gShaderEditorSystem->SetCompilerState(true);

    SetPriorityClass(
            piProcInfo.hProcess, //GetCurrentProcess(),
            BELOW_NORMAL_PRIORITY_CLASS
    );

#if 1
    int _ExitCodeOut = -1;
    DWORD __exitcode = 0;
    if (!CloseHandle(g_hChildStd_OUT_Wr)) {
        gShaderEditorSystem->QueueLog("Can't close shadercompile pipe!\n");
        return false;
    }

    {
        for (;;) {
            if (!GetExitCodeProcess(piProcInfo.hProcess, &__exitcode))
                break;
            if (__exitcode != STILL_ACTIVE) {
                _ExitCodeOut = __exitcode;
                //char __msg[MAX_PATH];
                //Q_snprintf( __msg, MAX_PATH, "exitcode: %i\n", __exitcode );
                //gShaderEditorSystem->QueueLog( __msg );
                //break;
            }

#define BUFSIZE 4096
            DWORD dwRead;
            CHAR chBuf[BUFSIZE];
            BOOL bSuccess = FALSE;
#	if 1
            for (;;) {
                bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
                if (!bSuccess || dwRead <= 0)
                    break;
                chBuf[dwRead] = '\0';
                gShaderEditorSystem->QueueLog(chBuf, dwRead);
            }
#	endif
            if (_ExitCodeOut != -1)
                break;
        }
    }
#endif

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    char tmp[MAX_PATH];
    ComposeShaderName_Compiled(_shaderData, false, false, tmp, MAX_PATH);
    int len = Q_strlen(tmp) + 2;
    _shaderData->shader->ProcVSName = new char[len];
    Q_snprintf(_shaderData->shader->ProcVSName, len, "%s", tmp);

    ComposeShaderName_Compiled(_shaderData, true, false, tmp, MAX_PATH);
    len = Q_strlen(tmp) + 2;
    _shaderData->shader->ProcPSName = new char[len];
    Q_snprintf(_shaderData->shader->ProcPSName, len, "%s", tmp);

    __threadcmds_CompileCallback *msg = new __threadcmds_CompileCallback(*_target);
    msg->_ExitCode = _ExitCodeOut;
    msg->_data = _shaderData;
    msg->_activeFlags = curFlags;
    _shaderData = NULL;
    gThreadCtrl->m_QueueCompileCallback.QueueMessage(msg);

    //chdir( old_wd );
    return true;
}
