#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "tile.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void tile_init(struct tile *tile, struct all_variations *variations, struct tile **neighbors, unsigned int num_neighbors)
{
    tile->num_neighbors = num_neighbors;
    tile->neighbors = neighbors;
    tile->is_set = false;
    tile->set_variation = NULL;

    tile->num_variations = variations->num_variations;
    tile->total_num_variations = variations->num_variations;
    tile->possible_variations = malloc(tile->num_variations * sizeof(struct variation *));
    tile->variation_weights = malloc(variations->num_variations * sizeof(*tile->variation_weights));
    for (unsigned int i = 0; i < variations->num_variations; i++)
    {
        tile->possible_variations[i] = &variations->variations[i];
        tile->variation_weights[i] = variations->variations[i].base_weight;
    }
}

static void tile_update(struct tile *tile, struct variation **allowed_variations, unsigned int num_allowed_variations)
{
    if (tile->is_set)
    {
        return;
    }

    unsigned int upper_bound = MIN(tile->num_variations, num_allowed_variations);
    struct variation **new_variations = malloc(upper_bound * sizeof(struct variation *));
    unsigned int idx = 0;
    for (unsigned int i = 0; i < num_allowed_variations; i++)
    {
        struct variation *var = allowed_variations[i];
        for (unsigned int j = 0; j < tile->num_variations; j++)
        {
            struct variation *var2 = tile->possible_variations[j];
            if (var->value == var2->value)
            {
                new_variations[idx++] = var;
                break;
            }
        }
    }

    if (idx < tile->num_variations)
    {
        // update tile
        free(tile->possible_variations);
        tile->num_variations = idx;
        tile->possible_variations = new_variations;

        // update neighbors
        unsigned int max_allowed_variations = 0;
        for (unsigned int i = 0; i < tile->num_variations; ++i)
        {
            struct variation *var = tile->possible_variations[i];
            max_allowed_variations += var->num_possible;
        }
        struct variation **neighbors_allowed = malloc(max_allowed_variations * sizeof(*neighbors_allowed));
        bool *variation_been_added = calloc(tile->total_num_variations, sizeof(bool));
        unsigned int neighbors_num_allowed = 0;
        for (unsigned int i = 0; i < tile->num_variations; ++i)
        {
            struct variation *var = tile->possible_variations[i];
            for (unsigned int j = 0; j < var->num_possible; ++j)
            {
                struct variation *var2 = var->possible_neighbors[j].neighbor;
                if (!variation_been_added[var2->index])
                {
                    variation_been_added[var2->index] = true;
                    neighbors_allowed[neighbors_num_allowed++] = var2;
                }
            }
        }

        for (unsigned int i = 0; i < tile->num_neighbors; i++)
        {
            tile_update(tile->neighbors[i], neighbors_allowed, neighbors_num_allowed);
        }

        free(neighbors_allowed);
        free(variation_been_added);
    }
    else
    {
        free(new_variations);
    }
}

void tile_set(struct tile *tile)
{
    assert(!tile->is_set);
    assert(tile->num_variations > 0);

    struct variation *var;

    unsigned int total_weight = 0;
    for (unsigned int i = 0; i < tile->num_variations; ++i)
    {
        var = tile->possible_variations[i];
        unsigned int var_weight = tile->variation_weights[var->index];
        total_weight += var_weight;
    }

    int rand_weight = rand() % total_weight;
    unsigned int var_index = 0;
    while (1)
    {
        if (rand_weight <= 0)
        {
            break;
        }
        var = tile->possible_variations[var_index];
        unsigned int var_weight = tile->variation_weights[var->index];
        rand_weight -= var_weight;
        ++var_index;
    }

    tile->set_variation = var;
    tile->is_set = true;
    tile->num_variations = 0;

    // update neighbors
    struct variation **neighbors_allowed = malloc(var->num_possible * sizeof(*neighbors_allowed));
    for (unsigned int i = 0; i < var->num_possible; i++)
    {
        neighbors_allowed[i] = var->possible_neighbors[i].neighbor;
    }
    for (unsigned int i = 0; i < tile->num_neighbors; i++)
    {
        struct tile *neighbor = tile->neighbors[i];
        tile_update(neighbor, neighbors_allowed, var->num_possible);

        // update weights
        for (unsigned int i = 0; i < var->num_possible; i++)
        {
            neighbor->variation_weights[var->possible_neighbors[i].neighbor->index] += var->possible_neighbors[i].weight;
        }
    }
    free(neighbors_allowed);
}

void tile_teardown(struct tile *tile)
{
    free(tile->neighbors);
    free(tile->possible_variations);
    free(tile->variation_weights);
}