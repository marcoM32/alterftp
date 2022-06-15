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
#include "config.h"

static unsigned int alterftp_sum(struct creds *c);
static int alterftp_check_config_sum(struct creds *c);

static unsigned int alterftp_sum(struct creds *c)
{
    unsigned int r = 0;
    int x;

    for(x = 0; x < 32; x++)
    {
        r += c->user[x];
        r += c->pwd[x];
    }

    return r;
}

static int alterftp_check_config_sum(struct creds *c)
{
    unsigned int sumcheck;

    sumcheck = alterftp_sum(c);

    return (sumcheck == (c->sumcheck));
}

int save_credentials(char *path, struct creds c)
{
    c.sumcheck = alterftp_sum(&c);
    fprintf(stdout, "CHECKSUM: %u\n", c.sumcheck);

    FILE *config_f = fopen(path, "wb");
    if(config_f == NULL)
    {
        perror("fopen");
        return 0;
    }
    fwrite(&c, sizeof(struct creds), 1, config_f);
    fclose(config_f);

    return 1;
}

int get_credentials(char *path, struct creds *c)
{
    FILE *config_f = fopen(path, "rb");
    if(config_f == NULL)
    {
        perror("fopen");
        return 0;
    }
    fread(c, sizeof(struct creds), 1, config_f);
    fclose(config_f);

    if(!alterftp_check_config_sum(c))
    {
        fprintf(stdout, "ERROR parsing credentials, please run <init>.\n");
        return 0;
    }

    return 1;
}
