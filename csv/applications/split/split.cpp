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

#ifdef WIN32
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread_time.hpp>
#include <comma/base/exception.h>
#include "./split.h"

namespace comma { namespace csv { namespace applications {

split::split( boost::optional< boost::posix_time::time_duration > period
            , const std::string& suffix
            , const comma::csv::options& csv )
    : ofstream_( boost::bind( &split::ofstream_by_time_, this ) )
    , period_( period )
    , suffix_( suffix )
{
    if( ( csv.has_field( "t" ) || csv.fields.empty() ) && !period ) { COMMA_THROW( comma::exception, "please specify --period" ); }
    if( csv.fields.empty() ) { return; }
    if( csv.binary() ) { binary_.reset( new comma::csv::binary< input >( csv ) ); }
    else { ascii_.reset( new comma::csv::ascii< input >( csv ) ); }
    if( csv.has_field( "block" ) ) { ofstream_ = boost::bind( &split::ofstream_by_block_, this ); }
    else if( csv.has_field( "id" ) ) { ofstream_ = boost::bind( &split::ofstream_by_id_, this ); }
}

void split::write( const char* data, unsigned int size )
{
    mode_ = std::ofstream::out | std::ofstream::binary;
    if( binary_ ) { binary_->get( current_, data ); }
    else { current_.timestamp = boost::get_system_time(); }
    ofstream_().write( data, size );
}

void split::write ( const std::string& line )
{
    mode_ = std::ofstream::out; // quick and dirty
    if( ascii_ ) { ascii_->get( current_, line ); }
    else { current_.timestamp = boost::get_system_time(); }
    std::ofstream& ofs = ofstream_();
    ofs.write( &line[0], line.size() );
    ofs.put( '\n' );
}

std::ofstream& split::ofstream_by_time_()
{
    if( !last_ || current_.timestamp > ( last_->timestamp + *period_ ) )
    {
        file_.close();
        std::string time = boost::posix_time::to_iso_string( current_.timestamp );
        if( time.find_first_of( '.' ) == std::string::npos ) { time += ".000000"; }
        file_.open( ( time + suffix_ ).c_str(), mode_ );
        last_ = current_;
    }
    return file_;
}

std::ofstream& split::ofstream_by_block_()
{
    if( !last_ || last_->block != current_.block )
    {
        file_.close();
        std::string name = boost::lexical_cast< std::string >( current_.block ) + suffix_;
        file_.open( name.c_str(), mode_ );
        last_ = current_;
    }
    return file_;    
}

std::ofstream& split::ofstream_by_id_()
{
    Files::iterator it = files_.find( current_.id );
    if( it == files_.end() )
    {
        #ifdef WIN32
        static unsigned int max_number_of_open_files = 128;
        #else 
        static struct rlimit r;
        static int q = getrlimit( RLIMIT_NOFILE, &r );
        if( q != 0 ) { COMMA_THROW( comma::exception, "getrlimit() failed" ); }
        static unsigned int max_number_of_open_files = static_cast< unsigned int >( r.rlim_cur );
        #endif
        if( files_.size() + 10 > max_number_of_open_files ) { files_.clear(); } // quick and dirty, may be too drastic...
        std::ios_base::openmode mode = mode_;
        if( seen_ids_.find( current_.id ) == seen_ids_.end() ) { seen_ids_.insert( current_.id ); }
        else { mode |= std::ofstream::app; }
        std::string name = boost::lexical_cast< std::string >( current_.id ) + suffix_;
        it = files_.insert( std::make_pair( current_.id, new std::ofstream( name.c_str(), mode ) ) ).first;
    }
    return *it->second;
}

} } } // namespace comma { namespace csv { namespace applications {
