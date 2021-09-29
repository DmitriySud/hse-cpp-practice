BUILD_DIR ?= /tmp/hse-build/build
CUR_PROJECT ?= !!!!

smart-format:
	@./scripts/format.sh --smart

format:
	@./scripts/format.sh

clean:
	@rm -rf /tmp/hse-build/build/*  
	@echo "Done!"

cmake:
	cmake -DCMAKE_PROJECTS="${CUR_PROJECT}" -S . -B ${BUILD_DIR}
