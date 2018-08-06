--
-- Cleanup
--

-- Manabonk game_event
UPDATE `game_event` SET `start_time`='0000-00-00 00:00:00', `end_time`='0000-00-00 00:00:00' WHERE `eventEntry`=33;
DELETE FROM `game_event_creature` WHERE `eventEntry` = 33 AND `guid` = 44457;
-- Manabonk spawn
DELETE FROM `creature` WHERE `guid` = 44457;

--
-- Templates
--

SET @ENTRY := 44000;
DELETE FROM `creature_template` WHERE `entry` BETWEEN @ENTRY AND @ENTRY+20;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
--                                                                     name,                                      subname, gossip_menu_id, minlevel, maxlevel, exp, faction, npcflag, speed_walk, speed_run, scale, rank, dmgschool, BaseAttackTime, RangeAttackTime, BaseVariance, RangeVariance, unit_class, unit_flags, unit_flags2, dynamicflags, family, trainer_type, trainer_spell, trainer_class, trainer_race, type, type_flags, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, PetSpellDataId, VehicleId, mingold, maxgold,    AIName, MovementType, HoverHeight, HealthModifier, ManaModifier, ArmorModifier, DamageModifier, ExperienceModifier, RacialLeader, movementId, RegenHealth, mechanic_immune_mask, spell_school_immune_mask, flags_extra,                     ScriptName,
(@ENTRY+00, 0, 0, 0, 0, 0, 28315, 0, 0, 0,              'Minigob Manacapón',                           'El mago travieso',              0,       82,       82,   2,      16,       0,    1.14286,   2.42857,     2,    3,         0,           2000,            2000,            1,             1,          8,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           2,            250,          350,             5,              1,                  1,            0,          0,           1,            650854399,                        0,           0, 'boss_minigob_manabonk_escape', 0),
(@ENTRY+01, 0, 0, 0, 0, 0, 28315, 0, 0, 0,              'Minigob Manacapón',                              'No es un clon',              0,       82,       82,   2,      16,       0,    1.14286,   2.42857,     1,    3,         0,           2000,            2000,            1,             1,          8,          0,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           2,            125,          175,             1,              1,                  1,            0,          0,           1,            617234399,                        0,           0,  'boss_minigob_manabonk_clone', 0),
(@ENTRY+02, 0, 0, 0, 0, 0, 16024, 0, 0, 0,                         'Rhonin',                        'Líder del Kirin Tor',              0,       83,       83,   2,    2007,       3,    1.14286,       1.6,     1,    3,         0,           2000,            2000,            1,             1,          2,      33538,        2048,            0,      0,            0,             0,             0,            0,    7,          4,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1000,          500,             1,             35,                  1,            0,         51,           1,            650854399,                        0,           2,     'npc_minigob_event_rhonin', 0),
(@ENTRY+03, 0, 0, 0, 0, 0, 16024, 0, 0, 0,                         'Rhonin',                        'Líder del Kirin Tor',              0,       83,       83,   2,    2007,       1,    1.14286,       1.6,     1,    3,         0,           2000,            2000,            1,             1,          2,      33538,        2048,            0,      0,            0,             0,             0,            0,    7,          4,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1000,          500,             1,             35,                  1,            0,         51,           1,            650854399,                        0,           2,                             '', 0),
(@ENTRY+04, 0, 0, 0, 0, 0, 16024, 0, 0, 0,                         'Rhonin',                        'Líder del Kirin Tor',              0,       83,       83,   2,    2007,       1,    1.14286,       1.6,     1,    3,         0,           2000,            2000,            1,             1,          2,      33538,        2048,            0,      0,            0,             0,             0,            0,    7,          4,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1000,          500,             1,             35,                  1,            0,         51,           1,            650854399,                        0,           2,                             '', 0),
(@ENTRY+05, 0, 0, 0, 0, 0, 16024, 0, 0, 0,                         'Rhonin',                        'Líder del Kirin Tor',              0,       83,       83,   2,    2007,       1,    1.14286,       1.6,     1,    3,         0,           2000,            2000,            1,             1,          2,      33538,        2048,            0,      0,            0,             0,             0,            0,    7,          4,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1000,          500,             1,             35,                  1,            0,         51,           1,            650854399,                        0,           2,                             '', 0),
(@ENTRY+06, 0, 0, 0, 0, 0, 16024, 0, 0, 0,                         'Rhonin',                        'Líder del Kirin Tor',              0,       83,       83,   2,    2007,       0,    1.14286,       1.6,     1,    3,         0,           2000,            2000,            1,             1,          2,      33538,        2048,            0,      0,            0,             0,             0,            0,    7,          4,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1000,          500,             1,             35,                  1,            0,         51,           1,            650854399,                        0,           2,                             '', 0),
(@ENTRY+07, 0, 0, 0, 0, 0, 29104, 0, 0, 0,           'Sistema de seguridad',              'Creación de Minigob Manacapón',              0,       83,       83,   2,      16,       0,        1.6,   2.42857,   1.5,    3,         0,           1000,            1000,            1,             1,          4,      32832,        2048,            0,      0,            0,             0,             0,            0,    5,     524352,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,            900,          200,             1,            100,                  1,            0,        381,           1,            650854399,                        0,           0,      'npc_minigob_event_robot', 0),
(@ENTRY+08, 0, 0, 0, 0, 0, 20578, 0, 0, 0,              'Cristiano Ronaldo',                     'Juventus Football Club',              0,       80,       80,   0,      35,       1,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,              1,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+09, 0, 0, 0, 0, 0,  3975, 0, 0, 0,                   'Amador Rivas',                             'Capitán Salami',              0,       80,       80,   0,      35,       0,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,              2,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+10, 0, 0, 0, 0, 0, 20579, 0, 0, 0,           'Pivón tomando el sol',                                           '',              0,       80,       80,   0,      35,       0,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,              1,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+11, 0, 0, 0, 0, 0, 29099, 0, 0, 0,         'Caballero de la Muerte',                              'Equipo Arthas',              0,       80,       80,   2,     974,       0,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,            100,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+12, 0, 0, 0, 0, 0, 20130, 0, 0, 0,               'Campeón Illidari',                             'Equipo Illidan',              0,       80,       80,   2,    1882,       0,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,            100,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+13, 0, 0, 0, 0, 0,  1928, 0, 0, 0,                'Fermín Trujillo',                                 'TeleEspeto',              0,       80,       80,   0,      35,       0,    1.14286,       1.6,     1,    0,         0,           2000,            2200,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    7,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,              1,            1,             1,              1,                  1,            0,          0,           1,                    0,                        0,           0,                             '', 0),
(@ENTRY+14, 0, 0, 0, 0, 0, 27571, 0, 0, 0,             'Illidan Tempestira',                                 'El Traidor',              0,       83,       83,   2,    1882,       0,          2,   2.42857,     1,    3,         0,           1500,            2000,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    3,        108,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,            560,          150,             1,             35,                  1,            0,        188,           1,            650854399,                        0,           0,                             '', 0),
(@ENTRY+15, 0, 0, 0, 0, 0, 30721, 0, 0, 0,                 'El Rey Exánime',                                           '',              0,       83,       83,   2,     974,       0,          2,   2.42857,     1,    3,         0,           1500,            2000,            1,             1,          2,        770,        2048,            0,      0,            0,             0,             0,            0,    6,        108,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0, 'SmartAI',            0,           1,            560,          200,             1,             35,                  1,            0,        188,           1,            650854399,                        0,           0,                             '', 0),
(@ENTRY+16, 0, 0, 0, 0, 0,   169, 0, 0, 0,                         'Rhonin',                                           '',              0,       80,       80,   0,     114,       0,          1,         1,     1,    0,         0,           2000,            2000,            1,             1,          1,   33555200,        2048,            0,      0,            0,             0,             0,            0,   10,   16778240,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1.35,            1,             1,            7.5,                  1,            0,          0,           1,                    0,                        0,         130,                             '', 0),
(@ENTRY+17, 0, 0, 0, 0, 0,   169, 0, 0, 0,                        'Trigger',                                           '',              0,       80,       80,   0,     114,       0,          1,         1,     1,    0,         0,           2000,            2000,            1,             1,          1,   33555200,        2048,            0,      0,            0,             0,             0,            0,   10,   16778240,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1.35,            1,             1,            7.5,                  1,            0,          0,           1,                    0,                        0,         130,    'npc_minigob_event_trigger', 0),
(@ENTRY+18, 0, 0, 0, 0, 0,   169, 0, 0, 0,           'Sistema de seguridad',                                           '',              0,       80,       80,   0,     114,       0,          1,         1,     1,    0,         0,           2000,            2000,            1,             1,          1,   33555200,        2048,            0,      0,            0,             0,             0,            0,   10,   16778240,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,           1.35,            1,             1,            7.5,                  1,            0,          0,           1,                    0,                        0,         130,                             '', 0),
(@ENTRY+19, 0, 0, 0, 0, 0, 21910, 0, 0, 0,             'Elemental de Hielo', 'Esbirro de emergencia de Minigob Manacapón',              0,       82,       82,   2,      16,       0,    1.14286,   1.14286,     3,    3,         0,           2000,            2000,            1,             1,          1,          0,        2048,            0,      0,            0,             0,             0,            0,    4,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,             75,          125,             1,             15,                  1,            0,          0,           1,            650854399,                        0,           0,                             '', 0),
(@ENTRY+20, 0, 0, 0, 0, 0,  5488, 0, 0, 0, 'Elemental de Fuego esclavizado',                                           '',              0,       82,       82,   2,      16,       0,    1.14286,   1.14286,     2,    3,         0,           2000,            2000,            1,             1,          2,          0,        2048,            0,      0,            0,             0,             0,            0,    4,          0,      0,              0,        0,           0,           0,           0,           0,           0,           0,      0,      0,      0,      0,      0,      0,      0,      0,              0,         0,       0,       0,        '',            0,           1,            100,          150,             1,              1,                  1,            0,          0,           1,            650854399,                        0,           0,                             '', 0);

