#include "engine_spatial_hash.h"
#include <stdlib.h>
#include <string.h>

// =============================================================================
// Hash Function
// =============================================================================

// Fast hash combining two 32-bit integers
// Uses FNV-1a style mixing for good distribution
static inline uint32_t hash_coords(int32_t x, int32_t y) {
    uint32_t hash = 2166136261u;  // FNV offset basis
    hash ^= (uint32_t)x;
    hash *= 16777619u;            // FNV prime
    hash ^= (uint32_t)y;
    hash *= 16777619u;
    return hash;
}

// Round up to next power of 2
static uint32_t next_power_of_2(uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

// =============================================================================
// Lifecycle
// =============================================================================

bool spatial_hash_init(SpatialHashMap* map, uint32_t capacity) {
    if (!map) return false;
    
    // Minimum capacity 16, round up to power of 2
    if (capacity < 16) capacity = 16;
    capacity = next_power_of_2(capacity);
    
    map->entries = (SpatialHashEntry*)malloc(capacity * sizeof(SpatialHashEntry));
    if (!map->entries) return false;
    
    map->capacity = capacity;
    map->count = 0;
    map->mask = capacity - 1;
    
    // Mark all entries as empty
    for (uint32_t i = 0; i < capacity; i++) {
        map->entries[i].key_x = SPATIAL_HASH_EMPTY_KEY;
        map->entries[i].key_y = SPATIAL_HASH_EMPTY_KEY;
    }
    
    return true;
}

void spatial_hash_shutdown(SpatialHashMap* map) {
    if (!map) return;
    free(map->entries);
    map->entries = NULL;
    map->capacity = 0;
    map->count = 0;
}

void spatial_hash_clear(SpatialHashMap* map) {
    if (!map || !map->entries) return;
    
    for (uint32_t i = 0; i < map->capacity; i++) {
        map->entries[i].key_x = SPATIAL_HASH_EMPTY_KEY;
        map->entries[i].key_y = SPATIAL_HASH_EMPTY_KEY;
    }
    map->count = 0;
}

// =============================================================================
// Operations
// =============================================================================

bool spatial_hash_insert(SpatialHashMap* map, int32_t x, int32_t y, uint16_t value) {
    if (!map || !map->entries) return false;
    
    // Don't let load factor exceed 70%
    if (map->count >= (map->capacity * 7) / 10) return false;
    
    uint32_t hash = hash_coords(x, y);
    uint32_t idx = hash & map->mask;
    
    // Linear probing
    for (uint32_t i = 0; i < map->capacity; i++) {
        SpatialHashEntry* entry = &map->entries[idx];
        
        // Empty slot - insert here
        if (entry->key_x == SPATIAL_HASH_EMPTY_KEY) {
            entry->key_x = x;
            entry->key_y = y;
            entry->value = value;
            map->count++;
            return true;
        }
        
        // Key already exists - update value
        if (entry->key_x == x && entry->key_y == y) {
            entry->value = value;
            return true;
        }
        
        // Collision - probe next slot
        idx = (idx + 1) & map->mask;
    }
    
    return false;  // Map is full (shouldn't happen with load factor check)
}

uint16_t spatial_hash_find(const SpatialHashMap* map, int32_t x, int32_t y) {
    if (!map || !map->entries) return SPATIAL_HASH_NOT_FOUND;
    
    uint32_t hash = hash_coords(x, y);
    uint32_t idx = hash & map->mask;
    
    // Linear probing
    for (uint32_t i = 0; i < map->capacity; i++) {
        const SpatialHashEntry* entry = &map->entries[idx];
        
        // Empty slot - key not found
        if (entry->key_x == SPATIAL_HASH_EMPTY_KEY) {
            return SPATIAL_HASH_NOT_FOUND;
        }
        
        // Found the key
        if (entry->key_x == x && entry->key_y == y) {
            return entry->value;
        }
        
        // Collision - probe next slot
        idx = (idx + 1) & map->mask;
    }
    
    return SPATIAL_HASH_NOT_FOUND;
}

bool spatial_hash_remove(SpatialHashMap* map, int32_t x, int32_t y) {
    if (!map || !map->entries) return false;
    
    uint32_t hash = hash_coords(x, y);
    uint32_t idx = hash & map->mask;
    
    // Linear probing to find the entry
    for (uint32_t i = 0; i < map->capacity; i++) {
        SpatialHashEntry* entry = &map->entries[idx];
        
        // Empty slot - key not found
        if (entry->key_x == SPATIAL_HASH_EMPTY_KEY) {
            return false;
        }
        
        // Found the key - remove it
        if (entry->key_x == x && entry->key_y == y) {
            // Mark as empty (note: this can break probe chains, but for our use case
            // we typically don't remove entries, and clear() resets everything)
            entry->key_x = SPATIAL_HASH_EMPTY_KEY;
            entry->key_y = SPATIAL_HASH_EMPTY_KEY;
            map->count--;
            return true;
        }
        
        // Collision - probe next slot
        idx = (idx + 1) & map->mask;
    }
    
    return false;
}

bool spatial_hash_contains(const SpatialHashMap* map, int32_t x, int32_t y) {
    return spatial_hash_find(map, x, y) != SPATIAL_HASH_NOT_FOUND;
}
