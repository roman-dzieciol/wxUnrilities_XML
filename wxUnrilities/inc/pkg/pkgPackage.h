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
//  pkgPackage.h
// ============================================================================
#ifndef PKGPACKAGE_H
#define PKGPACKAGE_H

class pkgPackageFormat;
class pkgPackageData;

enum EPackageLoadFlags
{
    PLF_None    = 0x0000
,   PLF_Force   = 0x0001
};


// ============================================================================
// pkgPackage
// ============================================================================
class pkgPackage
{
public:
    STATIC_ALLOCATOR(pkgPackage);

    // Special
    pkgPackage();
    ~pkgPackage();

    void OpenFile( wxFileName filename );

    // Accessors
    pkgPackageFormat& GetFormat() { return *PackageFormat; }
    pkgPackageData& GetData() { return *PackageData; }
    wxFileName GetFileName() { return FileName; }
    wxString GetName() { return FileName.GetName(); }
    dword GetLoadFlags() { return LoadFlags; }

    // Mutators
    void SetLoadFlags( dword d ) { LoadFlags = d; }

    // File
    void DumpOffset() const;

    friend pkgPackage& operator >> ( pkgPackage& P, qword& D )	{ P.Read( &D, sizeof(qword) ); P.CheckEOF(); /*wxUINT64_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, dword& D )	{ P.Read( &D, sizeof(dword) ); P.CheckEOF(); /*wxUINT32_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, word& D )	{ P.Read( &D, sizeof(word) ); P.CheckEOF(); /*wxUINT16_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, byte& D )	{ P.Read( &D, sizeof(byte) ); P.CheckEOF(); return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, int64& D )	{ P.Read( &D, sizeof(int64) ); P.CheckEOF(); /*wxINT64_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, int32& D )	{ P.Read( &D, sizeof(int32) ); P.CheckEOF(); /*wxINT32_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, int16& D )	{ P.Read( &D, sizeof(int16) ); P.CheckEOF(); /*wxINT16_SWAP_ON_?E(D);*/ return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, int8& D )	{ P.Read( &D, sizeof(int8) ); P.CheckEOF(); return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, char& D )	{ P.Read( &D, sizeof(int8) ); P.CheckEOF(); return P; }
    friend pkgPackage& operator >> ( pkgPackage& P, float& D )	{ P.Read( &D, sizeof(dword) ); P.CheckEOF(); /*wxINT32_SWAP_ON_?E(D);*/ return P; }
    
    size_t Read( void* buffer, size_t count ) 
    { 
        // read without guards
        if( GuardOffset == -1 )
            return File.Read(buffer,count); 

        // read with guards
        const wxFileOffset pos = File.Tell();
        if( pos >= GuardOffset && pos+count <= GuardMax )
            return File.Read(buffer,count);

        throw exception( wxString::Format( wxT("File access <0x%.8x 0x%.8x> outside valid range <0x%.8x 0x%.8x>")
        , static_cast<dword>(pos) 
        , static_cast<dword>(pos+count) 
        , static_cast<dword>(GuardOffset) 
        , static_cast<dword>(GuardMax) 
        ).c_str() );

    }

    void CheckEOF()
    {
        if( File.Eof() && File.Tell() > File.Length() )
            throw exception( wxString::Format( wxT("Unexpected EOF: %d"), static_cast<dword>(File.Tell()) ).c_str() );
    }

    wxFFile& GetFile() { return File; }

    void SetReadRange( wxFileOffset offset, wxFileOffset size )
    {
        GuardOffset = offset;
        GuardMax = offset + size;
    }

    void ResetReadRange()
    {
        GuardOffset = -1;
    }


protected:
    wxFileName FileName;
    wxFFile File;
    pkgPackageData* PackageData;
    pkgPackageFormat* PackageFormat;
    dword LoadFlags;
    wxFileOffset GuardOffset;
    wxFileOffset GuardMax;
};






#endif PKGPACKAGE_H
// ============================================================================