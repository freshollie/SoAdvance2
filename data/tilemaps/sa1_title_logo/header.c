#include "global.h"
#include "core.h"

ALIGNED(4)
static const u16 Palette_sa1_title_logo[]
    = INCBIN_U16("data/tilemaps/sa1_title_logo/palette.gbapal");
ALIGNED(4)
static const u8 Tiles_sa1_title_logo[]
    = INCBIN_U8("data/tilemaps/sa1_title_logo/tiles.4bpp");
ALIGNED(4)
static const u8 Tilemap_sa1_title_logo[]
    = INCBIN_U8("data/tilemaps/sa1_title_logo/tilemap.tilemap2");

ALIGNED(4)
const Tilemap sa1_title_logo = {
    .xTiles = 24,
    .yTiles = 10,
    .animTileSize = 0,
    .animFrameCount = 0,
    .animDelay = 0,
    .tiles = Tiles_sa1_title_logo,
    .tilesSize = sizeof(Tiles_sa1_title_logo),
    .palette = Palette_sa1_title_logo,
    .palOffset = 0,
    .palLength = sizeof(Palette_sa1_title_logo) / sizeof(u16),
    .map = (const u16 *)Tilemap_sa1_title_logo,
};