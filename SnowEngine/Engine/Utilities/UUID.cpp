#include "UUID.h"

#include <random>
#include <sstream>
#include <iomanip>

UUID GenerateUUID()
{
	static std::random_device rd;
	static std::mt19937_64 gen(rd());

	uint64_t high = gen();
	uint64_t low = gen();

	return UUID(high, low);
}

std::string UUIDToString(const UUID& id)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0')
		<< std::setw(16) << id.high
		<< std::setw(16) << id.low;
	return ss.str();
}

UUID UUIDFromString(const std::string& s)
{
	UUID id;
	std::stringstream ss(s);
	if(s.size() == 32)
	{
		std::string highStr = s.substr(0, 16);
		std::string lowStr = s.substr(16, 16);

		id.high = std::stoull(highStr, nullptr, 16);
		id.low= std::stoull(lowStr, nullptr, 16);

	}
	return id;
}
