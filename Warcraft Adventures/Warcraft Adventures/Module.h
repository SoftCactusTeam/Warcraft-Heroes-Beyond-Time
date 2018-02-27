#ifndef __MODULE_H__
#define __MODULE_H__

class App;

#include <string>

class Module
{
public:

	Module() : active(false)
	{}
	
	void Init()
	{
		active = true;
	}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

public:

	std::string	name;
	bool		active;

};

#endif