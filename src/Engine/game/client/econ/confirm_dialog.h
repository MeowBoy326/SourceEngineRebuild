﻿//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//

#ifndef CONFIRM_DIALOG_H
#define CONFIRM_DIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/CheckButton.h"
#include "inputsystem/iinputsystem.h"

//-----------------------------------------------------------------------------
// Purpose:
//   - Basic confirm dialog - derive from this and implement GetText().
//   - The user will have two options, essentially yes or no.
//   - A "ConfirmDlgResult" message is sent to the parent with the result.
//     Check the "confirmed" parameter.
//   - Panel deletes itself.
//   - See CConfirmDeleteDialog for a generic delete confirmation dialog.
//-----------------------------------------------------------------------------
class CExButton;
#ifdef PONDER_CLIENT_DLL
class CTFSpectatorGUIHealth;
#endif // PONDER_CLIENT_DLL

class CConfirmDialog : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CConfirmDialog, vgui::EditablePanel );
public:
	CConfirmDialog( vgui::Panel *parent );

	virtual const wchar_t *GetText() = 0;

	void Show( bool bMakePopup = true );
	void SetIconImage( const char *pszIcon );

protected:
	virtual void		OnSizeChanged(int nNewWide, int nNewTall );
	virtual void		ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void		OnCommand( const char *command );
	virtual void		OnKeyCodeTyped( vgui::KeyCode code );
	virtual void		OnKeyCodePressed( vgui::KeyCode code );
	virtual const char *GetResFile();

	void				FinishUp();		// Hide the panel, mark for deletion, remove from modal stack.

	CExButton		*m_pConfirmButton;
	CExButton		*m_pCancelButton;
	vgui::ImagePanel *m_pIcon;
};

//-----------------------------------------------------------------------------

typedef void (*GenericConfirmDialogCallback)( bool bConfirmed, void *pContext );

// An implementation of the Confirm Dialog that is "generic"
class CTFGenericConfirmDialog : public CConfirmDialog
{
	DECLARE_CLASS_SIMPLE( CTFGenericConfirmDialog, CConfirmDialog );
public:
	CTFGenericConfirmDialog( const char *pTitle, const char *pTextKey, const char *pConfirmBtnText,
		const char *pCancelBtnText, GenericConfirmDialogCallback callback, vgui::Panel *pParent );
	CTFGenericConfirmDialog( const char *pTitle, const wchar_t *pText, const char *pConfirmBtnText,
		const char *pCancelBtnText, GenericConfirmDialogCallback callback, vgui::Panel *pParent );
	virtual ~CTFGenericConfirmDialog();

	virtual const wchar_t *GetText();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void PerformLayout();
	virtual void OnCommand( const char *command );

	void SetStringTokens( KeyValues *pKeyValues );
	void AddStringToken( const char* pToken, const wchar_t* pValue );
	void SetContext( void *pContext );

protected:
	void CommonInit( const char *pTitle, const char *pConfirmBtnText, const char *pCancelBtnText,
		GenericConfirmDialogCallback callback, vgui::Panel *pParent );
		
	const char *m_pTitle;
	const char *m_pTextKey;	
	const char *m_pConfirmBtnText;
	const char *m_pCancelBtnText;

	KeyValues *m_pKeyValues;
	wchar_t m_wszBuffer[1024];
	GenericConfirmDialogCallback m_pCallback;
	void *m_pContext;
};

// A generic message dialog, which is just a generic confirm dialog w/o the cancel button
class CTFMessageBoxDialog : public CTFGenericConfirmDialog
{
	DECLARE_CLASS_SIMPLE( CTFMessageBoxDialog, CTFGenericConfirmDialog );
public:
	CTFMessageBoxDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent ) 
		:  CTFGenericConfirmDialog( pTitle, pText, pConfirmBtnText, NULL, callback, parent ) {}

	CTFMessageBoxDialog( const char *pTitle, const wchar_t *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent )
		:  CTFGenericConfirmDialog( pTitle, pText, pConfirmBtnText, NULL, callback, parent ) {}

	virtual const char* GetResFile();
};

// A generic message dialog, which is just a generic confirm dialog w/o the cancel button that plays a sound with optional delay
class CTFMessageBoxDialogWithSound : public CTFMessageBoxDialog
{
	DECLARE_CLASS_SIMPLE( CTFMessageBoxDialogWithSound, CTFMessageBoxDialog );
public:
	CTFMessageBoxDialogWithSound( const char *pTitle, const char *pText, const char *pszSound, float flDelay, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent );
	CTFMessageBoxDialogWithSound( const char *pTitle, const wchar_t *pText, const char *pszSound, float flDelay, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent );
	virtual void OnTick() OVERRIDE;

private:
	char	m_szSound[MAX_PATH];
	float	m_flSoundTime;
	bool	m_bPlayedSound;
};

// A dialog with an upgrade button that takes them to the mann co store
class CTFUpgradeBoxDialog : public CTFMessageBoxDialog
{
	DECLARE_CLASS_SIMPLE( CTFUpgradeBoxDialog, CTFMessageBoxDialog );
public:
	CTFUpgradeBoxDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent ) 
		:  CTFMessageBoxDialog( pTitle, pText, pConfirmBtnText, callback, parent ) {}

	CTFUpgradeBoxDialog( const char *pTitle, const wchar_t *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent )
		:  CTFMessageBoxDialog( pTitle, pText, pConfirmBtnText, callback, parent ) {}

	virtual const char *GetResFile()
	{
		return "Resource/UI/UpgradeBoxDialog.res";
	}
	virtual void OnCommand( const char *command );
};


