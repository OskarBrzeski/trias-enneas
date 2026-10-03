#include <string.h>

#define NOB_IMPLEMENTATION
#define BUILD_FOLDER "./build/"
#define SRC_FOLDER   "./src/"
#define LIBS_FOLDER  "./libs/"

#include "nob.h"

int
main(int argc, char** argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);
	if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

	Cmd cmd = {0};
	cmd_append(&cmd, "cc");
	cmd_append(&cmd, "-Wall", "-Wextra", "-Werror", "-Wpedantic");

	cmd_append(&cmd, "-o", BUILD_FOLDER "main", SRC_FOLDER "main.c");

	cmd_append(&cmd, "-I" LIBS_FOLDER);
	cmd_append(&cmd, "-L" LIBS_FOLDER);
	cmd_append(&cmd, "-lraylib", "-lm", "-lX11");

	if (!cmd_run(&cmd)) return 2;

	if (argc > 1) {
		if (!strcmp(argv[1], "run")) {
			cmd_append(&cmd, BUILD_FOLDER "main");
			if (!cmd_run(&cmd)) return 3;
		} else {
			return 4;
		}
	}

	return 0;
}
