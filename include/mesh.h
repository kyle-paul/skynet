#pragma once

class Mesh 
{
public:
	Mesh();
	~Mesh();
	
	void init(const std::string &path);
	void render();
};