#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <enet/enet.h>
#include "Variant.h"
#include "Packet.h"

class Player
{
public:
	Player(ENetPeer* peer);
	~Player();
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	void Login(const std::string& data, bool guest, bool new_login);
	void SendPacket(TankPacket& tp, uint8_t* extraData = nullptr, int extraDataLength = 0);
	void SendVariantPacket(Variant::VariantData& v);
	void SendVariant(const std::vector<Variant::ArgValue>& args, int32_t netID = -1, uint32_t delay = 0U);

	inline const std::string& GetReqName() const { return m_req_name; }
	inline const std::string& GetGrowID() const { return m_growid; }
	inline const std::string& GetPass() const { return m_pass; }
	inline const std::string& GetCountry() const { return m_country; }
	inline const std::string& GetProtocol() const { return m_protocol; }
	inline const std::string& GetIP() const { return m_ip; }
	inline const std::string& GetRID() const { return m_rid; }
	inline const std::string& GetMAC() const { return m_mac; }
	inline const std::string& GetWorld() const { return m_world; }
	inline const std::string& GetPlatformID() const { return m_platform_id; }
	inline const std::string& GetGameVersion() const { return m_game_version; }
	inline int GetNetID() const { return m_netId; }
	inline int GetPlayerX() const { return m_playerX; }
	inline int GetPlayerY() const { return m_playerY; }
	inline int GetGems() const { return m_gems; }
	inline bool IsGuest() const { return m_guest; }
	inline ENetPeer* GetPeer() const { return m_peer; }

	inline void SetReqName(const std::string& value) { m_req_name = value; }
	inline void SetGrowID(const std::string& value) { m_growid = value; }
	inline void SetPass(const std::string& value) { m_pass = value; }
	inline void SetCountry(const std::string& value) { m_country = value; }
	inline void SetProtocol(const std::string& value) { m_protocol = value; }
	inline void SetIP(const std::string& value) { m_ip = value; }
	inline void SetRID(const std::string& value) { m_rid = value; }
	inline void SetMAC(const std::string& value) { m_mac = value; }
	inline void SetWorld(const std::string& value) { m_world = value; }
	inline void SetPlatformID(const std::string& value) { m_platform_id = value; }
	inline void SetGameVersion(const std::string& value) { m_game_version = value; }
	inline void SetNetID(int value) { m_netId = value; }
	inline void SetPlayerX(int value) { m_playerX = value; }
	inline void SetPlayerY(int value) { m_playerY = value; }
	inline void SetGems(int value) { m_gems = value; }
	inline void SetGuest(bool value) { m_guest = value; }
	inline void SetPeer(ENetPeer* peer) { m_peer = peer; }

private:
	std::string m_req_name = "";
	std::string m_growid = "";
	std::string m_pass = "";
	std::string m_country = "";
	std::string m_protocol = "";
	std::string m_ip = "";
	std::string m_rid = "";
	std::string m_mac = "";
	std::string m_world = "";
	std::string m_platform_id = "";
	std::string m_game_version = "";
	int m_netId = -1;
	int m_playerX = -1, m_playerY = -1;
	int m_gems = 0;
	bool m_guest = false;
	ENetPeer* m_peer = nullptr;
};

#endif