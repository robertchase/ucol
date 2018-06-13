/*
   ------------------------------------------------------------
   Column

   Function :    ucol

   Purpose :     extract columns from text file

   Parms :       filename (or stdin)
                 column numbers
                 options

   Return :      0 = success

   ------------------------------------------------------------
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define COLMAX 256

static const char ver[] = "ucol.c 2.3.0 2018-06-13\n";
static const char pgm[] = "UCOL";

int
main(int argc, char * * argv) {
    FILE * fil = NULL;
    FILE * out = NULL;
    struct stat statbuf;
    char buf[4096];
    char * sta[COLMAX];
    int col[COLMAX + 1];
    char * ptr;
    int cur = 0;
    char del = ' ';
    char oDel = ' ';
    char null = 0;
    int i;
    int arg;

    if (argc == 1) {
        fprintf(stderr, "%s %s", pgm, ver);
        fprintf(stderr, "%s [-dDno] column-numbers [filename]\n", argv[0]);
        fprintf(stderr, "  -dc       use 'c' as input column delimiter\n");
        fprintf(stderr, "            (default whitespace)\n");
        fprintf(stderr, "  -Dc       use 'c' as output column delimiter\n");
        fprintf(stderr, "            (default space)\n");
        fprintf(stderr, "  -n        allow null columns\n");
        fprintf(stderr, "  -oFile    output filename\n");
        exit(1);
    }

    /* ---
     * --- parameters
     * ---
     */
    col[0] = 0;
    for (arg = 1; arg < argc; arg++) {
        /* ---
         * --- options
         * ---
         */
        if (argv[arg][0] == '-') {
            switch (argv[arg][1]) {
            case 'd':
                del = argv[arg][2];
                break;
            case 'D':
                oDel = argv[arg][2];
                break;
            case 'n':
                null = 1;
                break;
            case 'o':
                out = fopen(argv[arg] + 2, "w");
                if (out == NULL) {
                    fprintf(stderr, "unable to open:%s.\n", argv[arg] + 2);
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "invalid option '%s'.", argv[arg]);
            }
            continue;
        }
        /* ---
         * --- column numbers
         * ---
         */
        i = atoi(argv[arg]);
        if (i != 0) {
            if (cur == COLMAX) {
                fprintf(stderr, "maximum column exceeded (max=%d).\n", COLMAX);
                exit(1);
            }
            col[cur] = i;
            col[++cur] = 0;
            continue;
        }
        /* ---
         * --- filename
         * ---
         */
        if (0 != stat(argv[arg], & statbuf)) {
            fprintf(stderr, "unable to open:%s.\n", argv[arg]);
            exit(1);
        }
        fil = fopen(argv[arg], "r");
        if (fil == NULL) {
            fprintf(stderr, "unable to open:%s.\n", argv[arg]);
            exit(1);
        }
    }
    if (col[0] == 0) {
        fprintf(stderr, "no columns specified.\n");
        exit(1);
    }
    if (fil == NULL)
        fil = stdin;
    if (out == NULL)
        out = stdout;

    /* ---
     * --- for each line
     * ---
     */
    while (fgets(buf, 4096, fil)) {
        /* overlay end-of-line with null */
        buf[strlen(buf) - 1] = 0x00;
        cur = 0;
        ptr = buf;
        /* ---
         * --- mark each column
         * ---
         */
        while ( * ptr != 0x00 && cur < COLMAX) {
            /* find beginning of next column */
            if (null == 0)
                while ( * ptr == del)
                    ptr++;
            /* if there is a next column... */
            if ( * ptr != 0x00) {
                /* save start of column */
                sta[cur++] = ptr;
                /* find next delimiter or end of string */
                while ( * ptr != 0x00 && * ptr != del)
                    ptr++;
                /* terminate column with null */
                if ( * ptr != 0x00)
                    * ptr++ = 0x00;
            }
        }
        /* ---
         * --- spit out desired columns
         * ---
         */
        for (i = 0; i < COLMAX; i++) {
            if (col[i] == 0)
                break;
            if (col[i] <= cur)
                fprintf(out, "%s", sta[col[i] - 1]);
            fprintf(out, "%c", oDel);
        }
        fprintf(out, "\n");
    }

    /* ---
     * --- cleanup
     * ---
     */
    fclose(out);
    fclose(fil);
    return (0);
}
