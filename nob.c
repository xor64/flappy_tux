#define _GNU_SOURCE
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

#define SARRLEN(arr) sizeof(arr) / sizeof(arr[0])
#define OK true
#define ERR false

#define CC "gcc"
#define LD "gcc"
#define AR "ar"
#define OUT_BIN "bin/flappy_tux"

static char* CC_ARGS[] = {"-std=c2x", "-Isrc/include", "-Ilib/raylib/src/"};
static char* LD_ARGS[] = {"-lm"};

typedef struct {
    char* src;
    char* dst;
    char* cc;
    char** args; // NULL for default args
} target_t;

typedef struct {
    char** items;
    size_t count;
    size_t capacity;
} objects_t;

bool build(bool optimise);
bool build_object(target_t target, bool optimise);
bool link_objects(objects_t* objs, bool optimise);
bool setup_dirs();
bool clean_soft();
bool clean_full();
bool build_libs();
bool build_raylib();
bool clean_raylib();

static target_t targets[] = {
    {
        .src = "src/main.c",
        .dst = "bin/obj/main.o",
        .cc = CC,
        .args = NULL,
    },
    {
        .src = "src/renderer.c",
        .dst = "bin/obj/renderer.o",
        .cc = CC,
        .args = NULL,
    },
    {
        .src = "src/texture_mgr.c",
        .dst = "bin/obj/texture_mgr.o",
        .cc = CC,
        .args = NULL,
    },
};

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    const char* command = NULL;
    const char* subcommand = "";
    bool optimise = false;

    const char* program = nob_shift_args(&argc, &argv);

    if (argc <= 0) {
        command = "build";
    } else {
        command = nob_shift_args(&argc, &argv);
    }

    if (argc >= 1) {
        subcommand = nob_shift_args(&argc, &argv);
    }

    if (strcmp(subcommand, "release") == 0) {
        optimise = true;
    }

    if (strcmp(command, "build") == 0) {
        if (!build(optimise)) {
            return 1;
        }
        if (!clean_soft()) {
            return 1;
        }
    } else if (strcmp(command, "keep") == 0) {
        if (!build(optimise)) {
            return 1;
        }
    } else if (strcmp(command, "clean") == 0) {
        if (!clean_full()) {
            return 1;
        }
    } else {
        nob_log(NOB_INFO, "Unknown command %s", command);
    }
}

bool build(bool optimise) {
    if (!setup_dirs()) {
        return ERR;
    }
    objects_t objs = {0};


    for (size_t i = 0; i < SARRLEN(targets); i++) {
        nob_da_append(&objs, targets[i].dst);
        if (!build_object(targets[i], optimise)) {
            return ERR;
        }
    }

    build_libs(&objs);

    if (!link_objects(&objs, optimise)) {
        return ERR;
    }
    return OK;
}

bool build_object(target_t target, bool optimise) {
    nob_log(NOB_INFO, "Compiling '%s' -> '%s'", target.src, target.dst);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, target.cc, "-c", "-o", target.dst, target.src);

    if (optimise) {
        nob_cmd_append(&cmd, "-O3");
    } else {
        nob_cmd_append(&cmd, "-Og");
    }

    if (target.args) {
        nob_da_append_many(&cmd, target.args, SARRLEN(target.args));
    } else {
        nob_da_append_many(&cmd, CC_ARGS, SARRLEN(CC_ARGS));
    }

    if (!nob_cmd_run_sync(cmd)) {
        nob_log(NOB_ERROR, "Failed build obj");
        nob_cmd_free(cmd);
        return ERR;
    }
    nob_cmd_free(cmd);
    return OK;
}

bool link_objects(objects_t* objs, bool optimise) {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, LD, "-o", OUT_BIN);
    nob_da_append_many(&cmd, objs->items, objs->count);
    nob_da_append_many(&cmd, LD_ARGS, SARRLEN(LD_ARGS));

    if (optimise) {
        nob_cmd_append(&cmd, "-O3");
    } else {
        nob_cmd_append(&cmd, "-Og");
    }

    if (!nob_cmd_run_sync(cmd)) {
        nob_cmd_free(cmd);
        nob_log(NOB_ERROR, "Failed to link objects");
        return ERR;
    }
    nob_cmd_free(cmd);
    return OK;
}

bool setup_dirs() {
    nob_log(NOB_INFO, "Setting up dirs");
    nob_mkdir_if_not_exists("bin");
    nob_mkdir_if_not_exists("bin/obj");
    nob_mkdir_if_not_exists("bin/lib");
    return OK;
};

bool clean_soft() {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "rm", "-r", "bin/obj");
    if (!nob_cmd_run_sync(cmd)) return ERR;
    nob_cmd_free(cmd);
    return OK;
}

bool clean_full() {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "rm", "-r", "bin/");
    if (!nob_cmd_run_sync(cmd)) return ERR;
    nob_cmd_free(cmd);
    clean_raylib();
    return OK;
}

bool build_libs(objects_t* objs) {
    nob_da_append(objs, "bin/lib/libraylib.a");
    if (!build_raylib()) {
        nob_log(NOB_ERROR, "Failed to build raylib");
        return ERR;
    }

    return OK;
}

bool build_raylib() {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "make", "-E", "RAYLIB_RELEASE_PATH=../../../bin/lib/", "-C", "lib/raylib/src");
    nob_cmd_run_sync(cmd);
    nob_cmd_free(cmd);
    return OK;
}

bool clean_raylib() {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "make", "-C", "lib/raylib/src", "clean");
    nob_cmd_run_sync(cmd);
    nob_cmd_free(cmd);
    return OK;
}