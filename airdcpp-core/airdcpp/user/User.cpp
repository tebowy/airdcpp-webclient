/*
 * Copyright (C) 2001-2024 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#include "stdinc.h"
#include <airdcpp/user/User.h>

#include <airdcpp/hub/Client.h>
#include <airdcpp/util/text/StringTokenizer.h>
#include <airdcpp/favorites/FavoriteUser.h>
#include <airdcpp/core/geo/GeoManager.h>

#include <airdcpp/hub/ClientManager.h>
#include <airdcpp/core/localization/ResourceManager.h>
#include <airdcpp/favorites/FavoriteManager.h>

#include <airdcpp/events/LogManager.h>

namespace dcpp {

SharedMutex Identity::cs;

const string OnlineUser::CLIENT_PROTOCOL("ADC/1.0");
const string OnlineUser::SECURE_CLIENT_PROTOCOL_TEST("ADCS/0.10");
const string OnlineUser::ADCS_FEATURE("ADC0");
const string OnlineUser::TCP4_FEATURE("TCP4");
const string OnlineUser::TCP6_FEATURE("TCP6");
const string OnlineUser::UDP4_FEATURE("UDP4");
const string OnlineUser::UDP6_FEATURE("UDP6");
const string OnlineUser::NAT0_FEATURE("NAT0");
const string OnlineUser::SEGA_FEATURE("SEGA");
const string OnlineUser::SUD1_FEATURE("SUD1");
const string OnlineUser::ASCH_FEATURE("ASCH");
const string OnlineUser::CCPM_FEATURE("CCPM");

OnlineUser::OnlineUser(const UserPtr& ptr, const ClientPtr& client_, SID sid_) : identity(ptr, sid_), client(client_) {
}

OnlineUser::~OnlineUser() noexcept {
	// dcdebug("OnlineUser %s was destroyed (%s)\n", identity.getNick().c_str(), getHubUrl().c_str());
}

bool Identity::hasActiveTcpConnectivity(const ClientPtr& c) const noexcept {
	if (user->isSet(User::NMDC) || isMe()) {
		return isTcp4Active(c) || isTcp6Active();
	}

	return isActiveMode(adcTcpConnectMode);
}

bool Identity::isTcp4Active(const ClientPtr& c) const noexcept {
	if (user->isSet(User::NMDC)) {
		// NMDC
		return !user->isSet(User::PASSIVE);
	} else {
		// NMDC flag is not set for our own user (and neither the global User::PASSIVE flag can be used here)
		if (c && isMe()) {
			return c->isActiveV4();
		}

		// ADC
		return !getIp4().empty() && hasSupport(OnlineUser::TCP4_FEATURE);
	}
}

bool Identity::isTcp6Active() const noexcept {
	return !getIp6().empty() && hasSupport(OnlineUser::TCP6_FEATURE);
}

bool Identity::isUdp4Active() const noexcept {
	if(getIp4().empty() || getUdp4Port().empty())
		return false;
	return user->isSet(User::NMDC) ? !user->isSet(User::PASSIVE) : hasSupport(OnlineUser::UDP4_FEATURE);
}

bool Identity::isUdp6Active() const noexcept {
	if(getIp6().empty() || getUdp6Port().empty())
		return false;
	return user->isSet(User::NMDC) ? false : hasSupport(OnlineUser::UDP6_FEATURE);
}

string Identity::getUdpPort() const noexcept {
	if(getIp6().empty() || getUdp6Port().empty()) {
		return getUdp4Port();
	}

	return getUdp6Port();
}

string Identity::getTcpConnectIp() const noexcept {
	if (user->isNMDC()) {
		return getIp4();
	}

	return !allowV6Connections(adcTcpConnectMode) ? getIp4() : getIp6();
}

string Identity::getUdpIp() const noexcept {
	if (user->isNMDC()) {
		return getIp4();
	}

	return !allowV6Connections(adcUdpConnectMode) ? getIp4() : getIp6();
}

string Identity::getConnectionString() const noexcept {
	if (user->isNMDC()) {
		return getNmdcConnection();
	} else {
		return Util::toString(getAdcConnectionSpeed(false));
	}
}

int64_t Identity::getAdcConnectionSpeed(bool download) const noexcept {
	return Util::toInt64(download ? get("DS") : get("US"));
}

uint8_t Identity::getSlots() const noexcept {
	return static_cast<uint8_t>(Util::toInt(get("SL")));
}

void Identity::getParams(ParamMap& sm, const string& prefix, bool compatibility) const noexcept {
	{
		RLock l(cs);
		for (const auto& [name, value] : info) {
			sm[prefix + string((char*)(&name), 2)] = value;
		}
	}

	if(user) {
		sm[prefix + "NI"] = getNick();
		sm[prefix + "SID"] = getSIDString();
		sm[prefix + "CID"] = user->getCID().toBase32();
		sm[prefix + "TAG"] = getTag();
		sm[prefix + "CO"] = getNmdcConnection();
		sm[prefix + "DS"] = getDownloadSpeed();
		sm[prefix + "SSshort"] = Util::formatBytes(get("SS"));

		if(compatibility) {
			if(prefix == "my") {
				sm["mynick"] = getNick();
				sm["mycid"] = user->getCID().toBase32();
			} else {
				sm["nick"] = getNick();
				sm["cid"] = user->getCID().toBase32();
				sm["ip"] = get("I4");
				sm["tag"] = getTag();
				sm["description"] = get("DE");
				sm["email"] = get("EM");
				sm["share"] = get("SS");
				sm["shareshort"] = Util::formatBytes(get("SS"));
				sm["realshareformat"] = Util::formatBytes(get("RS"));
			}
		}
	}
}

bool Identity::isClientType(ClientType ct) const noexcept {
	int type = Util::toInt(get("CT"));
	return (type & ct) == ct;
}

string Identity::getTag() const noexcept {
	if(!get("TA").empty())
		return get("TA");
	if(get("VE").empty() || get("HN").empty() || get("HR").empty() || get("HO").empty() || get("SL").empty())
		return Util::emptyString;

	return "<" + getApplication() + ",M:" + getV4ModeString() + getV6ModeString() + 
		",H:" + get("HN") + "/" + get("HR") + "/" + get("HO") + ",S:" + get("SL") + ">";
}

string Identity::getV4ModeString() const noexcept {
	if (!getIp4().empty())
		return isTcp4Active() ? "A" : "P";
	else
		return "-";
}

string Identity::getV6ModeString() const noexcept {
	if (!getIp6().empty())
		return isTcp6Active() ? "A" : "P";
	else
		return "-";
}

Identity::Identity() : sid(0) { }

Identity::Identity(const UserPtr& ptr, dcpp::SID aSID) : user(ptr), sid(aSID) { }

Identity::Identity(const Identity& rhs) : Flags(), sid(0) { 
	*this = rhs;  // Use operator= since we have to lock before reading...
}

Identity& Identity::operator = (const Identity& rhs) {
	WLock l(cs);
	*static_cast<Flags*>(this) = rhs;
	user = rhs.user;
	sid = rhs.sid;
	info = rhs.info;
	supports = rhs.supports;
	adcTcpConnectMode = rhs.adcTcpConnectMode;
	return *this;
}

string Identity::getApplication() const noexcept {
	auto application = get("AP");
	auto version = get("VE");

	if(version.empty()) {
		return application;
	}

	if(application.empty()) {
		// AP is an extension, so we can't guarantee that the other party supports it, so default to VE.
		return version;
	}

	return application + ' ' + version;
}
string Identity::getCountry() const noexcept {
	bool v6 = !getIp6().empty();
	return GeoManager::getInstance()->getCountry(v6 ? getIp6() : getIp4());
}

string Identity::get(const char* name) const noexcept {
	RLock l(cs);
	auto i = info.find(*(short*)name);
	return i == info.end() ? Util::emptyString : i->second;
}

bool Identity::isSet(const char* name) const noexcept {
	RLock l(cs);
	auto i = info.find(*(short*)name);
	return i != info.end();
}


void Identity::set(const char* name, const string& val) noexcept {
	WLock l(cs);
	if (val.empty()) {
		info.erase(*(short*)name);
	} else {
		info[*(short*)name] = val;
	}
}

StringList Identity::getSupports() const noexcept {
	StringList ret;
	for (const auto& s : supports) {
		ret.push_back(AdcCommand::fromFourCC(s));
	}

	return ret;
}

void Identity::setSupports(const string& aSupports) noexcept {
	auto tokens = StringTokenizer<string>(aSupports, ',');

	WLock l(cs);
	supports.clear();
	for (const auto& support : tokens.getTokens()) {
		supports.push_back(AdcCommand::toFourCC(support.c_str()));
	}
}

bool Identity::hasSupport(const string& name) const noexcept {
	auto support = AdcCommand::toFourCC(name.c_str());

	RLock l(cs);
	for (const auto& s: supports) {
		if (s == support)
			return true;
	}

	return false;
}

bool Identity::isMe() const noexcept {
	return ClientManager::getInstance()->getMe() == user;
}

std::map<string, string> Identity::getInfo() const noexcept {
	std::map<string, string> ret;

	RLock l(cs);
	for(const auto& [name, value] : info) {
		ret[string((char*)(&name), 2)] = value;
	}

	return ret;
}

int Identity::getTotalHubCount() const noexcept {
	return Util::toInt(get("HN")) + Util::toInt(get("HR")) + Util::toInt(get("HO"));
}

Identity::Mode Identity::detectConnectMode(const Identity& aMe, const Identity& aOther, const ActiveMode& aActiveMe, const ActiveMode& aActiveOther, bool aNatTravelsal, const Client* aClient) noexcept {
	if (aMe.getUser() == aOther.getUser()) {
		return MODE_ME;
	}

	auto mode = MODE_NOCONNECT_IP;

	if (!aMe.getIp6().empty() && !aOther.getIp6().empty()) {
		// IPv6? active / NAT-T
		if (aActiveOther.v6) {
			mode = MODE_ACTIVE_V6;
		} else if (aActiveMe.v6 || aNatTravelsal) {
			mode = MODE_PASSIVE_V6;
		}
	}

	if (!aMe.getIp4().empty() && !aOther.getIp4().empty()) {
		if (aActiveOther.v4) {
			mode = mode == MODE_ACTIVE_V6 ? MODE_ACTIVE_DUAL : MODE_ACTIVE_V4;
		} else if (mode == MODE_NOCONNECT_IP && (aActiveMe.v4 || aNatTravelsal)) { //passive v4 isn't any better than passive v6
			mode = MODE_PASSIVE_V4;
		}
	}

	if (mode == MODE_NOCONNECT_IP) {
		// The hub doesn't support hybrid connectivity or we weren't able to authenticate the secondary protocol? We are passive via that protocol in that case
		if (aActiveOther.v4 && aClient->get(HubSettings::Connection) != SettingsManager::INCOMING_DISABLED) {
			mode = MODE_ACTIVE_V4;
		} else if (aActiveOther.v6 && aClient->get(HubSettings::Connection6) != SettingsManager::INCOMING_DISABLED) {
			mode = MODE_ACTIVE_V6;
		} else if (!aActiveMe.v4 && !aActiveMe.v6) {
			// Other user is passive with no NAT-T (or the hub is hiding all IP addresses)
			if (!aNatTravelsal && !aClient->isActive()) {
				mode = MODE_NOCONNECT_PASSIVE;
			}
		} else {
			// Could this user still support the same protocol? Can't know for sure
			mode = !aMe.getIp6().empty() ? MODE_PASSIVE_V6_UNKNOWN : MODE_PASSIVE_V4_UNKNOWN;
		}
	}

	return mode;
}

Identity::Mode Identity::detectConnectModeTcp(const Identity& aMe, const Identity& aOther, const Client* aClient) noexcept {
	return detectConnectMode(
		aMe, aOther, 
		{ aMe.isTcp4Active(), aMe.isTcp6Active() },
		{ aOther.isTcp4Active(), aOther.isTcp6Active() },
		aOther.hasSupport(OnlineUser::NAT0_FEATURE), aClient
	);
}

Identity::Mode Identity::detectConnectModeUdp(const Identity& aMe, const Identity& aOther, const Client* aClient) noexcept {
	return detectConnectMode(
		aMe, aOther, 
		{ aMe.isUdp4Active(), aMe.isUdp6Active() },
		{ aOther.isUdp4Active(), aOther.isUdp6Active() },
		false, aClient
	);
}

Identity::Mode Identity::getTcpConnectMode() const noexcept {
	if (isMe()) {
		return Mode::MODE_ME;
	}

	if (user->isNMDC()) {
		return isTcp4Active() ? Mode::MODE_ACTIVE_V4 : Mode::MODE_PASSIVE_V4;
	}

	return adcTcpConnectMode;
}

bool Identity::isUdpActive() const noexcept {
	if (user->isNMDC()) {
		return isUdp4Active();
	}

	return isActiveMode(adcUdpConnectMode);
}

bool Identity::updateAdcConnectModes(const Identity& me, const Client* aClient) noexcept {
	bool updated = false;

	{
		auto newModeTcp = detectConnectModeTcp(me, *this, aClient);
		if (adcTcpConnectMode != newModeTcp) {
			adcTcpConnectMode = newModeTcp;
			updated = true;
		}
	}

	{
		auto newModeUdp = detectConnectModeUdp(me, *this, aClient);
		if (adcUdpConnectMode != newModeUdp) {
			adcUdpConnectMode = newModeUdp;
			updated = true;
		}
	}

	return updated;
}

bool Identity::allowConnections(Mode aConnectMode) noexcept {
	return allowV4Connections(aConnectMode) || allowV6Connections(aConnectMode);
}

bool Identity::allowV4Connections(Mode aConnectMode) noexcept {
	return aConnectMode == MODE_PASSIVE_V4 || aConnectMode == MODE_ACTIVE_V4 || aConnectMode == MODE_PASSIVE_V4_UNKNOWN || aConnectMode == MODE_ACTIVE_DUAL;
}

bool Identity::allowV6Connections(Mode aConnectMode) noexcept {
	return aConnectMode == MODE_PASSIVE_V6 || aConnectMode == MODE_ACTIVE_V6 || aConnectMode == MODE_PASSIVE_V6_UNKNOWN || aConnectMode == MODE_ACTIVE_DUAL;
}

bool Identity::isActiveMode(Mode aConnectMode) noexcept {
	return aConnectMode == MODE_ACTIVE_V6 || aConnectMode == MODE_ACTIVE_V4 || aConnectMode == MODE_ACTIVE_DUAL;
}

bool Identity::isConnectModeParam(const string_view& aParam) noexcept {
	return aParam.starts_with("SU") || aParam.starts_with("I4") || aParam.starts_with("I6");
}

const string& OnlineUser::getHubUrl() const noexcept {
	return getClient()->getHubUrl();
}

bool OnlineUser::NickSort::operator()(const OnlineUserPtr& left, const OnlineUserPtr& right) const noexcept {
	return compare(left->getIdentity().getNick(), right->getIdentity().getNick()) < 0;
}

string OnlineUser::HubName::operator()(const OnlineUserPtr& u) const noexcept {
	return u->getClient()->getHubName(); 
}


User::User(const CID& aCID) : cid(aCID) {}
User::~User() {
	// dcdebug("User %s was destroyed\n", cid.toBase32().c_str());
}

void User::addQueued(int64_t aBytes) noexcept {
	queued += aBytes;
}

void User::removeQueued(int64_t aBytes) noexcept {
	queued -= aBytes;
	dcassert(queued >= 0);
}

string OnlineUser::getLogPath() const noexcept {
	ParamMap params;
	params["userNI"] = [this] { return getIdentity().getNick(); };
	params["hubNI"] = [this] { return getClient()->getHubName(); };
	params["myNI"] = [this] { return getClient()->getMyNick(); };
	params["userCID"] = [this] { return getUser()->getCID().toBase32(); };
	params["hubURL"] = [this] { return getClient()->getHubUrl(); };

	return LogManager::getInstance()->getPath(getUser(), params);
}

bool OnlineUser::supportsCCPM() const noexcept {
	return getIdentity().hasSupport(OnlineUser::CCPM_FEATURE);
}

} // namespace dcpp