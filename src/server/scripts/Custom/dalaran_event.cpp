/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2018 clopezramos <https://github.com/clopezramos>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CellImpl.h"
#include "Common.h"
#include "ConditionMgr.h"
#include "Containers.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "CustomUtilities.h"
#include "Duration.h"
#include "EventMap.h"
#include "GossipDef.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "GroupMgr.h"
#include "GroupReference.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include "ObjectDefines.h"
#include "ObjectGuid.h"
#include "Player.h"
#include "Position.h"
#include "QuestDef.h"
#include "Random.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SpellAuraDefines.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Unit.h"
#include "UnitAI.h"
#include <cmath>
#include <functional>
#include <list>
#include <map>
#include <unordered_set>
#include <vector>

enum MinigobEscapeMisc
{
    QUEST_MINIGOB_ESCAPE = 30000,

    ITEM_MINIGOB_ESCAPE_FREEZE_CANNON = 60000,

    NPC_MINIGOB_ESCAPE_RHONIN_ORIGINAL = 16128,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_0 = 44003,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_1 = 44004,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_2 = 44005,
    NPC_MINIGOB_ESCAPE_RHONIN_EVENT = 44002,
    NPC_MINIGOB_ESCAPE_MINIGOB = 44000,
    NPC_MINIGOB_ESCAPE_MINIGOB_COPY = 44001,
    NPC_MINIGOB_ESCAPE_RHONIN_ENCOUNTER = 44006,
    NPC_MINIGOB_ESCAPE_ROBOT = 44007,
    NPC_MINIGOB_ESCAPE_ROBOT_TRIGGER = 44018,
    NPC_MINIGOB_ESCAPE_RHONIN_TRIGGER = 44016,
    NPC_MINIGOB_ESCAPE_FROST_ELEMENTAL = 44019,
    NPC_MINIGOB_ESCAPE_FIRE_ELEMENTAL = 44020,

    GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0 = 44004,
    GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_1 = 44005
};

enum MinigobEscapeTextIds
{
    // Rhonin (Event)
    SAY_MINIGOB_ESCAPE_RHONIN_EVENT_OUT_0 = 0,

    // Minigob
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_0 = 0,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_1 = 1,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_2 = 2,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_3 = 3,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_4 = 4,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_5 = 5,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_6 = 6,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_7 = 7,
    SAY_MINIGOB_ESCAPE_MINIGOB_ENGAGE_8 = 8,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_0 = 9,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_1 = 10,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_2 = 11,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_3 = 12,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_4 = 22,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_5 = 23,
    SAY_MINIGOB_ESCAPE_MINIGOB_SPELL_6 = 27,
    SAY_MINIGOB_ESCAPE_MINIGOB_SLAY_0 = 13,
    SAY_MINIGOB_ESCAPE_MINIGOB_COMPLETED_0 = 28,
    SAY_MINIGOB_ESCAPE_MINIGOB_COMPLETED_1 = 14,
    SAY_MINIGOB_ESCAPE_MINIGOB_COMPLETED_2 = 15,
    SAY_MINIGOB_ESCAPE_MINIGOB_COMPLETED_3 = 16,
    SAY_MINIGOB_ESCAPE_MINIGOB_COMPLETED_4 = 17,
    SAY_MINIGOB_ESCAPE_MINIGOB_HELP_0 = 18,
    SAY_MINIGOB_ESCAPE_MINIGOB_HELP_1 = 19,
    SAY_MINIGOB_ESCAPE_MINIGOB_HELP_2 = 25,
    SAY_MINIGOB_ESCAPE_MINIGOB_HELP_3 = 26,
    SAY_MINIGOB_ESCAPE_MINIGOB_VANISH_0 = 20,
    SAY_MINIGOB_ESCAPE_MINIGOB_VANISH_1 = 21,
    SAY_MINIGOB_ESCAPE_MINIGOB_WIPE_0 = 24,
    SAY_MINIGOB_ESCAPE_MINIGOB_THINK_0 = 29,
    SAY_MINIGOB_ESCAPE_MINIGOB_THINK_1 = 30,
    SAY_MINIGOB_ESCAPE_MINIGOB_THINK_2 = 31,
    SAY_MINIGOB_ESCAPE_MINIGOB_THINK_3 = 32,
    SAY_MINIGOB_ESCAPE_MINIGOB_THINK_4 = 33,

