#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdint.h>

void enqueue(struct queue *q, struct game_state state) {
    uint64_t encoded = serialize(state);
    insert_at_tail(&q->data, encoded);
}

struct game_state dequeue(struct queue *q) {
    uint64_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    enqueue(&q, start);

    // --- Step 1: Define the hardcoded goal state ---
    struct game_state goal = {0};
    uint8_t val = 1;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (r == 3 && c == 3) {
                goal.tiles[r][c] = 0;
                goal.empty_row = r;
                goal.empty_col = c;
            } else {
                goal.tiles[r][c] = val++;
            }
        }
    }

    uint64_t goal_encoded = serialize(goal);

    // --- Step 2: Allocate visited table ---
    size_t visited_size = 1 << 24;  // 2^24 entries = 16M
    uint8_t *visited = calloc(visited_size, sizeof(uint8_t));
    if (!visited) return -1;

    visited[serialize(start) % visited_size] = 1;

    // --- Step 3: BFS traversal ---
    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);
        uint64_t current_encoded = serialize(current);

        if (current_encoded == goal_encoded) {
            free(visited);
            return current.num_steps;
        }

        struct game_state next;

        // UP
        if (current.empty_row < 3) {
            next = current;
            move_up(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        // DOWN
        if (current.empty_row > 0) {
            next = current;
            move_down(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        // LEFT
        if (current.empty_col < 3) {
            next = current;
            move_left(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }

        // RIGHT
        if (current.empty_col > 0) {
            next = current;
            move_right(&next);
            uint64_t enc = serialize(next);
            if (!visited[enc % visited_size]) {
                visited[enc % visited_size] = 1;
                enqueue(&q, next);
            }
        }
    }

    free(visited);
    return 0; // Goal not found
}
