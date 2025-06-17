#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include <vector>
#include "ItemDefinations.h"

class ItemDatabase
{
public:
	bool Decode(const std::string& path);
	uint32_t proton_hash(const void* data, uintmax_t data_len);
	static int items_data_size;
	static uint8_t* items_data;
	static uint32_t items_data_hash;
private:
	std::vector<Item> items;
};

#endif