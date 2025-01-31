/*
    Flexisip, a flexible SIP proxy server with media capabilities.
    Copyright (C) 2010-2022 Belledonne Communications SARL, All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <bctoolbox/logging.h>
#include <bctoolbox/tester.h>

#include <sofia-sip/su_log.h>

#ifdef HAVE_CONFIG_H
#include "flexisip-config.h"
#endif

#include <flexisip/logmanager.hh>
#include "flexisip-tester-config.hh"

#include "tester.hh"

namespace flexisip {
namespace tester {

std::string bcTesterFile(const std::string& name) {
	char* file = bc_tester_file(name.c_str());
	std::string ret(file);
	bc_free(file);
	return ret;
}

std::string bcTesterRes(const std::string& name) {
	char* file = bc_tester_res(name.c_str());
	std::string ret(file);
	bc_free(file);
	return ret;
}

static int verbose_arg_func(const char* arg) {
	LogManager::get().setLogLevel(BCTBX_LOG_DEBUG);
	su_log_set_level(nullptr, 9);
	return 0;
}

static int silent_arg_func(const char* arg) {
	LogManager::get().setLogLevel(BCTBX_LOG_FATAL);
	LogManager::get().enableUserErrorsLogs(false);
	su_log_set_level(nullptr, 0);
	return 0;
}

static void log_handler(int lev, const char* fmt, va_list args) {
#ifdef _WIN32
	vfprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, fmt, args);
	fprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, "\n");
#else
	va_list cap;
	va_copy(cap, args);
	/* Otherwise, we must use stdio to avoid log formatting (for autocompletion etc.) */
	vfprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, fmt, cap);
	fprintf(lev == BCTBX_LOG_ERROR ? stderr : stdout, "\n");
	va_end(cap);
#endif
}

void flexisip_tester_init() {
	// Initialize logs
	LogManager::Parameters logParams{};
	logParams.level = BCTBX_LOG_WARNING;
	logParams.enableSyslog = false;
	logParams.enableStdout = true;
	logParams.enableUserErrors = true;
	LogManager::get().initialize(logParams);

	su_log_redirect(
	    nullptr,
	    [](void*, const char* fmt, va_list ap) {
		    // remove final \n from SofiaSip
		    std::string copy{fmt, strlen(fmt) - 1};
		    LOGDV(copy.c_str(), ap);
	    },
	    nullptr);
	bc_tester_set_verbose_func(verbose_arg_func);
	bc_tester_set_silent_func(silent_arg_func);
	bc_tester_init(log_handler, BCTBX_LOG_MESSAGE, BCTBX_LOG_ERROR, ".");

	// Make the default resource dir point to the 'tester' directory in the source code
	bc_tester_set_resource_dir_prefix(FLEXISIP_TESTER_DATA_SRCDIR);
}

void flexisip_tester_uninit(void) {
	bc_tester_uninit();
}

} // namespace tester
} // namespace flexisip

int main(int argc, char* argv[]) {
	using namespace flexisip::tester;

	flexisip_tester_init();

	for (auto i = 1; i < argc; ++i) {
		auto ret = bc_tester_parse_args(argc, argv, i);
		if (ret > 0) {
			i += ret - 1;
			continue;
		} else if (ret < 0) {
			bc_tester_helper(argv[0], "");
		}
		return ret;
	}

	auto ret = bc_tester_start(argv[0]);
	flexisip_tester_uninit();
	return ret;
}