// An implementation of the Confirm Dialog with a persistant "opt out" checkbox stored via ConVar
class CTFGenericConfirmOptOutDialog : public CTFGenericConfirmDialog
{
	DECLARE_CLASS_SIMPLE( CTFGenericConfirmOptOutDialog, CTFGenericConfirmDialog );
public:
	CTFGenericConfirmOptOutDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, const char *pCancelBtnText, const char *pOptOutText, const char *pOptOutConVarName, GenericConfirmDialogCallback callback, vgui::Panel *parent ) ;
	virtual ~CTFGenericConfirmOptOutDialog() { }

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	MESSAGE_FUNC_PARAMS( OnButtonChecked, "CheckButtonChecked", pData );

protected:
	virtual const char *GetResFile();

	const char *m_optOutText;

	vgui::CheckButton *m_optOutCheckbox;
	const char *m_optOutConVarName;
};

#ifdef PONDER_CLIENT_DLL
// A dialog presented to dead players when being revived
class CTFReviveDialog : public CTFMessageBoxDialog
{
	DECLARE_CLASS_SIMPLE( CTFReviveDialog, CTFMessageBoxDialog );
public:
	CTFReviveDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent );
	virtual ~CTFReviveDialog() { }

	virtual void PerformLayout() OVERRIDE;
	virtual void OnTick() OVERRIDE;
	virtual const char *GetResFile() OVERRIDE { return "Resource/UI/ReviveDialog.res"; }
	void SetOwner( CBaseEntity *pEntity );

	CTFSpectatorGUIHealth *m_pTargetHealth;
	CHandle< C_BaseEntity >	m_hEntity;
	float m_flPrevHealth;
};

CTFReviveDialog *ShowRevivePrompt( CBaseEntity *pOwner,
								   const char *pTitle = "#TF_Prompt_Revive_Title", 
								   const char *pText = "#TF_Prompt_Revive_Message",
								   const char *pConfirmBtnText = "#TF_Prompt_Revive_Cancel",
								   GenericConfirmDialogCallback callback = NULL,
								   vgui::Panel *parent = NULL,
								   void *pContext = NULL );


// A generic message dialog, which is just a generic confirm dialog w/o the cancel button
class CEconRequirementDialog : public CTFGenericConfirmDialog
{
	DECLARE_CLASS_SIMPLE( CEconRequirementDialog, CTFGenericConfirmDialog );
public:
	CEconRequirementDialog( const char *pTitle, const char *pTextKey, const char *pItemDefName );

	virtual const char *GetResFile() OVERRIDE;
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme ) OVERRIDE;
	virtual void OnCommand( const char *command ) OVERRIDE;

	CSchemaItemDefHandle m_hItemDef;
};

void ShowEconRequirementDialog( const char *pTitle, const char *pText, const char *pItemDefName );
#endif // PONDER_CLIENT_DLL

//-----------------------------------------------------------------------------

CTFGenericConfirmOptOutDialog *ShowConfirmOptOutDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, const char *pCancelBtnText, const char *pOptOutText, const char *pOptOutConVarName, GenericConfirmDialogCallback callback, vgui::Panel *parent = NULL );

//-----------------------------------------------------------------------------

CTFGenericConfirmDialog *ShowConfirmDialog( const char *pTitle, const char *pText, const char *pConfirmBtnText, const char *pCancelBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent = NULL, void *pContext = NULL, const char *pSound = NULL );

//-----------------------------------------------------------------------------

CTFMessageBoxDialog *ShowMessageBox( const char *pTitle, const char *pText, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );
CTFMessageBoxDialog *ShowMessageBox( const char *pTitle, const wchar_t *pText, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );
CTFMessageBoxDialog *ShowMessageBox( const char *pTitle, const char *pText, KeyValues *pKeyValues, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );
CTFMessageBoxDialog *ShowUpgradeMessageBox( const char *pTitle, const char *pText );
CTFMessageBoxDialog *ShowUpgradeMessageBox( const char *pTitle, const char *pText, const char *pConfirmBtnText, GenericConfirmDialogCallback callback, vgui::Panel *parent = NULL, void *pContext = NULL );

//-----------------------------------------------------------------------------
CTFMessageBoxDialogWithSound *ShowMessageBoxWithSound( const char *pTitle, const char *pText, const char *pszSound, float flDelay = 0.0, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );
CTFMessageBoxDialogWithSound *ShowMessageBoxWithSound( const char *pTitle, const wchar_t *pText, const char *pszSound, float flDelay = 0.0, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );
CTFMessageBoxDialogWithSound *ShowMessageBoxWithSound( const char *pTitle, const char *pText, KeyValues *pKeyValues, const char *pszSound, float flDelay = 0.0, const char *pConfirmBtnText = "#GameUI_OK", GenericConfirmDialogCallback callback = NULL, vgui::Panel *parent = NULL, void *pContext = NULL );

#endif // CONFIRM_DIALOG_H
