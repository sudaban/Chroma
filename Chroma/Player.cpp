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

void Player::SendVariantPacket(Variant::VariantData& v)
{
	Logger("Sending..", LogType::Info);
	const uint8_t* data = v.packet.data();
	size_t dataSize = v.packet.size();

	ENetPacket* packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);

	if (!packet)
		return;

	if (enet_peer_send(m_peer, 0, packet) == 0)
	{
		Logger("Sent", LogType::Info);
	}
	else
	{
		enet_packet_destroy(packet);
		Logger("Failed to send packet", LogType::Error);
	}
}

void Player::SendVariant(const std::vector<Variant::ArgValue>& args, int32_t netID, uint32_t delay)
{
	Variant::VariantData vdata = Variant::call(args, netID, delay);

	auto parsed = Variant::from(vdata.packet);
	for (const auto& arg : parsed.args) {
		if (auto str = std::get_if<std::string>(&arg)) {
			Logger("Parsed string: " + *str, LogType::Info);
		}
		else if (auto i32 = std::get_if<int32_t>(&arg)) {
			Logger("Parsed int32: " + std::to_string(*i32), LogType::Info);
		}
		else if (auto u32 = std::get_if<uint32_t>(&arg)) {
			Logger("Parsed uint32: " + std::to_string(*u32), LogType::Info);
		}
		else if (auto floats = std::get_if<std::vector<float>>(&arg)) {
			std::string flist;
			for (float f : *floats) flist += std::to_string(f) + " ";
			Logger("Parsed floats: " + flist, LogType::Info);
		}
	}

	SendVariantPacket(vdata);
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


void GlobalMessage(ENetHost* server, const std::string& message) 
{
	for (ENetPeer* currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer) 
	{

		if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == nullptr)
			continue;

		Player* player = static_cast<Player*>(currentPeer->data);
		// There will be console message packet
	}
}