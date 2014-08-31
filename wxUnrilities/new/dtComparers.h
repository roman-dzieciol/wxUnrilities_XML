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
//  dtComparers.h
// ============================================================================
#ifndef DTCOMPARERS_H
#define DTCOMPARERS_H

#include "dtComparer.h"



// ============================================================================
// cmpTextNoCase
// ============================================================================
class cmpTextNoCase : public dtComparer
{
public:
    STATIC_CREATOR( cmpTextNoCase, dtComparer );

    virtual bool Compare( const wxString& a, const wxString& b ) const
    {
        return a.CmpNoCase(b) < 0;
    }

public:
    static const wxString Name;
};


// ============================================================================
// cmpText
// ============================================================================
class cmpText : public dtComparer
{
public:
    STATIC_CREATOR( cmpText, dtComparer );

    virtual bool Compare( const wxString& a, const wxString& b ) const
    {
        return a.Cmp(b) < 0;
    }

public:
    static const wxString Name;
};


// ============================================================================
// cmpNumber
// ============================================================================
class cmpNumber : public dtComparer
{
public:
    STATIC_CREATOR( cmpNumber, dtComparer );

    virtual bool Compare( const wxString& a, const wxString& b ) const
    {
        return unStringToDword(a) < unStringToDword(b);
    }

public:
    static const wxString Name;
};


#endif DTCOMPARERS_H
// ============================================================================