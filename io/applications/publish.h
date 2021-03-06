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


/// @author cedric wohlleber

#ifndef COMMA_IO_APPLICATIONS_PUBLISH_H_
#define COMMA_IO_APPLICATIONS_PUBLISH_H_

#include <map>
#include <boost/shared_ptr.hpp>
#include <comma/containers/cyclic_buffer.h>
#include <comma/io/publisher.h>

namespace comma { namespace io { namespace applications {

class publish
{
public:
    
    publish(const std::vector<std::string> file_names, unsigned int n = 10u, unsigned int c = 10u, unsigned int packet_size = 0, bool discard = true, bool flush = true );
    ~publish();
    void read_line();
    bool read_bytes();

private:
    std::size_t write(const char* buffer, std::size_t size);
    void push(const std::string& line);
    void push(char* buffer, std::size_t size);
    void pop();
    std::vector< boost::shared_ptr< io::publisher > > publishers_;
    io::select select_;

    boost::scoped_ptr< comma::cyclic_buffer< std::string > > line_buffer_;
    boost::scoped_ptr< comma::cyclic_buffer< std::vector<char> > > char_buffer_;
    std::vector<char> packet_; // for reassembly of packets
    unsigned int packet_offset_; // for reassembly of packets
    unsigned int packet_size_;
    unsigned long int packet_counter_;
    unsigned long int first_discarded_;
    bool buffer_discarding_;
    bool discard_;
};

} } } /// namespace comma { namespace io { namespace applications {

#endif // COMMA_IO_APPLICATIONS_PUBLISH_H_
