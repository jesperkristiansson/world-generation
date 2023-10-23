#ifndef TILE_VARIATION_H
#define TILE_VARIATION_H

#include <stdlib.h>

struct variation_neighbor
{
    struct variation *neighbor;
    unsigned int weight;
};

struct variation
{
    char symbol;
    unsigned int index;
    unsigned int base_weight;
    char *color_code;
    struct variation_neighbor *possible_neighbors;
    unsigned int num_possible;
};

struct all_variations
{
    struct variation *variations;
    unsigned int num_variations;
};

struct all_variations variation_get_default();
struct all_variations variation_read_config(const char *config_file);
void variation_cleanup(struct all_variations *all_vars);

#endif