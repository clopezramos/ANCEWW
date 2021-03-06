-- Gameobject spawns
SET @GGUID := 301000;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @GGUID AND @GGUID+6;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@GGUID+00, 185903, 1, 440, 1940, 1, 4096, -8821.3, -4765.1, 1.9, 3.7, 0, 0, 0, 0, 0, 100, 1, 0),
(@GGUID+01, 185899, 1, 440, 1940, 1, 4096, -8821.8, -4767.3, 1.9, 3.7, 0, 0, 0, 0, 0, 100, 1, 0),
(@GGUID+02, 185900, 1, 440, 1940, 1, 4096, -8821.8, -4767.3, 1.9, 3.7, 0, 0, 0, 0, 0, 100, 1, 0),
(@GGUID+03, 187265, 1, 440, 1940, 1, 4096, -8839.03, -4776.11, 2.26947, 3.309510, 0, 0, 0.996477, -0.0838613, 0, 100, 1, 0),
(@GGUID+04, 187265, 1, 440, 1940, 1, 4096, -8840.11, -4766.11, 2.09289, 3.22513 , 0, 0, 0.999128, -0.0417562, 0, 100, 1, 0),
(@GGUID+05, 187265, 1, 440, 1940, 1, 4096, -8839.08, -4771.18, 2.29583, 3.25022 , 0, 0, 0.998525, -0.0542869, 0, 100, 1, 0),
(@GGUID+06, 187265, 1, 440, 1940, 1, 4096, -8838.84, -4779.81, 1.92742, 3.239260, 0, 0, 0.998808, -0.0488136, 0, 100, 1, 0);

-- Creature spawns
SET @ENTRY := 44000;
SET @CGUID := 400000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID AND @CGUID+39;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+00, @ENTRY+02,   0,   1,   1,  1,          1, 0, 0, -5101.29, -1758.39,  497.84, 4.09, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+01, @ENTRY+03,   1, 876, 876,  1,          1, 0, 0,  16222.1,  16398.9,   -64.8,  0.6, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+02, @ENTRY+04,   1, 440, 1940, 1,       4096, 0, 0,  -8826.9,  -4755.8,     1.9,  6.2, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+03, @ENTRY+05, 571, 210, 4522, 1,       4096, 0, 0,   5651.1,   2018.4,  1544.1,  4.8, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+04, @ENTRY+08,   1, 876, 876,  1,          1, 0, 0,  16228.2,  16403.4,   -63.9,  3.2, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+05, @ENTRY+09,   1, 440, 1940, 1,       4096, 0, 0,  -8833.6,  -4775.8,     2.5,  3.3, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+06, @ENTRY+10,   1, 440, 1940, 1,       4096, 0, 0,  -8841.7, -4781.67,    1.47, 3.17, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+07, @ENTRY+10,   1, 440, 1940, 1,       4096, 0, 0, -8842.22, -4767.28,    1.72, 3.12, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+08, @ENTRY+10,   1, 440, 1940, 1,       4096, 0, 0, -8841.77, -4770.94,    1.84, 3.07, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+09, @ENTRY+10,   1, 440, 1940, 1,       4096, 0, 0, -8842.01, -4774.07,    1.83, 3.06, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+10, @ENTRY+10,   1, 440, 1940, 1,       4096, 0, 0, -8842.25, -4777.44,    1.68, 3.29, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+11, @ENTRY+13,   1, 440, 1940, 1,       4096, 0, 0, -8820.77, -4766.87,    1.83,  3.5, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+12, @ENTRY+14, 571, 210, 4522, 1,       4096, 0, 1,   5643.7,   1997.6,  1544.1,  1.9, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+13, @ENTRY+15, 571, 210, 4522, 1,       4096, 0, 1,   5639.5,   2005.6,  1544.1,  5.1, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+14, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5637.44,  2011.12, 1544.13, 5.38, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+15, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5632.18,  2005.05, 1544.13, 5.73, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+16, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5638.89,  2015.61, 1544.13, 4.96, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+17, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5635.07,  2009.72, 1544.13, 5.34, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+18, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5633.49,  2007.62, 1544.13, 5.74, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+19, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5635.66,  2014.49, 1544.13, 5.21, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+20, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5640.03,  2012.24, 1544.13, 4.88, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+21, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5631.21,  2002.45, 1544.13, 5.99, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+22, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5630.88,  2010.75, 1544.13, 5.66, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+23, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5628.31,     2006, 1544.13, 6.05, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+24, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5632.82,  2013.22, 1544.13, 5.25, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+25, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,  5629.02,  2008.52, 1544.13, 5.95, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+26, @ENTRY+11, 571, 210, 4522, 1,       4096, 0, 1,   5627.7,  2003.73, 1544.13, 0.12, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+27, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,     5653,  1992.19, 1544.13, 2.15, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+28, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,   5637.1,  1992.07, 1544.13, 1.48, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+29, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5650.55,  1994.95, 1544.07, 2.09, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+30, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5647.69,  1993.12, 1544.13, 2.02, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+31, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5646.11,     1989, 1544.13, 1.78, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+32, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5649.84,  1990.37, 1544.13, 1.95, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+33, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5644.24,  1991.84, 1544.03,    2, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+34, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5640.76,  1990.69, 1544.13, 1.88, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+35, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5642.28,  1988.11, 1544.13, 1.65, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+36, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,  5638.47,   1988.5, 1544.13, 1.49, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+37, @ENTRY+12, 571, 210, 4522, 1,       4096, 0, 1,   5634.7,  1989.64, 1544.13, 0.92, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+38, @ENTRY+17,   1, 440, 1940, 1, 4294967295, 0, 0,  -8826.9,  -4755.8,     1.9,  6.2, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID+39, @ENTRY+17, 571, 210, 4522, 1, 4294967295, 0, 0,  5640.94,  2000.78, 1544.13,  4.8, 604800, 0, 0, 0, 0, 0, 0, 0, 0, 0);
