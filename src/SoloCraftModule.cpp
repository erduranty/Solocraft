#include "SoloCraftModule.h"

#include "Util/Util.h"
#include "Groups/Group.h"
#include "Chat/Chat.h"

INSTANTIATE_SINGLETON_1(cmangos_module::SolocraftModule);

namespace cmangos_module
{
    SolocraftModule::SolocraftModule() : Module("SoloCraft", new SolocraftModuleConfig()) { }

//    SolocraftModuleModule::~SolocraftModuleModule() { }
    const SolocraftModuleConfig* SolocraftModule::GetConfig() const
    {
        return (SolocraftModuleConfig*)Module::GetConfig();
    }

    void SolocraftModule::OnInitialize()
    {
        sSolocraftModuleConfig.OnLoad();
    }

    void SolocraftModule::OnLogin(Player* player)
    {
        if (sSolocraftModuleConfig.enabled && sSolocraftModuleConfig.SoloCraftAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00SoloCraft |rmodule.");
        }

        OnMapChanged(player);
    }

    void SolocraftModule::OnLogout(Player* player)
    {
        if (sSolocraftModuleConfig.enabled)
        {
            std::map<ObjectGuid, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetObjectGuid());
            if (unitDifficultyIterator != _unitDifficulty.end())
            {
                _unitDifficulty.erase(unitDifficultyIterator);
            }

            std::map<ObjectGuid, int>::iterator unitBuffIterator = _unitBuff.find(player->GetObjectGuid());
            if (unitBuffIterator != _unitBuff.end())
            {
                _unitBuff.erase(unitBuffIterator);
            }
        }
    }

    void SolocraftModule::OnMapChanged(Player* player)
    {
        if (sSolocraftModuleConfig.enabled)
        {
            Map* map = player->GetMap();

            if (map->IsRaid() || map->IsDungeon())
            {
                int numInGroup = GetNumInGroup(player);
                int difficulty = CalculateDifficulty(map);

                std::map<ObjectGuid, int>::iterator unitBuffIterator = _unitBuff.find(player->GetObjectGuid());
                if (unitBuffIterator != _unitBuff.end())
                {
                    if (difficulty > unitBuffIterator->second)
                    {
                        ClearBuffs(player);
                        ApplyBuffs(player, map, difficulty, numInGroup);
                    }
                }
                else
                {
                    ApplyBuffs(player, map, difficulty, numInGroup);
                }
            }
            else
            {
                std::map<ObjectGuid, int>::iterator unitBuffIterator = _unitBuff.find(player->GetObjectGuid());
                if (unitBuffIterator != _unitBuff.end())
                {
                    ApplyBuffs(player, unitBuffIterator->second);
                }
            }
        }
    }

    // Set the instance difficulty
    float SolocraftModule::CalculateDifficulty(Map* map)
    {
        int difficulty = 1;
        if (map)
        {
            if (sSolocraftModuleConfig.diff_Multiplier.find(map->GetId()) == sSolocraftModuleConfig.diff_Multiplier.end())
            {
                if (map->IsRaid())
                {
                    difficulty = 40;
                }
                else if (map->IsDungeon())
                {
                    difficulty = 5;
                }
            }
            else
                difficulty = sSolocraftModuleConfig.diff_Multiplier[map->GetId()];
        }

        return difficulty;
    }

    // Get the group's size
    uint32 SolocraftModule::GetNumInGroup(Player* player)
    {
        int numInGroup = 1;
        Group* group = player->GetGroup();
        if (group)
        {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            numInGroup = groupMembers.size();
        }
        return numInGroup;
    }

    // Resets buffers
    void SolocraftModule::ClearBuffs(Player* player, Map* map)
    {
        std::map<ObjectGuid, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetObjectGuid());
        if (unitDifficultyIterator != _unitDifficulty.end())
        {
            int difficulty = unitDifficultyIterator->second;
            _unitDifficulty.erase(unitDifficultyIterator);

            if (sSolocraftModuleConfig.SoloCraftAnnounceModule)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Left to %s (removing difficulty = %d)",
                            map->GetMapName(), difficulty);
            }

            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), false);
            }
        }
    }

    void SolocraftModule::ClearBuffs(Player* player)
    {
        std::map<ObjectGuid, int>::iterator unitBuffIterator = _unitBuff.find(player->GetObjectGuid());
        if (unitBuffIterator != _unitBuff.end())
        {
            int multiplier = unitBuffIterator->second;
            _unitBuff.erase(unitBuffIterator);

            if (sSolocraftModuleConfig.SoloCraftAnnounceModule)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Removing Buff Multiplier = %d", multiplier);
            }

            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(multiplier * 100), false);
            }
        }
    }

    // Apply the player buffs
    void SolocraftModule::ApplyBuffs(Player* player, Map* map, int difficulty, int numInGroup)
    {
        ClearBuffs(player, map);
        if (difficulty > 1)
        {
            if (sSolocraftModuleConfig.SoloCraftAnnounceModule)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Entered %s (difficulty = %d, numInGroup = %d)",
                        map->GetMapName(), (difficulty - numInGroup), numInGroup);
            }

            _unitDifficulty[player->GetObjectGuid()] = (difficulty - numInGroup);
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float((difficulty - numInGroup) * 100), true);
            }

            //player->SetFullHealth();
            player->SetHealth(player->GetMaxHealth());
            if (player->GetPowerType() == POWER_MANA)
            {
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            }
        }
    }

    void SolocraftModule::ApplyBuffs(Player* player, int multiplier)
    {
        ClearBuffs(player);
        if (multiplier > 1)
        {
            if (sSolocraftModuleConfig.SoloCraftAnnounceModule)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Apply Buff Multiplier = %d", multiplier);
            }

            _unitBuff[player->GetObjectGuid()] = multiplier;
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, (multiplier * 100), true);
            }

            player->SetHealth(player->GetMaxHealth());
            if (player->GetPowerType() == POWER_MANA)
            {
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            }
        }
    }
}
