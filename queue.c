#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state){
    // associlated with a specific number
    uint64_t encoded = serialize(state);
    insert_at_tail(&q->data, encoded);
}

struct game_state dequeue(struct queue *q) {
    uint64_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

int number_of_moves(struct game_state start) { 
    // initialise the empty queue to hold game states
    struct queue q = {0};
    // Creates an empty linked list to track which states have been visited
    struct linked_list visited = {0};

    //Adds the starting game state to the queue
    enqueue(&q, start);
    insert_at_head(&visited, serialize(start));  // mark start state as visited
    
    while (q.data.head) {
        //Removes the next state from the queue and sets it as the "current" state 
        struct game_state current = dequeue(&q);
        // Check if the current board is in the goal state
        // Assume the current state is the goal 
        int match = 1;
        //compare each tile to what we expect in the solved puzzle
        uint8_t expected = 1;
        // Loop over each cell of the 4x4 board
        // stops if match is 0
        for (int i = 0; i < 4 && match; i++) {
            for (int j = 0; j < 4 && match; j++) {
                //The bottom-right cell must be 0 in the solved state.
                // the bottom right value shouldnt be 16
                if (i == 3 && j == 3) {
                    if (current.tiles[i][j] != 0)
                        match = 0;
                } else {
                    // expected keeps incrementing so we can compare all 15 values
                    if (current.tiles[i][j] != expected++)
                        match = 0;
                }
            }
        }
        // if the match is true then we can escape the function. 
        if (match) {
            free_list(visited);
            free_list(q.data);
            return current.num_steps;
        }
        
        // Tries to move the empty space up
        struct game_state next_up = current;
        move_up(&next_up);

        // If the move actually happened, then it continues
        //If the move is invalid ie you empty tile is already at the top,
        //it just does nothing — it doesn’t modify the board.
        
        if (next_up.num_steps != current.num_steps) {
            //Convert the board state into a unique integer for easy comparison and check for duplicate
            uint64_t encoded = serialize(next_up);
            // Set up to scan through the list of already visited states.
            struct list_node *cur = visited.head;
            bool seen = false;
            // visited list -> see if seen this new board.
            //If we have, mark it.
            while (cur) {
                if (cur->value == encoded) {
                    seen = true;
                    break;
                }
                cur = cur->next;
            }
            //basically the end of cur value 
            if (!seen) {
                //if not seen it then we add 
                enqueue(&q, next_up);
                //insert it to the heaof the visited list
                insert_at_head(&visited, encoded);
            }
        }
        //If not up then we move down and do the same thing
        // Try moving DOWN
        struct game_state next_down = current;
        move_down(&next_down);
        if (next_down.num_steps != current.num_steps) {
            uint64_t encoded = serialize(next_down);
            struct list_node *cur = visited.head;
            bool seen = false;
            while (cur) {
                if (cur->value == encoded) {
                    seen = true;
                    break;
                }
                cur = cur->next;
            }
            if (!seen) {
                enqueue(&q, next_down);
                insert_at_head(&visited, encoded);
            }
        }
        //If not down then we move left and do the same thing
        struct game_state next_left = current;
        move_left(&next_left);
        if (next_left.num_steps != current.num_steps) {
            uint64_t encoded = serialize(next_left);
            struct list_node *cur = visited.head;
            bool seen = false;
            while (cur) {
                if (cur->value == encoded) {
                    seen = true;
                    break;
                }
                cur = cur->next;
            }
            if (!seen) {
                enqueue(&q, next_left);
                insert_at_head(&visited, encoded);
            }
        }
        //If not left then we move right and do the same thing
        struct game_state next_right = current;
        move_right(&next_right);
        if (next_right.num_steps != current.num_steps) {
            uint64_t encoded = serialize(next_right);
            struct list_node *cur = visited.head;
            bool seen = false;
            while (cur) {
                if (cur->value == encoded) {
                    seen = true;
                    break;
                }
                cur = cur->next;
            }
            if (!seen) {
                enqueue(&q, next_right);
                insert_at_head(&visited, encoded);
            }
        }
    }    
    // free the malloc of visited
    free_list(visited);
    free_list(q.data);
    return 0; 
}
