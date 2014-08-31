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
//  pkgDataTypes.h
// ============================================================================
#ifndef PKGDATATYPES_H
#define PKGDATATYPES_H

#include "pkgDataType.h"


// ============================================================================
// dtIndex -- compressed index
// ============================================================================
class dtIndex : public pkgDataType
{
public:
    // Constructors
    dtIndex() : Data(0), SerialSize(0){}
    dtIndex( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtIndex );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return Data; }
    virtual dword GetSerialSize() const { return SerialSize; }
    virtual dword GetDataSize() const { return 4; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        Data = 0;
        SerialSize = 0;
        byte B0,B1,B2,B3,B4;

        A >> B0;
        ++SerialSize;
        if(B0 & 0x40){
            A >> B1;
            ++SerialSize;
            if(B1 & 0x80){
                A >> B2;
                ++SerialSize;
                if(B2 & 0x80){
                    A >> B3;
                    ++SerialSize;
                    if(B3 & 0x80){
                        A >> B4;
                        ++SerialSize;
                        Data = B4;
                    }Data = (Data << 7) | (B3 & 0x7f);
                }Data = (Data << 7) | (B2 & 0x7f);
            }Data = (Data << 7) | (B1 & 0x7f);
        }Data = (Data << 6) | (B0 & 0x3f);

        if(B0 & 0x80) 
            Data = -Data;

        DT_DUMPINFO;
    }
    
    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    int32	Data;
    byte	SerialSize;
    static const wxString Name;
}; 


// ============================================================================
// dtByte
// ============================================================================
class dtByte : public pkgDataType
{
public:
    // Constructors
    dtByte() : Data(0){}
    dtByte( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtByte );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return Data; }
    virtual dword GetSerialSize() const { return 1; }
    virtual dword GetDataSize() const { return 1; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        A >> Data;
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    byte	Data;
    static const wxString Name;
}; 


// ============================================================================
// dtWord
// ============================================================================
class dtWord : public pkgDataType
{
public:
    // Constructors
    dtWord() : Data(0){}
    dtWord( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtWord );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return Data; }
    virtual dword GetSerialSize() const { return 2; }
    virtual dword GetDataSize() const { return 2; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        A >> Data;
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    word	Data;
    static const wxString Name;
}; 


// ============================================================================
// dtDword
// ============================================================================
class dtDword : public pkgDataType
{
public:
    // Constructors
    dtDword() : Data(0){}
    dtDword( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtDword );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return Data; }
    virtual dword GetSerialSize() const { return 4; }
    virtual dword GetDataSize() const { return 4; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        A >> Data;
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    dword	Data;
    static const wxString Name;
};


// ============================================================================
// dtFloat
// ============================================================================
class dtFloat : public pkgDataType
{
public:
    // Constructors
    dtFloat() : Data(0){}
    dtFloat( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtFloat );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%f"),Data); }
    virtual dword GetData() const { return static_cast<dword>(Data); }
    virtual dword GetSerialSize() const { return 4; }
    virtual dword GetDataSize() const { return 4; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        A >> Data;
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    float	Data;
    static const wxString Name;
}; 


// ============================================================================
// dtToken
// ============================================================================
class dtToken : public pkgDataType
{
public:
    // Constructors
    dtToken(  ) : Data(0){}
    dtToken( pkgSerializer& A ){ Read(A); }
    dtToken( const dtToken& token );

    // Common
    DT_COMMON_INTERFACE( dtToken );

    // Accessors
    virtual wxString GetText() const;
    virtual dword GetData() const { return Data; }
    virtual dword GetSerialSize() const;
    virtual dword GetDataSize() const;
    virtual void DumpInfo();

    // Serialization
    virtual void Read( pkgSerializer& A );
    virtual void Write( pkgSerializer& A );

    // Tree
    pkgDataType* AddItem( pkgDataType* item ) 
    { 
        item->SetParent( static_cast<pkgDataType*>(this) ); 
        Items.push_back(item); 
        return item; 
    }

    // Token
    const wxString GetTokenName() const { return TokenName; }
    void SetTokenName( const wxString& tokenname )  { TokenName = tokenname; }
    void SetTokenData( byte data ) { Data = data; }

protected:
    byte Data;
    wxString TokenName;
    ptrvector<pkgDataType*> Items;
    static const wxString Name;
};


// ============================================================================
// dtAsciiZ
// ============================================================================
class dtAsciiZ : public pkgDataType
{
public:
    // Constructors
    dtAsciiZ() {}
    dtAsciiZ( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtAsciiZ );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return unStringToDword(Data); }
    virtual dword GetSerialSize() const { return Data.length()+1; }
    virtual dword GetDataSize() const { return Data.length()+1; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        Data.Clear();
        byte idx = 0;
        wxChar c;
        do		{ A >> c; Data.Append(c); }
        while	( c != 0 && idx++ < MaxLength );
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    wxString Data;
    static const dword MaxLength = 16777216;
    static const wxString Name;
};


// ============================================================================
// dtUnicodeZ
// ============================================================================
class dtUnicodeZ : public pkgDataType
{
public:
    // Constructors
    dtUnicodeZ() {}
    dtUnicodeZ( pkgSerializer& A ){ Read(A); }

    // Common
    DT_COMMON_INTERFACE( dtUnicodeZ );

    // Accessors
    virtual wxString GetText() const { return wxString::Format(wxT("%d"),Data); }
    virtual dword GetData() const { return unStringToDword(Data); }
    virtual dword GetSerialSize() const { return Data.length()+1; }
    virtual dword GetDataSize() const { return Data.length()+1; }

    // Serialization
    virtual void Read( pkgSerializer& A )
    {
        Data.Clear();
        byte idx = 0;
        wxChar c;
        do		{ A >> c; Data.Append(c); }
        while	( c != 0 && idx++ < MaxLength );
        DT_DUMPINFO;
    }

    virtual void Write( pkgSerializer& A )
    {
    }

protected:
    wxString Data;
    static const dword MaxLength = 16777216;
    static const wxString Name;
};

/*
namespace TokenReader
{
static pkgTokenReader Token;
static pkgTokenReader Index;
static pkgTokenReader IndexName;
static pkgTokenReader IndexObject;
static pkgTokenReader ArrayOf;
static pkgTokenReader ArrayEnd;
static pkgTokenReader Function;
static pkgTokenReader Byte;
static pkgTokenReader Word;
static pkgTokenReader Dword;
static pkgTokenReader Qword;
static pkgTokenReader Float;
static pkgTokenReader AsciiZ;
static pkgTokenReader UnicodeZ;
};
*/




#endif PKGDATATYPES_H
// ============================================================================