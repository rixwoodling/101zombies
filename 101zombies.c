// 101 Zombies
// Turn-based survival game for Flipper Zero.
// Copyright (c) 2026 Rix Woodling. All rights reserved.

#include <stdio.h>
#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>
#include <string.h>

// List of each user-driven and explicit state change.
typedef enum {
    StateTitle,
    StateRules,
    StateGame,
    StateResult,
    StateQuitMenu,
    StateWin,
    StateRetry,
} GameState;

// Define each data group
typedef struct {
    // UI flow state: tracks current screen and Yes/No menu selection
    GameState screen;
    GameState prev_screen;
    uint8_t yesno_selected;

    // Core gameplay state stored as integers (counts, health, and levels).
    int zombies;
    int remaining;
    int health;
    int weapon;
    int fatigue;

    // Track previous zombie count and whether the last round was a fight.
    int last_zombies;
    bool last_action_was_fight;

    // Define text buffers for the main game screen and result screen.
    char line1[32];
    char line2[32];
    char line3[32];
    char line4[48];
    char line5[48];
    char result1[32];
    char result2[48];
    char result3[48];
} AppState;

// Health-based descriptions, indexed by current health level.
static const char* health_emo[] = {
    "dead","very messed up","messed up","like total crap","like crap",
    "wounded","banged up","alright","pretty good","flippin' great","godlike"
};  

// Luck messages indexed by fight probability.
static const char* luck_text[] = {
    "you got no chance in hell.",
    "odds are slim.",
    "you'd better run.",
    "it's dicey at best.",
    "it'll be a tough fight.",
    "it's a toss up.",
    "you've seen worse.",
    "the odds are with you.",
    "this shouldn't be a problem.",
    "easy.",
    "you can't lose!"
};

// Weapon names indexed by weapon level.
static const char* weapon_names[] = {
    "a machete",
    "a chainsaw",
    "a shotgun",
    "an AK-47",
    "a minigun",
    "a flamethrower",
    "a rocket launcher",
    "the death ray",
};

// Calculate fight win probability from current game state, factoring in fatigue.
static float fight_probability(AppState* app_state) {
    // Estimate relative player strength vs enemy strength for odds calculation.
    float player = (app_state->health + (app_state->weapon * 10)) / 20.0f;
    float enemy  = (app_state->zombies * 5) / 10.0f;

    // Reduce player strength based on accumulated fatigue.
    float fatigue_penalty = app_state->fatigue * 0.6f;

    // Apply fatigue penalty, but ensure player strength never drops below a minimum.
    player -= fatigue_penalty;
    if(player < 1.0f) player = 1.0f;

    // Convert relative player vs enemy strength into a 0–100 win chance.
    float prob = 100.0f * (player / (player + enemy));

    // Clamp win probability so it never reaches guaranteed win or loss.
    if(prob < 5.0f) prob = 5.0f;
    if(prob > 95.0f) prob = 95.0f;

    // Return the clamped win probability as a percentage.
    return prob;
}

// Build next wave text based on game state (wave info, health, weapon, and odds).
static void update_wave_text(AppState* app_state) {
    // Limit the number of dots so the wave indicator always fits on screen.
    char dots[16];
    int z = app_state->zombies;
    int max = (int)sizeof(dots) - 1;
    if(z < 0) z = 0;
    if(z > max) z = max;

    // Create a dot string representing the current wave size.
    memset(dots, '.', z);
    dots[z] = '\0';

    // Display remaining zombies plus current wave dots.
    snprintf(app_state->line1, sizeof(app_state->line1),
            "%d %s", app_state->remaining, dots);

    // Describe the number of approaching zombies with proper singular/plural wording.
    if(app_state->zombies == 1) {
        snprintf(app_state->line2, sizeof(app_state->line2),
                "A lone zombie approaches.");
    } else {
        snprintf(app_state->line2, sizeof(app_state->line2),
                "%d zombies approach.", app_state->zombies);            
    }

    // Map health value to a text description range.
    int idx = app_state->health / 10;
    if(idx > 10) idx = 10;
    if(idx < 0) idx = 0;

    // Display a health-based description of how the player feels.
    snprintf(app_state->line3, sizeof(app_state->line3),
            "You're feeling %s.", health_emo[idx]);         

    // Display the player's currently equipped weapon.
    snprintf(app_state->line4, sizeof(app_state->line4),
            "Armed with %s,", weapon_names[app_state->weapon]);

    // Map fight probability to a luck text range.
    float prob = fight_probability(app_state);
    int y = (int)(prob / 10.0f);
    if(y < 0) y = 0;
    if(y > 10) y = 10;

    // Display the luck text for the current fight odds.
    snprintf(app_state->line5, sizeof(app_state->line5),
            "%s", luck_text[y]);
}

