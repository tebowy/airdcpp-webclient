/*
* Copyright (C) 2011-2015 AirDC++ Project
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef DCPLUSPLUS_WEBSERVER_STDINC_H
#define DCPLUSPLUS_WEBSERVER_STDINC_H

#ifndef _WEBSOCKETPP_CPP11_STL_
#define _WEBSOCKETPP_CPP11_STL_
#endif

#include <airdcpp/stdinc.h>
#include <airdcpp/StringTokenizer.h>

#include <nlohmann/json.hpp>

#include <web-server/Exception.h>

#include <websocketpp/http/constants.hpp>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>



#define CODE_UNPROCESSABLE_ENTITY 422

namespace webserver {
	// define types for two different server endpoints, one for each config we are
	// using
	typedef websocketpp::server<websocketpp::config::asio> server_plain;
	typedef websocketpp::server<websocketpp::config::asio_tls> server_tls;
	typedef websocketpp::http::status_code::value api_return;

	typedef std::function<void(api_return aStatus, const std::string& aOutput, const std::vector<std::pair<std::string, std::string>>& aHeaders)> HTTPFileCompletionF;
	typedef std::function<void(api_return aStatus, const json& aResponseJsonData, const json& aResponseErrorJson)> ApiCompletionF;
	typedef std::function<HTTPFileCompletionF()> FileDeferredHandler;
	typedef std::function<ApiCompletionF()> ApiDeferredHandler;

	using namespace dcpp;

	using json = nlohmann::json;

	using ArgumentException = webserver::JsonException;


	class ApiRequest;

	class ContextMenuItem;
	typedef std::shared_ptr<ContextMenuItem> ContextMenuItemPtr;
	typedef std::vector<ContextMenuItemPtr> ContextMenuItemList;

	class Extension;
	typedef shared_ptr<Extension> ExtensionPtr;
	typedef vector<ExtensionPtr> ExtensionList;

	class Session;
	typedef std::shared_ptr<Session> SessionPtr;
	typedef std::vector<SessionPtr> SessionList;
	typedef uint32_t LocalSessionId;

	typedef map<string, json> SettingValueMap;

	class Timer;
	typedef shared_ptr<Timer> TimerPtr;

	class WebSocket;
	typedef std::shared_ptr<WebSocket> WebSocketPtr;

	class WebServerManager;

	typedef std::function<void()> Callback;
	typedef std::function<void(const string&)> MessageCallback;
}

#endif // !defined(DCPLUSPLUS_WEBSERVER_STDINC_H)
