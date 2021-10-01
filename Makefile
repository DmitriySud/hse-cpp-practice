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

$(foreach cur_work,${PROJECTS},build-$(cur_work)): build-%: cmake-%
	@cd ${BUILD_DIR} && make $*_lib 

$(foreach cur_work,${PROJECTS},test-$(cur_work)): test-%: cmake-%
	@cd ${BUILD_DIR} && make $*_test && ${BUILD_DIR}/works/$*/test/$*_test 

$(foreach cur_work,${PROJECTS},run-$(cur_work)): run-%: cmake-%
	@cd ${BUILD_DIR} && make $*_exe && ${BUILD_DIR}/works/$*/src/$*_exe


# TODO : Add checks to build/run/test commands do we really need to restart cmake or make 
# maybe check date of last modifying...

# TODO : Add templates of CMakeLists and project structure
