#ifndef SKYNET_SERIALIZE_H
#define SKYNET_SERIALIZE_H

#include "core.h"
#include "scene.h"
#include "yaml-cpp/yaml.h"

class Serializer
{
public:
	Serializer();
	~Serializer();

	inline void setScene(ref<Scene> &scene) { this->scene=scene; }
	
	void serialize(const std::string &path);
	bool deserialize(const std::string &path);

private:	
	ref<Scene> scene;
};

#endif // SKYNET_SERIALIZE_H