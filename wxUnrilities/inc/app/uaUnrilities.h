// ============================================================================
//  Unrilities :: Cross-platform toolkit for Unreal Engine packages
// ============================================================================
//  Copyright (C) 2005  Roman Dzieciol <neai at o2.pl>
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// ============================================================================
//	uaUnrilities.h
// ============================================================================
#ifndef UAUNRILITIES_H
#define UAUNRILITIES_H

class wxSplashScreen;
class pkgPackage;
class unStatusBar;
class uaMainTabs;
class uwLogWindow;
class uaConfig;


// ============================================================================
// uaUnrilitiesApp
// ============================================================================
class uaUnrilitiesApp : public wxApp
{
public:
	virtual bool OnInit();
	wxRect CalcFrame();
    wxSplashScreen* ShowSplash( dword timeout );
};

DECLARE_APP(uaUnrilitiesApp);


// ============================================================================
// uaUnrilitiesFrame
// ============================================================================
class uaUnrilitiesFrame : public wxFrame
{
public:
	uaUnrilitiesFrame
	(	wxWindow*		parent
	,	wxWindowID		id
	,	const wxString&	title
	,	const wxPoint&	pos = wxDefaultPosition
	,	const wxSize&	size = wxDefaultSize
	,	long			style = wxDEFAULT_FRAME_STYLE
	,	const wxString&	name = wxT("uaUnrilitiesFrame")
	);

public:
    void ShowFrameAndLog( bool bshow );

protected:
	void OnOpen				( wxCommandEvent& event );
	void OnExit				( wxCommandEvent& event );
	void OnAbout			( wxCommandEvent& event );
	void OnViewLog			( wxCommandEvent& event );

	void DoFileOpen			( wxFileName filename );
	void DoFileClose		();

	void OnClose			( wxCloseEvent& event );

protected:
	wxMenuBar*			Menu;			// menus
	wxMenu*				MenuFile;		//
	wxMenu*				MenuView;		//
	wxMenu*				MenuHelp;		//

	wxStatusBar*		StatusBar;		//
	wxToolBar*			ToolBar;		//
	uaMainTabs*			MainTabs;		//
	uwLogWindow*		LogWindow;		//

    pkgPackage*         Package;        //

    auto_ptr<uaConfig>  Config;         //
	
public:
	static const wxString	Title;		//
	static const wxString	Build;		//

private:
	DECLARE_EVENT_TABLE()
};





#endif UAUNRILITIES_H
// ============================================================================