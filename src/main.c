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
static int quite_flag;

/************************************************************************
 *                       FUNCTION DECLERATIONS                          *
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

    // clang-format off
	static struct option long_opts[] = {
		{"quite", no_argument, &quite_flag, 1},
		{"output", required_argument, NULL, 'o'},
		{0, 0, 0, 0}};
    // clang-format on

    while ((opt = getopt_long(argc, argv, "hqo:", long_opts, &opts_index)) !=
           -1) {
        switch (opt) {
            case 0:
                if (long_opts[opts_index].flag != 0) {
                    break;
                }
                printf("option %s", long_opts[opts_index].name);
                if (optarg) {
                    printf(" with arg %s", optarg);
                }
                putchar('\n');
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
					printError("ERROR opening \"%s\": %s", optarg, strerror(errno));
					return EXIT_FAILURE;
				}
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

    generateMaze(&maze);

    fprintf(outFile, "%s", maze.str);

	if (outFile != stdout) {
		fclose(outFile);
	}

    freeMaze(maze);

    return EXIT_SUCCESS;
}

/************************************************************************
 *                       FUNCTION DEFINITIONS                           *
 ************************************************************************/

void help() { printf("TODO: Write help\n"); }

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
