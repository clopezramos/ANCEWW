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
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SpellAuraDefines.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Unit.h"
#include "UnitAI.h"
#include <functional>
#include <list>
#include <unordered_map>
#include <vector>

enum MinigobEscapeMisc
{
    QUEST_MINIGOB_ESCAPE = 30000,

    NPC_RHONIN_ORIGINAL = 16128,

    // Robot
    SAY_ESCAPE_DETECT_0_0 = 0,
    SAY_ESCAPE_DETECT_0_1 = 1,
    SAY_ESCAPE_RESET_0 = 2,
    SAY_ESCAPE_TERMINATED_0 = 3,
    SAY_ESCAPE_REMOVE_0 = 4,

    // Robot
    SPELL_LASER_BARRAGE = 64766,
    SPELL_LASER_BARRAGE_TRIGGER = 64769,
    SPELL_MAGNETIC_FIELD = 64668,

    // Robot
    EVENT_ENGAGE_COMBAT = 1,
    EVENT_SEARCH_TARGETS,
    EVENT_ROOT_TARGETS,
    EVENT_LASER_BARRAGE,
    EVENT_REMOVE_TARGETS_INMUNE_AURAS,
};

class RandomLocationPlayerSearcher
{
    public:
        RandomLocationPlayerSearcher(Unit const* source, float range) : _source(source), _range(range) { }

        bool operator()(Player* player)
        {
            if (player->IsGameMaster())
                return false;

            if (player->IsInMap(_source) && player->IsWithinDist(_source, _range) && player->GetQuestStatus(QUEST_MINIGOB_ESCAPE) == QUEST_STATUS_INCOMPLETE && !player->InSamePhase(_source))
            {
                if (MinigobEscapePlayerInfo* info = player->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
                    if (info->Status == MINIGOBESCAPE_PLAYERINFO_STATUS_NONE)
                        return true;
            }

            return false;
        }

    private:
        Unit const* _source;
        float _range;
};

struct npc_minigob_escape_trigger : public ScriptedAI
{
    npc_minigob_escape_trigger(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset() override
    {
        _events.ScheduleEvent(EVENT_SEARCH_TARGETS, Milliseconds(1));
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_SEARCH_TARGETS)
        {
            for (auto itr = _foundPlayers.begin(); itr != _foundPlayers.end();)
            {
                Player* player = ObjectAccessor::GetPlayer(*me, itr->first);
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
                        player->SetPhaseMask(itr->second, true);

                    itr = _foundPlayers.erase(itr);
                }
                else
                    ++itr;
            }

            std::list<Player*> players;
            RandomLocationPlayerSearcher check(me, searchDistance);
            Trinity::PlayerListSearcher<RandomLocationPlayerSearcher> searcher(me, players, check);
            Cell::VisitWorldObjects(me, searcher, searchDistance);

            for (Player* foundPlayer : players)
            {
                _foundPlayers.emplace(foundPlayer->GetGUID(), foundPlayer->GetPhaseMask());

                foundPlayer->SetPhaseMask(me->GetPhaseMask(), true);

                if (MinigobEscapePlayerInfo* info = foundPlayer->_customData.Get<MinigobEscapePlayerInfo>(minigobEscapeDataKey))
                    info->Status = MINIGOBESCAPE_PLAYERINFO_STATUS_RP;
            }

            _events.Repeat(Seconds(1));
        }
    }

private:
    float const searchDistance = 50.0f;

    EventMap _events;
    std::unordered_map<ObjectGuid, uint32 /*phaseMask*/> _foundPlayers;
};

static uint32 const MechanicImmunityList = (1 << MECHANIC_SHIELD) | (1 << MECHANIC_INVULNERABILITY) | (1 << MECHANIC_IMMUNE_SHIELD);
static std::list<AuraType> const AuraImmunityList =
{
    SPELL_AURA_MOD_STEALTH,
    SPELL_AURA_MOD_INVISIBILITY,
    SPELL_AURA_SCHOOL_IMMUNITY,
    SPELL_AURA_DEFLECT_SPELLS,
    SPELL_AURA_MANA_SHIELD,
    SPELL_AURA_SCHOOL_ABSORB,
    SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL
};

