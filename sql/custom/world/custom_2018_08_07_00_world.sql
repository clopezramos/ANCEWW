--
-- Rhonin (original)
--

-- New gossip condition - must have quest 30000
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=10140 AND `SourceEntry`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15, 10140, 0, 0, 0, 9, 0, 30000, 0, 0, 0, "", "Manabonk Escape - Rhonin (original) - gossip_menu_option OptionID 9 - quest 30000");

-- Assign quest
DELETE FROM `creature_queststarter` WHERE (quest = 30000) AND (id IN (16128));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(16128, 30000);

-- Updated SAI

-- Original SAI (for reference)
-- DELETE FROM `smart_scripts` WHERE (`entryorguid`=16128 AND `source_type`=0);
-- INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- (16128, 0, 0, 5, 20, 0, 100, 512, 13631,     0, 0, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0, 0, "Rhonin - On Quest Reward (All Is Well That Ends Well) - Run Script"),
-- (16128, 0, 1, 5, 20, 0, 100, 512, 13819,     0, 0, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0, 0, "Rhonin - On Quest Reward (Heroic: All Is Well That Ends Well) - Run Script"),
-- (16128, 0, 2, 0, 38, 0, 100, 512,     1,     1, 0, 0, 0, 80, 1612801, 2, 0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0, 0, "Rhonin - On Data Set 1 1 - Run Script 2"),
-- (16128, 0, 3, 0, 40, 0, 100, 512,    11, 16128, 0, 0, 0, 80, 1612802, 2, 0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0, 0, "Rhonin - On Reached WP11 - Run Script 3"),
-- (16128, 0, 4, 5, 38, 0, 100, 512,     6,     6, 0, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0, 0, "Rhonin - On Data Set 6 6 - Run Script"),
-- (16128, 0, 5, 0, 61, 0, 100, 512,     0,     0, 0, 0, 0, 45,       1, 1, 0, 0, 0, 0, 19, 30116, 0, 0, 0, 0, 0, 0, "Rhonin - Link - Set Data to Archmage Aethas Sunreaver <Kirin Tor>");

-- DELETE FROM `smart_scripts` WHERE (`entryorguid`=1612800 AND `source_type`=9);
-- INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
-- (1612800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 81,     0, 0,      0, 0, 0, 0, 1, 0, 0, 0,      0,       0,       0,       0, "Rhonin - Script - Set NPC Flags"),
-- (1612800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 34044, 1, 300000, 0, 0, 0, 8, 0, 0, 0, 5800.3, 822.453, 668.519, 4.81281, "Rhonin - Script - Summon Brann Bronzebeard"),
-- (1612800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0,  1,     0, 0,      0, 0, 0, 0, 8, 0, 0, 0,      0,       0,       0,       0, "Rhonin - Script - Say Line 1");

-- DELETE FROM `smart_scripts` WHERE (`id`=1612801);
-- INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- (1612801, 9, 0, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 53, 0, 16128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rhonin - Script 2 - Start WP');

SET @ENTRY := 16128;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY, 0, 0,   5, 20, 0, 100, 512, 13631,     0, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Quest 'All Is Well That Ends Well' Finished - Run Script"),
(@ENTRY, 0, 1,   5, 20, 0, 100, 512, 13819,     0, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Quest 'Heroic: All Is Well That Ends Well' Finished - Run Script"),
(@ENTRY, 0, 2,   0, 38, 0, 100, 512,     1,     1, 0, 0, 80, 1612801, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Data Set 1 1 - Run Script"),
(@ENTRY, 0, 3,   0, 40, 0, 100, 512,    11, 16128, 0, 0, 80, 1612802, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Waypoint 11 Reached - Run Script"),
(@ENTRY, 0, 4,   5, 38, 0, 100, 512,     6,     6, 0, 0, 80, 1612800, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Data Set 6 6 - Run Script"),
(@ENTRY, 0, 5,   0, 61, 0, 100, 512,     0,     0, 0, 0, 45,       1, 1, 0, 0, 0, 0, 19, 30116,  0, 0, 0, 0, 0, 0, "Rhonin - On Link - Set Data 1 1"),
(@ENTRY, 0, 6,  12, 62, 0, 100, 512, 10140,     0, 0, 0, 64,       1, 0, 0, 0, 0, 0,  7,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Gossip Option 0 Selected - Store Targetlist"),
(@ENTRY, 0, 7,  13, 38, 0, 100, 512,     9,     1, 0, 0, 11,   62940, 0, 0, 0, 0, 0, 12,     1,  0, 0, 0, 0, 0, 0, "Rhonin - On Data Set 9 1 - Cast 'Teleport'"),
(@ENTRY, 0, 8,   9, 61, 0, 100, 512,     0,     0, 0, 0, 72,       0, 0, 0, 0, 0, 0, 17,     0, 20, 0, 0, 0, 0, 0, "Rhonin - On Link - Close Gossip"),
(@ENTRY, 0, 9,   0, 61, 0, 100, 512,     0,     0, 0, 0, 80, 1612809, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Link - Run Script"),
(@ENTRY, 0, 10,  0, 62, 0, 100, 512, 10140,     0, 0, 0, 80, 1612810, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Gossip Option 0 Selected - Run Script"),
(@ENTRY, 0, 11,  0, 62, 0, 100, 512, 10140,     0, 0, 0, 80, 1612811, 2, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Gossip Option 0 Selected - Run Script"),
(@ENTRY, 0, 12,  8, 61, 0, 100, 512,     0,     0, 0, 0, 83,       1, 0, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Link - Remove Npc Flag Gossip"),
(@ENTRY, 0, 13,  0, 61, 0, 100, 512,     0,     0, 0, 0, 82,       1, 0, 0, 0, 0, 0,  1,     0,  0, 0, 0, 0, 0, 0, "Rhonin - On Link - Add Npc Flag Gossip");

SET @ENTRY := 1612809;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY, 9, 0, 0, 0, 0, 100, 0,    0,    0, 0, 0,  1, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Say Line 8"),
(@ENTRY, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0,  1, 9, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Say Line 9"),
(@ENTRY, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 45, 9, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Set Data 9 1");

SET @ENTRY := 1612810;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY, 9, 0, 0, 0, 0, 100, 0,    0,    0, 0, 0, 1, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rhonin - On Script - Say Line 10"),
(@ENTRY, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 1, 11, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rhonin - On Script - Say Line 11");

SET @ENTRY := 1612811;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 12, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rhonin - On Script - Say Line 12");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=7 AND `SourceEntry`=16128;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=12 AND `SourceEntry`=16128;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=11 AND `SourceEntry`=16128;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,  7, 16128, 0, 0, 32, 0, 0x0010, 0, 0, 0, "condition_minigob_event_in_group", ""),
(22, 12, 16128, 0, 0, 32, 0, 0x0010, 0, 0, 0,  "condition_minigob_event_in_raid", ""),
(22, 11, 16128, 0, 0, 32, 0, 0x0010, 0, 0, 0,    "condition_minigob_event_alone", "");

-- Random teleport spell
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (62940);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(62940, 47653, 1, 'Minigob Escape - Teleport Random');

DELETE FROM `spell_script_names` WHERE `spell_id` IN ('spell_minigob_escape_teleport_random');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(47653, 'spell_minigob_escape_teleport_random');
