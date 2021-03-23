<!--
SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>

SPDX-License-Identifier: MIT
-->

# Remote Alarm

Remote alarm for garage or box room. Designed at SUSE Hackweek 20.

The alarm is made up of two nodes. The sender is installed in the garage and has a door sensor, a relay module to control the light, an active buzzer and a RFID reader to armour and unarmour the alarm. It keeps a hearbeat signal and report events over an encrypted LoRa link to the receiver, installed at home. The receiver node report the events by email.

## Hardware

The schematics and PCB design is available in the pcb folder.

## Documentation

The state machine diagram and flowcharts are in the doc folder.

## License

MIT and CC0-1.0
