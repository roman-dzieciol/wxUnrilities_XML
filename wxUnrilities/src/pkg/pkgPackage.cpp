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
//	pkgPackage.cpp
// ============================================================================
#include "prec.h"
#include "pkgPackage.h"
#include "pkgPackageFormat.h"
#include "pkgPackageData.h"


// ============================================================================
// pkgPackage
// ============================================================================
pkgPackage::pkgPackage()
:   PackageData(NULL)
,   PackageFormat(NULL)
,   LoadFlags(0)
,   GuardOffset(-1)
,   GuardMax(-1)
{    
}

pkgPackage::~pkgPackage()
{
    delete PackageData;
    delete PackageFormat;
    if( File.IsOpened() )
        File.Close();
}

void pkgPackage::OpenFile( wxFileName filename )
{
    FileName = filename;

    // Open file
    File.Open( filename.GetFullPath(), wxT("rb") );
    if( !File.IsOpened() )
        throw exception( wxString::Format( wxT("Could not open file %s"), filename.GetFullPath().c_str() ).c_str() );


    delete PackageFormat;
    PackageFormat = new pkgPackageFormat;
    PackageFormat->OnReload();

    delete PackageData;
    PackageData = new pkgPackageData;
    PackageData->OnReload(*this);
}



// ============================================================================
// pkgPackage - File
// ============================================================================
void pkgPackage::DumpOffset() const 
{ 
    wxLogMessage( wxT("OFFSET: 0x%.8X"), static_cast<dword>(File.Tell()) ); 
}


// ============================================================================