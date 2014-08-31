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
//  pkgTypeBase.h
// ============================================================================
#ifndef PKGTYPEBASE_H
#define PKGTYPEBASE_H

#include "pkgType.h"
#include "pkgTypeRegistry.h"


// ============================================================================
// pkgTypeBaseRegistry
// ============================================================================
class pkgTypeBaseRegistry : public pkgTypeRegistry
{
public:
    // Initialization
    virtual void OnReload();
};


// ============================================================================
// pkgTypeBase
// ============================================================================
class pkgTypeBase : public pkgType
{
public:
    // Special
    pkgTypeBase( const wxString& tname=wxEmptyString, const wxString& name=wxEmptyString, const wxString& desc=wxEmptyString )
    :   pkgType(tname,name,desc)
    {
    }

    virtual ~pkgTypeBase()
    {

    }

    // Virtual ctors
    virtual pkgTypeBase* Clone() { exNO_IMPL; }
    virtual pkgTypeBase* Create() { exNO_IMPL; }
    virtual pkgTypeBase* Construct() { exNO_IMPL; }

    // Serialization
    virtual void Read( pkgPackage& package )
    {
        exNO_IMPL;
    }

    virtual void OnRead( pkgPackage& package, pkgType& parent );


    // Accessors
    virtual dword GetOffset() const { return TypeOffset; };

    // Mutators
    virtual void SetOffset( dword p ) { TypeOffset = p; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Ident
    virtual bool IsData() { return true; }

protected:
    dword TypeOffset;
};


// ============================================================================
// pkgTypeBase - common interface
// ============================================================================
#define TYPE_BASE_COMMON(NAME,PARENT) \
private: \
    static const wxString StaticTypeName; \
    static const wxString StaticDesc; \
    \
public: \
    virtual NAME* Clone() { return new NAME(*this); } \
    virtual NAME* Create() { return new NAME(); } \
    virtual NAME* Construct() { return new NAME(TypeName,Name,Desc); } \
    virtual void Read( pkgPackage& package ); \
    NAME( const wxString& tname=StaticTypeName, const wxString& name=StaticTypeName, const wxString& desc=StaticDesc ) :   PARENT(tname,name,desc) \



// ============================================================================
// tbBYTE
// ============================================================================
class tbBYTE : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbBYTE, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetUnsigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%u"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,Data); }

protected:
    byte Data;
};


// ============================================================================
// tbWORD
// ============================================================================
class tbWORD : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbWORD, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetUnsigned();; }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%u"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,Data); }


protected:
    word Data;
};


// ============================================================================
// tbDWORD
// ============================================================================
class tbDWORD : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbDWORD, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetUnsigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%u"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,Data); }


protected:
    dword Data;
};


// ============================================================================
// tbQWORD
// ============================================================================
class tbQWORD : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbQWORD, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetUnsigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return static_cast<dword>(Data); }
    virtual int32 GetSigned() { return static_cast<dword>(Data); }
    virtual wxString GetString() { return wxString::Format(wxT("%u"),static_cast<dword>(Data)); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,static_cast<dword>(Data)); }


protected:
    qword Data;
};


// ============================================================================
// tbINT8
// ============================================================================
class tbINT8 : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbINT8, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetSigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%d"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %d"),Name,Data); }


protected:
    int8 Data;
};


// ============================================================================
// tbINT16
// ============================================================================
class tbINT16 : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbINT16, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetSigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%d"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %d"),Name,Data); }

protected:
    int16 Data;
};


// ============================================================================
// tbINT32
// ============================================================================
class tbINT32 : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbINT32, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetSigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%d"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %d"),Name,Data); }

protected:
    int32 Data;
};


// ============================================================================
// tbFLOAT
// ============================================================================
class tbFLOAT : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbFLOAT, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return sizeof(Data); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetSigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return static_cast<dword>(Data); }
    virtual int32 GetSigned() { return static_cast<int32>(Data); }
    virtual wxString GetString() { return wxString::Format(wxT("%f"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = static_cast<float>(p); }
    virtual void SetSigned( int32 p ) { Data = static_cast<float>(p); }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %f"),Name,Data); }

protected:
    float Data;
};


// ============================================================================
// tbGUID
// ============================================================================
class tbGUID : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbGUID, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return sizeof(A)+sizeof(B)+sizeof(C)+sizeof(D); }
    virtual dword GetSerialSize() const { return sizeof(A)+sizeof(B)+sizeof(C)+sizeof(D); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return wxString::Format(wxT("%08X%08X%08X%08X"),A,B,C,D); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %S"),Name, GetString().c_str()); }

protected:
    dword A;
    dword B;
    dword C;
    dword D;
};


// ============================================================================
// tbINDEX
// ============================================================================
class tbINDEX : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbINDEX, pkgTypeBase )
    {
        SerialSize = 0;
    }

    // Size
    virtual dword GetSize() const { return sizeof(Data); }
    virtual dword GetSerialSize() const { return SerialSize; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { return Data == type->GetUnsigned(); }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return wxString::Format(wxT("%d"),Data); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; }
    virtual void SetSigned( int32 p ) { Data = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %d"),Name,Data); }


protected:
    int32 Data;
    byte SerialSize;
};


