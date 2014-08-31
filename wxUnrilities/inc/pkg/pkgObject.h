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
//  pkgObject.h
// ============================================================================
#ifndef PKGOBJECT_H
#define PKGOBJECT_H

#include "pkgType.h"




// ============================================================================
// pkgObject
// ============================================================================
class pkgObject : public pkgType
{
public:
    // Special
    pkgObject( const wxString& tname=wxEmptyString, const wxString& name=wxEmptyString, const wxString& desc=wxEmptyString );
    ~pkgObject();

    // Virtual ctors
    virtual pkgObject* Clone() { return new pkgObject(*this); }
    virtual pkgObject* Create() { return new pkgObject(); }
    virtual pkgObject* Construct() { return new pkgObject(TypeName,Name,Desc); }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return wxEmptyString; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

protected:
    const static wxString StaticTypeName;
};



#endif PKGOBJECT_H
// ============================================================================