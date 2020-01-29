#include "Utils.h"

namespace utils {

std::vector<std::string> StringSplit(const std::string& in, size_t piece_size) {
	auto it = in.begin();
	auto end = in.end();

	std::vector<std::string> result;
	while (it != end) {
		auto s = std::min(static_cast<size_t>(std::distance(it, end)), piece_size);
		result.emplace_back(it, std::next(it, s));
		std::advance(it, s);
	}
	return result;
}



}