DELETE FROM `creature_template_addon` WHERE `entry` IN (@ENTRY+09, @ENTRY+04, @ENTRY+11, @ENTRY+12, @ENTRY+14, @ENTRY+15);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@ENTRY+09, 0, 0, 0, 0,  10,    NULL), -- Amador Rivas
(@ENTRY+04, 0, 0, 0, 0,   0, '57626'), -- Rhonin 2
(@ENTRY+11, 0, 0, 0, 0, 333,    NULL), -- Caballero de la Muerte
(@ENTRY+12, 0, 0, 0, 0, 333,    NULL), -- Campeón Illidari
(@ENTRY+14, 0, 0, 0, 0, 333,    NULL), -- Illidan Tempestira
(@ENTRY+15, 0, 0, 0, 0, 333,    NULL); -- El Rey Exánime

DELETE FROM `creature_equip_template` WHERE `CreatureID` IN (@ENTRY+11, @ENTRY+12, @ENTRY+14, @ENTRY+15);
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(@ENTRY+11, 1, 49623,     0, 0, 0), -- Caballero de la Muerte
(@ENTRY+12, 1, 30208, 30209, 0, 0), -- Campeón Illidari
(@ENTRY+14, 1, 32837, 32838, 0, 0), -- Illidan Tempestira
(@ENTRY+15, 1, 36942,     0, 0, 0); -- El Rey Exánime
