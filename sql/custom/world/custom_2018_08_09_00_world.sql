SET @ENTRY := 44000;

-- Cristiano Ronaldo SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+08;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+08 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+08, 0, 0, 0,  1, 0, 100, 0,      4000,  4000, 10000, 20000,  1,     0,    0, 0, 0, 0, 0,  1, 0,  0, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - Out of Combat - Say Line 0"),
(@ENTRY+08, 0, 1, 2, 62, 0, 100, 0, @ENTRY+00,    1,      0,     0, 11, 61834,    0, 0, 0, 0, 0,  7, 0,  0, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Gossip Option 1 Selected - Cast 'Manabonked!'"),
(@ENTRY+08, 0, 2, 4, 61, 0, 100, 0,         0,    0,      0,     0,  1,     1,    0, 0, 0, 0, 0,  1, 0,  0, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Say Line 1"),
(@ENTRY+08, 0, 3, 5, 62, 0, 100, 0, @ENTRY+00,    0,      0,     0, 72,     2,    0, 0, 0, 0, 0, 17, 0, 20, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Gossip Option 0 Selected - Close Gossip"),
(@ENTRY+08, 0, 4, 0, 61, 0, 100, 0,         0,    0,      0,     0, 72,     0,    0, 0, 0, 0, 0, 17, 0, 20, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Close Gossip"),
(@ENTRY+08, 0, 5, 6, 61, 0, 100, 0,         0,    0,      0,     0,  1,     2, 1000, 0, 0, 0, 0,  1, 0,  0, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Say Line 2"),
(@ENTRY+08, 0, 6, 7, 61, 0, 100, 0,         0,    0,      0,     0,  5,    15,    0, 0, 0, 0, 0,  1, 0,  0, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Play Emote 15"),
(@ENTRY+08, 0, 7, 8, 61, 0, 100, 0,         0,    0,      0,     0, 75,  2825,    0, 0, 0, 0, 0, 17, 0, 20, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Add Aura 'Bloodlust'"),
(@ENTRY+08, 0, 8, 0, 61, 0, 100, 0,         0,    0,      0,     0,  4, 10030,    0, 0, 0, 0, 0, 17, 0, 20, 0, 0, 0, 0, 0, "Minigob Escape - Cristiano Ronaldo - On Link - Play Sound 10030");

-- Rhonin I SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+03;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+03 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+03, 0, 0, 1, 62, 0, 100, 0, @ENTRY+01, 0, 0, 0, 64,               1, 0, 0, 0, 0, 0,  7, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Gossip Option 0 Selected - Store Targetlist"),
(@ENTRY+03, 0, 1, 2, 61, 0, 100, 0,         0, 0, 0, 0, 72,               0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Close Gossip"),
(@ENTRY+03, 0, 2, 3, 61, 0, 100, 0,         0, 0, 0, 0,  1,               0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Say Line 0"),
(@ENTRY+03, 0, 3, 0, 61, 0, 100, 0,         0, 0, 0, 0, 11,           40163, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Cast 'Teleport'"),
(@ENTRY+03, 0, 4, 5, 62, 0, 100, 0, @ENTRY+01, 1, 0, 0, 64,               2, 0, 0, 0, 0, 0,  7, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Gossip Option 1 Selected - Store Targetlist"),
(@ENTRY+03, 0, 5, 6, 61, 0, 100, 0,         0, 0, 0, 0, 72,               0, 0, 0, 0, 0, 0, 12, 2, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Close Gossip"),
(@ENTRY+03, 0, 6, 0, 61, 0, 100, 0,         0, 0, 0, 0, 80, (@ENTRY+03)*100, 2, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Run Script"),
(@ENTRY+03, 0, 7, 0, 38, 0, 100, 0,         1, 1, 0, 0, 11,           40163, 0, 0, 0, 0, 0, 12, 2, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Data Set 1 1 - Cast 'Teleport'");

DELETE FROM `smart_scripts` WHERE `entryorguid`=(@ENTRY+03)*100 AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
((@ENTRY+03)*100, 9, 0, 0, 0, 0, 100, 0,    0,    0, 0, 0,  1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Script - Say Line 1"),
((@ENTRY+03)*100, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0,  1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Script - Say Line 1"),
((@ENTRY+03)*100, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 45, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Script - Set Data 1 1");

-- Amador SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+09;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+09 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+09, 0, 0, 0, 1, 0, 100, 0, 0, 0, 5000, 10000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Amador - Out of Combat - Say Line 0");

-- Fermín SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+13;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+13 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+13, 0, 0, 0, 1, 0, 100, 0, 0, 0, 5000, 25000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Fermín - Out of Combat - Say Line 0");

-- Pivón tomando el sol SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+10;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+10 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+10, 0, 0, 0, 1, 0, 100, 0, 0,      0, 5000,  10000,  1,     0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Pivón tomando el sol - Out of Combat - Say Line 0"),
(@ENTRY+10, 0, 1, 0, 1, 0, 100, 0, 0, 120000,    0, 300000, 11, 57626, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Pivón tomando el sol - Out of Combat - Cast 'Feign Death'"),
(@ENTRY+10, 0, 2, 0, 1, 0, 100, 0, 0, 120000,    0, 300000, 28, 57626, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Pivón tomando el sol - Out of Combat - Remove Aura 'Feign Death'");

-- Rhonin (Playa) SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+04;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+04 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+04, 0, 0, 1, 62, 0, 100, 0, @ENTRY+02, 0, 0, 0, 64,     1, 0, 0, 0, 0, 0,  7, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Gossip Option 0 Selected - Store Targetlist"),
(@ENTRY+04, 0, 1, 2, 61, 0, 100, 0,         0, 0, 0, 0, 72,     0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Close Gossip"),
(@ENTRY+04, 0, 2, 3, 61, 0, 100, 0,         0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Say Line 0"),
(@ENTRY+04, 0, 3, 0, 61, 0, 100, 0,         0, 0, 0, 0, 11, 40163, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Cast 'Teleport'");

-- Rhonin (ICC) SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+05;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+05 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+05, 0, 0, 1, 62, 0, 100, 0, @ENTRY+03, 0, 0, 0, 64,     1, 0, 0, 0, 0, 0,  7, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Gossip Option 0 Selected - Store Targetlist"),
(@ENTRY+05, 0, 1, 2, 61, 0, 100, 0,         0, 0, 0, 0, 72,     0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Close Gossip"),
(@ENTRY+05, 0, 2, 3, 61, 0, 100, 0,         0, 0, 0, 0,  1,     0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Say Line 0"),
(@ENTRY+05, 0, 3, 0, 61, 0, 100, 0,         0, 0, 0, 0, 11, 40163, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, "Minigob Escape - Rhonin - On Link - Cast 'Teleport'");

-- Caballero de la Muerte SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+11;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+11 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+11, 0, 0, 0, 1, 0, 100, 0, 0, 15000, 15000, 60000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Caballero de la Muerte - Out of Combat - Say Line 0");

-- Campeón Illidari SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+12;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+12 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+12, 0, 0, 0, 1, 0, 100, 0, 0, 15000, 15000, 60000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Campeón Illidari - Out of Combat - Say Line 0");

-- El Rey Exánime SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+15;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+15 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+15, 0, 0, 0, 1, 0, 100, 0, 0, 15000, 15000, 60000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - El Rey Exánime - Out of Combat - Say Line 0");

-- Illidan Tempestira SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+14;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+14 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+14, 0, 0, 0, 1, 0, 100, 0, 0, 15000, 15000, 60000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Illidan Tempestira - Out of Combat - Say Line 0");

-- Elemental de Hielo SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+19;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+19 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+19, 0, 0, 0, 0, 0, 100, 0, 5000,  5000, 15000, 15000, 11, 16869, 32, 0, 0, 0, 0,  6, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Hielo - In Combat - Cast 'Ice Tomb'"),
(@ENTRY+19, 0, 1, 0, 0, 0, 100, 0, 3000,  3000,  5000, 10000, 11, 72641,  0, 0, 0, 0, 0,  2, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Hielo - In Combat - Cast 'Frost Breath'"),
(@ENTRY+19, 0, 2, 0, 0, 0, 100, 0, 5000, 10000,  5000, 10000, 11, 72905,  2, 0, 0, 0, 1,  0, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Hielo - In Combat - Cast 'Frostbolt Volley'"),
(@ENTRY+19, 0, 3, 0, 4, 0, 100, 0,    0,     0,     0,     0, 11, 69187,  0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Hielo - On Aggro - Cast 'Frost Spiked Armor'"),
(@ENTRY+19, 0, 4, 0, 0, 0, 100, 0, 3000,  5000,  3000,  5000, 11, 65807,  2, 0, 0, 0, 0,  5, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Hielo - In Combat - Cast 'Frostbolt'");

-- Elemental de Fuego esclavizado SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+20;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+20 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+20, 0, 0, 0,  0, 0, 100, 0,  3000,  3000, 10000, 10000, 11, 56934, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Fuego esclavizado - In Combat - Cast 'Firebomb'"),
(@ENTRY+20, 0, 1, 0,  4, 0, 100, 0,     0,     0,     0,     0, 11, 47075, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Fuego esclavizado - On Aggro - Cast 'Immolation'"),
(@ENTRY+20, 0, 2, 0,  0, 0, 100, 0, 10000, 10000, 10000, 10000, 11, 60744, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Fuego esclavizado - In Combat - Cast 'Immolation'"),
(@ENTRY+20, 0, 3, 0, 23, 0, 100, 1, 70380,     1,     0,     0, 37,     0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Elemental de Fuego esclavizado - On Has Aura 'Deep Freeze' - Kill Self");

-- Minigob Manacapón SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY+01;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY+01 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY+01, 0, 0, 0, 0, 0, 100, 0, 1000, 4000, 2000, 5000, 11, 61834, 96, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Minigob Escape - Minigob Manacapón - In Combat - Cast 'Manabonked!'");