    // Rhonin (Encounter)
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_0 = 0,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_1 = 1,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_2 = 2,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_3 = 3,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_4 = 4,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_5 = 5,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_6 = 6,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_7 = 7,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_8 = 8,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_9 = 9,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_10 = 10,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_BEGIN_11 = 11,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_0 = 12,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_1 = 13,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_2 = 14,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_3 = 15,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_4 = 16,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_5 = 17,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_6 = 18,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_7 = 19,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_8 = 20,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_9 = 21,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_10 = 22,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_END_11 = 23,
    SAY_MINIGOB_ESCAPE_RHONIN_ENCOUNTER_CANNON = 24,

    // Robot
    SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_0 = 0,
    SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_1 = 1,
    SAY_MINIGOB_ESCAPE_ROBOT_RESET_0 = 2,
    SAY_MINIGOB_ESCAPE_ROBOT_TERMINATED_0 = 3,
    SAY_MINIGOB_ESCAPE_ROBOT_REMOVE_0 = 4,

    // Robot (Trigger)
    SAY_MINIGOB_ESCAPE_ROBOT_TRIGGER_ANSWER_0 = 0,

    // Rhonin (Trigger)
    SAY_MINIGOB_ESCAPE_RHONIN_TRIGGER_RESURRECT_0 = 0
};

enum MinigobEscapeSpellIds
{
    // Teleports
    SPELL_MINIGOB_ESCAPE_TELEPORT_RANDOM_0 = 62940,
    SPELL_MINIGOB_ESCAPE_TELEPORT_RANDOM_1 = 47653,
    SPELL_MINIGOB_ESCAPE_TELEPORT_IN = 40163,
    SPELL_MINIGOB_ESCAPE_TELEPORT_OUT = 7791,

    // Robot
    SPELL_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE = 64766,
    SPELL_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE_TRIGGER = 64769,
    SPELL_MINIGOB_ESCAPE_ROBOT_MAGNETIC_FIELD = 64668,

    // Minigob
    SPELL_MINIGOB_ESCAPE_MINIGOB_VANISH = 74648, // Meteor Strike
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_BURST = 64663,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_STORM = 61693,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_KNOCKUP = 51830,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_BLAST = 51797,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_HASTE = 50182,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_INFUSION = 51732,
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_OVERLOAD = 51743, // triggered
    SPELL_MINIGOB_ESCAPE_MINIGOB_ARCANE_SURGE = 61272,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FROST_FROSTBOLT = 63913,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FROST_FROSTBOLT_VOLLEY = 72905,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FROST_COLDFLAME = 69140,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FROST_NOVA = 71320,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FROST_BLISTERING_COLD = 70123,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FIRE_VISUAL_0 = 76006,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FIRE_VISUAL_1 = 76010,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FIREBALL = 63789,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FLAMESTRIKE = 72170,
    SPELL_MINIGOB_ESCAPE_MINIGOB_FIRE_SPIT = 67634,
    SPELL_MINIGOB_ESCAPE_MINIGOB_BLINK_NEAR_TARGET = 64662,
    SPELL_MINIGOB_ESCAPE_MINIGOB_MANABONKED = 61834,
    SPELL_MINIGOB_ESCAPE_MINIGOB_EVOCATE = 51602,
    SPELL_MINIGOB_ESCAPE_MINIGOB_MASS_RESURRECTION = 72429,
    SPELL_MINIGOB_ESCAPE_MINIGOB_REVIVE = 51918
};

enum MinigobEscapeEvents
{
    // Trigger
    EVENT_MINIGOB_ESCAPE_TRIGGER_SEARCH_TARGETS = 1,

    // Rhonin (Event)
    EVENT_MINIGOB_ESCAPE_RHONIN_SEARCH_PLAYERS,

    // Robot
    EVENT_MINIGOB_ESCAPE_ROBOT_ENGAGE_COMBAT,
    EVENT_MINIGOB_ESCAPE_ROBOT_SEARCH_TARGETS,
    EVENT_MINIGOB_ESCAPE_ROBOT_ROOT_TARGETS,
    EVENT_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE,
    EVENT_MINIGOB_ESCAPE_ROBOT_REMOVE_TARGETS_IMMUNE_AURAS,

    // Minigob
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_START,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_2,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_3,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_4,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_5,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_6,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_7,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_8,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_9,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_10,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_11,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_12,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_13,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_14,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_15,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_16,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_17,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_18,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_19,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_20,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_21,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_22,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_23,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_24,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_25,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTRO_ENGAGE,

    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SILENCE_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SILENCE_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_STORM,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_KNOCKUP,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_BLAST,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_HASTE,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_INFUSION,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SUMMON,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SURGE_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SURGE_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_ARCANE_SURGE_2,

    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_2,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_3,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_4,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_5,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_0_6,

