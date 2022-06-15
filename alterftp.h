/*
 * A small utility to work on ftp server with Curl
 *
 * Copyright 2016 @gilmoa for the public domain original code
 * Copyright 2022 Marco Magliano for this review of the file
 *
 * This file is part of alterftp#.
 *
 * alterftp# is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * alterftp# is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with alterftp#.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif
#include "config.h"

#define CODE_NAME "Boredom"
#define PROGRAM_VERSION "v1.0.0" " (" CODE_NAME ")"

#define CONF_PATH ".alterftp#_conf"
#define ALTERFTP_USER_AGENT "'" "AlterFTP# " PROGRAM_VERSION "'"
#define DEFAULT_CURL_FLAGS "-vSs#A " ALTERFTP_USER_AGENT

// https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
#define STARTS_WITH(string_to_check, prefix) (strncmp(string_to_check, prefix, \
        ((sizeof(prefix) / sizeof(prefix[0])) - 1)) ? 0 : \
        ((sizeof(prefix) / sizeof(prefix[0])) - 1))

static struct option long_options[] =
{
    { "init", no_argument, NULL, 'i' },
    { "mkdir", required_argument, NULL, 'm' },
    { "rmdir", required_argument, NULL, 'r' },
    { "send", required_argument, NULL, 's' },
    { "delete", required_argument, NULL, 'd' },
    { "list", no_argument, NULL, 'l' },
    { "download", required_argument, NULL, 'g' },
    { "help", no_argument, NULL, 'h' },
    { NULL, no_argument, NULL, 0 }
};

void print_usage(char *);
void do_init();
void do_execute(char, struct creds *, char *, char *);
int curl_cmd(char *[]);

#endif // MAIN_H_INCLUDED