// ============================================================================
// tbASCIIZ
// ============================================================================
class tbASCIIZ : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbASCIIZ, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return Data.Length(); }
    virtual dword GetSerialSize() const { return Data.Length()+1; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return ::unStringToDword(Data); }
    virtual int32 GetSigned() { return ::unStringToDword(Data); }
    virtual wxString GetString() { return Data; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = wxString::Format(wxT("%u"),p); }
    virtual void SetSigned( int32 p ) { Data = wxString::Format(wxT("%d"),p); }
    virtual void SetString( const wxString& p ) { Data = p; }

protected:
    wxString Data;
    static const dword MaxLength = 16777216;
};


// ============================================================================
// tbLASCIIZ
// ============================================================================
class tbLASCIIZ : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbLASCIIZ, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return Data.Length(); }
    virtual dword GetSerialSize() const { return Data.Length()+Length.GetSerialSize()+1; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return ::unStringToDword(Data); }
    virtual int32 GetSigned() { return ::unStringToDword(Data); }
    virtual wxString GetString() { return Data; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = wxString::Format(wxT("%u"),p); }
    virtual void SetSigned( int32 p ) { Data = wxString::Format(wxT("%d"),p); }
    virtual void SetString( const wxString& p ) { Data = p; }

protected:
    wxString Data;
    tbINDEX Length;
    static const dword MaxLength = 16777216;
};


// ============================================================================
// tbLASCII
// ============================================================================
class tbLASCII : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbLASCII, pkgTypeBase )
    {
    }

    // Size
    virtual dword GetSize() const { return Data.Length(); }
    virtual dword GetSerialSize() const { return Data.Length()+Length.GetSerialSize(); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return ::unStringToDword(Data); }
    virtual int32 GetSigned() { return ::unStringToDword(Data); }
    virtual wxString GetString() { return Data; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = wxString::Format(wxT("%u"),p); }
    virtual void SetSigned( int32 p ) { Data = wxString::Format(wxT("%d"),p); }
    virtual void SetString( const wxString& p ) { Data = p; }

protected:
    wxString Data;
    tbINDEX Length;
    static const dword MaxLength = 16777216;
};


// ============================================================================
// tbOBJECT
// ============================================================================
class tbOBJECT : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbOBJECT, pkgTypeBase )
    {
    }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Accessors
    virtual dword GetOffset() const { return pkgType::GetOffset(); };

    // Mutators
    virtual void SetOffset( dword p ) { pkgType::SetOffset(p); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return Name; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxEmptyString; }

};


// ============================================================================
// tbOBJVAR
// ============================================================================
class tbOBJVAR : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbOBJVAR, pkgTypeBase )
    {
    }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Accessors
    virtual dword GetOffset() const { return pkgType::GetOffset(); };

    // Mutators
    virtual void SetOffset( dword p ) { pkgType::SetOffset(p); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return Name; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxEmptyString; }

};


// ============================================================================
// tbOBJIND
// ============================================================================
class tbOBJIND : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbOBJIND, pkgTypeBase )
    {
    }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Accessors
    virtual dword GetOffset() const { return pkgType::GetOffset(); };

    // Mutators
    virtual void SetOffset( dword p ) { pkgType::SetOffset(p); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return Name; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxEmptyString; }

};


// ============================================================================
// tbPROP
// ============================================================================
class tbPROP : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbPROP, pkgTypeBase )
    {
    }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Accessors
    virtual dword GetOffset() const { return pkgType::GetOffset(); };

    // Mutators
    virtual void SetOffset( dword p ) { pkgType::SetOffset(p); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { return Name; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Debug
    virtual wxString GetInfo() { return wxEmptyString; }

protected:
    wxString GetPropertyName( byte b );
    dword GetPropertySize( pkgPackage& package, dword sizetype );
    dword GetPropertySerialSize( pkgPackage& package, dword sizetype );
	void ReadProperty( pkgPackage& package, pkgType& parent, pkgType* datatype );

    template <class T>
    T* CreateInstance( const wxString& name, pkgPackage& package, pkgType* parent )
    {
        T* prototype = new T();
		prototype->SetName(name);
        prototype->OnRead(package,*parent);
        T* instance = static_cast<T*>(parent->GetTypeArray().back());
		instance->SetName(name);
        instance->SetPrototype(this);
        return instance;
    }


};


// ============================================================================
// tbINTERNAL
// ============================================================================
class tbINTERNAL : public pkgTypeBase
{
public:
    TYPE_BASE_COMMON( tbINTERNAL, pkgTypeBase )
    {
        Data = 0;
        Size = 0;
        SerialSize = 0;
    }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );

    // Size
    virtual dword GetSize() const { return Size; }
    virtual dword GetSerialSize() const { return SerialSize; }

    virtual void SetSize( dword p ) { Size = p; }
    virtual void SetSerialSize( dword p ) { SerialSize = p; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return Data; }
    virtual int32 GetSigned() { return Data; }
    virtual wxString GetString() { return Text; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Data = p; Text = wxString::Format(wxT("%u"),p); }
    virtual void SetSigned( int32 p ) { Data = p; Text = wxString::Format(wxT("%d"),p); }
	virtual void SetString( const wxString& p ) { Text = p; Data = ::unStringToDword(p); }

protected:
    wxString Text;
    dword Data;
    dword Size;
    dword SerialSize;
};


#endif PKGTYPEBASE_H
// ============================================================================