    EVENT_MINIGOB_ESCAPE_MINIGOB_FROST_COLDFLAME,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FROST_FROSTBOLT,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FROST_FROSTBOLT_VOLLEY,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FROST_NOVA_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FROST_NOVA_1,

    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_2,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_3,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_4,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_5,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_6,
    EVENT_MINIGOB_ESCAPE_MINIGOB_INTERPHASE_1_7,

    EVENT_MINIGOB_ESCAPE_MINIGOB_FIRE_SUMMON,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FIRE_SUMMON_HELP,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FIRE_FIREBALL,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FIRE_SPIT,
    EVENT_MINIGOB_ESCAPE_MINIGOB_FIRE_FLAMESTRIKE,

    EVENT_MINIGOB_ESCAPE_MINIGOB_END_0,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_1,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_2,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_3,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_4,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_5,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_6,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_7,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_8,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_9,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_10,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_11,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_12,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_13,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_14,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_15,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_16,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_17,

    EVENT_MINIGOB_ESCAPE_MINIGOB_CHECK_PLAYERS,
    EVENT_MINIGOB_ESCAPE_MINIGOB_END_INTERPHASE,
    EVENT_MINIGOB_ESCAPE_MINIGOB_WIPE
};

enum MinigobEscapeEventPhases
{
    // Minigob
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_IDLE = 1,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_INTRO,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_COMBAT,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_INTERPHASE,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_WIPE,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_END,

    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_ARCANE,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_FROST,
    EVENT_PHASE_MINIGOB_ESCAPE_MINIGOB_FIRE
};

enum MinigobEscapeEventGroups
{
    // Minigob
    EVENT_GROUP_MINIGOB_ESCAPE_MINIGOB_DEFAULT = 1,
    EVENT_GROUP_MINIGOB_ESCAPE_MINIGOB_INTRO,
    EVENT_GROUP_MINIGOB_ESCAPE_MINIGOB_COMBAT,
    EVENT_GROUP_MINIGOB_ESCAPE_MINIGOB_INTERPHASE,
    EVENT_GROUP_MINIGOB_ESCAPE_MINIGOB_END
};

enum MinigobEscapeDataIds
{
    DATA_MINIGOB_ESCAPE_COLDFLAME_GUID = 0,
    DATA_MINIGOB_ESCAPE_REMOVE_GROUP,
    DATA_MINIGOB_ESCAPE_ADD_GROUP,
    DATA_MINIGOB_ESCAPE_RESUME_FROST_COMBAT
};

enum MinigobEscapeActionIds
{
    // Minigob
    ACTION_MINIGOB_ESCAPE_MINIGOB_START = 1,
    ACTION_MINIGOB_ESCAPE_MINIGOB_ANSWER,
    ACTION_MINIGOB_ESCAPE_MINIGOB_ENGAGE_COMBAT
};

static uint8 const MinigobEscapeMinimumPhaseBit = 12u;
static uint8 const MinigobEscapeMaximumPhaseBit = 31u;
static uint32 const MinigobEscapeMinimumPhase = std::pow(2u, MinigobEscapeMinimumPhaseBit);
static float const MinigobEscapePlayersOrientation = 4.57f;

static WorldLocation const MinigobEscapeTeleportInLocation = { 0u, Position(-5114.554f, -1777.687f, 497.83578f, 1.15f) };
static WorldLocation const MinigobEscapeTeleportOutLocation = { 571u, Position(5789.215f, 770.495f, 661.28241f, 5.8f) };

static Position const MinigobEscapeMinigobPosition = { -5106.3f, -1692.1f, 497.9f, 4.9f };
static Position const MinigobEscapeRhoninPosition = { -5101.297f, -1758.386f, 497.83578f, 4.09f };
static Position const MinigobEscapeRobotPosition = { -4983.6f, -1668.2f, 503.2f, 3.72f };
static Position const MinigobEscapeTriggerPosition = { -5102.835f, -1718.764f, 497.83578f, 4.9f };
static Position const MinigobEscapeIntroPostions[] =
{
    { -5104.319f, -1750.652f, 497.83600f, 0.0f },
    { -5089.192f, -1738.867f, 497.83600f, 0.0f },
    { -5101.810f, -1714.579f, 497.83597f, 0.0f }
};
static Position const MinigobEscapeInterphasePostion = { -5082.209f, -1709.779f, 497.85349f, 4.1f };

class MinigobEscapePlayerSearcher
{
    public:
        MinigobEscapePlayerSearcher(Unit const* source, float range, uint32 questId, uint32 phaseMask) : _source(source), _range(range), _questId(questId), _phaseMask(phaseMask) { }
        MinigobEscapePlayerSearcher(Unit const* source, float range) : _source(source), _range(range), _questId(0), _phaseMask(0) { }

