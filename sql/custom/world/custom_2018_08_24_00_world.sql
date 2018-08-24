DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_minigob_escape_magnetic_field', 'spell_minigob_escape_teleport_in', 'spell_minigob_escape_teleport_out');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(64668, 'spell_minigob_escape_magnetic_field'),
(40163, 'spell_minigob_escape_teleport_in'),
(7791,  'spell_minigob_escape_teleport_out');
