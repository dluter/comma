// This file is part of comma, a generic and flexible library
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this software
//    must display the following acknowledgement:
//    This product includes software developed by the The University of Sydney.
// 4. Neither the name of the The University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <boost/array.hpp>
//#include <google/profiler.h>
#include <comma/base/types.h>
#include <comma/csv/stream.h>

namespace comma { namespace csv { namespace test {

struct test_struct
{
    comma::uint32 x;
    comma::uint32 y;
    test_struct() : x( 0 ), y( 0 ) {}
    test_struct( comma::uint32 x, comma::uint32 y ) : x( x ), y( y ) {}
};

} } } // namespace comma { namespace csv { namespace test {

namespace comma { namespace visiting {

template <> struct traits< comma::csv::test::test_struct >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, const comma::csv::test::test_struct& p, Visitor& v )
    {
        v.apply( "x", p.x );
        v.apply( "y", p.y );
    }
    
    template < typename Key, class Visitor >
    static void visit( const Key&, comma::csv::test::test_struct& p, Visitor& v )
    {
        v.apply( "x", p.x );
        v.apply( "y", p.y );
    }    
};

} } // namespace comma { namespace visiting {

namespace comma { namespace csv { namespace test {

TEST( csv, stream )
{
//	std::cerr << "ProfileStream(): start" << std::endl;
//	const std::size_t size = 100000;
//	std::string istring( size * 4 * 2, 0 );
//	for( unsigned int i = 0; i < size; ++i ) // no need, really
//	{
//		::memcpy( &istring[i*4*2], &i, 4 );
//		::memcpy( &istring[4 + i*4*2], &i, 4 );
//	}
//	std::istringstream iss( istring );
//	std::ostringstream oss;
//	comma::csv::options csv;
//	csv.format( "%ui%ui" );
//	EXPECT_TRUE( csv.binary() );
//	comma::csv::input_stream< test_struct > istream( iss, csv );
//	comma::csv::output_stream< test_struct > ostream( oss, csv );
//	ProfilerStart( "csv_stream.prof" );	{
//	for( unsigned int i = 0; i < size; ++i )
//	{
//		const test_struct* s = istream.read();
//		test_struct t( s->x + 1, s->y + 1 );
//		//ostream.write( t, istream.last() );
//		ostream.binary().write( t, istream.binary().last() );
//	}
//	ProfilerStop(); }
//	std::cerr << "ProfileStream(): stop" << std::endl;
}

} } } // namespace comma { namespace csv { namespace test {

