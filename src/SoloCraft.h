#pragma once

#include "SoloCraftConfig.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Maps/Map.h"

#include "Module.h"

#include <map>
namespace cmangos_module
{
    class Solocraft : public Module
    {
        public:
            Solocraft();
            const SolocraftConfig* GetConfig() const override;          

            bool Initialize();
            void OnLogin(Player* player);
            void OnLogout(Player* player);
            //void OnAddMember(Player* player);
            void OnMapChanged(Player* player);
            //bool ChatFilter(Player* player, string text);

        private:
            std::map<ObjectGuid, int> _unitDifficulty;
            std::map<ObjectGuid, int> _unitBuff;

            float CalculateDifficulty(Map* map);
            uint32 GetNumInGroup(Player* payer);
            void ClearBuffs(Player* player, Map* map);
            void ClearBuffs(Player* player);
            void ApplyBuffs(Player* player, Map* map, int difficulty, int numInGroup);
            void ApplyBuffs(Player* player, int multiplier);
    };
}

#static Solocraft Solocraft;

#define sSolocraft MaNGOS::Singleton<Solocraft>::Instance()
