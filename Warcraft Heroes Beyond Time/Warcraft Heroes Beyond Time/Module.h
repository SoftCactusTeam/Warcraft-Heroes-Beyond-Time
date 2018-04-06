#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>
#include "PugiXml/src/pugixml.hpp"

class App;
class GUIElem;
enum class UIEvents;

class Module
{
public:

	Module() : active(false)
	{}
	
	virtual void Init()
	{
		active = true;
	}

	virtual bool Awake(pugi::xml_node&)
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

	virtual void Save(pugi::xml_node&) 
	{}

	virtual void Load(const pugi::xml_node&) 
	{}

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event) 
	{
		return true;
	}

	virtual void AddCommands() 
	{}

	bool Activate()
	{
		bool ret = true;
		if (!active)
		{
			ret = Start();
			active = true;
		}	
		return ret;
	}

	bool DeActivate()
	{
		bool ret = true;
		if (active)
		{
			ret = CleanUp();
			active = false;
		}
		return ret;
	}

	bool isActive()
	{
		return active;
	}

public:

	std::string	name;

protected:

	bool active;

};

#endif