############################################################################
# CMakeLists.txt
# Copyright (C) 2010-2022 Belledonne Communications, Grenoble France
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
############################################################################

target_sources(flexisip PRIVATE
	digest.cc digest.hh
	observable.hh
	rand.cc rand.hh
	sip-uri.cc
	string-formater.cc string-formater.hh
	string-utils.cc string-utils.hh
	thread/auto-thread-pool.cc thread/auto-thread-pool.hh
	thread/basic-thread-pool.cc thread/basic-thread-pool.hh
	thread/base-thread-pool.cc thread/base-thread-pool.hh
	thread/thread-pool.hh
	transport/http/http2client.cc transport/http/http2client.hh
	transport/http/http-headers.cc transport/http/http-headers.hh
	transport/http/http-message.cc transport/http/http-message.hh
	transport/http/http-response.cc transport/http/http-response.hh
	transport/http/ng-data-provider.cc transport/http/ng-data-provider.hh
	transport/tls-connection.cc transport/tls-connection.hh
	uri-utils.cc uri-utils.hh
)
