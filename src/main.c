/* Name: main.c
 * Auth: Blake Wingard - bats23456789@gmail.com
 * Date: 04/19/2024
 * Desc: The main file for Maze Generation.
 */

/************************************************************************
 *                              INCLUDES                                *
 ************************************************************************/
#include <ctype.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "MazeTools.h"
#include "growing_tree.h"

/************************************************************************
 *                               DEFINES                                *
 ************************************************************************/

#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 10
#define DEFAULT_SPLIT 0.5

/************************************************************************
 *                            STATIC VARS                               *
 ************************************************************************/
static int quite_flag = 0;    // option to silence output
static int verbose_flag = 0;  // option to print out everything

/************************************************************************
 *                       FUNCTION DECLARATIONS                          *
 ************************************************************************/
void help();
void growingTreeMethods(int depth);

int print(const char *frmt, ...);
int printError(const char *frmt, ...);
bool isinteger(const char *str);

/************************************************************************
 *                                MAIN                                  *
 ************************************************************************/
int main(int argc, char *argv[]) {
    int opt;
    int opts_index = 0;
    size_t height = DEFAULT_HEIGHT;
    size_t width = DEFAULT_HEIGHT;
    Maze_t maze;
    FILE *outFile = stdout;
    FILE *stepFile = NULL;
    bool foundAlgo = false;
    char *userChoice = NULL;
    genAlgo_t algorithm;
    growingTreeMethods_t method = INVALID_METHOD;
    double split = DEFAULT_SPLIT;

    // clang-format off
	static struct option long_opts[] = {
		{"algorithm", required_argument, NULL, 'a'},
		{"help", no_argument, NULL, 'h'},
		{"output", required_argument, NULL, 'o'},
		{"quite", no_argument, &quite_flag, 1},
		{"verbose", optional_argument, NULL, 'v'},
		{0, 0, 0, 0}};
    // clang-format on

    while ((opt = getopt_long(argc, argv, "a:ho:qv::", long_opts,
                              &opts_index)) != -1) {
        switch (opt) {
            case 0:
                break;

            case 'a': {
                size_t len = strlen(optarg);
                userChoice = malloc(sizeof(*userChoice) * (len + 1));

                for (size_t i = 0; i < len; i++) {
                    userChoice[i] = tolower(optarg[i]);
                }
                userChoice[len] = '\0';
            }

                algorithm = strToGenAlgo(userChoice);
                if (algorithm == INVALID_ALGORITHM) {
                    printError("ERROR: %s is not a valid algorithm\n", optarg);
                    help();
                    return EXIT_FAILURE;
                } else {
                    foundAlgo = true;
                }
                free(userChoice);
                break;

            case 'h':
                help();
                return EXIT_SUCCESS;
                break;

            case 'o':
                outFile = fopen(optarg, "w");
                if (!outFile) {
                    printError("ERROR opening \"%s\": %s", optarg,
                               strerror(errno));
                    return EXIT_FAILURE;
                }
                break;

            case 'q':
                quite_flag = 1;
                break;

            case 'v':
                if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
                    optarg = argv[optind++];
                }
                if (!optarg) {
                    stepFile = fopen("steps.step", "w");
                    if (!stepFile) {
                        printError("ERROR opening \"steps.step\": %s",
                                   strerror(errno));
                        return EXIT_FAILURE;
                    }
                } else {
                    stepFile = fopen(optarg, "w");
                    if (!stepFile) {
                        printError("ERROR opening \"%s\": %s", optarg,
                                   strerror(errno));
                        return EXIT_FAILURE;
                    }
                }
                verbose_flag = 1;
                break;

            case '?':
                return EXIT_FAILURE;
                break;

            default:
                puts("help");
                return EXIT_FAILURE;
                break;
        }
    }

    if (algorithm == growing_tree) {
        if (argv[optind] == NULL || isinteger(argv[optind])) {
            print("Using default algorithm Newest-Random with 0.5 split\n");
        } else {
            size_t len = strlen(argv[optind]);
            userChoice = malloc(sizeof(*userChoice) * (len + 1));

            for (size_t i = 0; i < len; i++) {
                userChoice[i] = tolower(argv[optind][i]);
            }
            userChoice[len] = '\0';
            method = strToTreeMethod(userChoice);

            free(userChoice);

            switch (method) {
                case newestTree:
                case middleTree:
                case oldestTree:
                case randomTree:
                    // ensure next option is NULL or a digit
                    if (argv[optind + 1] != NULL &&
                        !isinteger(argv[optind + 1])) {
                        printError("ERROR: %s does not support a split\n",
                                   argv[optind]);
                        puts("Valid growing tree methods are:");
                        growingTreeMethods(1);
                        return EXIT_FAILURE;
                    }
                    break;
                case newest_middleTree:
                case newest_oldestTree:
                case newest_randomTree:
                case middle_oldestTree:
                case middle_randomTree:
                case oldest_randomTree:
                    // if the next item is NULL or a digit, split is set to
                    // default
                    if (argv[optind + 1] == NULL ||
                        (isinteger(argv[optind + 1]) &&
                         strcmp(argv[optind + 1], "1") != 0 &&
                         strcmp(argv[optind + 1], "0") != 0)) {
                        print("Using default split value %lf\n", split);
                    } else {
                        optind++;
                        char *tmp;
                        split = strtod(argv[optind], &tmp);

                        if (errno != 0) {
                            printError("ERROR: %s\n", strerror(errno));
                            return EXIT_FAILURE;
                        } else if (argv[optind] == tmp) {
                            printError("Invalid value {%s} received\n",
                                       argv[optind]);
                            return EXIT_FAILURE;
                        }
                    }
                    break;
                case INVALID_METHOD:
                    printError("ERROR: %s is an invalid growing tree method\n",
                               argv[optind]);
                    puts("Valid growing tree methods are:");
                    growingTreeMethods(1);
                    return EXIT_FAILURE;
                    break;
            }
            // move past the method
            optind++;
        }
    }

    if (argv[optind] == NULL) {
        print("Using default options (width = %d, height = %d)\n", width,
              height);
    } else if (optind + 2 > argc) {
        printError("Not enough arguments");
        return EXIT_FAILURE;
    } else {
        char *tmp;
        width = strtoull(argv[optind++], &tmp, 10);
        if (width == 0) {
            if (errno != 0) {
                printError("ERROR: %s\n", strerror(errno));
                return EXIT_FAILURE;
            } else if (argv[optind - 1] == tmp) {
                printError("Invalid value {%s} received\n", argv[optind - 1]);
                return EXIT_FAILURE;
            }
        }
        height = strtoull(argv[optind++], &tmp, 10);
        if (height == 0) {
            if (errno != 0) {
                printError("ERROR: %s\n", strerror(errno));
                return EXIT_FAILURE;
            } else if (argv[optind - 1] == tmp) {
                printError("Invalid value {%s} received\n", argv[optind - 1]);
                return EXIT_FAILURE;
            }
        }
    }

    maze = createMazeWH(width, height);

    if (!foundAlgo) {
        algorithm = kruskal;
    } 

	if (algorithm == growing_tree) {
        if (verbose_flag) {
            growingTreeGenWithSteps(&maze, method, split, stepFile);
        } else {
            growingTreeGen(&maze, method, split);
        }
    } else {
        if (verbose_flag) {
            generateMazeWithSteps(&maze, algorithm, stepFile);
        } else {
            generateMaze(&maze, algorithm);
        }
    }

    fprintf(outFile, "%s", maze.str);

    if (outFile != stdout) {
        fclose(outFile);
    }

    if (verbose_flag) {
        fclose(stepFile);
    }

    freeMaze(maze);

    return EXIT_SUCCESS;
}

