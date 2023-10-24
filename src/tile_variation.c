#include "tile_variation.h"

#include <jansson.h>
#include <assert.h>
#include <string.h>

struct all_variations variation_get_default()
{
    const unsigned int num_variations = 6;

    struct all_variations all_vars;
    all_vars.num_variations = num_variations;
    all_vars.variations = (struct variation *)malloc(num_variations * sizeof(struct variation));

    // init all variations
    for (int i = 0; i < (int)num_variations; i++)
    {
        struct variation *var = &all_vars.variations[i];
        var->symbol = 'A' + i;
        var->index = (unsigned int)i;
        var->base_weight = 1;
        var->num_possible = 3;
        var->possible_neighbors = malloc(var->num_possible * sizeof(struct variation_neighbor));
        var->possible_neighbors[0] = (struct variation_neighbor){.neighbor = &all_vars.variations[(i - 1 + num_variations) % num_variations], .weight = 0};
        var->possible_neighbors[1] = (struct variation_neighbor){.neighbor = &all_vars.variations[i], .weight = 4};
        var->possible_neighbors[2] = (struct variation_neighbor){.neighbor = &all_vars.variations[(i + 1) % num_variations], .weight = 0};

        char *color = calloc(3, sizeof(char));
        color[0] = '3';
        color[1] = '1' + i;
        var->color_code = color;
    }

    return all_vars;
}

struct all_variations variation_read_config(const char *config_file)
{
    json_t *json;
    json_error_t error;

    // object
    json = json_load_file(config_file, 0, &error);
    if (!json)
    {
        fprintf(stderr, "Jansson error\n");
    }

    // array
    json_t *json_variations = json_object_get(json, "variations");

    struct all_variations all_vars;
    all_vars.num_variations = json_array_size(json_variations);
    all_vars.variations = (struct variation *)malloc(all_vars.num_variations * sizeof(struct variation));

    size_t index;
    json_t *value;
    json_array_foreach(json_variations, index, value)
    {
        json_t *sym_json = json_object_get(value, "symbol");
        json_t *id_json = json_object_get(value, "id");
        json_t *base_weight_json = json_object_get(value, "base_weight");
        json_t *color_json = json_object_get(value, "color");

        const char *sym_str = json_string_value(sym_json);
        assert(strlen(sym_str) == 1);
        char symbol = sym_str[0];

        unsigned int id = json_integer_value(id_json);
        unsigned int base_weight = json_integer_value(base_weight_json);

        const size_t color_len = json_string_length(color_json);
        char *color = calloc(color_len + 1, sizeof(char));
        strncpy(color, json_string_value(color_json), color_len);

        all_vars.variations[index].index = id;
        all_vars.variations[index].symbol = symbol;
        all_vars.variations[index].base_weight = base_weight;
        all_vars.variations[index].color_code = color;
    }

    json_t *json_relations = json_object_get(json, "relations");

    // count the number of neighbors for each variant
    unsigned int *neighbors_per_variant = calloc(all_vars.num_variations, sizeof(unsigned int));
    json_array_foreach(json_relations, index, value)
    {
        json_t *id1_json = json_object_get(value, "id1");
        json_t *id2_json = json_object_get(value, "id2");
        unsigned int id1 = json_integer_value(id1_json);
        unsigned int id2 = json_integer_value(id2_json);
        neighbors_per_variant[id1]++;
        if (id1 != id2)
        {
            neighbors_per_variant[id2]++;
        }
    }

    for (unsigned int i = 0; i < all_vars.num_variations; i++)
    {
        struct variation *var = &all_vars.variations[i];
        var->possible_neighbors = malloc(neighbors_per_variant[i] * sizeof(struct variation_neighbor));
        var->num_possible = 0; // gets incremented when adding neighbors
    }

    json_array_foreach(json_relations, index, value)
    {
        json_t *id1_json = json_object_get(value, "id1");
        json_t *id2_json = json_object_get(value, "id2");
        json_t *weight_json = json_object_get(value, "weight");
        unsigned int id1 = json_integer_value(id1_json);
        unsigned int id2 = json_integer_value(id2_json);
        unsigned int weight = json_integer_value(weight_json);

        struct variation *var1 = &all_vars.variations[id1];
        struct variation *var2 = &all_vars.variations[id2];

        var1->possible_neighbors[var1->num_possible].neighbor = var2;
        var1->possible_neighbors[var1->num_possible].weight = weight;
        ++var1->num_possible;

        if (id1 != id2)
        {
            var2->possible_neighbors[var2->num_possible].neighbor = var1;
            var2->possible_neighbors[var2->num_possible].weight = weight;
            ++var2->num_possible;
        }
    }

    free(neighbors_per_variant);
    json_decref(json);
    return all_vars;
}

void variation_cleanup(struct all_variations *all_vars)
{
    for (int i = 0; i < (int)all_vars->num_variations; i++)
    {
        struct variation *var = &all_vars->variations[i];
        free(var->possible_neighbors);
        free(var->color_code);
    }
    free(all_vars->variations);
}