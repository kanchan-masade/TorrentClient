#include <experimental/filesystem>
#include <fstream>
#include <map>

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>

#include "MetaInfo.h"
#include "bencode.hpp"

namespace fs = std::experimental::filesystem;

namespace torrent_client {

const std::string kAnnounce{"announce"};
const std::string kCreationDate {"creation date"};
const std::string kInfo{"info"};
const std::string kAnnounceList{"announce-list"};
const std::string kInfoLength{"length"};
const std::string kInfoName{"name"};
const std::string kInfoPieces{"pieces"};
const std::string kInfoPiecesLength{"piece length"};
const std::string kInvalidFile{"Invalid torrent file."};

MetaInfo::MetaInfo(
			std::string file
		) noexcept :
			file_(std::move(file)) {
}

std::string MetaInfo::ReadFile(const std::string& file, const size_t size) {
	std::ifstream ifs{file, std::ios::binary | std::ios::ate};
	ifs.seekg(0, std::ios::beg);

	std::string buffer(size, 0);
	if (ifs.read(buffer.data(), size)) {
		return buffer;
	}
	throw std::runtime_error("Reading from file " + file + " failed");
}

void MetaInfo::ParseInfo(const bencode::dict_view& info_dict) {
	const auto end_it = info_dict.end();

	auto it = info_dict.find(kInfoName);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	name_ = boost::get<std::string_view>(it->second);

	it = info_dict.find(kInfoLength);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	file_length_ = boost::get<bencode::integer_view>(it->second);

	it = info_dict.find(kInfoPiecesLength);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	piece_length_ = boost::get<bencode::integer_view>(it->second);

	it = info_dict.find(kInfoPieces);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	pieces_ = boost::get<bencode::string_view>(it->second);

	std::string encoded_info = bencode::encode(info_dict);
	CryptoPP::SHA1 hash;
	hash.Update((const CryptoPP::byte*)encoded_info.data(), encoded_info.size());
	hash.Final((CryptoPP::byte*) info_hash_.data());
}

void MetaInfo::Parse() {
	// open file
	fs::path path{file_};
	if (not fs::is_regular_file(path)) {
		throw std::invalid_argument(file_ + " is not a regular file");
	}
	// read all file data
	auto data = ReadFile(file_, fs::file_size(path));
	if (data.empty()) {
		throw std::invalid_argument(file_ + " is empty");
	}

	// parse the read buffer
	auto decoded = bencode::decode_view(data.begin(), data.end());
	if (decoded.empty()) {
		throw std::invalid_argument(file_ + " is not torrent file");
	}

	// populate internal structures
	auto d = boost::get<bencode::dict_view>(decoded);

	const auto end_it = d.end();
	auto it = d.find(kAnnounce);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	announce_ = boost::get<std::string_view>(it->second);

	it = d.find(kInfo);
	if (it == end_it) {
		throw std::invalid_argument(kInvalidFile);
	}
	ParseInfo(boost::get<bencode::dict_view>(it->second));

	it = d.find(kCreationDate);
	if (it != end_it) {
		creation_date_ = boost::get<bencode::integer_view>(it->second);
	}
}

const std::string MetaInfo::GetAnnounce() const noexcept {
	return announce_;
}

const std::string MetaInfo::GetName() const noexcept {
	return name_;
}

size_t MetaInfo::GetFileLength() const noexcept {
	return file_length_;
}

const std::string MetaInfo::GetPieces() const noexcept {
	return pieces_;
}

size_t MetaInfo::GetPieceLength() const noexcept {
	return piece_length_;
}
}
