/*
 * Copyright (C) 2001-2012 Jacek Sieka, arnetheduck on gmail point com
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

#ifndef DCPLUSPLUS_DCPP_SETTINGS_MANAGER_H
#define DCPLUSPLUS_DCPP_SETTINGS_MANAGER_H

#include "Util.h"
#include "Speaker.h"
#include "Singleton.h"
#include "HubSettings.h"

namespace dcpp {

class SimpleXML;
struct HubSettings;

class SettingsManagerListener {
public:
	virtual ~SettingsManagerListener() { }
	template<int I>	struct X { enum { TYPE = I };  };

	typedef X<0> Load;
	typedef X<1> Save;
	typedef X<2> ReloadPages; // if a settingspage modifies properties in another page, fire this to update the pages.
 
	virtual void on(Load, SimpleXML&) noexcept { }
	virtual void on(Save, SimpleXML&) noexcept { }
	virtual void on(ReloadPages, int) { }
};

class SettingsManager : public Singleton<SettingsManager>, public Speaker<SettingsManagerListener>
{
public:
	static bool lanMode;
	static StringList connectionSpeeds;

	enum StrSetting { STR_FIRST,
		NICK = STR_FIRST, UPLOAD_SPEED, DESCRIPTION, DOWNLOAD_DIRECTORY, EMAIL, EXTERNAL_IP, EXTERNAL_IP6,
		TEXT_FONT, TRANSFERVIEW_ORDER, TRANSFERVIEW_WIDTHS, HUBFRAME_ORDER, HUBFRAME_WIDTHS,
		LANGUAGE_FILE, SEARCHFRAME_ORDER, SEARCHFRAME_WIDTHS, FAVORITESFRAME_ORDER, FAVORITESFRAME_WIDTHS, 
		HUBLIST_SERVERS, QUEUEFRAME_ORDER, QUEUEFRAME_WIDTHS, PUBLICHUBSFRAME_ORDER, PUBLICHUBSFRAME_WIDTHS, 
		USERSFRAME_ORDER, USERSFRAME_WIDTHS, HTTP_PROXY, LOG_DIRECTORY, LOG_FORMAT_POST_DOWNLOAD, 
		LOG_FORMAT_POST_UPLOAD, LOG_FORMAT_MAIN_CHAT, LOG_FORMAT_PRIVATE_CHAT, FINISHED_ORDER, FINISHED_WIDTHS, 
		TEMP_DOWNLOAD_DIRECTORY, BIND_ADDRESS, BIND_ADDRESS6, SOCKS_SERVER, SOCKS_USER, SOCKS_PASSWORD, CONFIG_VERSION,
		DEFAULT_AWAY_MESSAGE, TIME_STAMPS_FORMAT, ADLSEARCHFRAME_ORDER, ADLSEARCHFRAME_WIDTHS, 
		FINISHED_UL_WIDTHS, FINISHED_UL_ORDER, PRIVATE_ID, SPYFRAME_WIDTHS, SPYFRAME_ORDER, 
		BEEPFILE, BEGINFILE, FINISHFILE, SOURCEFILE, UPLOADFILE, CHATNAMEFILE, WINAMP_FORMAT,
		KICK_MSG_RECENT_01, KICK_MSG_RECENT_02, KICK_MSG_RECENT_03, KICK_MSG_RECENT_04, KICK_MSG_RECENT_05, 
		KICK_MSG_RECENT_06, KICK_MSG_RECENT_07, KICK_MSG_RECENT_08, KICK_MSG_RECENT_09, KICK_MSG_RECENT_10, 
		KICK_MSG_RECENT_11, KICK_MSG_RECENT_12, KICK_MSG_RECENT_13, KICK_MSG_RECENT_14, KICK_MSG_RECENT_15, 
		KICK_MSG_RECENT_16, KICK_MSG_RECENT_17, KICK_MSG_RECENT_18, KICK_MSG_RECENT_19, KICK_MSG_RECENT_20,
		TOOLBAR, TOOLBARIMAGE, TOOLBARHOTIMAGE, UPLOADQUEUEFRAME_ORDER, UPLOADQUEUEFRAME_WIDTHS,
		SOUND_TTH, SOUND_EXC, SOUND_HUBCON,  SOUND_HUBDISCON, SOUND_FAVUSER, SOUND_TYPING_NOTIFY,
 		LOG_FILE_MAIN_CHAT, 
		LOG_FILE_PRIVATE_CHAT, LOG_FILE_STATUS, LOG_FILE_UPLOAD, LOG_FILE_DOWNLOAD, LOG_FILE_SYSTEM,
		LOG_FORMAT_SYSTEM, LOG_FORMAT_STATUS, DIRECTORYLISTINGFRAME_ORDER, DIRECTORYLISTINGFRAME_WIDTHS,
		MAINFRAME_VISIBLE, SEARCHFRAME_VISIBLE, QUEUEFRAME_VISIBLE, HUBFRAME_VISIBLE, UPLOADQUEUEFRAME_VISIBLE, 
		EMOTICONS_FILE, TLS_PRIVATE_KEY_FILE, TLS_CERTIFICATE_FILE, TLS_TRUSTED_CERTIFICATES_PATH,
		FINISHED_VISIBLE, FINISHED_UL_VISIBLE, DIRECTORYLISTINGFRAME_VISIBLE,
		RECENTFRAME_ORDER, RECENTFRAME_WIDTHS, MAPPER, COUNTRY_FORMAT,
		
		//AirDC
		BACKGROUND_IMAGE, MPLAYERC_FORMAT, ITUNES_FORMAT, WMP_FORMAT, SPOTIFY_FORMAT, WINAMP_PATH,
		ANTIVIR_PATH,
		SKIPLIST_SHARE, FREE_SLOTS_EXTENSIONS,
		POPUP_FONT, POPUP_TITLE_FONT, POPUPFILE, SKIPLIST_DOWNLOAD, HIGH_PRIO_FILES,
		MEDIATOOLBAR, PASSWORD, SKIPLIST_SEARCH, SKIP_MSG_01, SKIP_MSG_02, SKIP_MSG_03, DOWNLOAD_SPEED, HIGHLIGHT_LIST, ICON_PATH,
		AUTOSEARCHFRAME_ORDER, AUTOSEARCHFRAME_WIDTHS, TOOLBAR_POS, TB_PROGRESS_FONT, LAST_SEARCH_FILETYPE, LAST_SEARCH_DISABLED_HUBS, LAST_AS_FILETYPE,
		STR_LAST };

	enum IntSetting { INT_FIRST = STR_LAST + 1,
		INCOMING_CONNECTIONS = INT_FIRST, TCP_PORT, SLOTS,
		BACKGROUND_COLOR, TEXT_COLOR,
		BUFFER_SIZE, DOWNLOAD_SLOTS, MAX_DOWNLOAD_SPEED,
		MIN_UPLOAD_SPEED, MAIN_WINDOW_STATE,
		MAIN_WINDOW_SIZE_X, MAIN_WINDOW_SIZE_Y, MAIN_WINDOW_POS_X, MAIN_WINDOW_POS_Y,
		SOCKS_PORT, MAX_TAB_ROWS,
		MAX_COMPRESSION, DOWNLOAD_BAR_COLOR, UPLOAD_BAR_COLOR,
		SET_MINISLOT_SIZE, SHUTDOWN_TIMEOUT,
		EXTRA_SLOTS, EXTRA_PARTIAL_SLOTS,
		TEXT_GENERAL_BACK_COLOR, TEXT_GENERAL_FORE_COLOR,
		TEXT_MYOWN_BACK_COLOR, TEXT_MYOWN_FORE_COLOR,
		TEXT_PRIVATE_BACK_COLOR, TEXT_PRIVATE_FORE_COLOR,
		TEXT_SYSTEM_BACK_COLOR, TEXT_SYSTEM_FORE_COLOR,
		TEXT_SERVER_BACK_COLOR, TEXT_SERVER_FORE_COLOR,
		TEXT_TIMESTAMP_BACK_COLOR, TEXT_TIMESTAMP_FORE_COLOR,
		TEXT_MYNICK_BACK_COLOR, TEXT_MYNICK_FORE_COLOR,
		TEXT_FAV_BACK_COLOR, TEXT_FAV_FORE_COLOR,
		TEXT_OP_BACK_COLOR, TEXT_OP_FORE_COLOR,
		TEXT_URL_BACK_COLOR, TEXT_URL_FORE_COLOR, 
		PROGRESS_3DDEPTH,
		HUB_SLOTS, PROGRESS_TEXT_COLOR_DOWN, PROGRESS_TEXT_COLOR_UP, EXTRA_DOWNLOAD_SLOTS, ERROR_COLOR, TRANSFER_SPLIT_SIZE, 
		DISCONNECT_SPEED, DISCONNECT_FILE_SPEED, DISCONNECT_TIME, REMOVE_SPEED, MENUBAR_LEFT_COLOR, MENUBAR_RIGHT_COLOR, 
		DISCONNECT_FILESIZE, NUMBER_OF_SEGMENTS, MAX_HASH_SPEED, SHOW_LAST_LINES_LOG, SEARCH_ALTERNATE_COLOUR, SEARCH_TIME, DONT_BEGIN_SEGMENT_SPEED,
		MAGNET_ACTION, POPUP_TYPE, SHUTDOWN_ACTION, MINIMUM_SEARCH_INTERVAL, MAX_AUTO_MATCH_SOURCES, RESERVED_SLOT_COLOR, IGNORED_COLOR, FAVORITE_COLOR, NORMAL_COLOUR,
		PASIVE_COLOR, OP_COLOR, PROGRESS_BACK_COLOR, PROGRESS_COMPRESS_COLOR, PROGRESS_SEGMENT_COLOR, UDP_PORT,
		USERLIST_DBLCLICK, TRANSFERLIST_DBLCLICK, CHAT_DBLCLICK, SEARCH_HISTORY,OUTGOING_CONNECTIONS, SOCKET_IN_BUFFER, SOCKET_OUT_BUFFER,
		COLOR_DOWNLOADED, COLOR_RUNNING, COLOR_DONE, AUTO_REFRESH_TIME, AUTO_SEARCH_LIMIT, MAX_COMMAND_LENGTH, TLS_PORT, DOWNCONN_PER_SEC,
		PRIO_HIGHEST_SIZE, PRIO_HIGH_SIZE, PRIO_NORMAL_SIZE, PRIO_LOW_SIZE, 

		BANDWIDTH_LIMIT_START, BANDWIDTH_LIMIT_END, MAX_DOWNLOAD_SPEED_ALTERNATE,
		MAX_UPLOAD_SPEED_ALTERNATE, MAX_DOWNLOAD_SPEED_MAIN, MAX_UPLOAD_SPEED_MAIN,
		SLOTS_ALTERNATE_LIMITING, SLOTS_PRIMARY,
		
		//AirDC
		TAB_ACTIVE_BG, TAB_ACTIVE_TEXT, TAB_ACTIVE_BORDER, TAB_INACTIVE_BG, TAB_INACTIVE_BG_DISCONNECTED, TAB_INACTIVE_TEXT, TAB_INACTIVE_BORDER, TAB_INACTIVE_BG_NOTIFY, TAB_DIRTY_BLEND, TAB_SIZE, MEDIA_PLAYER,
		FAV_DL_SPEED, OPEN_FIRST_X_HUBS, POPUP_TIME, MAX_MSG_LENGTH, POPUP_BACKCOLOR, POPUP_TEXTCOLOR, POPUP_TITLE_TEXTCOLOR, AUTOSEARCH_EVERY, AUTOSEARCH_RECHECK_TIME, TB_IMAGE_SIZE, TB_IMAGE_SIZE_HOT, 
		DUPE_COLOR, TEXT_DUPE_BACK_COLOR, MIN_SEGMENT_SIZE, AUTO_SLOTS, MAX_RESIZE_LINES, INCOMING_REFRESH_TIME, TEXT_NORM_BACK_COLOR, TEXT_NORM_FORE_COLOR, SETTINGS_PROFILE, LOG_LINES,
		MAX_FILE_SIZE_SHARED, FAV_TOP, FAV_BOTTOM, FAV_LEFT, FAV_RIGHT, SYSLOG_TOP, SYSLOG_BOTTOM, SYSLOG_LEFT, SYSLOG_RIGHT, NOTEPAD_TOP, NOTEPAD_BOTTOM,
		NOTEPAD_LEFT, NOTEPAD_RIGHT, QUEUE_TOP, QUEUE_BOTTOM, QUEUE_LEFT, QUEUE_RIGHT, SEARCH_TOP, SEARCH_BOTTOM, SEARCH_LEFT, SEARCH_RIGHT, USERS_TOP, USERS_BOTTOM,
		USERS_LEFT, USERS_RIGHT, FINISHED_TOP, FINISHED_BOTTOM, FINISHED_LEFT, FINISHED_RIGHT, TEXT_TOP, TEXT_BOTTOM, TEXT_LEFT, TEXT_RIGHT, DIRLIST_TOP, DIRLIST_BOTTOM,
		DIRLIST_LEFT, DIRLIST_RIGHT, STATS_TOP, STATS_BOTTOM, STATS_LEFT, STATS_RIGHT, MAX_MCN_DOWNLOADS, MAX_MCN_UPLOADS, LIST_HL_BG_COLOR, LIST_HL_COLOR, QUEUE_COLOR, TEXT_QUEUE_BACK_COLOR,
		RECENT_BUNDLE_HOURS, DISCONNECT_MIN_SOURCES, AUTOPRIO_TYPE, AUTOPRIO_INTERVAL, AUTOSEARCH_EXPIRE_DAYS, DL_AUTOSELECT_METHOD, WTB_IMAGE_SIZE, TB_PROGRESS_TEXT_COLOR, TLS_MODE, UPDATE_METHOD, 
		QUEUE_SPLITTER_POS, FULL_LIST_DL_LIMIT, AS_DELAY_HOURS, LAST_LIST_PROFILE, MAX_HASHING_THREADS, HASHERS_PER_VOLUME, SKIP_SUBTRACT, BLOOM_MODE, 
		INT_LAST };

	enum BoolSetting { BOOL_FIRST = INT_LAST + 1,
		ADD_FINISHED_INSTANTLY = BOOL_FIRST, ADLS_BREAK_ON_FIRST,
		ALLOW_UNTRUSTED_CLIENTS, ALLOW_UNTRUSTED_HUBS, AUTO_AWAY,
		AUTO_DETECT_CONNECTION, AUTO_FOLLOW, AUTO_KICK, AUTO_KICK_NO_FAVS, AUTO_SEARCH,
		AWAY_COMP_LOCK, BOLD_FINISHED_DOWNLOADS, BOLD_FINISHED_UPLOADS, BOLD_HUB, BOLD_PM,
		BOLD_QUEUE, BOLD_SEARCH, BOLD_SYSTEM_LOG, CLEAR_SEARCH,
		COMPRESS_TRANSFERS, CONFIRM_ADLS_REMOVAL, CONFIRM_EXIT,
		CONFIRM_HUB_REMOVAL, CONFIRM_USER_REMOVAL, CORAL,
		DONT_DL_ALREADY_QUEUED, DONT_DL_ALREADY_SHARED, FAV_SHOW_JOINS, FILTER_MESSAGES,
		FOLLOW_LINKS, GET_USER_COUNTRY, GET_USER_INFO, HUB_USER_COMMANDS,
		IGNORE_BOT_PMS, IGNORE_HUB_PMS, JOIN_OPEN_NEW_WINDOW, KEEP_FINISHED_FILES, KEEP_LISTS,
		LOG_DOWNLOADS, LOG_FILELIST_TRANSFERS, LOG_FINISHED_DOWNLOADS, LOG_MAIN_CHAT,
		LOG_PRIVATE_CHAT, LOG_STATUS_MESSAGES, LOG_SYSTEM, LOG_UPLOADS, MAGNET_ASK,
		MAGNET_REGISTER, MINIMIZE_TRAY, NO_AWAYMSG_TO_BOTS, NO_IP_OVERRIDE,
		POPUP_BOT_PMS, POPUP_HUB_PMS, POPUP_PMS, POPUNDER_FILELIST, POPUNDER_PM,
		PRIO_LOWEST, PROMPT_PASSWORD, QUEUEFRAME_SHOW_TREE, SEARCH_FILTER_SHARED,
		SEARCH_ONLY_FREE_SLOTS, SEND_UNKNOWN_COMMANDS,
		SHARE_HIDDEN, SHOW_JOINS, SHOW_MENU_BAR, SHOW_STATUSBAR, SHOW_TOOLBAR,
		SHOW_TRANSFERVIEW, SKIP_ZERO_BYTE, SOCKS_RESOLVE, SORT_FAVUSERS_FIRST,
		STATUS_IN_CHAT, TIME_DEPENDENT_THROTTLE, TIME_STAMPS,
		TOGGLE_ACTIVE_WINDOW, URL_HANDLER, USE_CTRL_FOR_LINE_HISTORY, USE_SYSTEM_ICONS,
		USERS_FILTER_FAVORITE, USERS_FILTER_ONLINE, USERS_FILTER_QUEUE, USERS_FILTER_WAITING,
		AWAY_TIMESTAMP,

		PRIVATE_MESSAGE_BEEP, PRIVATE_MESSAGE_BEEP_OPEN, SHOW_PROGRESS_BARS, MDI_MAXIMIZED, SEARCH_PASSIVE, REMOVE_FORBIDDEN, SHOW_INFOTIPS, MINIMIZE_ON_STARTUP, CONFIRM_QUEUE_REMOVAL, EXPAND_QUEUE, 
		FILTER_ENTER, SHOW_SHELL_MENU, SPY_FRAME_IGNORE_TTH_SEARCHES, OPEN_WAITING_USERS, BOLD_WAITING_USERS, GROUP_SEARCH_RESULTS, TABS_ON_TOP, OPEN_PUBLIC, OPEN_FAVORITE_HUBS, OPEN_FAVORITE_USERS, OPEN_QUEUE, OPEN_FINISHED_DOWNLOADS, 
		OPEN_FINISHED_UPLOADS, OPEN_SEARCH_SPY, OPEN_NETWORK_STATISTICS, OPEN_NOTEPAD, SUPPRESS_MAIN_CHAT, PROGRESSBAR_ODC_STYLE, MULTI_CHUNK, POPUP_AWAY, POPUP_MINIMIZED, AWAY, POPUP_HUB_CONNECTED, POPUP_HUB_DISCONNECTED, POPUP_FAVORITE_CONNECTED, 
		POPUP_DOWNLOAD_START,POPUP_DOWNLOAD_FAILED, POPUP_DOWNLOAD_FINISHED, POPUP_UPLOAD_FINISHED, POPUP_PM, POPUP_NEW_PM, UPLOADQUEUEFRAME_SHOW_TREE, SEGMENTS_MANUAL, SOUNDS_DISABLED, REPORT_ALTERNATES,
		AUTO_PRIORITY_DEFAULT, USE_OLD_SHARING_UI, FREE_SLOTS_DEFAULT, 

		TEXT_GENERAL_BOLD, TEXT_GENERAL_ITALIC, TEXT_MYOWN_BOLD, TEXT_MYOWN_ITALIC, TEXT_PRIVATE_BOLD, TEXT_PRIVATE_ITALIC, TEXT_SYSTEM_BOLD, TEXT_SYSTEM_ITALIC, TEXT_SERVER_BOLD, TEXT_SERVER_ITALIC, TEXT_TIMESTAMP_BOLD, TEXT_TIMESTAMP_ITALIC, 
		TEXT_MYNICK_BOLD, TEXT_MYNICK_ITALIC, TEXT_FAV_BOLD, TEXT_FAV_ITALIC, TEXT_OP_BOLD, TEXT_OP_ITALIC, TEXT_URL_BOLD, TEXT_URL_ITALIC, PROGRESS_OVERRIDE_COLORS, PROGRESS_OVERRIDE_COLORS2, MENUBAR_TWO_COLORS, MENUBAR_BUMPED, DONT_BEGIN_SEGMENT,

		//AirDC
		USE_ADLS_OWN, AUTO_DETECTION_USE_LIMITED, LOG_SCHEDULED_REFRESHES, AUTO_COMPLETE_BUNDLES, SEARCH_SAVE_HUBS_STATE, CONFIRM_HUB_CLOSING, CONFIRM_AS_REMOVAL, ENABLE_SUDP, NMDC_MAGNET_WARN,
		UPDATE_IP_HOURLY, POPUNDER_TEXT, LOCK_TB, CLEAR_DIR_HISTORY, POPUNDER_PARTIAL_LIST, SHOW_TBSTATUS, HORIZONTAL_QUEUE, USE_SLOW_DISCONNECTING_DEFAULT, PRIO_LIST_HIGHEST, 
		USE_FTP_LOGGER, QI_AUTOPRIO, SHOW_SHARED_DIRS_FAV, REPORT_ADDED_SOURCES, EXPAND_BUNDLES, OVERLAP_SLOW_SOURCES, FORMAT_DIR_REMOTE_TIME, TEXT_QUEUE_BOLD, TEXT_QUEUE_ITALIC, UNDERLINE_QUEUE, LOG_HASHING,
		USE_PARTIAL_SHARING, POPUP_BUNDLE_DLS, POPUP_BUNDLE_ULS, LIST_HL_BOLD, LIST_HL_ITALIC, REPORT_SKIPLIST, SCAN_DL_BUNDLES, MCN_AUTODETECT, DL_AUTODETECT, UL_AUTODETECT, CHECK_USE_SKIPLIST, CHECK_IGNORE_ZERO_BYTE, 
		TEXT_DUPE_BOLD, TEXT_DUPE_ITALIC, UNDERLINE_LINKS, UNDERLINE_DUPES, DUPES_IN_FILELIST, DUPES_IN_CHAT, NO_ZERO_BYTE, CHECK_EMPTY_DIRS, CHECK_EMPTY_RELEASES, CHECK_MISSING, CHECK_SFV, 
		CHECK_NFO, CHECK_MP3_DIR, CHECK_EXTRA_SFV_NFO, CHECK_EXTRA_FILES, CHECK_DUPES, SORT_DIRS, DECREASE_RAM, WIZARD_RUN_NEW, FORMAT_RELEASE, TEXT_NORM_BOLD, TEXT_NORM_ITALIC, SYSTEM_SHOW_UPLOADS, SYSTEM_SHOW_DOWNLOADS, 
		USE_ADLS, DUPE_SEARCH, PASSWD_PROTECT, PASSWD_PROTECT_TRAY, DISALLOW_CONNECTION_TO_PASSED_HUBS, BOLD_HUB_TABS_ON_KICK, SEARCH_SKIPLIST,
		AUTO_ADD_SOURCE, ALLOW_NAT_TRAVERSAL, USE_EXPLORER_THEME, TESTWRITE, OPEN_SYSTEM_LOG, OPEN_LOGS_INTERNAL, UC_SUBMENU, SHOW_QUEUE_BARS, EXPAND_DEFAULT, 
		SHARE_SKIPLIST_USE_REGEXP, DOWNLOAD_SKIPLIST_USE_REGEXP, HIGHEST_PRIORITY_USE_REGEXP, USE_HIGHLIGHT, FLASH_WINDOW_ON_PM, FLASH_WINDOW_ON_NEW_PM, FLASH_WINDOW_ON_MYNICK, IP_UPDATE, SERVER_COMMANDS, CLIENT_COMMANDS,
		PM_PREVIEW, IGNORE_USE_REGEXP_OR_WC, NAT_SORT, HUB_BOLD_TABS, SHOW_WINAMP_CONTROL, BLEND_TABS, TAB_SHOW_ICONS, ALLOW_MATCH_FULL_LIST, SHOW_CHAT_NOTIFY,
		BOOL_LAST };

	enum Int64Setting { INT64_FIRST = BOOL_LAST + 1,
		TOTAL_UPLOAD = INT64_FIRST, TOTAL_DOWNLOAD, INT64_LAST, SETTINGS_LAST = INT64_LAST };

	enum {	INCOMING_DIRECT, INCOMING_FIREWALL_UPNP, INCOMING_FIREWALL_NAT,
		INCOMING_FIREWALL_PASSIVE };
	enum {	OUTGOING_DIRECT, OUTGOING_SOCKS5 };

	enum {  MAGNET_SEARCH, MAGNET_DOWNLOAD, MAGNET_OPEN };
	
	enum {  PROFILE_PUBLIC, PROFILE_RAR, PROFILE_PRIVATE, PROFILE_LAN };

	enum {  ORDER_ADDED, ORDER_RANDOM };

	enum {  PRIO_DISABLED, PRIO_BALANCED, PRIO_PROGRESS };

	enum {  TLS_DISABLED, TLS_ENABLED, TLS_FORCED };

	enum {  BLOOM_DISABLED, BLOOM_ENABLED, BLOOM_AUTO };

	enum AutoSelectMethod { SELECT_MOST_SPACE, SELECT_LEAST_SPACE };

	enum FileEvents { ON_FILE_COMPLETE, ON_DIR_CREATED};

	const string& get(StrSetting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? strSettings[key - STR_FIRST] : strDefaults[key - STR_FIRST];
	}

	int get(IntSetting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? intSettings[key - INT_FIRST] : intDefaults[key - INT_FIRST];
	}
	bool get(BoolSetting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? boolSettings[key - BOOL_FIRST] : boolDefaults[key - BOOL_FIRST];
	}
	int64_t get(Int64Setting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? int64Settings[key - INT64_FIRST] : int64Defaults[key - INT64_FIRST];
	}

	void set(StrSetting key, string const& value) {
		if ( (key == NICK) && (value.size() > 35) ) {
			strSettings[key - STR_FIRST] = value.substr(0, 35);
		} else if ( (key == DESCRIPTION) && (value.size() > 50) ) {
			strSettings[key - STR_FIRST] = value.substr(0, 50);
		} else if ( (key == EMAIL) && (value.size() > 64) ) {
			strSettings[key - STR_FIRST] = value.substr(0, 64);
		} else if (key == UPLOAD_SPEED || key == DOWNLOAD_SPEED) {
			boost::regex reg;
			reg.assign("(\\d+(\\.\\d+)?)");
			if (!regex_match(value, reg)) {
				strSettings[key - STR_FIRST] = connectionSpeeds[0];
			} else {
				strSettings[key - STR_FIRST] = value;
			}
		} else {
			strSettings[key - STR_FIRST] = value;
		}
		isSet[key] = !value.empty();
	}

	void set(IntSetting key, int value) {
		if((key == SLOTS) && (value <= 0)) {
			value = 1;
		}
		if((key == EXTRA_SLOTS) && (value < 1)) {
			value = 1;
		}

		if((key == AUTOSEARCH_EVERY) && (value < 1)) {
			value = 1;
		}
		if((key == AUTOSEARCH_RECHECK_TIME) && (value < 30)) {
			value = 30;
		}

		if((key == SET_MINISLOT_SIZE) && (value < 64)) {
			value = 64;
		}

		if((key == NUMBER_OF_SEGMENTS) && (value > 10)) {
			value = 10;
		}

		if((key == SEARCH_TIME) && (value < 5)) {
			value = 5;
		}

		if((key == MINIMUM_SEARCH_INTERVAL) && (value < 5)) {
			value = 5;
		}
		if((key == MAX_RESIZE_LINES) && (value < 1)) {
			value = 1;
		}


		intSettings[key - INT_FIRST] = value;
		isSet[key] = true;
	}

	void set(IntSetting key, const string& value) {
		if(value.empty()) {
			intSettings[key - INT_FIRST] = 0;
			isSet[key] = false;
		} else {
			intSettings[key - INT_FIRST] = Util::toInt(value);
			isSet[key] = true;
		}
	}

	void set(BoolSetting key, bool value) {
		boolSettings[key - BOOL_FIRST] = value;
		isSet[key] = true;
	}

	void set(BoolSetting key, const string& value) {
		if(value.empty()) {
			intSettings[key - INT_FIRST] = 0;
			isSet[key] = false;
		} else {
			intSettings[key - INT_FIRST] = Util::toInt(value) > 0 ? true : false;
			isSet[key] = true;
		}
	}

	void set(Int64Setting key, int64_t value) {
		int64Settings[key - INT64_FIRST] = value;
		isSet[key] = true;
	}

	void set(Int64Setting key, const string& value) {
		if(value.empty()) {
			int64Settings[key - INT64_FIRST] = 0;
			isSet[key] = false;
		} else {
			int64Settings[key - INT64_FIRST] = Util::toInt64(value);
			isSet[key] = true;
		}
	}

	void set(IntSetting key, bool value) { set(key, (int)value); }

	const string& getDefault(StrSetting key) const {
		return strDefaults[key - STR_FIRST];
	}

	int getDefault(IntSetting key) const {
		return intDefaults[key - INT_FIRST];
	}

	bool getDefault(BoolSetting key) const {
		return boolDefaults[key - BOOL_FIRST];
	}

	int64_t getDefault(Int64Setting key) const {
		return int64Defaults[key - INT64_FIRST];
	}

	void setDefault(StrSetting key, string const& value) {
		strDefaults[key - STR_FIRST] = value;
	}

	void setDefault(IntSetting key, int value) {
		intDefaults[key - INT_FIRST] = value;
	}

	void setDefault(BoolSetting key, bool value) {
		boolDefaults[key - BOOL_FIRST] = value;
	}

	void setDefault(Int64Setting key, int64_t value) {
		int64Defaults[key - INT64_FIRST] = value;
	}

	template<typename KeyT> bool isDefault(KeyT key) {
		return !isSet[key] || get(key, false) == getDefault(key);
	}

	void unset(size_t key) { isSet[key] = false; }

	void load() {
		Util::migrate(getConfigFile());
		load(getConfigFile());
	}
	void save() {
		save(getConfigFile());
	}

	void load(const string& aFileName);
	void save(const string& aFileName);
	void reloadPages(int group = 0) {
		fire(SettingsManagerListener::ReloadPages(), group);
	}

	HubSettings getHubSettings() const;

	TStringList getSearchHistory() const {
		return searchHistory;
	}

	TStringList getDirHistory() const {
		return dirHistory;
	}

	bool addSearchToHistory(const tstring& search);
	bool addDirToHistory(const tstring& dir);

	void clearSearchHistory() {
		searchHistory.clear();
	}

	void clearDirHistory() {
		dirHistory.clear();
	}

	StringPair getFileEvent(SettingsManager::FileEvents fe) {
		return fileEvents[fe];
	}
private:
	friend class Singleton<SettingsManager>;
	SettingsManager();
	~SettingsManager() { }

	static const string settingTags[SETTINGS_LAST+1];

	string strSettings[STR_LAST - STR_FIRST];
	int    intSettings[INT_LAST - INT_FIRST];
	bool boolSettings[BOOL_LAST - BOOL_FIRST];
	int64_t int64Settings[INT64_LAST - INT64_FIRST];

	string strDefaults[STR_LAST - STR_FIRST];
	int    intDefaults[INT_LAST - INT_FIRST];
	bool boolDefaults[BOOL_LAST - BOOL_FIRST];
	int64_t int64Defaults[INT64_LAST - INT64_FIRST];

	bool isSet[SETTINGS_LAST];

	TStringList searchHistory;
	TStringList dirHistory;

	StringPairList fileEvents;
	string getConfigFile() { return Util::getPath(Util::PATH_USER_CONFIG) + "DCPlusPlus.xml"; }
};

// Shorthand accessor macros
#define SETTING(k) (SettingsManager::getInstance()->get(SettingsManager::k, true))

} // namespace dcpp

#endif // !defined(SETTINGS_MANAGER_H)