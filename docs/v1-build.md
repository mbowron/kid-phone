# Toddler phone v1: BOM, hook switch and wiring

For Lainey and Turner, Christmas 2026. Firmware: `mbowron/kid-phone`, branch `hook-switch` (based on upstream 0.4.1).

v1 = five lit buttons, a hook switch, outgoing calls only. No incoming calls, no ringer, no amp or speaker.

## What changed in the simplified build

| Before | Now | Why |
|---|---|---|
| 5 MOSFETs + 10 resistors | One **ULN2803A** chip | Eight lamp switches in one chip, resistors built in |
| Perfboard shield with soldered headers and JST connectors | **Screw-terminal breakout** for the ESP32 + a small lamp-driver board | Wires go into screw terminals; no JST crimping |
| Separate USB-C power input, diode, capacitors | **Power through the board's COM port** | One port for power and flashing, brought to the back panel |
| Hand-crimped button wires | **Pre-crimped arcade harness** | Push onto the button tabs; no crimper needed |
| Soldered ground and 12 V runs | **Lever connectors** (Wago 221) | Bus wires snap together and come apart for service |
| Amp, speaker, grille | Gone | Not needed without incoming calls |

Soldering left: the USB-OTG pad on the board, and the ULN2803A board (socket, a few terminals, a handful of joints).

## 1. Parts to buy

### Electronics

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| ESP32-S3 **N16R8** dev board, dual USB-C (COM + USB) | 1 (2-pack) | $23 | The muonsortes board. Second one is your bench board. |
| Screw-terminal breakout / expansion board for **ESP32-S3-DevKitC-1** (44-pin) | 1 | $10 | The board plugs in; every pin gets a screw terminal. **Check the breakout's header row spacing matches your board** before ordering (DevKitC-1 clones are usually 25.4 mm). |
| USB-C retro telephone handset with its own USB audio chip | 1 | $8 | Same as the guide |
| EG STARTS 10x 12 V LED arcade push buttons (5 colors) | 1 pack | $25 | Two of each color = a full spare set |
| MT3608 boost converter module | 1 (5-pack) | $8 | 5 V → 12 V for the lamps. **Set to 12 V with a meter before connecting lamps** (13 V if the lamps look dim). |
| ULN2803A, DIP-18 | 2 | $3 | One spare |
| 18-pin DIP socket | 1 | $1 | So the chip is replaceable |
| Small perfboard (5 × 7 cm) + 2-pin screw terminal blocks, 5.08 mm | 1 + 8 | $8 | The lamp-driver board |
| USB-C charger 5 V / 2 A | 1 | $0–12 | |
| Panel-mount USB-C extension, male → female, **data-rated**, ~30 cm | 1 | $10 | COM port to the back panel: power and flashing through one port |

### Hook switch

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| Plastic-cased magnetic reed sensors, wired | 1 pack | $9 | Contacts **close** with the magnet near; check with a meter |
| Neodymium disc magnets 10 × 3 mm | 1 pack | $8 | Only if the earpiece magnet test fails (section 3) |
| Two-part 5-minute epoxy | 1 | $6 | |

### Wiring

| Part | Qty | ~Cost | Notes |
|---|---|---|---|
| Pre-crimped arcade button harness (quick-connect ends) | 1 set | $10 | Get one with **both** 4.8 mm (switch) and 2.8 mm (lamp) ends; measure the lamp tabs first |
| Wago 221 lever connectors, 3- and 5-port | 1 small kit | $12 | GND bus and 12 V bus |
| 22 AWG stranded hookup wire, 4 colors | 1 set | $10 | Red 12 V, black GND, yellow switches, blue lamp − |
| Heat shrink, cable clips, M3 standoffs | — | $15 | Probably in the shop already |

**Electronics total:** about $120–150 with spares and kits you'll keep.

### Enclosure (your design; what the electronics need)

| Item | Notes |
|---|---|
| Walnut or white oak, 1/2" | Counterbore under each button to leave a web the nut can clamp |
| Bottom panel, 1/4" ply | On threaded inserts so you can get inside |
| Clear acrylic 1/16" | Directory window |
| Felt pads | Cradle saddles |
| Rubber feet + some weight | Keeps it on the table when the cord gets yanked |
| Cord grommet | Handset cord entry |
| Child-safe finish | Hardwax oil or water-based poly, fully cured |

## 2. Tools

