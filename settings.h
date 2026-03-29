#ifndef SETTINGS_H
#define SETTINGS_H
//settings
#define MAP_SIZE 1000 //the max x or y coordinate
#define DISPLAY_SIZE 20 //the size of the display when displaying the map
#define BUILD_DISTANCE 7 //a tile must be within this distance to a structure in order to build on it
#define ROAD_COEFF 3 //how many seconds each unit of distance adds to transportaion time
#define EXPLORE_LIMIT 5
#define BIRTH_RATE 0.16
#define BIRTH_FREQUENCY 10 //number out of 100 that causes an increase of townsfolk

//land settings
#define FOREST_GEN 5.0 //forest generation number
#define FIELD_GEN 4.0 //fields generation number
#define MOUNTAIN_GEN 3.5 //mountains generation number
#define TUNDRA_GEN 3.0 //tundra generation number
#define SEA_GEN 1.0 //sea generation number

#define BIOME_CHANGE 0.08 //chance to change biomes

//combat settings
#define KILL_CHANCE 0.10 //the chance of a kill in battle

//minor structure constants
#define MINE_PRODUCTION 60 //How long it takes a mine to produce
#define QUARRY_PRODUCTION 45 //how long it takes for a quarry to produce
#define SAWMILL_PRODUCTION 30 //how many seconds it takes a sawmill to produce
#define PASTURE_PRODUCTION 15 //how long it takes a pasture to produce
#define GARDEN_PRODUCTION 5 //how long it takes a garden to produce

//village constants
#define VILLAGE_CAPACITY 50 //this is the total capacity of the village
#define VILLAGE_LIMIT 3 //this is the land limit for a village
#define VILLAGE_DEFENSE 0.0 //village defense ability

//castle constants
#define CASTLE_CAPACITY 300
#define CASTLE_LIMIT 11
#define CASTLE_DEFENSE 0.04

//fort constants
#define FORT_CAPACITY 50
#define FORT_LIMIT 0
#define FORT_DEFENSE 0.02

//barbarian constants
#define PILLAGE_FREQUENCY 4 //5% chance of a tribe pillaging (0-4: pillages 5-99 doesn't)
#define MAX_PILLAGE 20 //the most that a tribe can take when pillaging
#define SPREAD_INCREASE_RATE 0.25 //the amount the the chance of spreading increases
#define BARBARIAN_THRESHOLD 10 //the minimum number of people needed for a barbarian tribe to attack

//calculated constants

#endif
