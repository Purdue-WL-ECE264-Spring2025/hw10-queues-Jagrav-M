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

    // Setup goal state
    struct game_state goal_state = {0};
    uint8_t val = 1;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (r == 3 && c == 3) {
                goal_state.tiles[r][c] = 0;
                goal_state.empty_row = r;
                goal_state.empty_col = c;
            } else {
                goal_state.tiles[r][c] = val++;
            }
        }
    }

    uint64_t goal_encoded = serialize(goal_state);

    size_t visited_size = 1 << 24;
    uint8_t *visited = calloc(visited_size, sizeof(uint8_t));
    if (!visited) return -1;

    uint64_t start_encoded = serialize(start);
    visited[start_encoded % visited_size] = 1;

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);
        uint64_t curr_encoded = serialize(current);

        if (curr_encoded == goal_encoded) {
            free(visited);
            return current.num_steps;
        }

        // Generate up to 4 moves
        if (current.empty_row < 3) {
            struct game_state next = current;
            move_up(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        if (current.empty_row > 0) {
            struct game_state next = current;
            move_down(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        if (current.empty_col < 3) {
            struct game_state next = current;
            move_left(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        if (current.empty_col > 0) {
            struct game_state next = current;
            move_right(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }
    }

    free(visited);
    return 0;
}