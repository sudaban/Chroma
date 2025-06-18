#include <Action.h>
#include <ItemDatabase.h>

void Action::RefreshItemsData(Client cli)
{
	TankPacket t;
	t.Type = PACKET_SEND_ITEM_DATABASE_DATA;
	t.NetID = -1;
	t.State = 8;
	t.PacketLength = ItemDatabase::items_data_size;
	
	Player* p = cli.GetPlayer();
	p->SendPacket(t, ItemDatabase::items_data, ItemDatabase::items_data_size);
}

