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
//	uwHexCtrl.cpp
// ============================================================================
#include "prec.h"
#include "uwHexCtrl.h"
#include "asvNamedSize.h"
#include "asvNamedFormat.h"
#include "uwHexCtrl.h"
#include "wx/renderer.h"
#include <wx/dcbuffer.h>




// ============================================================================
// uwHexCtrl - Event Table
// ============================================================================
BEGIN_EVENT_TABLE   ( uwHexCtrl, wxScrolledWindow )
    EVT_SCROLLWIN           ( uwHexCtrl::OnScroll )
    EVT_SIZE                ( uwHexCtrl::OnSize )
    EVT_ERASE_BACKGROUND    ( uwHexCtrl::OnErase )
    EVT_PAINT               ( uwHexCtrl::OnPaint )
END_EVENT_TABLE()


// ============================================================================
// uwHexCtrl
// ============================================================================
uwHexCtrl::uwHexCtrl()
:   HexWidth(16)
,   FontDelta(0)
,   File(NULL)
,   PaintCount(0)
,   HighlightBegin(-1)
,   HighlightEnd(-1)
{
}

uwHexCtrl::uwHexCtrl
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxString&		name
) 
:   HexWidth(16)
,   FontDelta(0)
,   File(NULL)
,   PaintCount(0)
,   HighlightBegin(-1)
,   HighlightEnd(-1)
{
    Create(parent,id,pos,size,style,name);
}

uwHexCtrl::~uwHexCtrl()
{
}


// ============================================================================
// uwHexCtrl
// ============================================================================
bool uwHexCtrl::Create
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxString&		name
) 
{
    bool result = wxScrolledWindow::Create(parent,id,pos,size,style,name);

    // Init margins
    ColumnMarginX = 3;
    ColumnMarginY = 1;
    ItemMarginY = 2;


    // Init font
    Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    Font.SetPointSize( Font.GetPointSize() + FontDelta );
    SetFont(Font);


    // Init format strings
    FmtHexByte = GNamedFormat(wxT("HexByte"));
    FmtOffset = GNamedFormat(wxT("Offset"));


    // Init hex char table
    HexStrings.reserve(256);
    HexStrings.resize(256);
    for( dword i=0; i!=256; ++i )
    {
        HexStrings[i] = wxString::Format( FmtHexByte, i );
    }


    // Init ascii char table
    AsciiStrings.reserve(256);
    AsciiStrings.resize(256);
    for( dword i=0; i!=256; ++i )
    {
        AsciiStrings[i] = wxChar(i);
    }


    // Init font sizes
    wxClientDC dc(this);
    dc.SetFont(Font);
    dc.GetTextExtent(wxT("9"),&HexCharWidth,&HexCharHeight);
    dc.GetTextExtent(wxString::Format(FmtHexByte,0x99),&HexByteWidth,NULL);
    dc.GetTextExtent(wxString::Format(FmtOffset,0x99999999),&HexOffsetWidth,NULL);


    // Init item heights
    ColumnHeight = ColumnMarginY + HexCharHeight + ColumnMarginY;
    ItemHeight = ItemMarginY + HexCharHeight + ItemMarginY;
    

    // Add columns
    AddColumn( wxT("Abs Offset"), wxALIGN_LEFT, HexOffsetWidth );
    AddColumn( wxT("Rel Offset"), wxALIGN_LEFT, HexOffsetWidth );
    AddColumn( wxT(""), wxALIGN_LEFT, HexCharWidth );

    for( dword i=0; i!=HexWidth; ++i )
        AddColumn( wxString::Format( wxT("%X"), i%0x10 ), wxALIGN_CENTER, HexByteWidth );

    AddColumn( wxT(""), wxALIGN_LEFT, HexCharWidth );

    for( dword i=0; i!=HexWidth; ++i )
        AddColumn( wxString::Format( wxT("%X"), i%0x10 ), wxALIGN_CENTER, HexByteWidth );


    // Set column indices
    ColHexBegin = 3;
    ColHexEnd = ColHexBegin + HexWidth;
    ColAsciiBegin = ColHexEnd + 1;
    ColAsciiEnd = ColAsciiBegin + HexWidth;


    // Load data
    DataCache.reserve(DataSize);
    DataCache.resize(DataSize);

    HexLines = (DataSize/HexWidth)+1;
    //HexLines = static_cast<dword>(ceil(static_cast<double>(DataSize)/static_cast<double>(HexWidth)));

    File->Seek(DataOffset);
    File->Read(&DataCache.at(0),DataSize);


    // Init background
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));


    // Set virtual height
    VirtualHeight = ColumnHeight + ( ItemHeight * HexLines );
    VirtualWidth = Columns.back().BoxRect.x + Columns.back().BoxRect.width;
    SetVirtualSize( VirtualWidth, VirtualHeight );
    //SetVirtualSizeHints( Columns.at(0).BoxRect.width, ColumnHeight + ItemHeight );
    SetScrollRate( ColumnMarginX + HexByteWidth + ColumnMarginX, ItemHeight );


    return result;
}


