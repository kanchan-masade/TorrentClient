#pragma once

namespace torrent_client {

class TorrentFile;

class Tracker {
public:
	Tracker(const TorrentFile* torrent) noexcept;
	std::string RequestPeers(const PeerId& peer, int port) const;

private:
	std::string TrackerUrl() const;
private:
	const TorrentFile* torrent_{};
};

}
