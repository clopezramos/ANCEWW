DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_minigob_escape_magnetic_field', 'spell_minigob_escape_arcane_blast', 'spell_minigob_escape_overload', 'spell_minigob_escape_teleport_in', 'spell_minigob_escape_teleport_out');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(64668, 'spell_minigob_escape_magnetic_field'),
(51797, 'spell_minigob_escape_arcane_blast'),
(51743, 'spell_minigob_escape_overload'),
(40163, 'spell_minigob_escape_teleport_in'),
(7791,  'spell_minigob_escape_teleport_out');
