#include <stdio.h>
#include <argp.h>

static int parse_opt (int key, char *arg, struct argp_state *state) {
     switch (key) {
        case 's': printf("%s\n", arg); break;
     }

     return 0;
} 

int main(int argc, char **argv) {
//    struct argp_option options[] = {
//         { "storage", 's', "ENUM", 0, "Choose storage. Supported: [BW,]. Default: BW"},
//         { 0 }
//    };
//    struct argp argp = { options, parse_opt }; 
//
//    return argp_parse (&argp, argc, argv, 0, 0, 0);

    char *cmd = "bw list items --search google";    
    
    char buf[128];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    while (fgets(buf, 128, fp) != NULL) {
        // Do whatever you want here...
        printf("OUTPUT: %s", buf);
    }

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return 0;
}

