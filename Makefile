BUILD_DIR ?= /tmp/hse-build/build
CUR_PROJECT ?= !!!!

PROJECTS = $(patsubst works/%/,%,$(shell ls -d works/*/))

smart-format:
	@./scripts/format.sh --smart

format:
	@./scripts/format.sh

clean:
	@rm -rf /tmp/hse-build/build/*  
	@echo "Done!"

cmake:
	cmake -DTARGET_PROJECTS="${CUR_PROJECT}" -S . -B ${BUILD_DIR}

$(foreach cur_work,${PROJECTS},cmake-$(cur_work)): cmake-%:
	@$(MAKE) cmake CUR_PROJECT=works/$*

$(foreach cur_work,${PROJECTS},test-$(cur_work)): test-%: cmake-%
	@cd ${BUILD_DIR} && make $*_test && ${BUILD_DIR}/works/$*/test/$*_test 