        bool operator()(Player* player)
        {
            if (player->IsGameMaster())
                return false;

            if (!player->IsInMap(_source) || !player->IsWithinDist(_source, _range))
                return false;

            if (_questId && player->GetQuestStatus(_questId) != QUEST_STATUS_INCOMPLETE)
                return false;

            if (_phaseMask && player->InSamePhase(_phaseMask))
                return false;

            if (MinigobEscapePlayerInfo const* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
            {
                if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_NONE)
                    return true;
            }

            return false;
        }

    private:
        Unit const* _source;
        float const _range;
        uint32 const _questId;
        uint32 const _phaseMask;
};

struct npc_minigob_escape_trigger : public ScriptedAI
{
    npc_minigob_escape_trigger(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset() override
    {
        _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_TRIGGER_SEARCH_TARGETS, Milliseconds(1));
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_MINIGOB_ESCAPE_TRIGGER_SEARCH_TARGETS)
        {
            for (auto itr = _foundPlayers.begin(); itr != _foundPlayers.end();)
            {
                Player* player = ObjectAccessor::FindConnectedPlayer(*itr);
                if (!player)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                MinigobEscapePlayerInfo const* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey);
                if (!info)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                if (!player->IsInMap(me) || !player->IsWithinDist(me, searchDistance) || player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) != QUEST_STATUS_INCOMPLETE)
                {
                    if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_RP)
                        player->SetPhaseMask(player->GetPhaseMask() & ~MinigobEscapeMinimumPhase, true);

                    itr = _foundPlayers.erase(itr);
                }
                else
                    ++itr;
            }

            std::list<Player*> players;
            MinigobEscapePlayerSearcher check(me, searchDistance, QUEST_MINIGOB_ESCAPE, MinigobEscapeMinimumPhase);
            Trinity::PlayerListSearcher<MinigobEscapePlayerSearcher> searcher(me, players, check);
            Cell::VisitWorldObjects(me, searcher, searchDistance);

            for (Player* foundPlayer : players)
            {
                if (MinigobEscapePlayerInfo* info = foundPlayer->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
                {
                    _foundPlayers.insert(foundPlayer->GetGUID());
                    foundPlayer->SetPhaseMask(foundPlayer->GetPhaseMask() | MinigobEscapeMinimumPhase, true);
                    info->Status = MINIGOBESCAPE_PLAYERINFO_STATUS_RP;
                }
            }

            me->setActive(!players.empty() || !_foundPlayers.empty());

            _events.Repeat(Seconds(1));
        }
    }

private:
    float const searchDistance = 50.0f;

    EventMap _events;
    std::unordered_set<ObjectGuid> _foundPlayers;
};

struct npc_minigob_escape_rhonin : public ScriptedAI
{
    npc_minigob_escape_rhonin(Creature* creature) : ScriptedAI(creature), _summons(creature)
    {
    }

    void Reset() override
    {
        _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_RHONIN_SEARCH_PLAYERS, Milliseconds(1));
    }

    bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
    {
        uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        if (Group const* group = player->GetGroup())
        {
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    StartEncounter(group);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    GetIntoEncounter(group, player);
                    break;
                default:
                    break;
            }
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
            TeleportOut(player);

        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    bool GossipHello(Player* player) override
    {
        player->PlayerTalkClass->ClearMenus();

        Group const* group = player->GetGroup();
        if (!group)
        {
            AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, me->GetGUID());
            return true;
        }

        if (IsGroupStored(group->GetGUID()))
        {
            if (!group->isRaidGroup() && group->GetMembersCount() >= 3 && player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE)
            {
                AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_1, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                SendGossipMenuFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_1, me->GetGUID());
                return true;
            }
            else
            {
                AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                SendGossipMenuFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, me->GetGUID());
                return true;
            }
        }

        if (!group->isRaidGroup() && group->GetMembersCount() >= 3 && player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE)
            AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        else if (player->IsGameMaster())
            AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        AddGossipItemFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        SendGossipMenuFor(player, GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0, me->GetGUID());
        return true;
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        switch (id)
        {
            case DATA_MINIGOB_ESCAPE_REMOVE_GROUP:
                EndEncounter(guid);
                break;
            default:
                break;
        }
    }

    void JustSummoned(Creature* summon) override
    {
        _summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) override
    {
        _summons.Despawn(summon);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
        if (_events.ExecuteEvent() == EVENT_MINIGOB_ESCAPE_RHONIN_SEARCH_PLAYERS)
        {
            for (auto itr = _foundPlayers.begin(); itr != _foundPlayers.end();)
            {
                Player* player = ObjectAccessor::FindConnectedPlayer(*itr);
                if (!player)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                MinigobEscapePlayerInfo const* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey);
                if (!info)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                if (!player->IsInMap(me))
                {
                    if (info->Status != 0)
                        ResetPlayerPhaseMask(player);

                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                if (!player->IsWithinDist(me, searchDistance) || player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) != QUEST_STATUS_INCOMPLETE)
                {
                    if (info->Status != 0)
                    {
                        if (Group const* group = player->GetGroup())
                        {
                            if (!IsGroupStored(group->GetGUID()))
                            {
                                TeleportOut(player, info);
                                itr = _foundPlayers.erase(itr);
                            }
                        }
                        else
                        {
                            TeleportOut(player, info);
                            itr = _foundPlayers.erase(itr);
                        }
                    }
                    else
                        itr = _foundPlayers.erase(itr);
                }
                else
                    ++itr;
            }

            std::list<Player*> players;
            MinigobEscapePlayerSearcher check(me, searchDistance + 50.f);
            Trinity::PlayerListSearcher<MinigobEscapePlayerSearcher> searcher(me, players, check);
            Cell::VisitWorldObjects(me, searcher, searchDistance + 50.f);

            for (Player* foundPlayer : players)
            {
                if (!foundPlayer->IsWithinDistInMap(me, searchDistance))
                {
                    TeleportOut(foundPlayer);
                    continue;
                }

                _foundPlayers.insert(foundPlayer->GetGUID());
            }

            me->setActive(!players.empty() || !_foundPlayers.empty());

            _events.Repeat(Seconds(1));
        }
    }

