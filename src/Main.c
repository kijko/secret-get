#include <stdio.h>
#include <argp.h>

static int parse_opt (int key, char *arg, struct argp_state *state) {
     switch (key) {
        case 's': printf("%s\n", arg); break;
     }

     return 0;
} 

int main(int argc, char **argv) {
    struct argp_option options[] = {
         { "storage", 's', "ENUM", 0, "Choose storage. Supported: [BW,]. Default: BW"},
         { 0 }
    };
    struct argp argp = { options, parse_opt }; 

    return argp_parse (&argp, argc, argv, 0, 0, 0);
}

