/*
 * Hook switch: a lever microswitch under the handset cradle (APP_HOOK_GPIO).
 *
 * Lifting the handset arms the contact buttons and starts the dial tone;
 * putting it down ends a call or a secret. A press while the handset is down
 * does nothing but blink that button's light twice, so the child learns to
 * pick up first. With APP_HOOK_GPIO < 0 the handset always counts as lifted
 * and the phone behaves as it did before the hook switch.
 *
 * hook_init() and hook_poll() belong to the button task. The other calls are
 * safe from any task.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Configure the hook GPIO (input, pull-up). Call once from the button task. */
void hook_init(void);

/* Debounce the hook switch; call every APP_BUTTON_POLL_MS. Returns +1 when
 * the handset was just lifted, -1 when it was just put down, 0 otherwise. */
int hook_poll(void);

/* True while the handset is lifted (always true with no hook switch). */
bool hook_off(void);

/* True when the phone has a hook switch configured. */
bool hook_present(void);

/* Something happened with the handset up (a call, a prompt, a secret):
 * restarts the dial tone timeout. */
void hook_activity(void);

/* Handset up and nothing has happened for less than APP_DIAL_TONE_TIMEOUT_MS:
 * the idle earpiece should play the status tone (dial tone when ready). */
bool hook_dial_tone_due(void);

/* A contact button was pressed with the handset down: blink its light. */
void hook_nudge(int slot);

/* While slot's nudge blink is running, returns true and sets *on to the
 * light level for now_ms; returns false when there is no blink to show. */
bool hook_nudge_light(int slot, int64_t now_ms, bool *on);
