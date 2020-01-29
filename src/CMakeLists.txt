include_directories(include)

set(TorrentClientFiles
	MetaInfo.cpp
	TorrentFile.cpp
	Utils.cpp
)

set(LibrariesToLink
	pthread
	glog
	gflags
	stdc++fs
)

set(GtestLibs
	gflags
	gtest
)

add_library(TorrentClientLib STATIC
	${TorrentClientFiles}
)

add_dependencies(TorrentClientLib TargetLibCryptoPP)
add_subdirectory(tests)
