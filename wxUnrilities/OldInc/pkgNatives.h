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
//  pkgNatives.h
// ============================================================================
#ifndef PKGNATIVES_H
#define PKGNATIVES_H

#include "pkgSerializer.h"
class dtToken;

// ============================================================================
// pkgStateFrame
// ============================================================================
class pkgStateFrame
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgStateFrame& D );

public:
    unIndex Node;
    unIndex StateNode;
    qword   ProbeMask;
    int32   LatentAction;
    unIndex Offset;
};


// ============================================================================
// pkgStateFrame - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgStateFrame& D )
{
    A >> D.Node;
    A >> D.StateNode;
    A >> D.ProbeMask;
    A >> D.LatentAction;
    if( D.Node != 0 )
        A >> D.Offset;
    return A;
}


// ============================================================================
// pkgDependency
// ============================================================================
class pkgDependency
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgDependency& D );

public:
    unIndex     Class;
    dword       Deep;
    dword       ScriptTextCRC;
};


// ============================================================================
// pkgDependency - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgDependency& D )
{
    A >> D.Class;
    A >> D.Deep;
    A >> D.ScriptTextCRC;
    return A;
}


// ============================================================================
// pkgField
// ============================================================================
class pkgField
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgField& D );



public:
    unIndex     SuperField;
    unIndex     Next;
};


// ============================================================================
// pkgField - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgField& D )
{
    A >> D.SuperField;
    A >> D.Next;
    return A;
}


// ============================================================================
// pkgStruct
// ============================================================================
class pkgStruct : public pkgField
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgStruct& D );
    bool HasScript() const { return ScriptSize > 0; }
    void ParseScript( pkgSerializer& A, dword scriptsize, vector<dtToken*>& stream );

public:
    unIndex     ScriptText;
    unIndex     Children;
    unIndex     FriendlyName; 
    dword       UnknownA;
    dword       Line;
    dword       TextPos;
    dword       ScriptSize;

    vector<dtToken*> ScriptStream;
};


// ============================================================================
// pkgStruct - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgStruct& D )
{
    A >> static_cast<pkgField&>(D);
    A >> D.ScriptText;
    A >> D.Children;
    A >> D.FriendlyName;
    A >> D.UnknownA;
    A >> D.Line;
    A >> D.TextPos;
    A >> D.ScriptSize;
    if( D.ScriptSize > 0 )
        D.ParseScript(A, D.ScriptSize,D.ScriptStream);
    return A;
}


// ============================================================================
// pkgState
// ============================================================================
class pkgState : public pkgStruct
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgState& D );

public:
    qword       ProbeMask;
    qword       IgnoreMask;
    word        LabelTableOffset;
    unFlag      StateFlags;
};


// ============================================================================
// pkgState - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgState& D )
{
    A >> static_cast<pkgStruct&>(D);
    A >> D.ProbeMask;
    A >> D.IgnoreMask;
    A >> D.LabelTableOffset;
    A >> D.StateFlags;
    return A;
}


// ============================================================================
// pkgClass
// ============================================================================
class pkgClass : public pkgState
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgClass& D );

public:
    unFlag      ClassFlags;
    unGuid      ClassGuid;

    unIndex     DependenciesCount;
    vector<pkgDependency> Dependencies;

    unIndex     PackageImportsCount;
    vector<unIndex> PackageImports;

    unIndex     ClassWithin;
    unIndex     ClassConfigName;

    unIndex     HideCategoriesCount;
    vector<unIndex> HideCategories;
};


// ============================================================================
// pkgClass - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgClass& D )
{
    A >> static_cast<pkgState&>(D);
    A >> D.ClassFlags;
    A >> D.ClassGuid;

    A >> D.DependenciesCount;
    D.Dependencies.resize(D.DependenciesCount);
    for( vector<pkgDependency>::iterator it=D.Dependencies.begin(); it!=D.Dependencies.end(); ++it )
    {
        A >> static_cast<pkgDependency&>(*it);
    }

    A >> D.PackageImportsCount;
    D.PackageImports.resize(D.PackageImportsCount);
    for( vector<unIndex>::iterator it=D.PackageImports.begin(); it!=D.PackageImports.end(); ++it )
    {
        A >> static_cast<unIndex&>(*it);
    }

    A >> D.ClassWithin;
    A >> D.ClassConfigName;

    A >> D.HideCategoriesCount;
    D.HideCategories.resize(D.HideCategoriesCount);
    for( vector<unIndex>::iterator it=D.HideCategories.begin(); it!=D.HideCategories.end(); ++it )
    {
        A >> static_cast<unIndex&>(*it);
    }

    return A;
}


// ============================================================================
// pkgFunction
// ============================================================================
class pkgFunction : public pkgStruct
{
public:
    friend pkgSerializer& operator >> ( pkgSerializer& A, pkgFunction& D );

    /*virtual void DumpToXml( pkgSerializer& s, const unXmlDocument& document )
    {
        //document.
    }*/

public:
    word        iNative;
    byte        OperatorPrecedence;
    unFlag      FunctionFlags;
    //word        ReplicationOffset;
};


// ============================================================================
// pkgFunction - Inline
// ============================================================================
inline pkgSerializer& operator >> ( pkgSerializer& A, pkgFunction& D )
{
    A >> static_cast<pkgStruct&>(D);
    A >> D.iNative;
    A >> D.OperatorPrecedence;
    A >> D.FunctionFlags;
    //A >> D.ReplicationOffset;

    wxLogMessage( "%d", D.iNative );
    return A;
}


#endif PKGNATIVES_H
// ============================================================================