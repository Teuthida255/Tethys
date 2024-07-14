/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2020, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <jo/jo.h>


#ifndef __TETHYS_KEYBOARD_FUNCS_H__
# define __TETHYS_KEYBOARD_FUNCS_H__

#define KEYBOARD_SCANCODE_SIZE __JO_KEYBOARD_MAPPING_SIZE + 16
#define NUM_PRINTABLE_CHARS 0x5F // 0x7E - 0x20 + 1
#define NUM_SPECIAL_CHARS 40
#define ASCII_TO_INDEX 0x20

typedef struct {
	const char code;
	short state;
    const char numpad_code;
} ScancodeState;

enum KEY_STATE {
    KEY_NOT_PRESSED = 0,
    KEY_JUST_PRESSED = 1,
    KEY_HELD = 2,
    KEY_JUST_RELEASED = 3
};

enum SPECIAL_KEY {
    KEY_LEFT = 1,
    KEY_RIGHT = 2,
    KEY_UP = 3,
    KEY_DOWN = 4,
    KEY_PAGEUP = 5,
    KEY_PAGEDOWN = 6,
    KEY_TAB = 7,
    KEY_LEFT_ALT = 8,
    KEY_LEFT_SHIFT = 9,
    KEY_LEFT_CTRL = 10,
    KEY_LEFT_GUI = 11,
    KEY_RIGHT_ALT = 12,
    KEY_RIGHT_SHIFT = 13,
    KEY_RIGHT_CTRL = 14,
    KEY_RIGHT_GUI = 15,
    KEY_MENU = 16,
    KEY_HOME = 17,
    KEY_END = 18,
    KEY_ENTER = 19,
    KEY_BACKSPACE = 20,
    KEY_ESCAPE = 21,
    KEY_CAPSLOCK = 22,
    KEY_NUMLOCK = 23,
    KEY_SCROLLLOCK = 24,
    KEY_INSERT = 25,
    KEY_PAUSE = 26,
    KEY_PRINTSCREEN = 27,
    KEY_DELETE = 28,
    KEY_F1 = 29,
    KEY_F2 = 30,
    KEY_F3 = 31,
    KEY_F4 = 32,
    KEY_F5 = 33,
    KEY_F6 = 34,
    KEY_F7 = 35,
    KEY_F8 = 36,
    KEY_F9 = 37,
    KEY_F10 = 38,
    KEY_F11 = 39,
    KEY_F12 = 40,
    KEY_NOTHING = 41
};

enum MODIFIER_KEY {
    MOD_CTRL = 0,
    MOD_ALT = 1,
    MOD_SHIFT = 2,
    MOD_GUI = 3,
};

enum KEYBOARD_LOCK {
    SCROLL_LOCK = 0,
    NUM_LOCK = 1,
    CAPS_LOCK = 2,
};

typedef struct {
    ScancodeState keyStates[KEYBOARD_SCANCODE_SIZE];
    char lastCharacter;
    short lastScancode;
    bool isSpecial;
    bool isNumpad;
    bool keyOn;
    bool newSignal;
} KeyboardMap;

