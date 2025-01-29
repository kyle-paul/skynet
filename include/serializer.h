#pragma once

#include "core.h"
#include "scene.h"
#include "yaml-cpp/yaml.h"

class Serializer
{
public:
	Serializer(ref<Scene> &scene);
	~Serializer();

	void serialize(const std::string &path);
	bool deserialize(const std::string &path);
	void serializeEntity(YAML::Emitter &out, const std::string &name, ref<Mesh> &object);

private:	
	ref<Scene> scene;
};