// ============================================================================
// uwHexCtrl
// ============================================================================
void uwHexCtrl::AddColumn( const wxString& caption, dword align, long width )
{
    long margin = 0;

    if( width == -1 )
    {
        wxClientDC dc(this);
        dc.SetFont(Font);
        dc.GetTextExtent( caption, &width, NULL );
    }
    else if( align == wxALIGN_CENTER )
    {
        long w;
        wxClientDC dc(this);
        dc.SetFont(Font);
        dc.GetTextExtent( caption, &w, NULL );
        if( width > w )
            margin = (width-w)/2;
    }

    long x = Columns.empty() ? 0 : Columns.back().BoxRect.x + Columns.back().BoxRect.width;
    wxRect box( x, 0, ColumnMarginX + width + ColumnMarginX, ColumnMarginY + HexCharHeight + ColumnMarginY );
    wxRect text( x + margin + ColumnMarginX, ColumnMarginY, width, HexCharHeight );
    Columns.push_back( uwHexCtrlColumn( caption, align, width, box, text) );
}

void uwHexCtrl::SetHexWidth( dword w )
{
    HexWidth = w;
}

void uwHexCtrl::SetFontSize( int delta )
{
    FontDelta = delta;
}


// ============================================================================
// uwHexCtrl - Mutators
// ============================================================================
void uwHexCtrl::SetData( wxFFile* file, wxFileOffset offset, dword size )
{
    File = file;
    DataOffset = static_cast<dword>(offset);
    DataSize = size;
}

void uwHexCtrl::SetHighlight( dword offset, dword size )
{
    HighlightBegin = offset-DataOffset;
    HighlightEnd = HighlightBegin+size;
}

void uwHexCtrl::ClearHighlight()
{
    HighlightBegin = -1;
}

// ============================================================================
// uwHexCtrl - Events
// ============================================================================
void uwHexCtrl::OnScroll( wxScrollWinEvent& event )
{
    Refresh();
    event.Skip();
}

void uwHexCtrl::OnSize( wxSizeEvent& event )
{
    Refresh();
    event.Skip();
}

void uwHexCtrl::OnErase( wxEraseEvent& event )
{
    event.Skip();
}

void uwHexCtrl::PaintRectangle( wxDC& dc, wxColour& colour, const wxRect& rect )
{
    if( !colour.Ok() )
        colour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);

    dc.SetBrush(wxBrush(colour));
    dc.SetPen(wxPen(colour, 1));
    dc.DrawRectangle( rect );
}