// Helper to reset game state and generate initial wave.
static void reset_game_state(AppState* app_state) {
    app_state->remaining = 101;
    app_state->health = 100;
    app_state->weapon = 0;
    app_state->fatigue = 0;
    app_state->zombies = rand() % 10 + 1;

    update_wave_text(app_state);
}

// Draw the current screen based on game state (UI-only, no game logic).
static void draw_callback(Canvas* canvas, void* ctx) {
    // Draw the current screen when called (system callback).
    furi_assert(ctx);
    AppState* state = ctx;
    canvas_clear(canvas);

    // Select which screen to draw based on the current game state.
    switch(state->screen) {
        case StateTitle:
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 30, 15, "101 Zombies!");
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 1, 30, "Choose to FIGHT or RUN away");
            canvas_draw_str(canvas, 1, 40, "from the zombie horde!");
            canvas_draw_str(canvas, 1, 50, "Chance for new weapons");
            canvas_draw_str(canvas, 1, 60, "when fighting. However,");
            break;

        case StateRules:
            canvas_draw_str(canvas, 1, 10, "Consecutive fighting increases");
            canvas_draw_str(canvas, 1, 20, "zombie damage. Chance for");
            canvas_draw_str(canvas, 1, 30, "medikits when running.");
            canvas_draw_str(canvas, 1, 40, "However, constant running is");
            canvas_draw_str(canvas, 1, 50, "for weak little girls!");
            canvas_draw_str(canvas, 1, 60, "Good luck.");
            break;

        case StateQuitMenu:
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 20, 10, "rixwoodling.github.io"); 
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 20, 40, "Give up already?");
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 5, 60, state->yesno_selected ? "> Yes" : "  Yes"); 
            canvas_draw_str(canvas, 90, 60, !state->yesno_selected ? "> No" : "  No");
            break;

        case StateGame:
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 1, 10, state->line1);
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 1, 20, state->line2);
            canvas_draw_str(canvas, 1, 30, state->line3);
            canvas_draw_str(canvas, 1, 40, state->line4);
            canvas_draw_str(canvas, 1, 50, state->line5);
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 5, 60, "< Run");
            canvas_draw_str(canvas, 80, 60, "Fight >");
            break;

        case StateResult:
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 1, 10, state->result1);
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 1, 20, state->result2);
            canvas_draw_str(canvas, 1, 30, state->result3);
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 25, 60, "< Next Wave >");
            break;

        case StateWin:
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 5, 25, "Few ever make it to the end.");
            canvas_draw_str(canvas, 5, 35, "But you slaughtered them all.");
            canvas_draw_str(canvas, 5, 45, "You are an absolute HERO!");
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 5, 60,
                    state->yesno_selected ? "> Retry" : "  Retry");
            canvas_draw_str(canvas, 80, 60,
                    !state->yesno_selected ? "> Exit" : "  Exit");
            break;

        case StateRetry:
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 50, 15, "DEAD!");
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 1, 27, "Unfortunately, your wounds");
            canvas_draw_str(canvas, 1, 37, "bring you down. And now you");
            canvas_draw_str(canvas, 1, 47, "turn into a zombie!");
            canvas_set_font(canvas, FontKeyboard);
            canvas_draw_str(canvas, 5, 60, state->yesno_selected ? "> Retry" : "  Retry");
            canvas_draw_str(canvas, 80, 60, !state->yesno_selected ? "> Exit" : "  Exit");
            break;
    }
}

// Forward input events to the main event queue (system callback).
static void input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

