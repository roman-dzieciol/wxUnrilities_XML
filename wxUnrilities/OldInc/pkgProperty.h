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
//  pkgProperty.h
// ============================================================================
#ifndef PKGPROPERTY_H
#define PKGPROPERTY_H

#include "pkgSerializer.h"

class pkgProperty;


// ============================================================================
// PropertyInfo
// ============================================================================
#define PR_MASK_TYPE    0x0f
#define PR_MASK_SIZE    0x70
#define PR_MASK_ARRAY   0x80


// ============================================================================
// pkgProperties
// ============================================================================
typedef ptrvector<pkgProperty*> pkgProperties;


// ============================================================================
// PKG_PROPERTY
// ============================================================================

#define PR_ALLOC(Class) \
    static pkgProperty* Create( pkgSerializer& S, pkgArchive& A, const pkgProperty& P ) { return new Class(S,A,P); } \
    virtual void Destroy() { delete this; }

#define PR_INIT(T) \
    PR_ALLOC(T) \
    explicit T( pkgSerializer& S, pkgArchive& A, const pkgProperty& P ) \
    :   pkgProperty( S,A,P )


// ============================================================================
// pkgProperty
// ============================================================================
class pkgProperty
{
public:
    // Special
    pkgProperty( pkgSerializer& S, pkgArchive& A );
    pkgProperty( pkgSerializer& S, pkgArchive& A, const pkgProperty& P );
    virtual ~pkgProperty()
    {
    }

    // Serialization
    dword ReadArray( pkgArchive& A );
    dword ReadSize( pkgArchive& A );
    const pkgNameItem ReadNameItem( pkgSerializer& S );

    // Accessors
    wxFileOffset GetOffset() const { return Offset; }
    pkgNameItem GetNameItem() const { return Name; }
    byte GetInfo() const { return Info; }
    byte GetType() const { return Type; }
    byte GetSize() const { return Size; }
    byte GetArray() const { return Array; }

    // Accessors
    bool IsArray() const { return Array != -1; }
    dword GetNameIdx() const { return Name.ItemIndex; }
    wxString GetName() const { return Name.Name; }

    // Debug
    virtual wxString DumpInfo();

    // Memory Allocation
    PR_ALLOC(pkgProperty);

protected:
    wxFileOffset    Offset;     // File offset
    pkgNameItem     Name;       // NameTable item
    byte            Info;       // Info byte
    byte            Type;       // Type index
    dword           Size;       // Serial size, may be invalid!
    dword           Array;      // Array item index, -1 means no array

protected:
    static const wxString   PropertyTerminator;
};


// ============================================================================
// pkgProperty - Inline
// ============================================================================
inline dword pkgProperty::ReadArray( pkgArchive& A )
{
    if( (Info & PR_MASK_ARRAY) != 0 )
    {
        byte idxinfo, index;
        A >> idxinfo;

        if( idxinfo < 128 ) 
        {
            index = static_cast<dword>(idxinfo);
        }
        else 
        {
            A.Seek( -1, wxFromCurrent );
            word idxword;
            A >> idxword;
            idxword = idxword & 0x7fff;
            if( idxword < 16384 )
            {
                index = static_cast<dword>(idxword);
            }
            else 
            {
                A.Seek( -2, wxFromCurrent );
                A >> index;
                index = index & 0x3fffffff;
            }
        }

        if( index < 0 )
            throw exFName( wxString::Format( wxT("Invalid array index: %d"), index ).c_str() );

        return index;
    }
    else
        return -1;
}

inline dword pkgProperty::ReadSize( pkgArchive& A )
{
    dword sizetype = Info & PR_MASK_SIZE;
    switch( sizetype ) 
    {
    case	0x00:	return 1;	
    case	0x10:	return 2;											
    case	0x20:	return 4;											
    case	0x30:	return 12;										
    case	0x40:	return 16;										
    case	0x50:	byte  psizeb; A >> psizeb; return psizeb;       
    case	0x60:	word  psizew; A >> psizew; return psizew;	      
    case	0x70:	dword psized; A >> psized; return psized;
    default:		
        throw exFName( wxString::Format( wxT("Unknown size type: %d"), sizetype ).c_str() );
    }
}

inline const pkgNameItem pkgProperty::ReadNameItem( pkgSerializer& S )
{
    return S.GetNameItem( unIndex(S) );
}


#endif PKGPROPERTY_H
// ============================================================================