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
//  uwHexCtrl.h
// ============================================================================
#ifndef UWHEXCTRL_H
#define UWHEXCTRL_H


class uwHexCtrlColumn
{
public:
    uwHexCtrlColumn( const wxString& caption, dword align=0, dword width=0, wxRect boxrect=wxRect(), wxRect textrect=wxRect() )
    :   Caption(caption)
    ,   Align(align)
    ,   Width(width)
    ,   BoxRect(boxrect)
    ,   TextRect(textrect)
    {
    }

    wxString Caption;
    dword Align;
    dword Width;
    wxRect BoxRect;
    wxRect TextRect;
};


// ============================================================================
// uwHexCtrl
// ============================================================================
class uwHexCtrl : public wxScrolledWindow
{
public:
    // Special
    uwHexCtrl();
    uwHexCtrl
    (	wxWindow*			parent
    ,	wxWindowID			id = wxID_ANY
    ,	const wxPoint&		pos = wxDefaultPosition
    ,	const wxSize&		size = wxDefaultSize
    ,	long				style = wxScrolledWindowStyle | wxSUNKEN_BORDER
    ,	const wxString&		name = wxT("uwHexCtrl")
    );

    virtual ~uwHexCtrl();

    // Delayed ctor
    bool Create
    (	wxWindow*			parent
    ,	wxWindowID			id = wxID_ANY
    ,	const wxPoint&		pos = wxDefaultPosition
    ,	const wxSize&		size = wxDefaultSize
    ,	long				style = wxScrolledWindowStyle | wxSUNKEN_BORDER
    ,	const wxString&		name = wxT("uwHexCtrl")
    );

    // Mutators
    void SetData( wxFFile* file, wxFileOffset offset, dword size );
    void SetHexWidth( dword w );
    void SetFontSize( int delta );
    void SetHighlight( dword offset, dword size );
    void ClearHighlight();

    void AddColumn( const wxString& caption, dword align=wxALIGN_LEFT, long width=-1 );
    void PaintRectangle( wxDC& dc, wxColour& colour, const wxRect& rect );

    // Events
    void OnPaint(wxPaintEvent& event);
    void OnScroll( wxScrollWinEvent& event );
    void OnSize( wxSizeEvent& event );
    void OnErase( wxEraseEvent& event );

protected:
    dword HexWidth;
    dword HexLines;
    dword VirtualWidth;
    dword VirtualHeight;

    wxFont Font;
    int FontDelta;

    wxCoord ColumnMarginX;
    wxCoord ColumnMarginY;
    wxCoord ItemMarginY;

    wxString FmtHexByte;
    wxString FmtOffset;

    vector<wxString> HexStrings;
    vector<wxString> AsciiStrings;

    wxCoord HexCharHeight;
    wxCoord HexCharWidth;
    wxCoord HexByteWidth;
    wxCoord HexOffsetWidth;

    wxCoord ItemHeight;
    wxCoord ColumnHeight;

    wxFFile* File;

    dword DataOffset;
    dword DataSize;
    vector<byte> DataCache;


    vector<uwHexCtrlColumn> Columns;

    dword ColHexBegin;
    dword ColHexEnd;
    dword ColAsciiBegin;
    dword ColAsciiEnd;

    dword PaintCount;

    dword HighlightBegin;
    dword HighlightEnd;

private:
    DECLARE_EVENT_TABLE()
};


#endif UWHEXCTRL_H
// ============================================================================