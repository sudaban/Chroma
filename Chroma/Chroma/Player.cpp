#include "Player.h"
#include "Log.h"
#include "Text.h"
#include "ItemDatabase.h"
#include "VariantSender.h"

Player::Player(ENetPeer* peer)
{
	if (peer)
		this->m_peer = peer;
	else return;

	peer->data = this;
	
}
Player::~Player() {/*destructor*/ }

void Player::Login(const std::string& data, bool guest, bool new_login)
{
	//Logger(data, LogType::Debug);

	if (guest)
	{
		m_req_name = Text::parser("requestedName", data);
		Logger("Guest Login Attempt: " + m_req_name, LogType::Debug);
		m_guest = true;
	}

	if (new_login)
	{
		m_growid = Text::parser("growId", data);
		m_pass = Text::parser("password", data);
		
		if (m_growid == "_register_" && m_pass == "_register_") // custom login page settings (will be changed later..)
		{
			m_req_name = "Guest_" + std::to_string(rand() % 899 + 100);
			Logger("Guest Login Attempt Received", LogType::Debug);
			m_guest = true;
		}
		else Logger("Login Attempt: " + m_growid, LogType::Debug);

	}
	else
	{
		m_growid = Text::parser("tankIDName", data);
		m_pass = Text::parser("tankIDPass", data);
		Logger("Login Attempt: " + m_growid, LogType::Debug);
	}

	m_game_version = Text::parser("game_version", data);
	m_country = Text::parser("country", data);
	m_rid = Text::parser("rid", data);
	m_mac = Text::parser("mac", data);
	m_platform_id = Text::parser("platformID", data);

	VariantSender::OnSuperMain(this, ItemDatabase::items_data_hash, "cdn.growps.eu", "cache", "chroma-based-source", "proto=200|choosemusic=audio/mp3/western.mp3|active_holiday=0|wing_week_day=0|ubi_week_day=0|server_tick=8310098|clash_active=1|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|", 0);
}


void Player::SendPacket(TankPacket& tp, uint8_t* extraData, int extraDataLength)
{
	if (m_peer == nullptr)
		return;

	uint8_t* dataToSend = nullptr;
	int DataLength = 0;

	if (extraData == nullptr || extraDataLength == 0) {
		tp.PacketLength = 0;
		DataLength = sizeof(TankPacket);
		dataToSend = new uint8_t[DataLength];
		std::memcpy(dataToSend, &tp, sizeof(TankPacket));
	}
	else {
		tp.PacketLength = extraDataLength;
		DataLength = sizeof(TankPacket) + extraDataLength;
		dataToSend = new uint8_t[DataLength];
		std::memcpy(dataToSend, &tp, sizeof(TankPacket));
		std::memcpy(dataToSend + sizeof(TankPacket), extraData, extraDataLength);
	}

	if (tp.PacketLength > 0 && dataToSend[sizeof(TankPacket)] == 4)
	{
		DataLength -= 1;
	}

	ENetPacket* packet = enet_packet_create(nullptr, DataLength + 4, ENET_PACKET_FLAG_RELIABLE);

	if (!packet)
		return;

	*(int*)packet->data = NET_MESSAGE_GAME_PACKET;

	std::memcpy(packet->data + 4, dataToSend, DataLength);
	enet_peer_send(m_peer, 0, packet);

	delete[] dataToSend;
}
