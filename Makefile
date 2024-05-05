
.DEFAULT_GOAL := run

.PHONY: build
build:
	mkdir -p ./build
	gcc main.c -o ./build/snaky

.PHONY: run
run: build
	@echo "---"
	@./build/snaky

.PHONY: clean
clean:
	rm -rf ./build

