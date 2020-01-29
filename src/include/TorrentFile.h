#pragma once

#include <string>
#include <vector>

namespace torrent_client {

class TorrentFile {
public:
	TorrentFile(std::string file) noexcept;
	void Parse();
	void DumpInfo() const;

private:
	const std::string file_;

	std::string announce_;
	std::string name_;
	size_t piece_length_{};
	size_t file_length_{};

	std::vector<std::string> pieces_;
};

}
