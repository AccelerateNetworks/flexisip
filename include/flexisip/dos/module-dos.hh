/*
    Flexisip, a flexible SIP proxy server with media capabilities.
    Copyright (C) 2010-2022 Belledonne Communications SARL, All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <set>

#include "flexisip/dos/dos-executor/ban-executor.hh"
#include "flexisip/module.hh"

namespace flexisip {

struct DosContext {
	uint64_t recv_msg_count_since_last_check = 0;
	double last_check_recv_msg_check_time = 0;
	double packet_count_rate = 0;
};

class ModuleDoSProtection : public Module, ModuleToolbox {
public:
	explicit ModuleDoSProtection(Agent* ag);

	~ModuleDoSProtection() = default;

#ifdef ENABLE_UNIT_TESTS
	void clearWhiteList() {
		mWhiteList.clear();
	}

	void setBanExecutor(const std::shared_ptr<BanExecutor>& executor) {
		if (mBanExecutor) {
			mBanExecutor->onUnload();
		}
		mBanExecutor = executor;
		mBanExecutor->onLoad(nullptr);
	}
#endif

private:
	static ModuleInfo<ModuleDoSProtection> sInfo;
	int mTimePeriod;
	int mPacketRateLimit;
	int mBanTime;
	bool mExecutorConfigChecked = false;
	std::set<BinaryIp> mWhiteList;
	std::unordered_map<std::string, DosContext> mDosContexts;
	std::unordered_map<std::string, DosContext>::iterator mDOSHashtableIterator;
	std::unique_ptr<ThreadPool> mThreadPool;
	std::shared_ptr<BanExecutor> mBanExecutor;

	void onDeclare(GenericStruct* module_config);
	void onLoad(const GenericStruct* mc);
	void onUnload() {
		mBanExecutor->onUnload();
	}
	void onRequest(std::shared_ptr<RequestSipEvent>& ev);
	void onResponse(std::shared_ptr<ResponseSipEvent>& ev){};
	void onIdle();

	virtual bool isValidNextConfig(const ConfigValue& value);

	bool isIpWhiteListed(const char* ip);

	void registerUnbanTimer(const std::string& ip, const std::string& port, const std::string& protocol);
	void unbanIP(const std::string& ip, const std::string& port, const std::string& protocol);
};
} // namespace flexisip
