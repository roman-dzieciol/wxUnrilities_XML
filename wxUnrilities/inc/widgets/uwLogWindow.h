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
//  uwLogWindow.h
// ============================================================================
#ifndef UWLOGWINDOW_H
#define UWLOGWINDOW_H

class uwLogWindow;


// ============================================================================
// uwLogFrame
// ============================================================================
class uwLogFrame : public wxFrame
{
public:
    uwLogFrame( wxWindow* parent, uwLogWindow* log, const wxChar* caption );
    virtual ~uwLogFrame();

public:
    wxTextCtrl *TextCtrl() const { return Text; }

protected:
    virtual void DoClose();
    virtual void SaveLog();
    void OnCloseWindow(wxCloseEvent& event);

protected:
    wxTextCtrl*		Text;
    uwLogWindow*	Log;

private:
    DECLARE_EVENT_TABLE()
};


// ============================================================================
// uwLogWindow
// ============================================================================
class uwLogWindow : public wxLogPassThrough
{
public:
    uwLogWindow( wxWindow* parent,	const wxChar* caption, bool bpassmsg = false );
    virtual ~uwLogWindow();

public:
    virtual void Show( bool bshow = true );
    virtual wxFrame* GetFrame() const;

protected:
    virtual void DoLog( wxLogLevel level, const wxChar *str, time_t t );
    virtual void DoLogString( const wxChar *str, time_t t );

protected:
    uwLogFrame* LogFrame; 
};



#endif UWLOGWINDOW_H
// ============================================================================