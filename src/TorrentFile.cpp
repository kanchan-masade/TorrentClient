#include <iostream>

#include "TorrentFile.h"
#include "MetaInfo.h"
#include "Utils.h"

constexpr size_t kSha1HashSize = 20;

namespace torrent_client {

TorrentFile::TorrentFile(
			std::string file
		) noexcept :
			file_(std::move(file)) {
}

void TorrentFile::Parse() {
	torrent_client::MetaInfo meta(file_);
	meta.Parse();

	announce_ = meta.GetAnnounce();
	name_ = meta.GetName();
	piece_length_ = meta.GetPieceLength();
	file_length_ = meta.GetFileLength();

	pieces_ = utils::StringSplit(meta.GetPieces(), kSha1HashSize);
	if (pieces_.empty() or
			pieces_.back().size() != kSha1HashSize or
			pieces_.size() * piece_length_ < file_length_) {
		throw std::invalid_argument("Invalid torrent file.");
	}
}

void TorrentFile::DumpInfo() const {
	std::cout << "\nTorrent Info"
		<< "\nFile = " << file_
		<< "\nAnnounce = " << announce_
		<< "\nFile Length = " << file_length_
		<< "\nPiece Length = " << piece_length_
		<< "\nPieces = " << pieces_.size()
		<< std::endl;
}
}

#if 0
int main(int argc, char* argv[]) {
	torrent_client::TorrentFile torrent(argv[1]);
	torrent.Parse();
	torrent.DumpInfo();
	return 0;
}
#endif
