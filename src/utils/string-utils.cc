/*
	Flexisip, a flexible SIP proxy server with media capabilities.
	Copyright (C) 2010  Belledonne Communications SARL.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include <stdexcept>

#include <flexisip/logmanager.hh>

#include "string-utils.hh"

using namespace std;

vector<string> StringUtils::split (const string &str, const string &delimiter) noexcept {
	vector<string> out;

	if (!str.empty()) {
		size_t pos = 0, oldPos = 0;
		for (; (pos = str.find(delimiter, pos)) != string::npos; oldPos = pos + delimiter.length(), pos = oldPos)
			out.push_back(str.substr(oldPos, pos - oldPos));
		out.push_back(str.substr(oldPos));
	}

	return out;
}

std::string StringUtils::strip(const char *str, char c) noexcept {
	auto start = str, end = const_cast<const char *>(index(str, '\0'));
	strip(start, end, c);
	return string{start, end};
}

std::string StringUtils::strip(const std::string &str, char c) noexcept {
	auto start = str.cbegin(), end = str.cend();
	strip(start, end, c);
	return string{start, end};
}


std::string StringUtils::stripAll(const char *str, char c) {
	const char *start = str;
	const char *end = index(str, '\0');
	while (end > start && *end == c) end--;
	while (end > start && *start == c) start++;
	return string(start, end-start);
}

std::string StringUtils::stripAll(const std::string &str, char c) {
	auto start = str.cbegin();
	auto end = str.cend();
	stripAll(start, end, c);
	return string(start, end);
}

void StringUtils::stripAll(std::string::const_iterator &start, std::string::const_iterator &end, char c) {
	while (end > start && *(end-1) == c) end--;
	while (end > start && *start == c) start++;
}

std::string StringUtils::removePrefix(const std::string &str, const std::string &prefix) {
	if (str.compare(0, prefix.size(), prefix) != 0) {
		ostringstream os;
		os << "'" << prefix << "' is not a prefix of '" << str << "'";
		throw invalid_argument(os.str());
	}
	return str.substr(prefix.size());
}

std::string &StringUtils::searchAndReplace(std::string &str, const std::string &key, const std::string &value) noexcept {
	for (auto i = str.find(key); i != string::npos;) {
		str.replace(i, key.size(), value);
		i += value.size();
	}
	return str;
}

std::string StringUtils::transform(const std::string &str, const std::map<char, std::string> &transMap) noexcept {
	string res{};
	for (const auto &c : str) {
		auto transEntry = transMap.find(c);
		if (transEntry != transMap.cend()) {
			res.append(transEntry->second);
		} else {
			res.push_back(c);
		}
	}
	return res;
}

#ifdef HAVE_LIBLINPHONECXX
flexisip::stl_backports::optional<linphone::MediaEncryption> StringUtils::string2MediaEncryption(const std::string& str) {
	using enc = linphone::MediaEncryption;
	if (str == "zrtp") {
		return enc::ZRTP;
	} else if (str == "sdes") {
		return enc::SRTP;
	} else if (str == "dtls-srtp") {
		return enc::DTLS;
	} else if (str == "none") {
		return enc::None;
	}

	SLOGE << "Invalid encryption mode: " << str << " valids modes are : zrtp, sdes, dtls-srtp, none. Ignore this setting";
	return {};
}
#endif // HAVE_LIBLINPHONECXX
