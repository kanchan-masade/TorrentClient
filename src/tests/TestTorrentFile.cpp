#include <string>
#include <gtest/gtest.h>

#include "TorrentFile.h"

static const std::string kTorrentFile = "ubuntu-19.10-desktop-amd64.iso.torrent";

TEST(TestTorrentFile, Parse) {
	torrent_client::TorrentFile torrent(kTorrentFile);
	EXPECT_NO_THROW(torrent.Parse());
}
