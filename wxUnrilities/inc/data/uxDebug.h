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
//  uxDebug.h
// ============================================================================
#ifndef UXDEBUG_H
#define UXDEBUG_H



// ============================================================================
// EDebugLevel
// ============================================================================
enum EDebugLevel
{   DL_Error        = 0x0001
,   DL_Warning      = 0x0002
,   DL_Notice       = 0x0004
,   DL_Debug        = 0x0008
};


// ============================================================================
// uxDebugLog
// ============================================================================
class uxDebugFilter
{
public:
    static uxDebugFilter Global;

public:
    uxDebugFilter()
    {
        ResetMask();
    }

    dword GetMask() const { return Mask; }
    void SetMask( dword mask ) { Mask = mask; }
    void ResetMask() { Mask = DefaultMask; }

protected:
    dword Mask;
    static const dword DefaultMask
        = 0
        | DL_Error 
        | DL_Warning 
        | DL_Notice 
        //| DL_Debug
        ;
};

// ============================================================================
// uxDebugLog
// ============================================================================
inline void uxDebugLog( dword level, const char* text, ... )
{
    if((level & uxDebugFilter::Global.GetMask()) != level )
        return;

    va_list args;
    va_start(args, text);
    wxVLogMessage( text, args );
    va_end(args);
}

// Uncomment this macro to remove all uxDebugLog calls from the build.
#define uxDebugLog //uxDebugLog

#define uxDebugMutator(Obj,Param) wxLogMessage(wxT("!\t[DebugMutator] %s, this: %p, ptr: %p, param: %p"), __FUNCTION__, this, Obj, Param ) 
#define uxDebugAccessor(Obj) wxLogMessage(wxT("!\t[DebugAccessor] %s, this: %p, ptr: %p"), __FUNCTION__, this, Obj )


#endif UXDEBUG_H
// ============================================================================