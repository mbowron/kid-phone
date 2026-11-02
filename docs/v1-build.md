# Toddler phone v1: BOM, hook switch and wiring

For Lainey and Turner, Christmas 2026. Firmware: `mbowron/kid-phone`, branch `hook-switch` (based on upstream 0.4.1).

v1 = five lit buttons, a hook switch, outgoing calls only. Incoming calls (ringer) are v2. The amp and speaker are listed as optional so v2 only needs a firmware update.

## 1. Parts to buy

### Electronics

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| ESP32-S3 **N16R8** dev board, dual USB-C (COM + USB) | 1 (2-pack) | $23 | The board from the muonsortes guide. Pins come pre-soldered on the linked one. The second board is your bench/test board. |
| USB-C retro telephone handset with its own USB audio chip | 1 | $8 | Same as the guide. Check it works on a phone or laptop first. |
| EG STARTS 10x 12 V LED arcade push buttons (5 colors) | 1 pack | $25 | 24–28 mm hole, 32 mm across, 65 mm deep. Two of each color = a full spare set. |
| MT3608 boost converter module (2 A, adjustable) | 1 (5-pack) | $8 | 5 V → 12 V for the button lamps. **Set to 12 V with a meter before connecting lamps.** |
| 2N7000 N-channel MOSFET, TO-92 | 5 (10-pack) | $5 | One per lamp |
| Resistor 100 Ω, 1/4 W | 5 | — | Gate resistors (from the assortment) |
| Resistor 100 kΩ, 1/4 W | 5 | — | Gate pull-downs: lamps stay dark while the board boots |
| Resistor assortment kit | 1 | $10 | Skip if you have one |
| USB-C power breakout board **with 5.1 kΩ CC resistors** | 1 (pack) | $7 | Power input. Without CC resistors a USB-C charger supplies nothing. |
| Schottky diode 1N5822 (3 A) | 1 (pack) | $5 | Blocks back-feed between the power input and the COM port |
| Electrolytic cap 470 µF 10 V + 0.1 µF ceramic | 1 each | $5 (kit) | Across the 5 V rail at the shield, for WiFi current spikes |
| USB-C charger 5 V / 2 A or more | 1 | $0–12 | Any phone charger |
| USB-C data cable | 1 | $0–8 | For flashing; must carry data |

### Hook switch (see section 3)

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| Plastic-cased magnetic reed sensors (door/window contacts), wired | 1 pack of 5 | $9 | Pick a pack whose contacts **close** when the magnet is next to them; check with a meter |
| Neodymium disc magnets 10 × 3 mm (N52) | 1 pack | $8 | One goes **sealed inside** the handset |
| Two-part 5-minute epoxy | 1 | $6 | Magnet fixing |

