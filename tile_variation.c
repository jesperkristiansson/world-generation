#include "tile_variation.h"

static const unsigned int num_variations = 6;

struct all_variations variation_get_all()
{
    struct all_variations all_vars;
    all_vars.num_variations = num_variations;
    all_vars.variations = (struct variation *)malloc(num_variations * sizeof(struct variation));

    // init all variations
    for (int i = 0; i < (int)num_variations; i++)
    {
        struct variation *var = &all_vars.variations[i];
        var->value = i;
        var->index = (unsigned int)i;
        var->base_weight = 1;
        var->num_possible = 3;
        var->possible_neighbors = malloc(var->num_possible * sizeof(struct variation_neighbor));
        var->possible_neighbors[0] = (struct variation_neighbor){.neighbor = &all_vars.variations[(i - 1 + num_variations) % num_variations], .weight = 0};
        var->possible_neighbors[1] = (struct variation_neighbor){.neighbor = &all_vars.variations[i], .weight = 4};
        var->possible_neighbors[2] = (struct variation_neighbor){.neighbor = &all_vars.variations[(i + 1) % num_variations], .weight = 0};
    }

    return all_vars;
}

void variation_cleanup(struct all_variations *all_vars)
{
    for (int i = 0; i < (int)all_vars->num_variations; i++)
    {
        struct variation *var = &all_vars->variations[i];
        free(var->possible_neighbors);
    }
    free(all_vars->variations);
}