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


/// @author vsevolod vlaskine

#include <comma/csv/options.h>
#include <comma/string/string.h>

namespace comma { namespace csv {

const csv::format& options::format() const { return *format_; }

csv::format& options::format() { return *format_; }

void options::format( const std::string& s ) { format_ = csv::format( s ); }

void options::format( const csv::format& f ) { format_ = f; }

bool options::binary() const { return format_; }

namespace impl {

inline static void init( comma::csv::options& csvoptions, const comma::command_line_options& options, const std::string& defaultFields )
{
    csvoptions.full_xpath = options.exists( "--full-xpath" );
    csvoptions.fields = options.value( "--fields", defaultFields );
    if( options.exists( "--binary" ) )
    {
        boost::optional< std::string > format = options.optional< std::string >( "--binary" );
        if( format )
        {
            csvoptions.format( options.value< std::string >( "--binary" ) );
        }
    }
    csvoptions.precision = options.value< unsigned int >( "--precision", 6 );
    csvoptions.delimiter = options.exists( "--delimiter" ) ? options.value( "--delimiter", ',' ) : options.value( "-d", ',' );
}

} // namespace impl {

options::options() : full_xpath( false ), delimiter( ',' ), precision( 6 ) {}

options::options( int argc, char** argv, const std::string& defaultFields )
{
    impl::init( *this, comma::command_line_options( argc, argv ), defaultFields );
}

options::options( const comma::command_line_options& options, const std::string& defaultFields )
{
    impl::init( *this, options, defaultFields );
}

std::string options::usage()
{
    std::ostringstream oss;
    oss << "    --binary,-b <format> : use binary format" << std::endl;
    oss << "    --delimiter,-d <delimiter> : default: ','" << std::endl;
    oss << "    --fields,-f <names> : field names, e.g. t,,x,y,z" << std::endl;
    oss << "    --full-xpath : expect full xpaths as field names; default: false" << std::endl;
    oss << "                   default false was a wrong choice, but changing it" << std::endl;
    oss << "                   to true now may break too many things" << std::endl;
    oss << "    --precision <precision> : floating point precision; default: 6" << std::endl;
    oss << format::usage();
    return oss.str();
}

bool options::has_field( const std::string& field ) const
{
    const std::vector< std::string >& v = split( fields, ',' );
    const std::vector< std::string >& f = split( field, ',' );
    for( unsigned int i = 0; i < f.size(); ++i ) { if( std::find( v.begin(), v.end(), f[i] ) == v.end() ) { return false; } }
    return true;
}

} } // namespace comma { namespace csv {