/************************************************************************
 *                       FUNCTION DEFINITIONS                           *
 ************************************************************************/

void help() {
    puts("Usage:");
    puts("  MazeCreator                     Generate a 10x10 maze");
    puts("  MazeCreator [W] [H]             Generate a maze of size WxH");
    puts("  MazeCreator [options] [W] [H]   Generates a maze of size WxH");
    puts("");
    puts("Options:");
    puts("  -a, --algorithm <algorithm>     Specifies the algorithm");
    puts("  -h, --help                      Print this message");
    puts("  -o <file>, --output <file>      Output maze to <file>");
    puts("  -q, --quite                     Silence all output");
    puts("  -v [file], --verbose [file]     Send each step for maze to <file>");
    puts("");
    puts("Algorithms:");
    puts("  Kruskal");
    puts("  Prim");
    puts("  Back (Recursive Backtracking)");
    puts("  Aldous-Broder");
    puts("  Growing-Tree <method>");
	puts("  Hunt-and-Kill");
	puts("  Wilson");
	puts("  Eller");
	puts("  Divide (Recursive Division)");
    puts("");
    puts("Growing-Tree methods:");
    puts("  The growing tree has several methods for adding new cells.");
    puts("  They are as follows:");
    growingTreeMethods(2);
}

void growingTreeMethods(int depth) {
    char spacing[2 * depth + 1];

    for (size_t i = 0; i < 2 * depth; i++) {
        spacing[i] = ' ';
    }
    spacing[2 * depth] = '\0';

    printf("%sNewest\n", spacing);
    printf("%sMiddle\n", spacing);
    printf("%sOldest\n", spacing);
    printf("%sRandom\n", spacing);
    printf("%sNewest-Middle <ratio>\n", spacing);
    printf("%sNewest-Oldest <ratio>\n", spacing);
    printf("%sNewest-Random <ratio>\n", spacing);
    printf("%sMiddle-Oldest <ratio>\n", spacing);
    printf("%sMiddle-Random <ratio>\n", spacing);
    printf("%sOldest-Random <ratio>\n", spacing);
    printf("\n%s<ratio> can be any number between 0.0-1.0\n", spacing);
}

int printError(const char *frmt, ...) {
    va_list args;
    int results = 0;

    va_start(args, frmt);
    if (!quite_flag) {
        results = vfprintf(stderr, frmt, args);
    }
    va_end(args);

    return results;
}

int print(const char *frmt, ...) {
    va_list args;
    int results = 0;

    va_start(args, frmt);
    if (!quite_flag) {
        results = vprintf(frmt, args);
    }
    va_end(args);

    return results;
}

bool isinteger(const char *str) {
    size_t i = 0;

    for (i = 0; str[i] != '\0'; ++i) {
        if (str[i] != '-' && str[i] != '+' && !isdigit(str[i])) {
            return false;
        }
    }

    return i != 0;
}
