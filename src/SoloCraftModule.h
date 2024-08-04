#pragma once

#include "SoloCraftModuleConfig.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Maps/Map.h"

#include "Module.h"

#include <map>
namespace cmangos_module
{
    class SolocraftModule : public Module
    {
        public:
            SolocraftModule();
            const SolocraftModuleConfig* GetConfig() const override;

            void OnInitialize() override;
            void OnLogin(Player* player) override;
            void OnLogout(Player* player) override;
            //void OnAddMember(Player* player);
            void OnMapChanged(Player* player) override;
            //bool ChatFilter(Player* player, string text);

        private:
            SolocraftModuleConfig sSolocraftModuleConfig;
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

