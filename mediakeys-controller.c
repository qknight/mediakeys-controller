/*
 * Based on keytable.c by Mauro Carvalho Chehab
 * Modified by Joachim Schiele <js@lastlog.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <linux/input.h>

#include <string.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_KEEPING_PRESSED 2

#include "parse.h"

void prtcode(int codes) {
    struct parse_key *p;

    for (p = keynames; p->name != NULL; p++) {
        if (p->value == (unsigned) codes) {
            printf("scancode %s (0x%02x)\n", p->name, codes);
            return;
        }
    }

    if (isprint(codes)) {
        printf("scancode '%c' (0x%02x)\n", codes, codes);
    } else {
        printf("scancode 0x%02x\n", codes);
    }
}

int main (int argc, char *argv[]) {
    int i, fd;
    struct input_event ev[64];

    if (argc != 2) {
        fprintf(stderr, "usage: %s event-device (/dev/input/eventX)\n", argv[0]);
        return 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("Couldn't open input device");
        return 1;
    }

    while (1) {
        size_t rb = read(fd, ev, sizeof(ev));

        if (rb < (int) sizeof(struct input_event)) {
            perror("short read");
            return 1;
        }

        for (i = 0; i < (int) (rb / sizeof(struct input_event)); i++) {
            if (EV_KEY == ev[i].type) {
                if ((ev[i].value == KEY_PRESS) || (ev[i].value == KEY_KEEPING_PRESSED)) {
                    //prtcode(ev[i].code);
                    //printf("type %d code %d value %d\n", ev[i].type, ev[i].code, ev[i].value);


                        // mute - scancode KEY_MUTE (0x71) // type 1 code 113 value 1
			if (ev[i].type == 1 && ev[i].code == 113 && ev[i].value == 1) {
				system("sh volume.sh mute");
                    		printf("calling script to toggle mute state\n");
			}
                        //voldown - scancode KEY_VOLUMEDOWN (0x72) type 1 code 114 value 1
			if (ev[i].type == 1 && ev[i].code == 114 && ev[i].value == 1) {
				system("sh volume.sh down");
                    		printf("calling script for volume down\n");
			}
                        //volup - scancode KEY_VOLUMEUP (0x73) type 1 code 115 value 1
			if (ev[i].type == 1 && ev[i].code == 115 && ev[i].value == 1) {
				system("sh volume.sh up");
                    		printf("calling script for volume up\n");
			}
                }
            }
        }
    }

    return 0;
}
