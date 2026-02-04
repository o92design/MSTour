#ifndef ENGINE_SPATIAL_HASH_H
#define ENGINE_SPATIAL_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Spatial Hash Map
// 
// O(1) lookup for 2D integer coordinate pairs (chunk coords, tile coords, etc.)
// Uses open addressing with linear probing for simplicity and cache efficiency.
// 
// Usage:
//   SpatialHashMap map;
//   spatial_hash_init(&map, 256);  // 256 buckets
//   spatial_hash_insert(&map, chunk_x, chunk_y, chunk_index);
//   uint16_t idx = spatial_hash_find(&map, chunk_x, chunk_y);  // SPATIAL_HASH_NOT_FOUND if missing
//   spatial_hash_shutdown(&map);
// =============================================================================

#define SPATIAL_HASH_NOT_FOUND 0xFFFF
#define SPATIAL_HASH_EMPTY_KEY 0x7FFFFFFF

typedef struct SpatialHashEntry {
    int32_t key_x;
    int32_t key_y;
    uint16_t value;
    uint16_t _pad;
} SpatialHashEntry;

typedef struct SpatialHashMap {
    SpatialHashEntry* entries;
    uint32_t capacity;      // Must be power of 2
    uint32_t count;
    uint32_t mask;          // capacity - 1, for fast modulo
} SpatialHashMap;

// Initialize hash map with given capacity (rounded up to power of 2)
bool spatial_hash_init(SpatialHashMap* map, uint32_t capacity);

// Shutdown and free memory
void spatial_hash_shutdown(SpatialHashMap* map);

// Clear all entries (keeps allocated memory)
void spatial_hash_clear(SpatialHashMap* map);

// Insert or update entry, returns false if map is full
bool spatial_hash_insert(SpatialHashMap* map, int32_t x, int32_t y, uint16_t value);

// Find entry, returns SPATIAL_HASH_NOT_FOUND if not present
uint16_t spatial_hash_find(const SpatialHashMap* map, int32_t x, int32_t y);

// Remove entry, returns true if found and removed
bool spatial_hash_remove(SpatialHashMap* map, int32_t x, int32_t y);

// Check if key exists
bool spatial_hash_contains(const SpatialHashMap* map, int32_t x, int32_t y);

// Get current entry count
static inline uint32_t spatial_hash_count(const SpatialHashMap* map) {
    return map ? map->count : 0;
}

#ifdef __cplusplus
}
#endif

#endif // ENGINE_SPATIAL_HASH_H