private:
    bool IsGroupStored(ObjectGuid const& groupGuid) const
    {
        return _encounters.find(groupGuid) != _encounters.end();
    }

    uint8 GetFreePhaseMaskBit() const
    {
        uint8 result = MinigobEscapeMinimumPhaseBit + 1u;
        for (auto itr = _encounters.begin(); itr != _encounters.end() && result; ++itr)
        {
            if (result <= itr->second)
                result = itr->second + 1;

            if (result > MinigobEscapeMaximumPhaseBit)
                result = 0;
        }

        return result;
    }

    void StartEncounter(Group const* group)
    {
        if (!group)
            return;

        if (_encounters.find(group->GetGUID()) != _encounters.end())
            return;

        uint8 phaseMaskBit = GetFreePhaseMaskBit();
        if (phaseMaskBit <= MinigobEscapeMinimumPhaseBit)
            return;

        _encounters.emplace(group->GetGUID(), phaseMaskBit);

        uint32 phaseMask = std::pow(2u, phaseMaskBit);

        if (TempSummon* summon = me->SummonCreature(NPC_MINIGOB_ESCAPE_MINIGOB, MinigobEscapeMinigobPosition, TEMPSUMMON_MANUAL_DESPAWN))
        {
            summon->SetPhaseMask(phaseMask, true);
            summon->AI()->SetGUID(group->GetGUID(), DATA_MINIGOB_ESCAPE_ADD_GROUP);
            summon->AI()->DoAction(ACTION_MINIGOB_ESCAPE_MINIGOB_START);
        }

        for (auto itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* member = itr->GetSource())
                if (member->IsWithinDistInMap(me, 100.0f))
                    member->SetPhaseMask(phaseMask, true);
        }
    }

    void GetIntoEncounter(Group const* group, Player* player)
    {
        if (!group || !player)
            return;

        auto itr = _encounters.find(group->GetGUID());
        if (itr != _encounters.end())
            player->SetPhaseMask(itr->second, true);
    }

    void EndEncounter(ObjectGuid const& groupGuid)
    {
        _encounters.erase(groupGuid);

        Group const* group = sGroupMgr->GetGroupByGUID(groupGuid);
        if (!group)
            return;

        for (auto itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* member = itr->GetSource())
            {
                if (!member->IsAlive())
                    member->ResurrectPlayer(1.0f);

                TeleportOut(member);
            }
        }
    }

    void ResetPlayerPhaseMask(Player* player)
    {
        if (!player)
            return;

        player->SetGameMaster(false);
    }

    void TeleportOut(Player* player, MinigobEscapePlayerInfo const* info = nullptr)
    {
        if (!player)
            return;

        if (!info)
            info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey);

        if (info && info->Status != 0)
            ResetPlayerPhaseMask(player);

        Talk(SAY_MINIGOB_ESCAPE_RHONIN_EVENT_OUT_0, player);
        DoCast(player, SPELL_MINIGOB_ESCAPE_TELEPORT_OUT);
    }

    float const searchDistance = 100.0f;

    EventMap _events;
    SummonList _summons;
    std::map<ObjectGuid, uint8> _encounters;
    std::unordered_set<ObjectGuid> _foundPlayers;
};