class RobotTargetSearcher
{
    public:
        RobotTargetSearcher(Unit const* source, float range) : _source(source), _range(range) { }

        bool operator()(Unit* unit)
        {
            if (unit == _source)
                return false;

            if (unit->ToPlayer() && unit->ToPlayer()->IsGameMaster())
                return false;

            if (unit->IsAlive() && !_source->IsFriendlyTo(unit) && unit->IsInMap(_source) && unit->IsWithinDist(_source, _range))
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
        _events.ScheduleEvent(EVENT_SEARCH_TARGETS, Milliseconds(1));
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
        Talk(SAY_ESCAPE_DETECT_0_0);
        _events.ScheduleEvent(EVENT_ENGAGE_COMBAT, Seconds(2));
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        Talk(SAY_ESCAPE_TERMINATED_0);
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

        Talk(SAY_ESCAPE_RESET_0);

        Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SEARCH_TARGETS:
                    SearchTargets();
                    if (!_targets.empty())
                        EngageTargets();

                    if (_targets.empty() && me->IsInCombat())
                        EnterEvadeMode(EvadeReason::EVADE_REASON_NO_HOSTILES);
                    else
                        _events.Repeat(Milliseconds(1));
                    break;
                case EVENT_ENGAGE_COMBAT:
                    Talk(SAY_ESCAPE_DETECT_0_1);
                    _events.ScheduleEvent(EVENT_ROOT_TARGETS, Milliseconds(1));
                    _events.ScheduleEvent(EVENT_LASER_BARRAGE, Seconds(4));
                    _events.ScheduleEvent(EVENT_REMOVE_TARGETS_INMUNE_AURAS, Seconds(1));
                    break;
                case EVENT_ROOT_TARGETS:
                    for (ObjectGuid targetGuid : _targets)
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (!target->HasAura(SPELL_MAGNETIC_FIELD))
                                me->AddAura(SPELL_MAGNETIC_FIELD, target);
                    }
                    _events.Repeat(Milliseconds(1));
                    break;
                case EVENT_LASER_BARRAGE:
                    if (ObjectGuid targetGuid = Trinity::Containers::SelectRandomContainerElement(_targets))
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (!CheckAuras(target))
                                DoCast(target, SPELL_LASER_BARRAGE);
                    }
                    _events.Repeat(Seconds(1));
                    break;
                case EVENT_REMOVE_TARGETS_INMUNE_AURAS:
                    for (ObjectGuid targetGuid : _targets)
                    {
                        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                            if (CheckAuras(target))
                            {
                                RemoveAuras(target);
                                Talk(SAY_ESCAPE_REMOVE_0, target);
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

        if (target->HasAuraWithMechanic(MechanicImmunityList))
            return true;

        for (AuraType type : AuraImmunityList)
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

        target->RemoveAurasWithMechanic(MechanicImmunityList);

        for (AuraType type : AuraImmunityList)
            target->RemoveAurasByType(type);
    }

    void SearchTargets()
    {
        std::list<Unit*> targetsFound;
        RobotTargetSearcher check(me, combatDistance);
        Trinity::UnitListSearcher<RobotTargetSearcher> searcher(me, targetsFound, check);
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

static WorldLocation const randomTeleport[] =
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
        if (GetCaster()->GetEntry() == NPC_RHONIN_ORIGINAL)
            return true;

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
            target->TeleportTo(randomTeleport[urand(0, 2)]);
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
                    member->TeleportTo(randomTeleport[urand(0, 2)]);
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_minigob_escape_teleport_random::Teleport);
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
    RegisterCreatureAI(npc_minigob_escape_robot);

    RegisterSpellScript(spell_minigob_escape_teleport_random);

    new condition_minigob_event_alone();
    new condition_minigob_event_in_group();
    new condition_minigob_event_in_raid();
}
