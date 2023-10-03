#include "Object.h"

namespace lady
{
	void Object::Read(const rapidjson::Value& value)
	{
		READ_DATA(value, name);
	}
}