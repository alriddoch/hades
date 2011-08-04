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

#ifndef HADES_SOCKET_MANAGER_H
#define HADES_SOCKET_MANAGER_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <set>

class base_connection;

class socket_manager
  : private boost::noncopyable
{
  public:
    void add(boost::shared_ptr<base_connection> c);

    void remove(boost::shared_ptr<base_connection> c);

  private:
    std::set<boost::shared_ptr<base_connection> > sockets_;
};

#endif // HADES_SOCKET_MANAGER_H
