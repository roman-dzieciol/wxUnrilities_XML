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
//	uaUnrilities.cpp
// ============================================================================
#include "prec.h"
#include <wx/splash.h>
#include <wx/image.h>
#include "uaUnrilities.h"

#include "pkgPackage.h"
#include "uwLogWindow.h"
#include "uaMainTabs.h"
#include "uaConfig.h"


// ----------------------------------------------------------------------------
// images
// ----------------------------------------------------------------------------
#include "open.xpm"
#include "log.xpm"
#include "help.xpm"


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	Menu_FileOpen = wxID_OPEN,
	Menu_FileExit = wxID_EXIT,
	Menu_HelpAbout = wxID_ABOUT,

	Toolbar_Open = wxID_OPEN,
	Toolbar_About = wxID_ABOUT,

	Menu_ViewLog = wxID_HIGHEST + 1,

	Toolbar_Log = Menu_ViewLog,

	ID_TOOLBAR,
	ID_NAVTREE,
	ID_GENLIST,
	ID_NOTEBOOK
};


const wxString	uaUnrilitiesFrame::Title = wxT("Unrilities");
const wxString	uaUnrilitiesFrame::Build = wxT("v0.3");


// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE	( uaUnrilitiesFrame, wxFrame )

	EVT_MENU		( wxID_OPEN,	uaUnrilitiesFrame::OnOpen )
	EVT_MENU		( wxID_EXIT,	uaUnrilitiesFrame::OnExit )
	EVT_MENU		( wxID_ABOUT,	uaUnrilitiesFrame::OnAbout )

	EVT_MENU		( Menu_ViewLog,	uaUnrilitiesFrame::OnViewLog )

	EVT_CLOSE		( uaUnrilitiesFrame::OnClose )

END_EVENT_TABLE()



// ============================================================================
// uaUnrilitiesApp
// ============================================================================
bool uaUnrilitiesApp::OnInit()
{
    // load image handlers
    //wxImage::AddHandler( new wxPNGHandler() );

    // show splash during init
    //auto_ptr<wxSplashScreen> splash( ShowSplash(0) );

	// calc window position
	wxRect win = CalcFrame();

	// create frame
	uaUnrilitiesFrame* frame = new uaUnrilitiesFrame( NULL, wxID_ANY, uaUnrilitiesFrame::Title, win.GetPosition(), win.GetSize() );
	frame->SetIcon(wxICON(ICO_App));
	frame->ShowFrameAndLog(true);
	frame->SetFocus();

    // show splash after init
    //ShowSplash(1000);

	return true;
}

// ----------------------------------------------------------------------------
// CalcFrame
// ----------------------------------------------------------------------------
wxRect uaUnrilitiesApp::CalcFrame()
{
	wxSize screen( wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) );
	wxRect win;

	if( screen.IsFullySpecified() )
	{
		win.SetSize( wxSize(	static_cast<int>(screen.GetWidth()*0.95f), 
								static_cast<int>(screen.GetHeight()*0.66f) ));

		win.SetPosition( wxPoint(	static_cast<int>((screen.GetWidth()-win.GetWidth())*0.5f), 
									static_cast<int>((screen.GetHeight()-win.GetHeight())*0.5f) - 
									static_cast<int>(screen.GetHeight()*0.12f) ) );
	}
	else
	{
		win.SetSize( wxDefaultSize );
		win.SetPosition( wxDefaultPosition );
	}
	return win;
}

// ----------------------------------------------------------------------------
// ShowSplash
// ----------------------------------------------------------------------------
wxSplashScreen* uaUnrilitiesApp::ShowSplash( dword timeout )
{
    wxSplashScreen* splash = NULL;
    wxBitmap bitmap;
    if( bitmap.LoadFile("./Img/splash.png", wxBITMAP_TYPE_PNG) )
    {
        splash = new wxSplashScreen( bitmap, wxSPLASH_CENTRE_ON_SCREEN | (timeout==0?wxSPLASH_NO_TIMEOUT:wxSPLASH_TIMEOUT), timeout, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxFRAME_TOOL_WINDOW );
    }
    //wxYield();
    return splash;
}

IMPLEMENT_APP(uaUnrilitiesApp)


