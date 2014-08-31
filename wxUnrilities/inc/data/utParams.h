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
//  utParams.h
// ============================================================================
#ifndef UTPARAMS_H
#define UTPARAMS_H


// ============================================================================
// utParams_0
// ============================================================================
template<class TR> 
struct utParams_0
{
    typedef TR TR;
};

// Creators
template<class TR> 
inline utParams_0<TR> make_utParams_0()
{
    return utParams_0<TR>();
}


// ============================================================================
// utParams_1
// ============================================================================
template<class TR, class TA> 
struct utParams_1
{ 
    typedef TR TR;
    typedef TA TA;
    TA A;
    utParams_1( TA a ) : A(a) {}
};

// Creators
template<class TR, class TA> 
inline utParams_1<TR,TA> make_utParams_1( TA a )
{
    return utParams_1<TR,TA>(a);
}


// ============================================================================
// utParams_2
// ============================================================================
template<class TR, class TA, class TB> 
struct utParams_2
{ 
    typedef TR TR;
    typedef TA TA;
    typedef TB TB;
    TA A;
    TB B;
    utParams_2( TA a, TB b ) : A(a), B(b) {}
};

// Creators
template<class TR, class TA, class TB> 
inline utParams_2<TR,TA,TB> make_utParams_2( TA a, TB b )
{
    return utParams_2<TR,TA,TB>(a,b);
}


// ============================================================================
// utParams_3
// ============================================================================
template<class TR, class TA, class TB, class TC> 
struct utParams_3
{ 
    typedef TR TR;
    typedef TA TA;
    typedef TB TB;
    typedef TC TC;
    TA A;
    TB B;
    TC C;
    utParams_3( TA a, TB b, TC c ) : A(a), B(b), C(c) {}
};

// Creators
template<class TR, class TA, class TB, class TC>
inline utParams_3<TR,TA,TB,TC> make_utParams_3( TA a, TB b, TC c )
{
    return utParams_3<TR,TA,TB,TC>(a,b,c);
}


// Creator - result from function ptr and params explicit
template<class TR> 
inline utParams_0<TR> make_utParams_0_mf()
{
    return utParams_0<TR>();
}


#endif UTPARAMS_H
// ============================================================================