| Tool | Notes |
|---|---|
| Soldering iron | OTG pad + the lamp-driver board |
| Multimeter | Setting the MT3608, checking the reed sensor |
| Small flat screwdriver | Screw terminals |
| Wire strippers, flush cutters | |
| 1-1/8" Forstner bit | Button holes; test-fit in scrap |
| Computer with Chrome or Edge | Flashing |

No crimper needed.

## 3. Hook switch

**Try the free version first.** The earpiece speaker already has a magnet. Tape a reed sensor under each fork position (wired in parallel), meter on continuity, and set the handset down both ways round. If it trips reliably every time, you're done: no magnet, no opening the handset.

**Otherwise:** a reed sensor in a pocket routed from below under the center saddle (2–3 mm wood skin), and one 10 × 3 mm magnet **epoxied inside** the handle at its midpoint.

**Never glue a magnet to the outside of the handset or anywhere a child can reach.** Swallowed neodymium magnets are a surgical emergency. If the handset can't be opened, use a hinge-lever microswitch under a dowel plunger in one fork instead.

**Bench test before cutting wood:** meter on the reed leads, lower the handset through a scrap of your wood, and note where it beeps. You want at least 5 mm of margin beyond your design gap, in all four orientations. Lifted 3–4 cm, it must stop.

**Wiring:** reed leads to GPIO 10 and GND; polarity doesn't matter.

| Handset | Phone |
|---|---|
| Lifted | Dial tone (quiet after 30 s), buttons armed |
| Put down | Hangs up, or stops a secret message |
| Down + button pressed | That button's light winks twice |
| Hook wire broken | Reads as lifted; the phone still calls |

The setup page's Handset row shows "picked up" / "on the hook".

## 4. Pin map and wiring

| Function | GPIO |
|---|---|
| Buttons 1–5 (left to right) | 4, 5, 6, 7, 8 |
| Lights 1–5 | 15, 16, 17, 18, 9 |
| Hook sensor | 10 |

Confirm pin positions against your board's silkscreen.

**Buttons:** for each button, switch tab A → its button GPIO terminal, switch tab B → GND bus. Polarity doesn't matter.

**Lamps through the ULN2803A:**

| From | → To |
|---|---|
| Light GPIO 15, 16, 17, 18, 9 | ULN2803A inputs 1B–5B (pins 1–5) |
| ULN2803A GND (pin 9) | GND bus |
| ULN2803A outputs 1C–5C (pins 18–14) | Lamp − of buttons 1–5 |
| Every lamp + | 12 V bus |
| ULN2803A COM (pin 10) | Leave unconnected |

Input 1 (pin 1) pairs with output 1 (pin 18), input 2 (pin 2) with output 2 (pin 17), and so on. If a lamp stays dark, swap its two wires.

**Power:**

| From | → To |
|---|---|
| Charger → panel USB-C → board **COM** port | Powers everything |
| Board **5V** pin | MT3608 IN+ |
| Board GND | MT3608 IN−, GND bus |
| MT3608 OUT+ | 12 V bus |
| MT3608 OUT− | GND bus |
| Handset | Board **USB** port (OTG pad bridged) |

**Voltage levels:** 12 V only reaches the lamps and the ULN2803A outputs. The GPIOs are 3.3 V and only touch ULN2803A inputs, the button switches and the reed sensor.

**Rules:**
- The handset goes in the **USB** port only. The charger and computer go in **COM** only, through the back-panel port.
- **Check the board can carry the load from COM:** the 5 V path from COM usually runs through a small diode. Read its marking. An SS34 or similar 2–3 A part is fine. If it's tiny or you can't tell, leave the lamps on for 10 minutes on the bench and feel it.

## 5. Power budget (5 V, from COM)

| Load | Typical | Peak |
|---|---|---|
| ESP32-S3 with WiFi | 100 mA | 350 mA |
| USB handset | 50 mA | 100 mA |
| 5 lamps via MT3608 (85 mA at 12 V) | 250 mA | 250 mA |
| **Total** | **~400 mA** | **~700 mA** |

A 5 V / 2 A charger is plenty. A laptop port can usually flash it. If the board resets while flashing with the lamps on, unplug the MT3608 or use a powered hub.

## 6. Order of work

1. Bridge the OTG pad, flash the bench board, plug in the handset, and make a test call with one button.
2. Breadboard the hook sensor: does the earpiece magnet trip it? Confirm on the setup page.
3. Set the MT3608 to 12 V, then build the ULN2803A board and light one lamp.
4. Wire all five buttons with the harness and lever connectors on the bench.
5. Build the enclosure around the tested harness.
6. Directory card last.
