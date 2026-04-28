# Fruit Weight Scale - AVR Embedded System

An embedded fruit weight scale built on the **ATmega32** microcontroller. It weighs fruit using an HX711 load cell amplifier, calculates the price, displays results on a 16x4 LCD, and stores transaction history in EEPROM.

---

## Hardware Components

| Component | Description |
|---|---|
| ATmega32 | 8 MHz AVR microcontroller |
| HX711 | 24-bit load cell amplifier |
| Load cell | Weight sensor |
| 16x4 LCD | Display (4-bit mode) |
| 4x4 Keypad | User input |
| EEPROM (internal) | Stores settings & transaction history |

---

## Pin Connections (Wiring)

### HX711 Load Cell Amplifier
| HX711 Pin | ATmega32 Pin |
|---|---|
| DOUT | PA0 |
| SCK | PA1 |
| VCC | 5V |
| GND | GND |

### 16x4 LCD (4-bit mode)
| LCD Pin | ATmega32 Pin |
|---|---|
| D4–D7 | PC4–PC7 (PORTC) |
| RS | PD0 |
| RW | PD1 |
| EN | PD2 |
| VSS / RW | GND |
| VDD | 5V |

### 4x4 Keypad
| Keypad | ATmega32 Pin |
|---|---|
| Rows & Cols | PB0–PB7 (PORTB) |

> Full wiring diagram: [`docs/wiring the project.pdf`](docs/wiring%20the%20project.pdf)

---

## Features

**User Mode**
- Real-time weight display in grams
- Select from 5 fruits: Apple, Banana, Orange, Grapes, Mango
- Automatic price calculation (EGP/kg)
- Print receipt and save to EEPROM history

**Admin Mode** (password protected, default: `1234`)
- View transaction history (up to 78 records)
- Edit fruit prices per kg
- Calibrate scale with 1 kg reference weight
- Set zero / tare
- Change admin password
- Clear all history
- Factory reset via jumper

---

## Fruit Default Prices

| Fruit | Price (EGP/kg) |
|---|---|
| Apple | 25.00 |
| Banana | 15.00 |
| Orange | 20.00 |
| Grapes | 45.00 |
| Mango | 35.00 |

Prices are saved to EEPROM and persist across power cycles.

---

## EEPROM Memory Map

| Address | Data |
|---|---|
| 0x000 | Zero/tare value (4 bytes) |
| 0x004 | Scale factor (4 bytes) |
| 0x008 | Admin password (4 bytes) |
| 0x00C | Write pointer (2 bytes) |
| 0x010 | Transaction count (2 bytes) |
| 0x012 | Fruit price flags (5 bytes) |
| 0x017 | Fruit prices (20 bytes) |
| 0x02B | Setup flag |
| 0x02C | Transaction records (up to 78) |

---

## Project Structure

```
├── code/GccApplication1/GccApplication1/
│   ├── GccApplication1.c   # Main entry point
│   ├── hx711.c / .h        # Load cell driver
│   ├── lcd.c / .h          # LCD driver
│   ├── keypad.c / .h       # Keypad driver
│   ├── eeprom.c / .h       # EEPROM storage
│   ├── fruits.c / .h       # Fruit data & pricing
│   ├── admin.c / .h        # Admin menu
│   ├── user.c / .h         # User screen
│   └── globals.h           # Shared globals
├── docs/
│   └── wiring the project.pdf
├── projects lib/           # Proteus simulation libraries
└── Project Backups/
    └── weight scale.pdsprj # Proteus simulation file
```

---

## How to Build

1. Open `code/GccApplication1/GccApplication1.atsln` in **Atmel Studio 7**
2. Set MCU to **ATmega32**, clock **8 MHz**
3. Build → produces `Debug/GccApplication1.hex`
4. Flash the `.hex` to the ATmega32 using AVR ISP or USBasp

## Simulation

Open `Project Backups/weight scale.pdsprj` in **Proteus 8** to run the full simulation.
