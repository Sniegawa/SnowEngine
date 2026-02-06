#pragma once

#include <cstdint>
#include <functional>
#include <string>

struct UUID
{
	uint64_t high = 0;
	uint64_t low = 0;

	UUID() = default;
	UUID(uint64_t h, uint64_t l) : high(h), low(l) {}

	bool operator==(const UUID&) const = default;
	bool operator!=(const UUID&) const = default;

	explicit operator bool() const
	{
		return high || low;
	}

};

UUID GenerateUUID();
std::string UUIDToString(const UUID& id);
UUID UUIDFromString(const std::string& s);

namespace std
{
	template<>
	struct hash<UUID>
	{
		size_t operator()(const UUID& id) const noexcept
		{
			size_t h1 = hash<uint64_t>()(id.high);
			size_t h2 = hash<uint64_t>()(id.low);
			return h1 ^ (h2 << 1);
		}
	};
}