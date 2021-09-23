BUILD_DIR ?= /tmp/hse-build/build

smart-format:
	@./scripts/format.sh --smart

format:
	@./scripts/format.sh
