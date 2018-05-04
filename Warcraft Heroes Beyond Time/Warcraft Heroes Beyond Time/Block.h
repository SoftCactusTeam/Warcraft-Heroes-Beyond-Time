#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Projectile.h"

struct BlockInfo : public ProjectileInfo
{
	BlockInfo() {};
	BlockInfo(const BlockInfo& info) : ProjectileInfo((const ProjectileInfo&)info) {};
};

class Block : public Projectile
{
public:

	Block(const BlockInfo* info, Projectile_type type);
	~Block();

	bool Update(float dt);
	bool Draw() const;

private:

	BlockInfo* toData = nullptr;
};

#endif