#pragma once

#include "solocraft/SoloCraftConfig.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Maps/Map.h"

#include <map>

class Solocraft
{
    public:
        Solocraft();
        virtual ~Solocraft();
        static Solocraft& instance()
        {
            static Solocraft instance;
            return instance;
        }

        bool Initialize();
        void OnLogin(Player* player);
        void OnLogout(Player* player);
        //void OnAddMember(Player* player);
        void OnMapChanged(Player* player);
        //bool ChatFilter(Player* player, string text);

    private:
        SolocraftConfig sSolocraftConfig;
	    std::map<ObjectGuid, int> _unitDifficulty;
        std::map<ObjectGuid, int> _unitBuff;
        
        float CalculateDifficulty(Map* map);
        uint32 GetNumInGroup(Player* payer);
        void ClearBuffs(Player* player, Map* map);
        void ClearBuffs(Player* player);
        void ApplyBuffs(Player* player, Map* map, int difficulty, int numInGroup);
        void ApplyBuffs(Player* player, int multiplier);
};

#define sSolocraft MaNGOS::Singleton<Solocraft>::Instance()