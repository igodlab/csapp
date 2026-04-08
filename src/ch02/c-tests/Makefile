# src/ch02/tests/Makefile
#
# Usage:
#   make                  build & run all test suites
#   make test_prob2-89    build & run one specific suite
#   make clean            remove build artefacts

CC     := gcc
CFLAGS := -Wall -Wextra -g

UNITY  := ../../../vendor/unity
SOLS   := ../c-solutions
NOTES  := ../c-notes
BUILD  := build

# ── Discover all tests ────────────────────────────────────────────────────────
TEST_SRCS := $(wildcard test_*.c)
TEST_BINS := $(patsubst %.c,$(BUILD)/%,$(TEST_SRCS))

# ── Default: build & run all ──────────────────────────────────────────────────
.PHONY: all
all: $(TEST_BINS)
	@for bin in $(TEST_BINS); do \
		echo "\033[1mRunning $$bin\033[0m"; \
		$$bin; \
		echo ""; \
	done

# ── Generic pattern rule ──────────────────────────────────────────────────────
# Compiles test_foo.c + unity.c + (c-solutions/foo.c or notes/foo.c).
# Override CFLAGS or source below for special cases.
$(BUILD)/test_%: test_%.c | $(BUILD)
	$(CC) $(CFLAGS) $(CFLAGS_$@) \
		-I$(UNITY) -I$(SOLS) -I$(NOTES) \
		$< \
		$(UNITY)/unity.c \
		$(firstword $(wildcard $(SOLS)/$*.c) $(wildcard $(NOTES)/$*.c)) \
		-lm -o $@

# ── Per-test overrides ────────────────────────────────────────────────────────
# Add flags for tests that need non-vanilla compilation.
CFLAGS_$(BUILD)/test_prob2-89 := -mfpmath=sse -msse2 -fno-associative-math -mno-fma

$(BUILD):
	mkdir -p $@

# ── Run a single suite ────────────────────────────────────────────────────────
.PHONY: test_%
test_%: $(BUILD)/test_%
	$<

.PHONY: clean
clean:
	rm -rf $(BUILD)