// Credit to Plutiedev for the keyboard scancode mappings
// https://plutiedev.com/saturn-keyboard
static KeyboardMap keyboardMap = {
    {
        {0, KEY_NOT_PRESSED, 0}, /*0x00*/
        {-KEY_F9, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {-KEY_F5, KEY_NOT_PRESSED, 0},
        {-KEY_F3, KEY_NOT_PRESSED, 0},
        {-KEY_F1, KEY_NOT_PRESSED, 0},
        {-KEY_F2, KEY_NOT_PRESSED, 0},
        {-KEY_F12, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {-KEY_F10, KEY_NOT_PRESSED, 0},
        {-KEY_F8, KEY_NOT_PRESSED, 0},
        {-KEY_F6, KEY_NOT_PRESSED, 0},
        {-KEY_F4, KEY_NOT_PRESSED, 0},
        {-KEY_TAB, KEY_NOT_PRESSED, 0},
        {'`', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x10*/
        {-KEY_LEFT_ALT, KEY_NOT_PRESSED, 0},
        {-KEY_LEFT_SHIFT, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {-KEY_LEFT_CTRL, KEY_NOT_PRESSED, 0},
        {'q', KEY_NOT_PRESSED, 0},
        {'1', KEY_NOT_PRESSED, 0},
        {-KEY_RIGHT_ALT, KEY_NOT_PRESSED, 0},
        {-KEY_RIGHT_CTRL, KEY_NOT_PRESSED, 0},
        {-KEY_ENTER, KEY_NOT_PRESSED, 0},
        {'z', KEY_NOT_PRESSED, 0},
        {'s', KEY_NOT_PRESSED, 0},
        {'a', KEY_NOT_PRESSED, 0},
        {'w', KEY_NOT_PRESSED, 0},
        {'2', KEY_NOT_PRESSED, 0},
        {-KEY_LEFT_GUI, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x20*/
        {'c', KEY_NOT_PRESSED, 0},
        {'x', KEY_NOT_PRESSED, 0},
        {'d', KEY_NOT_PRESSED, 0},
        {'e', KEY_NOT_PRESSED, 0},
        {'4', KEY_NOT_PRESSED, 0},
        {'3', KEY_NOT_PRESSED, 0},
        {-KEY_RIGHT_GUI, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {' ', KEY_NOT_PRESSED, 0},
        {'v', KEY_NOT_PRESSED, 0},
        {'f', KEY_NOT_PRESSED, 0},
        {'t', KEY_NOT_PRESSED, 0},
        {'r', KEY_NOT_PRESSED, 0},
        {'5', KEY_NOT_PRESSED, 0},
        {-KEY_MENU, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x30*/
        {'n', KEY_NOT_PRESSED, 0},
        {'b', KEY_NOT_PRESSED, 0},
        {'h', KEY_NOT_PRESSED, 0},
        {'g', KEY_NOT_PRESSED, 0},
        {'y', KEY_NOT_PRESSED, 0},
        {'6', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {'m', KEY_NOT_PRESSED, 0},
        {'j', KEY_NOT_PRESSED, 0},
        {'u', KEY_NOT_PRESSED, 0},
        {'7', KEY_NOT_PRESSED, 0},
        {'8', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x40*/
        {',', KEY_NOT_PRESSED, 0},
        {'k', KEY_NOT_PRESSED, 0},
        {'i', KEY_NOT_PRESSED, 0},
        {'o', KEY_NOT_PRESSED, 0},
        {'0', KEY_NOT_PRESSED, 0},
        {'9', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {'.', KEY_NOT_PRESSED, 0},
        {'/', KEY_NOT_PRESSED, 0},
        {'l', KEY_NOT_PRESSED, 0},
        {';', KEY_NOT_PRESSED, 0},
        {'p', KEY_NOT_PRESSED, 0},
        {'-', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x50*/
        {0, KEY_NOT_PRESSED, 0},
        {'\'', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {'[', KEY_NOT_PRESSED, 0},
        {'=', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {-KEY_CAPSLOCK, KEY_NOT_PRESSED, 0},
        {-KEY_RIGHT_SHIFT, KEY_NOT_PRESSED, 0},
        {-KEY_ENTER, KEY_NOT_PRESSED, 0},
        {']', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {'\\', KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {0, KEY_NOT_PRESSED, 0}, /*0x60*/
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {-KEY_BACKSPACE, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {'1', KEY_NOT_PRESSED, KEY_END},
        {0, KEY_NOT_PRESSED, 0},
        {'4', KEY_NOT_PRESSED, KEY_LEFT},
        {'7', KEY_NOT_PRESSED, KEY_HOME},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {'0', KEY_NOT_PRESSED, KEY_INSERT}, /*0x70*/
        {'.', KEY_NOT_PRESSED, KEY_DELETE},
        {'2', KEY_NOT_PRESSED, KEY_DOWN},
        {'5', KEY_NOT_PRESSED, KEY_NOTHING},
        {'6', KEY_NOT_PRESSED, KEY_RIGHT},
        {'8', KEY_NOT_PRESSED, KEY_UP},
        {-KEY_ESCAPE, KEY_NOT_PRESSED, 0},
        {-KEY_NUMLOCK, KEY_NOT_PRESSED, 0},
        {-KEY_F11, KEY_NOT_PRESSED, 0},
        {'+', KEY_NOT_PRESSED, 0},
        {'3', KEY_NOT_PRESSED, KEY_PAGEDOWN},
        {'-', KEY_NOT_PRESSED, 0},
        {'*', KEY_NOT_PRESSED, 0},
        {'9', KEY_NOT_PRESSED, KEY_PAGEUP},
        {-KEY_SCROLLLOCK, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},

        {'/', KEY_NOT_PRESSED, 0}, /*0x80*/
        {-KEY_INSERT, KEY_NOT_PRESSED, 0},
        {-KEY_PAUSE, KEY_NOT_PRESSED, 0},
        {-KEY_F7, KEY_NOT_PRESSED, 0},
        {-KEY_PRINTSCREEN, KEY_NOT_PRESSED, 0},
        {-KEY_DELETE, KEY_NOT_PRESSED, 0},
        {-KEY_LEFT, KEY_NOT_PRESSED, 0},
        {-KEY_HOME, KEY_NOT_PRESSED, 0},
        {-KEY_END, KEY_NOT_PRESSED, 0},
        {-KEY_UP, KEY_NOT_PRESSED, 0},
        {-KEY_DOWN, KEY_NOT_PRESSED, 0},
        {-KEY_PAGEUP, KEY_NOT_PRESSED, 0},
        {-KEY_PAGEDOWN, KEY_NOT_PRESSED, 0},
        {-KEY_RIGHT, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
        {0, KEY_NOT_PRESSED, 0},
    },
    '\0', 0, false, false, false, false
};

// Caps Lock agnostic - get_last_key() is the
// better solution for checking specific characters
static short charToScancode[NUM_PRINTABLE_CHARS] = {
    0x29, // ' '
    0x16, // '!'
    0x51, // '"'
    0x25, // '$'
    0x2E, // '%'
    0x3D, // '&'
    0x51, // '''
    0x46, // '('
    0x45, // ')'
    0x38, // '*'
    0x55, // '+'
    0x41, // ','
    0x4E, // '-'
    0x49, // '.'
    0x4A, // '/'
    0x45, // '0'
    0x16, // '1'
    0x1E, // '2'
    0x26, // '3'
    0x25, // '4'
    0x2E, // '5'
    0x36, // '6'
    0x3D, // '7'
    0x3E, // '8'
    0x46, // '9'
    0x4C, // ':'
    0x4C, // ';'
    0x41, // '<'
    0x55, // '='
    0x49, // '>'
    0x4A, // '?'
    0x1E, // '@'
    0x1C, // 'A'
    0x32, // 'B'
    0x21, // 'C'
    0x23, // 'D'
    0x24, // 'E'
    0x2B, // 'F'
    0x34, // 'G'
    0x33, // 'H'
    0x43, // 'I'
    0x3B, // 'J'
    0x42, // 'K'
    0x4B, // 'L'
    0x3A, // 'M'
    0x31, // 'N'
    0x44, // 'O'
    0x4D, // 'P'
    0x15, // 'Q'
    0x2D, // 'R'
    0x1B, // 'S'
    0x2C, // 'T'
    0x3C, // 'U'
    0x2A, // 'V'
    0x1D, // 'W'
    0x22, // 'X'
    0x35, // 'Y'
    0x1A, // 'Z'
    0x54, // '['
    0x5C, // '\'
    0x5B, // ']'
    0x36, // '^'
    0x4E, // '_'
    0x0E, // '`'
    0x1C, // 'a'
    0x32, // 'b'
    0x21, // 'c'
    0x23, // 'd'
    0x24, // 'e'
    0x2B, // 'f'
    0x34, // 'g'
    0x33, // 'h'
    0x43, // 'i'
    0x3B, // 'j'
    0x42, // 'k'
    0x4B, // 'l'
    0x3A, // 'm'
    0x31, // 'n'
    0x44, // 'o'
    0x4D, // 'p'
    0x15, // 'q'
    0x2D, // 'r'
    0x1B, // 's'
    0x2C, // 't'
    0x3C, // 'u'
    0x2A, // 'v'
    0x1D, // 'w'
    0x22, // 'x'
    0x35, // 'y'
    0x1A, // 'z'
    0x54, // '{'
    0x5C, // '|'
    0x5B, // '}'
    0x0E, // '~'
};

static short specialToScancode[NUM_SPECIAL_CHARS + 1] = {
    0x00, // sorry nothing
    0x86, // KEY_LEFT
    0x8D, // KEY_RIGHT
    0x89, // KEY_UP
    0x8A, // KEY_DOWN
    0x8B, // KEY_PAGEUP
    0x8C, // KEY_PAGEDOWN
    0x0D, // KEY_TAB
    0x11, // KEY_LEFT_ALT
    0x12, // KEY_LEFT_SHIFT
    0x14, // KEY_LEFT_CTRL
    0x1F, // KEY_LEFT_GUI
    0x17, // KEY_RIGHT_ALT
    0x59, // KEY_RIGHT_SHIFT
    0x18, // KEY_RIGHT_CTRL
    0x27, // KEY_RIGHT_GUI
    0x2F, // KEY_MENU
    0x87, // KEY_HOME
    0x88, // KEY_END
    0x2A, // KEY_ENTER
    0x66, // KEY_BACKSPACE
    0x76, // KEY_ESCAPE
    0x58, // KEY_CAPSLOCK
    0x77, // KEY_NUMLOCK
    0x7E, // KEY_SCROLLLOCK
    0x81, // KEY_INSERT
    0x82, // KEY_PAUSE
    0x84, // KEY_PRINTSCREEN
    0x85, // KEY_DELETE
    0x01, // KEY_F1
    0x02, // KEY_F2
    0x04, // KEY_F3
    0x0C, // KEY_F4
    0x03, // KEY_F5
    0x0B, // KEY_F6
    0x83, // KEY_F7
    0x0A, // KEY_F8
    0x01, // KEY_F9
    0x09, // KEY_F10
    0x78, // KEY_F11
    0x06, // KEY_F12
};

bool is_keyboard_lock_on(short cond, short lock) {
    return (cond & (1 << (lock + 4)));
}

bool is_keyboard_key_down(short scancode) {
    return (keyboardMap.keyStates[scancode].state == KEY_JUST_PRESSED);
}

bool is_keyboard_key_pressed(short scancode) {
    return (keyboardMap.keyStates[scancode].state == KEY_JUST_PRESSED || keyboardMap.keyStates[scancode].state == KEY_HELD);
}

bool is_keyboard_key_up(short scancode) {
    return (keyboardMap.keyStates[scancode].state == KEY_JUST_RELEASED);
}

bool is_keyboard_key_released(short scancode) {
    return (keyboardMap.keyStates[scancode].state == KEY_JUST_RELEASED || keyboardMap.keyStates[scancode].state == KEY_NOT_PRESSED);
}


bool is_char_key_down(char character) {
    short index = character - ASCII_TO_INDEX;
    if (index < 0 || index >= NUM_PRINTABLE_CHARS)
        return false;
    return is_keyboard_key_down(charToScancode[index - 1]);
}

bool is_char_key_pressed(char character) {
    short index = character - ASCII_TO_INDEX;
    if (index < 0 || index >= NUM_PRINTABLE_CHARS)
        return false;
    return is_keyboard_key_pressed(charToScancode[index - 1]);
}

bool is_char_key_up(char character) {
    short index = character - ASCII_TO_INDEX;
    if (index < 0 || index >= NUM_PRINTABLE_CHARS)
        return false;
    return is_keyboard_key_up(charToScancode[index - 1]);
}

bool is_char_key_released(char character) {
    short index = character - ASCII_TO_INDEX;
    if (index < 0 || index >= NUM_PRINTABLE_CHARS)
        return true;
    return is_keyboard_key_released(charToScancode[index - 1]);
}


bool is_special_key_down(short key) {
    return is_keyboard_key_down(specialToScancode[key]);
}

bool is_special_key_pressed(short key) {
    return is_keyboard_key_pressed(specialToScancode[key]);
}

bool is_special_key_up(short key) {
    return is_keyboard_key_up(specialToScancode[key]);
}

bool is_special_key_released(short key) {
    return is_keyboard_key_released(specialToScancode[key]);
}

bool is_modifier_held(short key) {
    short left_code, right_code;
    switch (key) {
    case MOD_CTRL:
        left_code = specialToScancode[KEY_LEFT_CTRL];
        right_code = specialToScancode[KEY_RIGHT_CTRL];
        break;
    case MOD_ALT:
        left_code = specialToScancode[KEY_LEFT_ALT];
        right_code = specialToScancode[KEY_RIGHT_ALT];
        break;
    case MOD_SHIFT:
        left_code = specialToScancode[KEY_LEFT_SHIFT];
        right_code = specialToScancode[KEY_RIGHT_SHIFT];
        break;
    case MOD_GUI:
        left_code = specialToScancode[KEY_LEFT_GUI];
        right_code = specialToScancode[KEY_RIGHT_GUI];
        break;
    default:
        return false;
    }
    return (is_keyboard_key_pressed(left_code) || is_keyboard_key_pressed(right_code));
}


char get_last_key(void) {
    return keyboardMap.lastCharacter;
}

short get_last_scancode(void) {
    return keyboardMap.lastScancode;
}

bool is_last_key_special(void) {
    return keyboardMap.isSpecial;
}

bool is_last_key_numpad(void) {
    return keyboardMap.isNumpad;
}

bool is_last_key_pressed(void) {
    return keyboardMap.keyOn;
}

bool new_signal_found(void) {
    return keyboardMap.newSignal;
}

void update_keyboard_state(void) {
    PerKeyBoard* keyboard;
    ScancodeState* key_map = keyboardMap.keyStates;
    keyboardMap.newSignal = false;

    keyboard = (PerKeyBoard*)&Smpc_Peripheral[0];

    for (short i = 0; i < KEYBOARD_SCANCODE_SIZE; i++) {
        switch (key_map[i].state) {
        case KEY_JUST_PRESSED:
            key_map[i].state = KEY_HELD;
            break;
        case KEY_JUST_RELEASED:
            key_map[i].state = KEY_NOT_PRESSED;
            break;
        }
    }

    // Key was just pressed
    bool should_set = false;
    if (keyboard->cond & PER_KBD_MK) {
        if (key_map[keyboard->code].state != KEY_HELD) {
            key_map[keyboard->code].state = KEY_JUST_PRESSED;
            keyboardMap.keyOn = true;
            should_set = true;
        }
    }
    // Key was just released
    else if (keyboard->cond & PER_KBD_BR) {
        if (key_map[keyboard->code].state == KEY_HELD) {
            key_map[keyboard->code].state = KEY_JUST_RELEASED;
            keyboardMap.keyOn = false;
            should_set = true;
        }
    }

    if (should_set) {
        keyboardMap.lastScancode = keyboard->code;
        keyboardMap.isNumpad = (key_map[keyboard->code].numpad_code);
        keyboardMap.newSignal = true;
        if (keyboardMap.isNumpad) {
            if (is_keyboard_lock_on(keyboard->cond, NUM_LOCK)) {
                keyboardMap.lastCharacter = key_map[keyboard->code].code;
            }
            else {
                keyboardMap.lastCharacter = -(key_map[keyboard->code].numpad_code);
                // kludge ahead
                keyboardMap.lastCharacter = (keyboardMap.lastCharacter == -KEY_NOTHING) ? 0 : keyboardMap.lastCharacter;
            }
        }
        else {
            keyboardMap.lastCharacter = key_map[keyboard->code].code;
        }
        keyboardMap.isSpecial = (keyboardMap.lastCharacter < 0);
        if (keyboardMap.isSpecial) {
            keyboardMap.lastCharacter *= -1;
        }
        else if (!keyboardMap.isNumpad) {
            bool caps = is_keyboard_lock_on(keyboard->cond, CAPS_LOCK);
            bool shift = is_modifier_held(MOD_SHIFT);
            if (shift) {
                keyboardMap.lastCharacter = __internal_keyboard_caps_lock_mapping[keyboard->code];
            }
            else if (caps && __internal_keyboard_normal_mapping[keyboard->code] >= 'a' && __internal_keyboard_normal_mapping[keyboard->code] <= 'z') {
                keyboardMap.lastCharacter = __internal_keyboard_caps_lock_mapping[keyboard->code];
            }
            else {
                keyboardMap.lastCharacter = __internal_keyboard_normal_mapping[keyboard->code];
            }

        }
    }
}

#endif