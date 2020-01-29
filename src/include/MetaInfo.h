#pragma once

#include <string>
#include <vector>

#include "bencode.hpp"

namespace torrent_client {

class MetaInfo {
public:
	MetaInfo(std::string file) noexcept;
	void Parse();

	const std::string GetAnnounce() const noexcept;
	const std::string GetName() const noexcept;
	size_t GetFileLength() const noexcept;

	const std::string GetPieces() const noexcept;
	size_t GetPieceLength() const noexcept;

private:
	std::string ReadFile(const std::string& file, const size_t size);
	void ParseInfo(const bencode::dict_view& info_dict);

private:
	const std::string file_;
	std::string announce_;
	uint64_t creation_date_{};

	std::string name_;
	std::string pieces_;
	size_t file_length_{};
	size_t piece_length_{};

	std::array<std::byte, 20> info_hash_;
};
}