// ============================================================================
// uaUnrilitiesFrame
// ============================================================================
uaUnrilitiesFrame::uaUnrilitiesFrame
(	wxWindow*		parent
,	wxWindowID		id
,	const wxString&	title
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
)	
:	Package(NULL)
,	wxFrame( parent, id, title, pos, size, style, name )
{
	SW_DEF( sw );

	// Log
	LogWindow = new uwLogWindow( this, wxT("Unrilities Log") );
	LogWindow->GetFrame()->SetSize( GetPosition().x, GetPosition().y+GetSize().GetHeight(), GetSize().GetWidth(), static_cast<int>(GetSize().GetHeight()*0.3f) );
	LogWindow->GetFrame()->SetIcon( wxICON(ICO_Log) );
	LogWindow->GetFrame()->SetFont( GetFont() );

	// menu file
	MenuFile = new wxMenu;
	MenuFile->Append(Menu_FileOpen, wxT("&Open"), wxT("Open file"));
	MenuFile->AppendSeparator();
	MenuFile->Append(Menu_FileExit, wxT("E&xit"), wxT("Exit"));

	// menu view
	MenuView = new wxMenu;
	MenuView->Append(Menu_ViewLog, wxT("&Log"), wxT("Toggle Log Window"));

	// menu help
	MenuHelp = new wxMenu;
	MenuHelp->Append(Menu_HelpAbout, wxT("&About"), wxT("About"));

	// menu bar
	Menu = new wxMenuBar;
	Menu->Append(MenuFile, wxT("&File"));
	Menu->Append(MenuView, wxT("&View"));
	Menu->Append(MenuHelp, wxT("&Help"));
	SetMenuBar(Menu);

	// StatusBar
	StatusBar = CreateStatusBar();

	// ToolBar
	ToolBar = CreateToolBar( wxTB_NODIVIDER | wxTB_VERTICAL | wxTB_FLAT, wxID_ANY );
	ToolBar->AddTool(Toolbar_Open, wxBitmap(open_xpm), wxNullBitmap, false, -1, -1, NULL, wxT("Open file"));
	ToolBar->AddTool(Toolbar_Log, wxBitmap(log_xpm), wxNullBitmap, false, -1, -1, NULL, wxT("Toggle Log"));
	ToolBar->AddTool(Toolbar_About, wxBitmap(help_xpm), wxNullBitmap, false, -1, -1, NULL, wxT("About Unrilities"));
	ToolBar->Realize();

	// Notebook
	MainTabs = new uaMainTabs( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );

	SetOwnBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_MENU ) );

    SW_LOG( sw, "Init: %s" );

    // Load config
    guard;
    Config.reset( new uaConfig() );
    Config->OnReload();
    unguard;
}

// ----------------------------------------------------------------------------
// ShowFrameAndLog
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::ShowFrameAndLog( bool bshow )
{
    Show( bshow );
    LogWindow->Show( bshow );
}

// ----------------------------------------------------------------------------
// OnOpen
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::OnOpen( wxCommandEvent& WXUNUSED(event) )
{
	wxFileDialog fd( this, wxT("Open file"), wxEmptyString, wxEmptyString, 
        wxT("U files (*.u*)|*.u*|All files (*.*)|*.*"), wxOPEN );

	try
    {
        if( fd.ShowModal() == wxID_OK )
        { 
            wxFileName filename = fd.GetPath();
            filename.Normalize();
            DoFileOpen( filename );
        }
	}
	catch( exception& e )
	{
		wxLogError(wxT("%s -- %s"), fd.GetPath().c_str(), e.what()); 
		LogWindow->Show(true);
		DoFileClose();
	}
}

// ----------------------------------------------------------------------------
// OnExit
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::OnExit( wxCommandEvent& WXUNUSED(event) )
{
	ShowFrameAndLog(false);
	Close(true);
}

// ----------------------------------------------------------------------------
// OnViewLog
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::OnViewLog( wxCommandEvent& WXUNUSED(event) )
{
	if( LogWindow->GetFrame()->IsShown() )
			LogWindow->Show(false);
	else	LogWindow->Show(true);
}

// ----------------------------------------------------------------------------
// OnAbout
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxString about;
	about << Title << wxT(" ") << Build << wxT("\nCross-platform toolkit for Unreal Engine package files.\n\nCopyright © 2005 Roman Dzieciol - neai@o2.pl");
	wxMessageBox( about, wxT("About Unrilities"), wxOK | wxICON_INFORMATION, this );
}

// ----------------------------------------------------------------------------
// OnClose
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::OnClose( wxCloseEvent& event )      
{
    ShowFrameAndLog(false);

	// close old file if open
	if( Package != NULL )
		DoFileClose();

	delete wxLog::SetActiveTarget(NULL);
	event.Skip();
}


// ----------------------------------------------------------------------------
// DoFileOpen
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::DoFileOpen( wxFileName filename )
{
	SW_DEF( sw );

	// close old file if open
	if( Package != NULL )
		DoFileClose();

    // open new file
    alloc_create(Package);
    Package->OpenFile(filename);

	// open file
	MainTabs->OnOpenFile( Package );

	// Update frame
	//SetTitle( filename.GetFullName() );
	//SetStatusText( filename.GetFullPath() );

	SW_LOGV( sw, wxT("Open: %s %s"), filename.GetFullPath().c_str() );
}


// ----------------------------------------------------------------------------
// DoFileClose
// ----------------------------------------------------------------------------
void uaUnrilitiesFrame::DoFileClose()
{
	SW_DEF( sw );

	// Update Frame
	SetStatusText( wxT("") );
	SetTitle( uaUnrilitiesFrame::Title );

	// Clear Notebook
	MainTabs->OnCloseFile();

    // Close file
    alloc_destroy(Package);

	SW_LOG( sw, "Close: %s" ); wxLogMessage(wxT(""));
}

// ============================================================================