struct boss_minigob_escape : public ScriptedAI
{
    boss_minigob_escape(Creature* creature) : ScriptedAI(creature), _summons(creature)
    {
    }

    void Reset() override
    {
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    float const searchDistance = 50.0f;

    EventMap _events;
    SummonList _summons;
    std::unordered_set<ObjectGuid> _foundPlayers;
};

static uint32 const MinigobEscapeMechanicImmunityList = (1 << MECHANIC_SHIELD) | (1 << MECHANIC_INVULNERABILITY) | (1 << MECHANIC_IMMUNE_SHIELD);
static std::list<AuraType> const MinigobEscapeAuraImmunityList =
{
    SPELL_AURA_MOD_STEALTH,
    SPELL_AURA_MOD_INVISIBILITY,
    SPELL_AURA_SCHOOL_IMMUNITY,
    SPELL_AURA_DEFLECT_SPELLS,
    SPELL_AURA_MANA_SHIELD,
    SPELL_AURA_SCHOOL_ABSORB,
    SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL
};

class MinigobEscapeRobotTargetSearcher
{
    public:
        MinigobEscapeRobotTargetSearcher(Unit const* source, float range) : _source(source), _range(range) { }

        bool operator()(Unit* unit)
        {
            if (unit == _source)
                return false;

            if (unit->ToPlayer() && unit->ToPlayer()->IsGameMaster())
                return false;

            if (unit->IsAlive() && !_source->IsFriendlyTo(unit) && unit->IsWithinDistInMap(_source, _range))
                return true;

            return false;
        }

    private:
        Unit const* _source;
        float _range;
};

struct npc_minigob_escape_robot : public ScriptedAI
{
    npc_minigob_escape_robot(Creature* creature) : ScriptedAI(creature)
    {
        creature->setActive(true);
        creature->SetFarVisible(true);
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_ROBOT_SEARCH_TARGETS, Milliseconds(1));
    }

    void MoveInLineOfSight(Unit* /*who*/) override
    {
    }

    void AttackStart(Unit* who) override
    {
        if (!who || me->IsInCombatWith(who))
            return;

        me->Attack(who, true);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        Talk(SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_0);
        _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_ROBOT_ENGAGE_COMBAT, Seconds(2));
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        Talk(SAY_MINIGOB_ESCAPE_ROBOT_TERMINATED_0);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        _events.Reset();
        if (!me->IsAlive())
            return;

        me->RemoveAurasOnEvade();
        me->GetThreatManager().ClearAllThreat();
        me->CombatStop(true);
        me->LoadCreaturesAddon();
        me->SetLootRecipient(nullptr);
        me->ResetPlayerDamageReq();
        me->SetLastDamagedTime(0);
        me->SetCannotReachTarget(false);
        me->DoNotReacquireTarget();
        me->ModifyHealth(me->GetMaxHealth());

        Talk(SAY_MINIGOB_ESCAPE_ROBOT_RESET_0);

        Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_MINIGOB_ESCAPE_ROBOT_SEARCH_TARGETS:
                    SearchTargets();
                    if (!_targets.empty())
                        EngageTargets();

                    if (_targets.empty() && me->IsInCombat())
                        EnterEvadeMode(EvadeReason::EVADE_REASON_NO_HOSTILES);
                    else
                        _events.Repeat(Milliseconds(1));
                    break;
                case EVENT_MINIGOB_ESCAPE_ROBOT_ENGAGE_COMBAT:
                    Talk(SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_1);
                    _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_ROBOT_ROOT_TARGETS, Milliseconds(1));
                    _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE, Seconds(4));
                    _events.ScheduleEvent(EVENT_MINIGOB_ESCAPE_ROBOT_REMOVE_TARGETS_IMMUNE_AURAS, Seconds(1));
                    break;
                case EVENT_MINIGOB_ESCAPE_ROBOT_ROOT_TARGETS:
                    for (ObjectGuid const& targetGuid : _targets)
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (!target->HasAura(SPELL_MINIGOB_ESCAPE_ROBOT_MAGNETIC_FIELD))
                                me->AddAura(SPELL_MINIGOB_ESCAPE_ROBOT_MAGNETIC_FIELD, target);
                    }
                    _events.Repeat(Milliseconds(1));
                    break;
                case EVENT_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE:
                    if (ObjectGuid targetGuid = Trinity::Containers::SelectRandomContainerElement(_targets))
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (!CheckAuras(target))
                                DoCast(target, SPELL_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE);
                    }
                    _events.Repeat(Seconds(1));
                    break;
                case EVENT_MINIGOB_ESCAPE_ROBOT_REMOVE_TARGETS_IMMUNE_AURAS:
                    for (ObjectGuid const& targetGuid : _targets)
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (CheckAuras(target))
                            {
                                RemoveAuras(target);
                                Talk(SAY_MINIGOB_ESCAPE_ROBOT_REMOVE_0, target);
                            }
                    }
                    _events.Repeat(Seconds(1));
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }
    }

    void JustDied(Unit* /*killer*/) override
    {
        _events.Reset();
    }

    void LeavingWorld() override
    {
        _events.Reset();
    }