### Connectors and wiring

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| Double-sided perfboard 7 × 9 cm | 1 (pack) | $8 | The shield |
| Female header strips 2.54 mm | 4 | $6 | The board plugs in and stays removable |
| JST-XH kit (2-, 3-, 4-pin plugs, headers, crimp pins) | 1 | $12 | One 4-pin per button, 2-pin for hook and power |
| Female quick-disconnect crimp terminals, 4.8 mm (.187") | 20 | $6 | Microswitch tabs. **Measure the lamp-holder tabs** (often 2.8 mm) and buy those too. |
| 22 AWG stranded hookup wire, 6 colors | 1 set | $15 | Suggested: red 12 V, orange 5 V, black GND, yellow switch, blue lamp −, green hook |
| Heat shrink assortment | 1 | $8 | |
| M3 brass standoffs and screws kit | 1 | $10 | Shield and boost module mounting |
| Adhesive cable clips / P-clips | ~10 | $6 | Cable runs and handset cord strain relief |

### Optional (recommended)

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| Panel-mount USB-C extension, male to female, **data-rated**, ~30 cm | 1 | $10 | Brings the COM port to the back panel so you can flash firmware without opening the case |
| MAX98357A I2S amp breakout | 1 | $6 | v2 ringer. Wire it now, use it later. |
| Speaker 4 Ω 3 W, 40–50 mm | 1 | $6 | v2 ringer, behind a drilled grille |

**Electronics total:** about $150–175, including kits you'll keep. The core parts are about $90.

### Enclosure materials (your call; these are the parts the electronics need)

| Item | Notes |
|---|---|
| Hardwood for the body (walnut in the render) | Top panel at the buttons must be **no thicker than the button's threaded body**. Measure a button; counterbore from below if the top is thicker. |
| Removable bottom panel (1/4" ply or hardwood) | Screws into threaded inserts so everything is serviceable |
| Clear acrylic 1/16" (1.5 mm) | Directory card cover, slides into a routed slot |
| Felt or leather pads | Lining for the cradle saddles, no rattle and no scratches |
| Rubber feet (≥ 4) + some lead or steel weight in the base | Keeps the phone on the table when the cord gets yanked |
| Threaded inserts M3 or #6 | For the bottom panel |
| Child-safe finish | Hardwax oil or water-based poly, fully cured before gifting |
| Cord grommet | Where the handset cord enters the body |

## 2. Tools

| Tool | Notes |
|---|---|
| Soldering iron + solder | One blob on the board's USB-OTG pad, plus the shield |
| **Multimeter** | Setting the MT3608 to 12 V, checking the reed sensor, continuity |
| Ratcheting crimper for JST-XH and quick-disconnects | Two different dies, or one tool with interchangeable dies (~$25–35) |
| Wire strippers, flush cutters | |
| 1-1/8" Forstner bit (28.6 mm) | Button holes. Test-fit in scrap first; the range is 24–28 mm. |
| Forstner bits for counterbores and the speaker grille | Grille: a pattern of 4–5 mm holes |
| Computer with Chrome or Edge | Flashing the stock installer, or PlatformIO for the fork |
| Hot glue or zip ties | Strain relief on the handset cord inside the body |

## 3. Hook switch design

### Why a magnet and not a mechanical switch

The classic design is a spring plunger in the cradle that the handset presses down. With toddlers it has three problems:
- The handset weighs only ~100–150 g, spread across two cradle points, so a mechanical switch with a stiff spring may never trip.
- The plunger is a pinch point and a dust trap.
- A 3-year-old will press, pull and jam anything that moves.

A **reed sensor hidden under the cradle, with a magnet sealed inside the handset**, has no moving parts. It can't jam and it's invisible. Electrically it's just a switch that closes when the magnet is near, which is exactly what the firmware expects: closed to GND = handset resting.

### Layout

```
          handset (side view, resting)
   ┌──────────────────────────────────────────┐
   │ ear cup        [magnet in handle]   mouth │
   └──┬─────────────────────▼─────────────┬───┘
      │ ▽ fork               ▄ saddle    ▽ │ fork
 ─────┴─────────────────────┬┴┬───────────┴─────  top surface (slope)
                            │R│  ← reed sensor in a pocket under a 2–3 mm wood skin
```

- **Two forks**, one near each cup, carry the handset. Line their V-saddles with felt.
- **A low center saddle** under the middle of the handle locates it. Under its 2–3 mm wood skin, route a pocket from below for the reed sensor.
- **Magnet:** one 10 × 3 mm disc **inside** the handle at its midpoint, centered in the cavity. Centered, it's the same distance from the reed whichever way the kids put the handset down, end-to-end or upside down.
- **Reed sensor:** both leads go to the shield's `HOOK` connector, one to GPIO 10 and one to GND. Polarity doesn't matter.

### Safety: the magnet must be sealed inside the handset

**Never glue a neodymium magnet to the outside of the handset or anywhere a child can reach.** Swallowed neodymium magnets can attract each other through intestine walls, and that's a surgical emergency.
- Open the handset. Most have screws under the ear-cup cap or a seam with clips.
- Epoxy the magnet inside the handle, away from the speaker and the mic.
- Close it back up.
- If your handset can't be opened, use the mechanical fallback below instead.

### Bench test before you cut wood

1. Set the meter to continuity and connect it to the two reed leads.
2. With the magnet in the handset, lower the handset toward the sensor through a 3 mm scrap of your wood. Note the gap where it beeps; aim for at least 5 mm of margin beyond your design gap.
3. Try it end-to-end flipped and upside down. All four positions must trip.
4. Lift the handset 3–4 cm. The beep must stop.
5. If the margin is thin, stack two magnets or thin the wood skin over the reed.

**Check before adding any magnet:** the earpiece speaker already contains a magnet. Put a reed sensor under each fork, wired in parallel, and test whether the earpiece alone trips whichever one it lands on. If it does reliably in all four positions, you don't need to open the handset at all. Treat that as a bonus, not the plan.

### Fallback: mechanical plunger

If the handset can't be opened:
- Use a hardwood dowel plunger, 12 mm, in a 12.5 mm bore through one fork, with a stop collar so it can't pull out, resting on the lever of a hinge-lever microswitch underneath. Omron SS-5GL-type switches are the usual pick.
- **Weigh the handset first.** Pick a switch whose datasheet operating force is well under the weight the plunger actually carries, which is roughly half the handset's weight.
- Wire it so the contact that closes when the plunger is pressed goes to GPIO 10, and COM goes to GND.

### What the firmware does with it

| Handset | Phone |
|---|---|
| Lifted | Dial tone (quiet again after 30 s of nothing), buttons armed |
| Put down | Hangs up a call, or stops a secret message |
| Down + button pressed | That button's light winks twice, nothing else |
| Hook wire broken | Reads as lifted: the phone still calls like the stock firmware |

The setup page shows the state on the Handset row ("picked up" / "on the hook"), which helps when you're tuning the gap.

## 4. Pin map and wiring

| Function | GPIO | Notes |
|---|---|---|
| Buttons 1–5 | 4, 5, 6, 7, 8 | Default wiring in the fork, left to right |
| Lights 1–5 | 15, 16, 17, 18, 9 | To MOSFET gates |
| Hook sensor | 10 | Reserved, not assignable on the setup page |
| Amp BCLK / LRC / DIN (v2) | 11 / 12 / 13 | Optional now |

Header positions follow the ESP32-S3-DevKitC-1 pinout; **confirm against your board's silkscreen** before soldering.

**Per button (one 4-pin JST-XH):**

| Button side | → Shield |
|---|---|
| Microswitch tab A | Button GPIO (internal pull-up) |
| Microswitch tab B | GND |
| Lamp + | **12 V rail** |
| Lamp − | That button's MOSFET drain |

If a lamp stays dark, swap its two wires; LEDs only work one way round.

**MOSFET driver (×5, 2N7000):** light GPIO → 100 Ω → gate; gate → 100 kΩ → GND; source → GND; drain → lamp −.

**Power:**

| From | → To |
|---|---|
| USB-C breakout VBUS | 1N5822 anode |
| 1N5822 cathode | 5 V rail → board **5V** pin, MT3608 IN+, (amp VIN) |
| Breakout GND | GND rail → board GND, MT3608 IN−, all grounds |
| MT3608 OUT+ | 12 V rail (lamp + only) |
| MT3608 OUT− | GND rail |
| 470 µF + 0.1 µF | Across the 5 V rail, near the board |

**Voltage levels:**
- 12 V only ever reaches the lamps and the MOSFET drains.
- GPIOs are 3.3 V and only touch MOSFET gates, button switches and the reed sensor.
- Nothing above 3.3 V touches a GPIO.

**Rules:**
- The handset goes in the board's **USB** port only. The computer and charger go in **COM** only.
- Unplug the power input whenever a laptop is on COM.
- After the 1N5822 the handset sees about 4.7 V. Confirm it works on the breadboard before building the shield.

## 5. Power budget

| Load | Typical | Peak |
|---|---|---|
| ESP32-S3 with WiFi | 100 mA | 350 mA |
| USB handset | 50 mA | 100 mA (estimate) |
| 5 lamps via MT3608 (85 mA at 12 V) | 250 mA | 250 mA |
| Amp (v2, ringing at low gain) | 0 | ~300 mA |
| **Total at 5 V** | **~400 mA** | **~1 A** |

A 5 V / 2 A charger leaves 2× margin.

## 6. Order of work

1. Flash the bench board, put the handset on it, and make a test call with one button on the breadboard.
2. Breadboard the hook sensor and confirm "picked up / on the hook" on the setup page.
3. Set the MT3608 to 12 V, then breadboard one lamp through a MOSFET.
4. Build the shield.
5. Build the enclosure around the shield and harness.
6. Directory card last.
