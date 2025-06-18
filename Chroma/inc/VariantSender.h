#ifndef VARIANTSENDER_H
#define VARIANTSENDER_H

#include <string>
#include "Variant.h"
#include "Player.h"

class VariantSender
{
public:
    
    static void OnConsoleMessage(Player* p, const std::string& text)
    {
        Variant v;
        v.add("OnConsoleMessage");
        v.add(text);
        v.send(p->GetPeer());
    }
    
    static void OnTextOverlay(Player* p, const std::string& text)
    {
        Variant v;
        v.add("OnTextOverlay");
        v.add(text);
        v.send(p->GetPeer());
    }
    
    static void OnTalkBubble(Player* p, int netid, const std::string& text)
    {
        Variant v(0, netid);
        v.add("OnTalkBubble");
        v.add(text);
        v.send(p->GetPeer());
    }
    
    static void OnDialogRequest(Player* p, const std::string& text)
    {
        Variant v;
        v.add("OnDialogRequest");
        v.add(text);
        v.send(p->GetPeer());
    }
    
    static void OnRequestWorldMenu(Player* p, const std::string& text = "")
    {
        Variant v;
        v.add("OnRequestWorldSelectMenu");
        v.add("\nadd_floater|START|START||1|-2016482305" + text);
        v.send(p->GetPeer());
    }
    
    static void OnSuperMain(Player* p, uint32_t itemhash, const std::string cdn_path, const std::string cache_path, const std::string blocked, const std::string settings, int ply_tribute_hash)
    {
        Variant v;
        v.add("OnSuperMainStartAcceptLogonHrdxs47254722215a");
        v.add(itemhash);
        v.add(cdn_path);
        v.add(cache_path);
        v.add(blocked);
        v.add(settings);
        //v.add(ply_tribute_hash);
        v.send(p->GetPeer());
    }
    
    static void OnSpawn(Player* p, const std::string data)
    {
        Variant v;
        v.add("OnSpawn");
        v.add(data);
        v.send(p->GetPeer());
    }
   
    static void OnRemove(Player* p, const std::string data)
    {
        Variant v;
        v.add("OnRemove");
        v.add(data);
        v.send(p->GetPeer());
    }
   
    static void OnSetBux(Player* p, int count)
    {
        int gems = p->GetGems();
        gems += count;
        Variant v;
        v.add("OnSetBux");
        v.add(gems);
        v.send(p->GetPeer());
        // check later
    }
   
    static void OnAddNotification(Player* p, const std::string& text, const std::string& file, const std::string& audio)
    {
        Variant v;
        v.add("OnAddNotification");
        v.add(text);
        v.add(file);
        v.add(audio);
        v.send(p->GetPeer());
    }
    
    static void OnPlayPositioned(Player* p, int netid, const std::string file)
    {
        Variant v(0, netid);
        v.add("OnPlayPositioned");
        v.add(file);
        v.send(p->GetPeer());
    }
    
    static void OnSetPos(Player* p, int netid, int pos_x, int pos_y)
    {
        
        Variant v(0, netid);
        v.add((float)pos_x, (float)pos_y);
        v.send(p->GetPeer());
    }
    
    static void SetHasAccountSecured(Player* p, int status)
    {
        Variant v;
        v.add("SetHasAccountSecured");
        v.add(status); // generally 1
        v.send(p->GetPeer());
    }
    
    static void SetHasGrowID(Player* p, const std::string& name, const std::string pass)
    {
        Variant v;
        v.add("SetHasGrowID");
        v.add(1);
        v.add(name);
        v.add(pass);
        v.send(p->GetPeer());
    }
    
    static void OnSetCurrentWeather(Player* p, int weather_id)
    {
        Variant v;
        v.add("OnSetCurrentWeather");
        v.add(weather_id);
        v.send(p->GetPeer());
    }
    
    static void OnNameChanged(Player* p, int netid, const std::string name)
    {
        Variant v(0, netid);
        v.add("OnNameChanged");
        v.add(name);
        v.send(p->GetPeer());
    }

    static void OnSendToServer(Player* p, int port, int token, int player_id, const std::string& ip, const std::string& door_id, const std::string& uuidtoken, int lmode, const std::string nickname)
    {
        Variant v;
        v.add("OnSendToServer");
        v.add(port);
        v.add(token);
        v.add(player_id);
        v.add(ip + "|" + door_id + "|" + uuidtoken); // uuidtoken = server login: RID, world login: -1
        v.add(lmode); // server login: 1, world login: 3
        v.add(nickname);
        v.send(p->GetPeer());
    }

    static void OnCountdownStart(Player* p, int netid, int score, int time)
    {
        Variant v(0, netid);
        v.add("OnCountdownStart");
        if (score >= 0)
            v.add(score);
        v.add(time);
        v.send(p->GetPeer());
    }

    static void OnCountdownUpdate(Player* p, int netid, int score)
    {
        Variant v(0, netid);
        v.add("OnCountdownUpdate");
        v.add(score);
        v.send(p->GetPeer());
    }

    static void OnChangeSkin(Player* p, int netid, int color)
    {
        Variant v(0, netid);
        v.add("OnChangeSkin");
        v.add(color);
        v.send(p->GetPeer());
    }

    static void OnCountdownEnd(Player* p)
    {
        Variant v;
        v.add("OnCountdownEnd");
        v.send(p->GetPeer());
    }

    static void OnFailedToEnterWorld(Player* p)
    {
        Variant v;
        v.add("OnFailedToEnterWorld");
        v.send(p->GetPeer());
    }

    static void OnRaceStart(Player* p, int netid)
    {
        Variant v(0, netid);
        v.add("OnRaceStart");
        v.send(p->GetPeer());
    }

    static void OnRaceEnd(Player* p, int netid)
    {
        Variant v(0, netid);
        v.add("OnRaceEnd");
        v.send(p->GetPeer());
    }

    static void Ping(Player* p)
    {
        Variant v;
        v.add("Ping");
        v.send(p->GetPeer());
    }
    
    static void OnSetFreezeState(Player* p, int netid, int state)
    {
        Variant v(0, netid);
        v.add("OnSetFreezeState");
        v.add(state);
        v.send(p->GetPeer());
    }

    static void OnInvis(Player* p, int netid, int state)
    {
        Variant v(0, netid);
        v.add("OnInvis");
        v.add(state);
        v.send(p->GetPeer());
    }

    static void OnKilled(Player* p, int netid)
    {
        Variant v(0, netid);
        v.add("OnKilled");
        v.add(1);
        v.send(p->GetPeer());
    }

    static void OnAction(Player* p, int netid, const std::string& command)
    {
        Variant v(0, netid);
        v.add("OnAction");
        v.add(command);
        v.send(p->GetPeer());
    }

    static void OnParticleEffect(Player* p, int effect, int pos_x, int pos_y)
    {
        Variant v;
        v.add("OnParticleEffect");
        v.add(effect);
        v.add((float)pos_x, (float)pos_y);
        v.send(p->GetPeer());
    }
};

#endif
