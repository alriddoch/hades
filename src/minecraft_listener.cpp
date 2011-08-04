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

#include "minecraft_listener.h"

#include "minecraft_connection.h"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include <cassert>

using boost::asio::ip::tcp;
using boost::bind;

namespace minecraft {

listener::listener(boost::asio::io_service & s, socket_manager &) :
    m_io_service(s),
    m_acceptor(s, tcp::endpoint(tcp::v4(), 25565))
{
  setup_accept();
}

void listener::setup_accept()
{
  m_new_connection = new connection(m_io_service);
  m_acceptor.async_accept(m_new_connection->socket(),
      bind(&listener::handle_accept, this, boost::asio::placeholders::error));
}

void listener::handle_accept(const boost::system::error_code & e)
{
  if (!e) {
    assert(m_new_connection != 0);
    m_new_connection->start();
    // accept

    setup_accept();
  }
}

} // namespace minecraft
