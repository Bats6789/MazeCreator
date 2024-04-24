/* Name: main.c
 * Auth: Blake Wingard - bats23456789@gmail.com
 * Date: 04/19/2024
 * Desc: The main file for Maze Generation.
 */

/************************************************************************
 *                              INCLUDES                                *
 ************************************************************************/
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MazeTools.h"

/************************************************************************
 *                               DEFINES                                *
 ************************************************************************/

#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 10

/************************************************************************
 *                            STATIC VARS                               *
 ************************************************************************/
static int quite_flag = 0;    // option to silence output
static int verbose_flag = 0;  // option to print out everything

/************************************************************************
 *                       FUNCTION DECLARATIONS                          *
 ************************************************************************/
void help();

int print(const char *frmt, ...);
int printError(const char *frmt, ...);

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
    FILE *stepFile;

    // clang-format off
	static struct option long_opts[] = {
		{"quite", no_argument, &quite_flag, 1},
		{"output", required_argument, NULL, 'o'},
		{"verbose", optional_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}};
    // clang-format on

    while ((opt = getopt_long(argc, argv, "hqo:v::", long_opts, &opts_index)) !=
           -1) {
        switch (opt) {
            case 0:
                break;

            case 'h':
                help();
                return EXIT_SUCCESS;
                break;

            case 'q':
                quite_flag = 1;
                break;

            case 'o':
                outFile = fopen(optarg, "w");
                if (!outFile) {
                    printError("ERROR opening \"%s\": %s", optarg,
                               strerror(errno));
                    return EXIT_FAILURE;
                }
                break;
            case 'v':
				if (optarg == NULL && optind < argc && argv[optind][0] != '-'){
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

    if (argv[optind] == NULL) {
        print("Using default options (height = %d, width = %d)\n", height,
              width);
    } else if (optind + 2 < argc) {
        printError("Not enough arguments");
        return EXIT_FAILURE;
    } else {
        height = strtoull(argv[optind++], NULL, 10);
        width = strtoull(argv[optind++], NULL, 10);
    }

    maze = createMazeWH(width, height);

	if (verbose_flag) {
		generateMazeWithSteps(&maze, stepFile);
	} else {
		generateMaze(&maze);
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
	puts("  -q, --quite                     Silence all output");
	puts("  -o <file>, --output <file>      Output maze to <file>");
	puts("  -v [file], --verbose [file]     Send each step for maze to <file>");
	puts("  -h, --help                      Print this message");
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
