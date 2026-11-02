#include "hook.h"

#include <stdatomic.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "app_config.h"

static const char *TAG = "hook";

/* Debounced state, written by the button task only. Starts lifted, so a
 * phone whose hook switch is missing or miswired still calls (fail open). */
static _Atomic bool s_off = true;
static int s_last_raw = 1;
static int s_same_ms;

/* Last time something happened with the handset up (dial tone timeout). */
static _Atomic int64_t s_activity_ms;

/* The light that is blinking because its button was pressed on hook. */
static _Atomic int s_nudge_slot = -1;
static _Atomic int64_t s_nudge_start_ms;

static int64_t now_ms(void)
{
    return esp_timer_get_time() / 1000;
}

bool hook_present(void)
{
    return APP_HOOK_GPIO >= 0;
}

void hook_init(void)
{
    if (!hook_present()) {
        ESP_LOGI(TAG, "no hook switch: the handset always counts as lifted");
        return;
    }
    const gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << APP_HOOK_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
    /* Take the level at boot as settled: a handset resting on the cradle at
     * power-up must not read as a fresh lift (and start a dial tone). */
    s_last_raw = gpio_get_level(APP_HOOK_GPIO);
    s_same_ms = APP_HOOK_DEBOUNCE_MS;
    atomic_store(&s_off, s_last_raw == 1);
    atomic_store(&s_activity_ms, now_ms());
    ESP_LOGI(TAG, "hook switch on GPIO %d: handset %s", APP_HOOK_GPIO, s_last_raw ? "lifted" : "down");
}

int hook_poll(void)
{
    if (!hook_present()) {
        return 0;
    }
    /* Switch closed to GND = handset resting on the lever. A broken wire
     * reads high = lifted, the same as the phone without a hook switch. */
    const int raw = gpio_get_level(APP_HOOK_GPIO);
    if (raw != s_last_raw) {
        s_last_raw = raw;
        s_same_ms = 0;
        return 0;
    }
    if (s_same_ms < APP_HOOK_DEBOUNCE_MS) {
        s_same_ms += APP_BUTTON_POLL_MS;
        return 0;
    }
    const bool off = raw == 1;
    if (off == atomic_load(&s_off)) {
        return 0;
    }
    atomic_store(&s_off, off);
    if (off) {
        hook_activity();
    }
    ESP_LOGI(TAG, "handset %s", off ? "lifted" : "down");
    return off ? 1 : -1;
}

bool hook_off(void)
{
    return atomic_load(&s_off);
}

void hook_activity(void)
{
    atomic_store(&s_activity_ms, now_ms());
}

bool hook_dial_tone_due(void)
{
    return hook_off() && now_ms() - atomic_load(&s_activity_ms) < APP_DIAL_TONE_TIMEOUT_MS;
}

void hook_nudge(int slot)
{
    atomic_store(&s_nudge_start_ms, now_ms());
    atomic_store(&s_nudge_slot, slot);
}

bool hook_nudge_light(int slot, int64_t now, bool *on)
{
    if (slot != atomic_load(&s_nudge_slot)) {
        return false;
    }
    const int64_t elapsed = now - atomic_load(&s_nudge_start_ms);
    if (elapsed < 0 || elapsed >= 2LL * APP_HOOK_NUDGE_BLINKS * APP_HOOK_NUDGE_HALF_MS) {
        return false;
    }
    /* Starts dark: a lit light visibly winks, an unlit one flashes. */
    *on = (elapsed / APP_HOOK_NUDGE_HALF_MS) % 2 == 1;
    return true;
}
