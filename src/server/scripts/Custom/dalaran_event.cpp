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
#include "GroupReference.h"
#include "Object.h"
#include "ObjectAccessor.h"
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

    NPC_MINIGOB_ESCAPE_RHONIN_ORIGINAL = 16128,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_0 = 44003,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_1 = 44004,
    NPC_MINIGOB_ESCAPE_RHONIN_RANDOM_2 = 44005,
    NPC_MINIGOB_ESCAPE_RHONIN_EVENT = 44002,
    NPC_MINIGOB_ESCAPE_ROBOT = 44007,
    NPC_MINIGOB_ESCAPE_MINIGOB = 44000,

    // Robot
    SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_0 = 0,
    SAY_MINIGOB_ESCAPE_ROBOT_DETECT_0_1 = 1,
    SAY_MINIGOB_ESCAPE_ROBOT_RESET_0 = 2,
    SAY_MINIGOB_ESCAPE_ROBOT_TERMINATED_0 = 3,
    SAY_MINIGOB_ESCAPE_ROBOT_REMOVE_0 = 4,

    // Rhonin (Event)
    SAY_MINIGOB_ESCAPE_RHONIN_OUT_0 = 0,

    // Rhonin (Event)
    GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_0 = 44004,
    GOSSIP_MENU_MINIGOB_ESCAPE_RHONIN_1 = 44005,

    // Teleports
    SPELL_MINIGOB_ESCAPE_TELEPORT_RANDOM_0 = 62940,
    SPELL_MINIGOB_ESCAPE_TELEPORT_RANDOM_1 = 47653,
    SPELL_MINIGOB_ESCAPE_TELEPORT_IN = 40163,
    SPELL_MINIGOB_ESCAPE_TELEPORT_OUT = 7791,

    // Robot
    SPELL_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE = 64766,
    SPELL_MINIGOB_ESCAPE_ROBOT_LASER_BARRAGE_TRIGGER = 64769,
    SPELL_MINIGOB_ESCAPE_ROBOT_MAGNETIC_FIELD = 64668,
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
};

enum MinigobEscapeDataIds
{
    DATA_MINIGOB_ESCAPE_REMOVE_GROUP = 1,
};

static uint8 const MinigobEscapeMinimumPhaseBit = 12;
static uint8 const MinigobEscapeMaximumPhaseBit = 31;
static uint32 const MinigobEscapeMinimumPhase = std::pow(2u, MinigobEscapeMinimumPhaseBit);

static WorldLocation const MinigobEscapeTeleportIn = { 0u, Position(-5114.554f, -1777.687f, 497.83578f, 1.15f) };
static WorldLocation const MinigobEscapeTeleportOut = { 571u, Position(5789.215f, 770.495f, 661.28241f, 5.8f) };

class MinigobEscapePlayerSearcher
{
    public:
        MinigobEscapePlayerSearcher(Unit const* source, float range, uint32 phaseMask) : _source(source), _range(range), _phaseMask(phaseMask) { }

        bool operator()(Player* player)
        {
            if (player->IsGameMaster())
                return false;

            if (player->IsInMap(_source) && player->IsWithinDist(_source, _range) && player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE && !player->InSamePhase(_phaseMask))
            {
                if (MinigobEscapePlayerInfo* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
                    if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_NONE)
                        return true;
            }

            return false;
        }

