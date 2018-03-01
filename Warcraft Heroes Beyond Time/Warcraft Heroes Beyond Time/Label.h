#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>

class Label : public GUIElem
{
	friend class ModuleGUI;

public:

	Label(fPoint position, GUIElem* parent) : GUIElem(GUIElemType::LABEL, position, parent) {}
	
	virtual ~Label() {}

	bool Update(float dt)
	{
		return true;
	}



private:

	std::string text;
	TTF_Font* font;



};


#endif