private:
    bool CheckAuras(Unit const* target) const
    {
        if (target->HasAura(31224 /*ROGUE_CLOAK_OF_SHADOWS*/) || target->HasAura(65961 /*ROGUE_CLOAK_OF_SHADOWS*/))
            return true;

        if (target->HasAuraWithMechanic(MinigobEscapeMechanicImmunityList))
            return true;

        for (AuraType type : MinigobEscapeAuraImmunityList)
        {
            if (target->HasAuraType(type))
                return true;
        }

        return false;
    }

    void RemoveAuras(Unit* target)
    {
        if (target->HasAura(31224 /*ROGUE_CLOAK_OF_SHADOWS*/))
            target->RemoveAura(31224);
        if (target->HasAura(65961 /*ROGUE_CLOAK_OF_SHADOWS*/))
            target->RemoveAura(65961);

        target->RemoveAurasWithMechanic(MinigobEscapeMechanicImmunityList);

        for (AuraType type : MinigobEscapeAuraImmunityList)
            target->RemoveAurasByType(type);
    }

    void SearchTargets()
    {
        std::list<Unit*> targetsFound;
        MinigobEscapeRobotTargetSearcher check(me, combatDistance);
        Trinity::UnitListSearcher<MinigobEscapeRobotTargetSearcher> searcher(me, targetsFound, check);
        Cell::VisitAllObjects(me, searcher, combatDistance);

        _targets.clear();
        for (Unit const* foundTarget : targetsFound)
            _targets.push_back(foundTarget->GetGUID());
    }

    void EngageTargets()
    {
        for (ObjectGuid const& targetGuid : _targets)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                AttackStart(target);
        }
    }

    float const combatDistance = 80.0f;
    EventMap _events;
    GuidVector _targets;
};

static WorldLocation const MinigobEscapeRandomTeleportLocations[] =
{
    { 1u,   Position(16220.393f, 16403.613f, -64.37996f,  6.23f) },
    { 571u, Position(5664.837f,  2013.375f,  1544.07031f, 3.58f) },
    { 1u,   Position(-8827.265f, -4758.390f, 1.91664f,    4.57f) }
};

// 47653 - Teleport Self
class spell_minigob_escape_teleport_random : public SpellScript
{
    PrepareSpellScript(spell_minigob_escape_teleport_random);

    bool Load() override
    {
        if (Unit* originalCaster = GetOriginalCaster())
        {
            if (originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_RHONIN_ORIGINAL)
                return true;
        }

        return false;
    }

    void Teleport()
    {
        PreventHitEffect(EFFECT_0);

        Player* target = GetHitPlayer();
        if (!target)
            return;

        if (target->IsGameMaster())
        {
            target->TeleportTo(Trinity::Containers::SelectRandomContainerElement(MinigobEscapeRandomTeleportLocations));
            return;
        }

        Group const* group = target->GetGroup();
        if (!group)
            return;

        bool valid = false;
        for (auto itr = group->GetFirstMember(); itr != nullptr && !valid; itr = itr->next())
        {
            if (Player* member = itr->GetSource())
                if (member->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE)
                    valid = true;
        }

        if (!valid)
            return;

        for (auto itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* member = itr->GetSource())
                if (member->IsAlive() && member->IsWithinDistInMap(GetCaster(), GetSpellInfo()->GetMaxRange()))
                    member->TeleportTo(Trinity::Containers::SelectRandomContainerElement(MinigobEscapeRandomTeleportLocations));
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_teleport_random::Teleport);
    }
};

// 40163 - Teleport
class spell_minigob_escape_teleport_in : public SpellScript
{
    PrepareSpellScript(spell_minigob_escape_teleport_in);

    bool Load() override
    {
        if (Unit* originalCaster = GetOriginalCaster())
        {
            if (originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_0 || originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_1 || originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_2)
                return true;
        }

        return false;
    }

    void Teleport()
    {
        PreventHitEffect(EFFECT_0);

        Player* target = GetHitPlayer();
        if (!target)
            return;

        target->TeleportTo(MinigobEscapeTeleportInLocation);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_teleport_in::Teleport);
    }
};

