/*
 * Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#ifndef _ELUNA_CREATURE_AI_H
#define _ELUNA_CREATURE_AI_H

#include "LuaEngine.h"

struct ScriptedAI;

struct ElunaCreatureAI : ScriptedAI
{
    // used to delay the spawn hook triggering on AI creation
    bool justSpawned;
    // used to delay movementinform hook (WP hook)
    std::vector< std::pair<uint32, uint32> > movepoints;

    ElunaCreatureAI(Creature* creature) : ScriptedAI(creature), justSpawned(true)
    {
    }
    ~ElunaCreatureAI() { }

    //Called at World update tick
    void UpdateAI(uint32 diff) override
    {
        if (justSpawned)
        {
            justSpawned = false;

            JustRespawned();
        }

        if (!movepoints.empty())
        {
            for (auto& point : movepoints)
            {
                if (!sEluna->MovementInform(me, point.first, point.second))
                    ScriptedAI::MovementInform(point.first, point.second);
            }
            movepoints.clear();
        }

        if (!sEluna->UpdateAI(me, diff))
        {
            if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                ScriptedAI::UpdateAI(diff);
        }
    }

    // Called for reaction when initially engaged - this will always happen _after_ JustEnteredCombat
    // Called at creature aggro either by MoveInLOS or Attack Start
    void JustEngagedWith(Unit* target) override
    {
        if (!sEluna->EnterCombat(me, target))
            ScriptedAI::JustEngagedWith(target);
    }

    // Called at any Damage from any attacker (before damage apply)
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask) override
    {
        if (!sEluna->DamageTaken(me, attacker, damage))
        {
            ScriptedAI::DamageTaken(attacker, damage, damagetype, damageSchoolMask);
        }
    }

    //Called at creature death
    void JustDied(Unit* killer) override
    {
        if (!sEluna->JustDied(me, killer))
            ScriptedAI::JustDied(killer);
    }

    //Called at creature killing another unit
    void KilledUnit(Unit* victim) override
    {
        if (!sEluna->KilledUnit(me, victim))
            ScriptedAI::KilledUnit(victim);
    }

    // Called when the creature summon successfully other creature
    void JustSummoned(Creature* summon) override
    {
        if (!sEluna->JustSummoned(me, summon))
            ScriptedAI::JustSummoned(summon);
    }

    // Called when a summoned creature is despawned
    void SummonedCreatureDespawn(Creature* summon) override
    {
        if (!sEluna->SummonedCreatureDespawn(me, summon))
            ScriptedAI::SummonedCreatureDespawn(summon);
    }

    //Called at waypoint reached or PointMovement end
    void MovementInform(uint32 type, uint32 id) override
    {
        // delayed since hook triggers before actually reaching the point
        // and starting new movement would bug
        movepoints.push_back(std::make_pair(type, id));
    }

    // Called before EnterCombat even before the creature is in combat.
    void AttackStart(Unit* target) override
    {
        if (!sEluna->AttackStart(me, target))
            ScriptedAI::AttackStart(target);
    }

    // Called for reaction at stopping attack at no attackers or targets
    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        if (!sEluna->EnterEvadeMode(me))
            ScriptedAI::EnterEvadeMode();
    }

    // Called when creature is spawned or respawned (for reseting variables)
    void JustRespawned() override
    {
        if (!sEluna->JustRespawned(me))
            ScriptedAI::JustRespawned();
    }

    // Called at reaching home after evade
    void JustReachedHome() override
    {
        if (!sEluna->JustReachedHome(me))
            ScriptedAI::JustReachedHome();
    }

    // Called at text emote receive from player
    void ReceiveEmote(Player* player, uint32 emoteId) override
    {
        if (!sEluna->ReceiveEmote(me, player, emoteId))
            ScriptedAI::ReceiveEmote(player, emoteId);
    }

    // called when the corpse of this creature gets removed
    void CorpseRemoved(uint32& respawnDelay) override
    {
        if (!sEluna->CorpseRemoved(me, respawnDelay))
            ScriptedAI::CorpseRemoved(respawnDelay);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!sEluna->MoveInLineOfSight(me, who))
            ScriptedAI::MoveInLineOfSight(who);
    }

    // Called when hit by a spell
    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (!sEluna->SpellHit(me, caster, spell))
            ScriptedAI::SpellHit(caster, spell);
    }

    // Called when spell hits a target
    void SpellHitTarget(Unit* target, SpellInfo const* spell) override
    {
        if (!sEluna->SpellHitTarget(me, target, spell))
            ScriptedAI::SpellHitTarget(target, spell);
    }

    // Called when the creature is summoned successfully by other creature
    void IsSummonedBy(WorldObject* summoner) override
    {
        if (!summoner->ToUnit() || !sEluna->OnSummoned(me, summoner->ToUnit()))
            ScriptedAI::IsSummonedBy(summoner);
    }

    void SummonedCreatureDies(Creature* summon, Unit* killer) override
    {
        if (!sEluna->SummonedCreatureDies(me, summon, killer))
            ScriptedAI::SummonedCreatureDies(summon, killer);
    }

    // Called when owner takes damage
    void OwnerAttackedBy(Unit* attacker) override
    {
        if (!sEluna->OwnerAttackedBy(me, attacker))
            ScriptedAI::OwnerAttackedBy(attacker);
    }

    // Called when owner attacks something
    void OwnerAttacked(Unit* target) override
    {
        if (!sEluna->OwnerAttacked(me, target))
            ScriptedAI::OwnerAttacked(target);
    }
};

#endif
