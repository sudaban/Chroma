#include "Player.h"
#include "Log.h"
#include "Text.h"

Player::Player(ENetPeer* peer)
{
	if (peer)
		this->m_peer = peer;
	else return;
}
Player::~Player() {/*destructor*/ }

void Player::Login(const std::string& data, bool guest, bool new_login)
{
	//Logger(data, LogType::Debug);

	if (guest)
	{
		m_guest = true;
	}

	if (new_login)
	{
		m_growid = Text::parser("growId", data);
		m_pass = Text::parser("password", data);
		Logger("Logging: " + m_growid, LogType::Debug);

		if (m_growid == "_register_" && m_pass == "_register_")
		{  // NYRU.net login
			m_guest = true;
		}
	}
	else
	{
		m_growid = Text::parser("tankIDName", data);
		m_pass = Text::parser("tankIDPass", data);
		Logger("Logging: " + m_growid, LogType::Debug);
	}

	std::string requestedName = Text::parser("requestedName", data);

	if (requestedName.empty()) {
		requestedName = "Guest_";
	}

	m_req_name = requestedName + std::to_string(rand() % 899 + 100);

	m_game_version = Text::parser("game_version", data);
	m_country = Text::parser("country", data);
	m_rid = Text::parser("rid", data);
	m_mac = Text::parser("mac", data);
	m_platform_id = Text::parser("platformID", data);
}


void Player::SendPacket(TankPacket& tp, uint8_t* extraData, int extraDataLength)
{
	if (m_peer == nullptr)
		return;

	uint8_t* dataToSend = nullptr;
	int DataLength = 0;

	if (extraData == nullptr || extraDataLength == 0) {
		tp.PacketLenght = 0;
		DataLength = sizeof(TankPacket);
		dataToSend = new uint8_t[DataLength];
		std::memcpy(dataToSend, &tp, sizeof(TankPacket));
	}
	else {
		tp.PacketLenght = extraDataLength;
		DataLength = sizeof(TankPacket) + extraDataLength;
		dataToSend = new uint8_t[DataLength];
		std::memcpy(dataToSend, &tp, sizeof(TankPacket));
		std::memcpy(dataToSend + sizeof(TankPacket), extraData, extraDataLength);
	}

	if (tp.PacketLenght > 0 && dataToSend[sizeof(TankPacket)] == 4)
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
