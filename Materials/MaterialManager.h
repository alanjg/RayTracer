#pragma once
#include "Material.h"

class MaterialManager
{
public:
	Material* Get(std::string name)
	{
		return materialMap[name];
	}
	void Add(std::string name, Material* mat)
	{
		materialMap[name] = mat;
		materials.emplace_back(std::unique_ptr<Material>(mat));
	}
	void AddUntracked(Material* mat)
	{
		materials.emplace_back(std::unique_ptr<Material>(mat));
	}
private:
	std::map<std::string, Material*> materialMap;
	std::vector<std::unique_ptr<Material>> materials;
};

extern MaterialManager g_MaterialManager;