// Run the main game loop and handle input, state updates, and screen changes.
int32_t zombies_main(void* p) {
    // Ignore unused startup parameter and prepare input event storage.
    UNUSED(p);
    InputEvent event;

    // Initialize persistent game state to starting values.
    static AppState app_state;
    memset(&app_state, 0, sizeof(AppState));
    app_state.screen = StateTitle;
    app_state.remaining = 101;

    // Set up input handling and connect game state to drawing callbacks.
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, &app_state);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Attach the game view to the system GUI so it becomes visible on screen.
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // Enter the main event loop and wait for the next input event.
    while(true) {
        furi_check(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk);

        // Ignore up/down button presses globally.
        if(event.key == InputKeyUp || event.key == InputKeyDown) {
            continue;
        }

        // Ignore the OK button during active gameplay.
        if(app_state.screen == StateGame &&
                event.key == InputKeyOk) {
            continue;
        }

        // Toggle the quit menu when the Back button is pressed.
        if(event.key == InputKeyBack && event.type == InputTypeShort) {
            if(app_state.screen != StateQuitMenu) {
                app_state.prev_screen = app_state.screen;
                app_state.screen = StateQuitMenu;
                view_port_update(view_port);
            } else {
                app_state.screen = app_state.prev_screen;
                view_port_update(view_port);
            }

        // Handle navigation and selection within the quit menu.
        } else if(app_state.screen == StateQuitMenu) {
            if(event.key == InputKeyLeft || event.key == InputKeyRight) {
                app_state.yesno_selected = !app_state.yesno_selected;
                view_port_update(view_port);
            } else if(event.type == InputTypeShort && event.key == InputKeyOk) {
                if(app_state.yesno_selected) {
                    break; // exit app
                } else {
                    app_state.screen = app_state.prev_screen;
                    view_port_update(view_port);
                    continue;
                }
            }

        // Advance screens when OK is pressed on the title or rules screens.
        } else if(
                event.key == InputKeyOk &&
                event.type == InputTypeShort &&
                (app_state.screen == StateTitle || app_state.screen == StateRules)
                ) {
            if(app_state.screen == StateTitle) {
                app_state.screen = StateRules;
                view_port_update(view_port);
                continue;
            } else if(app_state.screen == StateRules) {
                reset_game_state(&app_state);
                app_state.screen = StateGame;
                view_port_update(view_port);
                continue;
            }

        // GAMEPLAY LOGIC
        // Run core game logic in response to player input.
        } else if(app_state.screen == StateGame && event.type == InputTypeShort) {

            // remember how many zombies were in THIS wave.
            app_state.last_zombies = app_state.zombies;
            if(app_state.last_zombies < 0) app_state.last_zombies = 0;
            if(app_state.last_zombies > 15) app_state.last_zombies = 15;

            // prepare result buffers for this turn.
            app_state.result3[0] = '\0';

            // Branch gameplay logic based on which button was pressed.
            switch(event.key) {

                // Handle FIGHT action.
                case InputKeyRight: {

                // Mark last action as a fight (to convert dots to asterisks).
                app_state.last_action_was_fight = true;

                // Calculate chance to win the fight and roll outcome.
                float prob = fight_probability(&app_state);
                int roll = rand() % 100 + 1; // random 1-100

                if(roll < prob) {
                    // Remove defeated zombies from the total remaining count.
                    app_state.remaining -= app_state.zombies;
                    if(app_state.remaining < 0) app_state.remaining = 0;

                    // Attempt weapon upgrade on victory.
                    if((rand() % 100) > 70 && app_state.weapon < 7) {
                        app_state.weapon++;

                        // Report victory with weapon upgrade.
                        snprintf(app_state.result2, sizeof(app_state.result2),
                                "You fight and WIN!");
                        snprintf(app_state.result3, sizeof(app_state.result3),
                                "You find %s! +1", weapon_names[app_state.weapon]);

                    } else {
                        // Report victory without weapon upgrade.
                        snprintf(app_state.result2, sizeof(app_state.result2),
                                "You fight and WIN!");
                    }

                    // Slightly increase fatigue after winning a fight.
                    app_state.fatigue += 2.0f;

                } else {
                    // Increase fatigue significantly to discourage repeated failed fights.
                    app_state.fatigue += 4.0f;

                    // Apply damage based on randomness and fatigue.
                    int damage = (rand() % 15 + 1) + (int)app_state.fatigue;
                    app_state.health -= damage;

                    // Remove defeated zombies from the total remaining count.
                    app_state.remaining -= app_state.zombies;
                    if(app_state.remaining < 0) app_state.remaining = 0;

                    // Report surviving a losing fight.
                    if(app_state.health > 0) {
                        snprintf(app_state.result2, sizeof(app_state.result2), "Zombies do some damage");
                        snprintf(app_state.result3, sizeof(app_state.result3), "but you kill 'em all!");
                    }
                }

                // Generate the next wave if the player is still alive.
                if(app_state.health > 0) {
                    app_state.zombies = rand() % (app_state.weapon + 10) + 1;

                    // Clamp the next wave size so it does not exceed remaining zombies.
                    if(app_state.zombies > app_state.remaining) {
                        app_state.zombies = app_state.remaining;
                    }
                }

            } break;

                // Handle RUN action.
                case InputKeyLeft: {

                   // Mark last action as a run (skips dot to asterisk conversion).
                   app_state.last_action_was_fight = false;

                   // Fully reset fatigue on a successful escape attempt.
                   app_state.fatigue = 0;

                   // Roll outcome for the run attempt.
                   int roll = rand() % 100 + 1;

                   if(roll <= 50) {
                       // Roll for a medkit reward after successful escape.
                       if((rand() % 100) < 70) { // 70% chance
                           int heal = (rand() % 2) ? 5 : 10;
                           app_state.health += heal;
                           if(app_state.health > 100) app_state.health = 100;

                           // Report successful escape with healing.
                           snprintf(app_state.result2, sizeof(app_state.result2),
                                   "You retreat unscathed!");
                           snprintf(app_state.result3, sizeof(app_state.result3),
                                   "Medkit found! +%d", heal);
                       } else {
                           // Report successful escape without healing.
                           snprintf(app_state.result2, sizeof(app_state.result2), 
                                   "You retreat unscathed!");
                       }
                   } else {
                       // Apply fatigue penalty for a failed escape attempt.
                       app_state.fatigue += 3.0f;

                       // Apply minor damage from the failed run.                        
                       int damage = rand() % 10;
                       app_state.health -= damage;

                       // Report surviving a failed escape.
                       if(app_state.health > 0) {
                           snprintf(app_state.result2, sizeof(app_state.result2),
                                   "You barely get out alive!");
                       }
                   }

                   // Generate the next wave if the player is still alive.
                   if(app_state.health > 0) {
                       app_state.zombies = rand() % 10 + 1;

                       // Clamp the next wave size so it does not exceed remaining zombies.
                       if(app_state.zombies > app_state.remaining) {
                           app_state.zombies = app_state.remaining;
                       }
                   }
               } break;            

                default:
                    break;
            }

            // DEATH CHECK
            // Check for end-of-game conditions or transition to results.
            if(app_state.remaining <=0) {

                // Handle victory when all zombies are defeated.
                app_state.remaining = 0;
                app_state.screen = StateWin;        

            } else if(app_state.health <=0) {

                // Handle player death and prompt for retry.
                app_state.health = 0;    
                app_state.yesno_selected = 1;
                app_state.screen = StateRetry;

            } else {

                // Build result display showing remaining zombies and action outcome.
                char marks[16];
                int max = (int)sizeof(marks) - 1;
                int count = app_state.last_zombies;

                // Clamp mark count to display limits.
                if(count < 0) count = 0;
                if(count > max) count = max;

                // Use '*' for fights and '.' for runs.
                memset(marks, app_state.last_action_was_fight ? '*' : '.', count);
                marks[count] = '\0';

                // Show remaining zombies and wave result markers.
                snprintf(app_state.result1, sizeof(app_state.result1),
                        "%d %s", app_state.remaining, marks);

                // Advance to result screen.
                app_state.screen = StateResult;
            }

            // Request a screen redraw to reflect updated game state.
            view_port_update(view_port);

        // Handle input on the result screen.
        } else if(app_state.screen == StateResult) {

            if(event.type == InputTypeShort) {
                // Acknowledge results and advance to the next wave.
                app_state.screen = StateGame;
                update_wave_text(&app_state);
                view_port_update(view_port);
            } 

        // Handle input on the retry screen after player death.
        } else if(app_state.screen == StateRetry) {

            if(event.key == InputKeyLeft || event.key == InputKeyRight) {
                // Toggle between Retry and Exit options.
                app_state.yesno_selected = !app_state.yesno_selected;
                view_port_update(view_port);

            } else if(event.key == InputKeyOk) {
                // Confirm selected option.
                if(app_state.yesno_selected) {
                    // Restart the game from the beginning.
                    reset_game_state(&app_state);
                    app_state.screen = StateGame;
                } else {
                    // Exit the game.
                    break;
                }
                view_port_update(view_port);
            }

        // Handle input on the victory screen after winning the game.
        } else if(app_state.screen == StateWin) {

            if(event.key == InputKeyLeft || event.key == InputKeyRight) {
                // Toggle between Retry and Exit options.
                app_state.yesno_selected = !app_state.yesno_selected;
                view_port_update(view_port);

            } else if(event.key == InputKeyOk) {
                // Confirm selected option.
                if(app_state.yesno_selected) {
                    // Restart the game from the beginning.
                    reset_game_state(&app_state);
                    app_state.screen = StateGame;
                } else {
                    // Exit the game.
                    break; // quit game
                }
                view_port_update(view_port);
            }
        }
    }

    // Clean up allocated resources before exiting the application.
    furi_message_queue_free(event_queue);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    // Exit the application successfully.
    return 0;
}

