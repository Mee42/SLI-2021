#pragma once

#include "Core.h"

#include <cstdint>
#include <cstdlib>

#include "Packet.h"

class PacketBuffer
{
public:
	PacketBuffer(uint8_t* buf, uint32_t capacity)
		: m_Buf(buf), m_Header(reinterpret_cast<PacketHeader*>(m_Buf)), m_Capacity(capacity), m_Offset(sizeof(PacketHeader))
	{
		SLI_ASSERT(capacity >= sizeof(PacketHeader), "Buffer is not large enough!");
	}

	PacketBuffer(const PacketBuffer& other) : m_Buf(other.m_Buf), m_Header(other.m_Header), m_Capacity(other.m_Capacity), m_Offset(other.m_Offset) {}

	inline uint8_t* Begin() { return m_Buf; }
	inline const uint8_t* Begin() const { return m_Buf; }

	virtual bool Read(void* dest, std::size_t bytes);

	template<typename T> T* As(std::size_t skipBytes)
	{
		SLI_ASSERT(m_Offset + skipBytes <= m_Capacity, "Buffer is not large enough!");
		T* result = reinterpret_cast<T*>(m_Buf + m_Offset);
		m_Offset += skipBytes; return result;
	}

	template<typename T> T* Ptr() { return As<T>(sizeof(T)); }

	inline uint32_t Offset() const { return m_Offset; }

	inline PacketHeader* Header() { return m_Header; }
	virtual ~PacketBuffer() {}


private:
	uint8_t* m_Buf;
	PacketHeader* m_Header;
	uint32_t m_Capacity;
	uint32_t m_Offset;

};


template<std::size_t Cap, std::size_t RealCap = Cap + sizeof(PacketHeader)>
class StackBuffer : public PacketBuffer
{
public:
	StackBuffer() : PacketBuffer(m_Buffer, RealCap) {}

private:
	uint8_t m_Buffer[RealCap];
};

