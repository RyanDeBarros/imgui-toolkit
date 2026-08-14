#include "payload.hpp"

namespace imtk::prop
{
	payload::payload(const void* data, size_t size, imp::type_erasure type)
		: type(type)
	{
		this->data.resize(size);
		std::memcpy(this->data.data(), data, size);
	}
}
