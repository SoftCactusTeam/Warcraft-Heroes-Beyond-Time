#include "p2Defs.h"

enum ItemFunctions
{
	GetItem,
	UpdateLogic,
	ByeByeItem
};

class Item
{
protected:

	enum class ItemType
	{
		no_item_type = -1,
		active_item_type,
		passive_item_type
	} type;

	char* name = nullptr;
	uint rarity = 0u;

public:

	Item();
	Item(char* name, ItemType type, uint rarity);
	~Item();

	virtual bool GetItem();
	virtual bool UpdateLogic();
	virtual bool ByeByeItem();

};