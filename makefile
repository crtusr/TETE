CC = gcc

# Base flags shared by both builds
BASE_CFLAGS = -Wall -Werror -Wpedantic -MMD -MP
LDFLAGS = 
LDLIBS = -lpdcurses

SRCS = main.c fondos.c menu.c dbftool.c ntxtool.c protete.c inputfields.c utilities.c

# ==========================================
# DEBUG TRACK
# ==========================================
DEBUG_CFLAGS = $(BASE_CFLAGS) -O0 -g -DDEBUG
DEBUG_OBJ_DIR = obj_debug
DEBUG_OBJS = $(SRCS:%.c=$(DEBUG_OBJ_DIR)/%.o)
DEBUG_TARGET = TETE_DEBUG

# ==========================================
# RELEASE TRACK
# ==========================================
RELEASE_CFLAGS = $(BASE_CFLAGS) -O3 -DNDEBUG
RELEASE_OBJ_DIR = obj_release
RELEASE_OBJS = $(SRCS:%.c=$(RELEASE_OBJ_DIR)/%.o)
RELEASE_TARGET = TETE

# Combine dependency files from both tracks
DEPS = $(DEBUG_OBJS:.o=.d) $(RELEASE_OBJS:.o=.d)

# ==========================================
# RULES
# ==========================================

# Default target now requires BOTH executables
all: $(DEBUG_TARGET) $(RELEASE_TARGET)

# 1. Link Debug Executable
$(DEBUG_TARGET): $(DEBUG_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# 2. Link Release Executable
$(RELEASE_TARGET): $(RELEASE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# 3. Compile Debug Objects
$(DEBUG_OBJ_DIR)/%.o: %.c | $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

# 4. Compile Release Objects
$(RELEASE_OBJ_DIR)/%.o: %.c | $(RELEASE_OBJ_DIR)
	$(CC) $(RELEASE_CFLAGS) -c $< -o $@

# 5. Create Directories
# This rule applies to both directories dynamically using $@
$(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf obj $(DEBUG_TARGET) $(RELEASE_TARGET)

-include $(DEPS)
.PHONY: all clean
