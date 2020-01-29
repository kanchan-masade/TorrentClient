add_executable(TorrentClientTests
	TestMain.cpp
	TestTorrentFile.cpp
)

target_link_libraries(TorrentClientTests
	-Wl,--start-group
	TorrentClientLib
	${LibrariesToLink}
	${GtestLibs}
	${LibCryptopp}
	-Wl,--end-group
)

add_test(TorrentClientTests
	TorrentClientTests
	${CMAKE_SOURCE_DIR}
)

add_custom_command(TARGET TorrentClientTests
	POST_BUILD
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/ubuntu-19.10-desktop-amd64.iso.torrent .
)
