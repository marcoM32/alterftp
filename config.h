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
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdio.h>

struct creds
{
    char user[32];
    char pwd[32];
    unsigned int sumcheck;
};

int save_credentials(char *, struct creds);
int get_credentials(char *, struct creds *);

#endif // CONFIG_H_INCLUDED
