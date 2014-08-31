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

#include "pkgSerializer.h"

class pkgProperty;
class pkgStateFrame;
class pkgClass;
class pkgFunction;


// ============================================================================
// pkgProperties
// ============================================================================
typedef ptrvector<pkgProperty*> pkgProperties;


// ============================================================================
// pkgObject
// ============================================================================
class pkgObject
{
public:
    // Constructors
    pkgObject( pkgSerializer* serializer, dword index );
    ~pkgObject();

    // Serialization
    friend pkgArchive& operator >> ( pkgArchive& A, pkgObject& D );

    // Serialization
    bool InspectObject();
    void LoadObject();
    void LoadProperties();
    void Decompile();
    void GetNativeFunctionIndex();
    void GetNativeFunctionIndex2();

    // Debug
    void DumpProperties();

    // Fixes for unconsistent package format
    void FixProperties();
    void FixArrayProperty( pkgProperty* p );

protected:
    pkgSerializer*          Serializer;
    dword                   ExportIndex;
    const pkgExportItem&    ExportItem;

    pkgStateFrame*          StateFrame;
    pkgClass*               Class;
    pkgFunction*            Function;
    unFlag                  Flags;
    pkgProperties           Properties;

protected:
    static const wxString   FunctionName;
};


// ============================================================================
// pkgObject - Inline
// ============================================================================
inline pkgArchive& operator >> ( pkgArchive& A, pkgObject& D )
{
    D.LoadObject();
    return A;
}


#endif PKGOBJECT_H
// ============================================================================