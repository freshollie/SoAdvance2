#include "global.h"
#include "core.h"

ALIGNED(4)
static const u16 Palette_zone_6_act_1_fg[]
    = INCBIN_U16("data/tilemaps/zone_6_act_1_fg/palette.gbapal");
ALIGNED(4)
static const u8 Tileset_zone_6_act_1_fg[]
    = INCBIN_U8("data/tilemaps/zone_6_act_1_fg/tileset.4bpp");
ALIGNED(4)
static const u8 CollHeightMap_zone_6_act_1_fg[]
    = INCBIN_U8("data/tilemaps/zone_6_act_1_fg/height_map.coll");
ALIGNED(4)
static const u8 CollTileRot_zone_6_act_1_fg[]
    = INCBIN_U8("data/tilemaps/zone_6_act_1_fg/tile_rot.coll");
ALIGNED(4)
static const u8 CollFlags_zone_6_act_1_fg[]
    = INCBIN_U8("data/tilemaps/zone_6_act_1_fg/flags.coll");
ALIGNED(4)
static const u16 Metatiles_zone_6_act_1_fg[]
    = INCBIN_U16("data/tilemaps/zone_6_act_1_fg/metatiles.tilemap2");

ALIGNED(4)
static const u16 Map_zone_6_act_1_fg0[]
    = INCBIN_U16("data/tilemaps/zone_6_act_1_fg/map_front.bin");

ALIGNED(4)
static const u16 Map_zone_6_act_1_fg1[]
    = INCBIN_U16("data/tilemaps/zone_6_act_1_fg/map_back.bin");

ALIGNED(4) const struct MapHeader MapHeader_zone_6_act_1_fg0 = 
{
    .h = {
        .xTiles = 12,
        .yTiles = 12,
        .animTileSize = 800,
        .animFrameCount = 4,
        .animDelay = 5,
        .tiles = Tileset_zone_6_act_1_fg,
        .tilesSize = sizeof(Tileset_zone_6_act_1_fg),
        .palette = Palette_zone_6_act_1_fg,
        .palOffset = 0,
        .palLength = sizeof(Palette_zone_6_act_1_fg) / sizeof(u16),
        .map = Metatiles_zone_6_act_1_fg,
    },
    .metatileMap = Map_zone_6_act_1_fg0,
    .mapWidth = 313,
    .mapHeight = 46,
};

ALIGNED(4) const struct MapHeader MapHeader_zone_6_act_1_fg1 = 
{
    .h = {
        .xTiles = 12,
        .yTiles = 12,
        .animTileSize = 800,
        .animFrameCount = 4,
        .animDelay = 5,
        .tiles = Tileset_zone_6_act_1_fg,
        .tilesSize = sizeof(Tileset_zone_6_act_1_fg),
        .palette = Palette_zone_6_act_1_fg,
        .palOffset = 0,
        .palLength = sizeof(Palette_zone_6_act_1_fg) / sizeof(u16),
        .map = Metatiles_zone_6_act_1_fg,
    },
    .metatileMap = Map_zone_6_act_1_fg1,
    .mapWidth = 313,
    .mapHeight = 46,
};

ALIGNED(4)
const Collision CollHeader_zone_6_act_1_fg = { CollHeightMap_zone_6_act_1_fg,
                                               CollTileRot_zone_6_act_1_fg,
                                               Metatiles_zone_6_act_1_fg,
                                               Map_zone_6_act_1_fg0,
                                               Map_zone_6_act_1_fg1,
                                               CollFlags_zone_6_act_1_fg,
                                               313,
                                               46,
                                               0x60,
                                               0x75,
                                               0x00,
                                               0x00,
                                               0x1140,
                                               0x0000 };