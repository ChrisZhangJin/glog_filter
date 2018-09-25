#include <stdio.h>
#include <string.h>
//#include <pcre2.h>
#include <unistd.h>
#include <stdlib.h>


#define LINE_HEAD_PATTERN "[A-Z]\\d{4}\\s\\d{2}:\\d{2}:\\d{2}.\\d{6}"
#define LINE_HEAD_LEN 22


void process_line(FILE *ostream, char *line, size_t len, const char *id, short *last) {
    if (len <= LINE_HEAD_LEN && *last == 1) {
        printf("this is continue line from the last one!\n");
        fputs(line, ostream);
        return;
    }

    char *p = line + LINE_HEAD_LEN;
    while(*p == ' ') p++;

    // find the filter id
    if (strstr(p, id) == p && *(p+strlen(id)) == ' ') {
        fputs(line, ostream);
        if (*last != 1) {
            *last = 1;
        }
    } else {
        if (*last == 1) {
            *last = 0;
        }
    }
}

void process(const char* path, const char *id) {
    FILE *ifile = fopen(path, "r");
    if (!ifile) {
        fprintf(stderr, "open file %s failed!\n", path);
        return;
    }

    FILE *ofile = fopen(id, "w+");
    if (!ofile) {
        fprintf(stderr, "create file %s failed!\n", id);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    short last = 0;
    while (getline(&line, &len, ifile) != -1) {
        process_line(ofile, line, len, id, &last);
    }

    if (line) {
        free(line);
    }
    fclose(ifile);

    fseek(ofile, 0, SEEK_END);
    if (ftell(ofile) == 0){
        printf("no content found. unlink this file!\n");
        unlink(id);
    }
    fclose(ofile);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "illegal parameters!\n");
        return 1;
    }

    const char *path = argv[1];
    const char *id = argv[2];
    process(path, id);
    printf("done!");
    return 0;
}