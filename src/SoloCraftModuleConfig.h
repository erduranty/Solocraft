#pragma once

#include "ModuleConfig.h"

#include <unordered_map>
namespace cmangos_module
{
    class SolocraftModuleConfig : public ModuleConfig
    {
        public:
            SolocraftModuleConfig();
            bool OnLoad() override;
            /* static SolocraftConfig& instance()
            {
                static SolocraftConfig instance;
                return instance;
            }*/

//            bool Initialize();

            bool enabled;
            bool SoloCraftAnnounceModule;
            bool SoloCraftDebuffEnable;
            bool SolocraftXPBalEnabled;
            bool SolocraftXPEnabled;
            bool SolocraftNoXPFlag;
            float SoloCraftSpellMult;
            float SoloCraftStatsMult;
            float SoloCraftXPMod;
            uint32 SolocraftLevelDiff;
            uint32 SolocraftDungeonLevel;
            std::unordered_map<uint8, uint32> classes;
            std::unordered_map<uint32, uint32> dungeons;
            std::unordered_map<uint32, float> diff_Multiplier;
            std::unordered_map<uint32, float> diff_Multiplier_Heroics;
            float D5 = 1.0;
            float D10 = 1.0;
            float D25 = 1.0;
            float D40 = 1.0;
            float D649H10 = 1.0;
            float D649H25 = 1.0;

        private:
//          Config config;
    };
}
// #define sSolocraftConfig MaNGOS::Singleton<SolocraftConfig>::Instance()
