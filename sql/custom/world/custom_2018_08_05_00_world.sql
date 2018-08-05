--
-- Cleanup
--

-- Manabonk game_event
UPDATE `game_event` SET `start_time`='0000-00-00 00:00:00', `end_time`='0000-00-00 00:00:00' WHERE `eventEntry`=33;
DELETE FROM `game_event_creature` WHERE `eventEntry` = 33 AND `guid` = 44457;
-- Manabonk spawn
DELETE FROM `creature` WHERE `guid` = 44457;
