
/* Copyright (C) 2011 Alistair Riddoch <alriddoch@googlemail.com>
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "http_connection.h"

#include <boost/bind.hpp>

#include <iostream>

using boost::bind;

namespace http {

connection::connection(boost::asio::io_service & s) :
    m_socket(s)
{
}

void connection::start()
{
    std::cout << "New http connection" << std::endl << std::flush;
    boost::asio::async_read_until(m_socket, m_data, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));
/*
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
 */
}

void connection::handle_header_read(const boost::system::error_code & e)
{
    if (e) {
        // stop
        std::cout << boost::asio::error::operation_aborted << std::endl << std::flush;
        return;
    }
    std::streamsize c = m_data.in_avail();
    char * buf = new char[c + 1];
    std::istream is(&m_data);
    is.getline(buf, c);

    std::cout << "HEDDA " << " " << c << " " << buf << " " << e << std::endl << std::flush;

    boost::asio::async_read_until(m_socket, m_data, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));
}

void connection::handle_read(const boost::system::error_code & e,
                             std::size_t bytes)
{
    std::cout << "New http data " << bytes << " " << e << std::endl << std::flush;
    if (e) {
        // stop
        std::cout << boost::asio::error::operation_aborted << std::endl << std::flush;
        return;
    }
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
}

} // namespace http