void uwHexCtrl::OnPaint(wxPaintEvent& event)
{
    // Setup DC
    wxBufferedPaintDC dc(this);
    PrepareDC(dc);
    dc.BeginDrawing();
    dc.SetFont(Font);
    dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

    // Get native renderer
    wxRendererNative& renderer = wxRendererNative::Get();

    // Get client rectangle
    wxRect rectClient(wxPoint(0,0), GetClientSize());
    wxRect rectUnscrolled(rectClient);
    CalcUnscrolledPosition(rectClient.x, rectClient.y, &rectUnscrolled.x, &rectUnscrolled.y);


    // Paint headers background
    wxRect columnRect(rectClient);
    columnRect.y = 0;
    columnRect.height = ColumnHeight+1;
    CalcUnscrolledPosition(columnRect.x, columnRect.y, &columnRect.x, &columnRect.y);
    PaintRectangle( dc, wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), columnRect );


    // Paint items background
    wxRect windowRect(rectClient);
    windowRect.y = columnRect.height;
    windowRect.height -= windowRect.y;
    CalcUnscrolledPosition(windowRect.x, windowRect.y, &windowRect.x, &windowRect.y);
    PaintRectangle( dc, GetBackgroundColour(), windowRect );


    // return if no columns
    if( Columns.empty() )
        return;

    // Ignore first event, client size is equal virtual size
    if( ++PaintCount == 1 )
        return;

    // Paint headers
    for( dword column=0; column!=Columns.size(); ++column )
    {
        wxRect rbox(Columns.at(column).BoxRect);
        wxRect rtext(Columns.at(column).TextRect);
        CalcUnscrolledPosition(rbox.x-rectUnscrolled.x, rbox.y, &rbox.x, &rbox.y);
        CalcUnscrolledPosition(rtext.x-rectUnscrolled.x, rtext.y, &rtext.x, &rtext.y);
        renderer.DrawHeaderButton( this, dc, rbox, 0 );
        dc.DrawText( Columns.at(column).Caption, rtext.x, rtext.y );
    }

    // Paint header filler
    wxCoord cwidth = Columns.back().BoxRect.x + Columns.back().BoxRect.width-rectUnscrolled.x;
    wxRect rbox( cwidth, 0, rectClient.width-cwidth-1, ColumnHeight );
    if( rbox.width > 0 )
    {
        CalcUnscrolledPosition(rbox.x, rbox.y, &rbox.x, &rbox.y);
        renderer.DrawHeaderButton( this, dc, rbox, 0 );
    }


    // Get potential line numbers
    size_t lineFrom = (rectUnscrolled.y+ColumnHeight) / ItemHeight;
    size_t lineTo = (rectUnscrolled.GetBottom()) / ItemHeight;

    // Cap line numbers
    if( lineTo > HexLines - 1)
        lineTo = HexLines - 1;

    // Get first item pos
    wxCoord y = ColumnHeight + (lineFrom * ItemHeight) + ItemMarginY;
    wxCoord x = 0;

    for( size_t line = lineFrom; line <= lineTo; ++line )
    {
        //wxCoord yPhys;
        //CalcScrolledPosition(0, y, NULL, &yPhys);
        //wxLogMessage("[%d] lineFrom=%d lineTo=%d y=%d yPhys=%d upd=%d,%d uns=%d,%d"
        //    ,line,lineFrom,lineTo,y,yPhys,rectClient.y,rectClient.height,rectUnscrolled.y,rectUnscrolled.height);

        for( dword column=0; column!=Columns.size(); ++column )
        {
            const uwHexCtrlColumn& it = Columns.at(column);

            if( column == 0 )
            {
                dword pos = (line * HexWidth) + DataOffset;
                wxString item = wxString::Format( FmtOffset, pos );
                dc.DrawText( item, it.BoxRect.x+ColumnMarginX, y );
            }
            else if ( column == 1 )
            {
                dword pos = (line * HexWidth);
                wxString item = wxString::Format( FmtOffset, pos );
                dc.DrawText( item, it.BoxRect.x+ColumnMarginX, y );
            }
            else if( column >= ColHexBegin && column < ColHexEnd )
            {
                dword pos = (line * HexWidth) + (column - ColHexBegin);
                if( pos < DataCache.size() )
                {
                    if( HighlightBegin != -1 && pos >= HighlightBegin && pos < HighlightEnd )
                    {
                        wxRect hlrect = it.BoxRect;
                        hlrect.y = y;
                        PaintRectangle( dc, wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW), hlrect );
                        dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
                        dc.DrawText( HexStrings.at(DataCache.at(pos)), it.BoxRect.x+ColumnMarginX, y );
                        dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
                    }
                    else
                    {
                        dc.DrawText( HexStrings.at(DataCache.at(pos)), it.BoxRect.x+ColumnMarginX, y );
                    }

                }
            }
            else if( column >= ColAsciiBegin && column < ColAsciiEnd )
            {
                dword pos = (line * HexWidth) + column - ColAsciiBegin;
                if( pos < DataCache.size() )
                {
                    if( HighlightBegin != -1 && pos >= HighlightBegin && pos < HighlightEnd )
                    {
                        wxRect hlrect = it.BoxRect;
                        hlrect.y = y;
                        PaintRectangle( dc, wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW), hlrect );
                        dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
                        dc.DrawText( AsciiStrings.at(DataCache.at(pos)), it.BoxRect.x+ColumnMarginX, y );
                        dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
                    }
                    else
                    {
                        dc.DrawText( AsciiStrings.at(DataCache.at(pos)), it.BoxRect.x+ColumnMarginX, y );
                    }
                }
            }
        }
      
        y += HexCharHeight + ItemMarginY + ItemMarginY;
    }

    dc.EndDrawing();
}


// ============================================================================