    private:
        Unit const* _source;
        float const _range;
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
                Player* player = ObjectAccessor::GetPlayer(*me, *itr);
                if (!player)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                MinigobEscapePlayerInfo* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey);
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
            MinigobEscapePlayerSearcher check(me, searchDistance, MinigobEscapeMinimumPhase);
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

            me->setActive(players.empty() && _foundPlayers.empty());

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
        if (Group* group = player->GetGroup())
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

        Group* group = player->GetGroup();
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

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
        if (_events.ExecuteEvent() == EVENT_MINIGOB_ESCAPE_RHONIN_SEARCH_PLAYERS)
        {
            for (auto itr = _foundPlayers.begin(); itr != _foundPlayers.end();)
            {
                Player* player = ObjectAccessor::GetPlayer(*me, *itr);
                if (!player)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                MinigobEscapePlayerInfo* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey);
                if (!info)
                {
                    itr = _foundPlayers.erase(itr);
                    continue;
                }

                if (!player->IsInMap(me))
                {
                    if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_EVENT)
                        ResetPlayerPhaseMask(player);

                    itr = _foundPlayers.erase(itr);
                }
                else if (!player->IsWithinDist(me, searchDistance) || player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) != QUEST_STATUS_INCOMPLETE)
                {
                    if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_EVENT)
                        TeleportOut(player, info);

                    itr = _foundPlayers.erase(itr);
                }
                else
                    ++itr;
            }

            std::list<Player*> players;
            MinigobEscapePlayerSearcher check(me, searchDistance + 50.f, MinigobEscapeMinimumPhase);
            Trinity::PlayerListSearcher<MinigobEscapePlayerSearcher> searcher(me, players, check);
            Cell::VisitWorldObjects(me, searcher, searchDistance + 50.f);

            for (Player* foundPlayer : players)
            {
                if (!foundPlayer->IsWithinDistInMap(me, searchDistance))
                {
                    TeleportOut(foundPlayer);
                    continue;
                }

                if (MinigobEscapePlayerInfo* info = foundPlayer->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
                {
                    _foundPlayers.insert(foundPlayer->GetGUID());
                    foundPlayer->SetPhaseMask(MinigobEscapeMinimumPhase, true);
                    info->Status = MINIGOBESCAPE_PLAYERINFO_STATUS_EVENT;
                }
            }

            me->setActive(players.empty());

            _events.Repeat(Seconds(1));
        }
    }

private:
    bool IsGroupStored(ObjectGuid const& groupGuid) const
    {
        return _encounters.find(groupGuid) != _encounters.end();
    }

    uint32 GetFreePhaseMask() const
    {
        uint8 result = MinigobEscapeMinimumPhaseBit + 1u;
        for (auto itr = _encounters.begin(); itr != _encounters.end() && result; ++itr)
        {
            if (result <= itr->second)
                result = itr->second + 1;

            if (result > MinigobEscapeMaximumPhaseBit)
                result = 0;
        }

        return std::pow(2u, result);
    }

    void StartEncounter(Group const* group) { }
    void GetIntoEncounter(Group const* group, Player const* player) { }
    void EndEncounter(ObjectGuid const& groupGuid) { }

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

        if (info && info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_EVENT)
            ResetPlayerPhaseMask(player);

        Talk(SAY_MINIGOB_ESCAPE_RHONIN_OUT_0, player);
        DoCast(player, SPELL_MINIGOB_ESCAPE_TELEPORT_OUT);
    }

    float const searchDistance = 100.0f;

    EventMap _events;
    SummonList _summons;
    std::map<ObjectGuid, uint8> _encounters;
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
                    for (ObjectGuid targetGuid : _targets)
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
                    for (ObjectGuid targetGuid : _targets)
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
    bool CheckAuras(Unit* target)  const
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
        for (Unit* foundTarget : targetsFound)
            _targets.push_back(foundTarget->GetGUID());
    }

    void EngageTargets()
    {
        for (ObjectGuid targetGuid : _targets)
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

        Group* group = target->GetGroup();
        if (!group)
            return;

        bool valid = false;
        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr && !valid; itr = itr->next())
        {
            if (Player* member = itr->GetSource())
                if (member->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE)
                    valid = true;
        }

        if (!valid)
            return;

        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
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

        target->TeleportTo(MinigobEscapeTeleportIn);
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

        target->TeleportTo(MinigobEscapeTeleportOut);
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
            WorldObject* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player* player = target->ToPlayer())
            {
                if (Group* group = player->GetGroup())
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
            WorldObject* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player* player = target->ToPlayer())
            {
                if (Group* group = player->GetGroup())
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
            WorldObject* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (Player* player = target->ToPlayer())
            {
                if (player->IsGameMaster())
                    return false;

                if (Group* group = player->GetGroup())
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