// 7791 - Teleport
class spell_minigob_escape_teleport_out : public SpellScript
{
    PrepareSpellScript(spell_minigob_escape_teleport_out);

    bool Load() override
    {
        if (Unit* originalCaster = GetOriginalCaster())
        {
            if (originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_RHONIN_EVENT)
                return true;
        }

        return false;
    }

    void Teleport()
    {
        PreventHitEffect(EFFECT_0);

        Player* target = GetHitPlayer();
        if (!target)
            return;

        target->TeleportTo(MinigobEscapeTeleportOutLocation);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_teleport_out::Teleport);
    }
};

// 51797 - Arcane Blast (Rank 1)
class spell_minigob_escape_arcane_blast : public SpellScript
{
    PrepareSpellScript(spell_minigob_escape_arcane_blast);

    bool Load() override
    {
        if (Unit* originalCaster = GetOriginalCaster())
        {
            if (originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_MINIGOB)
                return true;
        }

        return false;
    }

    void CalculateDamage()
    {
        SetHitDamage(GetHitDamage() * 18);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_arcane_blast::CalculateDamage);
    }
};

// 51743 - Overload
class spell_minigob_escape_overload : public SpellScript
{
    PrepareSpellScript(spell_minigob_escape_overload);

    bool Load() override
    {
        if (Unit* originalCaster = GetOriginalCaster())
        {
            if (originalCaster->GetEntry() == NPC_MINIGOB_ESCAPE_MINIGOB)
                return true;
        }

        return false;
    }

    void CalculateDamage()
    {
        SetHitDamage(GetHitDamage() * 10);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_overload::CalculateDamage);
    }
};

// 64668 - Magnetic Field
class spell_minigob_escape_magnetic_field : public AuraScript
{
    PrepareAuraScript(spell_minigob_escape_magnetic_field);

    bool Load() override
    {
        if (GetCaster()->GetEntry() == NPC_MINIGOB_ESCAPE_ROBOT)
            return true;

        return false;
    }

    void ModifyDuration(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        SetMaxDuration(-1);
        SetDuration(-1);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_minigob_escape_magnetic_field::ModifyDuration, EFFECT_0, SPELL_AURA_MOD_ROOT, AURA_EFFECT_HANDLE_REAL);
    }
};

class condition_minigob_event_in_group : public ConditionScript
{
    public:
        condition_minigob_event_in_group() : ConditionScript("condition_minigob_event_in_group") { }

        bool OnConditionCheck(Condition const* condition, ConditionSourceInfo& sourceInfo) override
        {
            WorldObject const* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player const* player = target->ToPlayer())
            {
                if (Group const* group = player->GetGroup())
                    if (!group->isRaidGroup() && group->GetMembersCount() >= 3)
                        return true;

                if (player->IsGameMaster())
                    return true;
            }

            return false;
        }
};

class condition_minigob_event_in_raid : public ConditionScript
{
    public:
        condition_minigob_event_in_raid() : ConditionScript("condition_minigob_event_in_raid") { }

        bool OnConditionCheck(Condition const* condition, ConditionSourceInfo& sourceInfo) override
        {
            WorldObject const* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player const* player = target->ToPlayer())
            {
                if (Group const* group = player->GetGroup())
                    if (group->isRaidGroup())
                        return true;
            }

            return false;
        }
};

class condition_minigob_event_alone : public ConditionScript
{
    public:
        condition_minigob_event_alone() : ConditionScript("condition_minigob_event_alone") { }

        bool OnConditionCheck(Condition const* condition, ConditionSourceInfo& sourceInfo) override
        {
            WorldObject const* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player const* player = target->ToPlayer())
            {
                if (player->IsGameMaster())
                    return false;

                if (Group const* group = player->GetGroup())
                {
                    if (group->GetMembersCount() < 3)
                        return true;
                }
                else
                    return true;
            }

            return false;
        }
};

void AddDalaranEventScripts()
{
    RegisterCreatureAI(npc_minigob_escape_trigger);
    RegisterCreatureAI(npc_minigob_escape_rhonin);
    RegisterCreatureAI(boss_minigob_escape);
    RegisterCreatureAI(npc_minigob_escape_robot);

    RegisterSpellScript(spell_minigob_escape_teleport_random);
    RegisterSpellScript(spell_minigob_escape_teleport_in);
    RegisterSpellScript(spell_minigob_escape_teleport_out);
    RegisterSpellScript(spell_minigob_escape_arcane_blast);
    RegisterSpellScript(spell_minigob_escape_overload);
    RegisterAuraScript(spell_minigob_escape_magnetic_field);

    new condition_minigob_event_alone();
    new condition_minigob_event_in_group();
    new condition_minigob_event_in_raid();
}
