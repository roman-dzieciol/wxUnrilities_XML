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
//  pkgProperties.h
// ============================================================================
#ifndef PKGPROPERTIES_H
#define PKGPROPERTIES_H

#include "pkgProperty.h"
#include "pkgPropertyFactory.h"
#include "unFlags.h"


// ============================================================================
// pkgpByte
// ============================================================================
class pkgpByte : public pkgProperty
{
public:
    PR_INIT(pkgpByte)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    byte Value;
};


// ============================================================================
// pkgpInt
// ============================================================================
class pkgpInt : public pkgProperty
{
public:
    PR_INIT(pkgpInt)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    int32 Value;
};


// ============================================================================
// pkgpBool
// ============================================================================
class pkgpBool : public pkgProperty
{
public:
    PR_INIT(pkgpBool)
    {
        Size  = ReadSize(A);  // Read property size

        // Read property value
        Value = (Info & PR_MASK_ARRAY) != 0;
        Array = -1; 
    }

public:
    bool Value;
};


// ============================================================================
// pkgpFloat
// ============================================================================
class pkgpFloat : public pkgProperty
{
public:
    PR_INIT(pkgpFloat)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    float Value;
};


// ============================================================================
// pkgpObject
// ============================================================================
class pkgpObject : public pkgProperty
{
public:
    PR_INIT(pkgpObject)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unIndex Value;
};


// ============================================================================
// pkgpName
// ============================================================================
class pkgpName : public pkgProperty
{
public:
    PR_INIT(pkgpName)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unIndex Value;
};


// ============================================================================
// pkgpString
// ============================================================================
class pkgpString : public pkgProperty
{
public:
    PR_INIT(pkgpString)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        //A >> Value;     
        A.Seek( Size, wxFromCurrent );
    }

public:
    unStringZ Value;
};


// ============================================================================
// pkgpClass
// ============================================================================
class pkgpClass : public pkgProperty
{
public:
    PR_INIT(pkgpClass)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
    }

public:
};


// ============================================================================
// pkgpArray
// ============================================================================
class pkgpArray : public pkgProperty
{
public:
    PR_INIT(pkgpArray)
    {
        Size  = ReadSize(A);  // Read property size, may be invalid!
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> ItemCount;     
        A.Seek( Size-ItemCount.GetBytesRead(), wxFromCurrent );
    }

    virtual wxString DumpInfo()
    {
        return wxString::Format( wxT("%s(%d)"), GetName().c_str(), ItemCount.GetData() );
    }

public:
    unIndex        ItemCount;  // Number of items in array
    pkgProperties  Properties;
};


// ============================================================================
// pkgpStruct
// ============================================================================
class pkgpStruct : public pkgProperty
{
public:
    PR_INIT(pkgpStruct)
    {
        SType = ReadNameItem(S); // Read struct type, may be invalid!
        Size  = ReadSize(A);     // Read property size
        Array = ReadArray(A);    // Read array info

        // ASSUME: 
        // Native structs have special-case serialization code.
        // Therefore just read/skip specified number of bytes.
        if( SType.Flags.HasFlag( *S.GetFlags()->FindFlag(wxT("Object"),wxT("Native")) ) )
        {
            A.Seek( Size, wxFromCurrent );
        }

        // ASSUME: 
        // Non-Native structs are saved as block of None-terminated properties
        // TODO:
        // Find out why Size is off by few bytes sometimes
        else
        {
            wxFileOffset readrange = A.Tell() + Size;
            while( A.Tell() < readrange )
            {
                if( !Properties.push_existing( GPropertyFactory.Create(S,A) ) )
                    break;
            }
        }
    }

public:
    pkgNameItem    SType;
    pkgProperties  Properties;
};


// ============================================================================
// pkgpVector
// ============================================================================
class pkgpVector : public pkgProperty
{
public:
    PR_INIT(pkgpVector)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
    }

public:
};


// ============================================================================
// pkgProperty
// ============================================================================
class pkgpRotator : public pkgProperty
{
public:
    PR_INIT(pkgpRotator)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
    }

public:
};


// ============================================================================
// pkgpStr
// ============================================================================
class pkgpStr : public pkgProperty
{
public:
    PR_INIT(pkgpStr)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> Value;     
    }

public:
    unName Value;
};


// ============================================================================
// pkgpMap
// ============================================================================
class pkgpMap : public pkgProperty
{
public:
    PR_INIT(pkgpMap)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
    }

public:
};


// ============================================================================
// pkgpFixedArray
// ============================================================================
class pkgpFixedArray : public pkgProperty
{
public:
    PR_INIT(pkgpFixedArray)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A.Seek( Size, wxFromCurrent );
    }

public:
};


// ============================================================================
// pkgpArrayAlt
// ============================================================================
class pkgpArrayAlt : public pkgProperty
{
public:
    PR_INIT(pkgpArrayAlt)
    {
        Size  = ReadSize(A);  // Read property size
        Array = ReadArray(A); // Read array info

        // Read property value
        A >> ItemCount;     

        // skip number of 0-bytes equal to array size
        byte b, count = 0;
        while( count < ItemCount )
        {
            A >> b;
            if( b == 0 )
                ++count;
        }
    }

    virtual wxString DumpInfo()
    {
        return wxString::Format( wxT("%s(%d)"), GetName().c_str(), ItemCount.GetData() );
    }

public:
    unIndex        ItemCount;  // Number of items in array
    pkgProperties  Properties;
};


#endif PKGPROPERTIES_H
// ============================================================================