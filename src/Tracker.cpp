#include "TorrentFile.h"
#include "Tracker.h"

namespace torrent_client {

Tracker::Tracker(
			const TorrentFile* torrent
		) noexcept :
			torrent_(torrent) {
}

std::string Tracker::RequestPeers() const {
	const std::string& url = torrent_->GetAnnounce();
	if (url.empty()) {
		throw std::invalid_argument("Invalid torrent file. Announce URL is empty.");
	}

	cpr::Get(
		cpr::Url{url},
		cpr::Parameters{
			{}
		}
	);
	return;
}
}
