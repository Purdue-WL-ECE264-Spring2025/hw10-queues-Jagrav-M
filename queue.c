#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void enqueue(struct queue *q, struct game_state state) 
{
    uint64_t encoded = serialize(state);
    insert_at_tail(&q->data, encoded);
}

struct game_state dequeue(struct queue *q) 
{ 
    uint64_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

int number_of_moves(struct game_state start) 
{ 
    struct queue q = {0}; 

    enqueue(&q, start);

   
    size_t visited_size = 1 << 24; 
    uint8_t *visited = calloc(visited_size, sizeof(uint8_t));
    if (!visited) return -1;

    size_t encoded = serialize(start);
    visited[encoded % visited_size] = 1;

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);

        if (is_goal(current)) {
            free(visited);
            return current.num_steps;
        }

        struct game_state neighbors[4];
        int count = possible_moves(current, neighbors);

        for (int i = 0; i < count; ++i) 
        {
            size_t next_encoded = serialize(neighbors[i]);
            size_t index = next_encoded % visited_size;

            if (!visited[index]) 
            {
                visited[index] = 1;
                enqueue(&q, neighbors[i]);
            }
        }
    }

    free(visited);
    return 0; 
}
