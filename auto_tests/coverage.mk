# This file needs to be improved!


.PHONY: coverage_a clean_coverage

COVERAGE_INFO_FILE_NAME := auto_test_coverage

# Создание отчёта покрытия кода тестами
coverage_a: | $(TEST_OUT_DIR)
	gcc --coverage -g -Wall -Wextra -pedantic -o tmp_coverage.$(EXE_EXT) \
		./src/fill_in_testing_output.c ../src/reading_and_calculating.c ../geom_lib/src/reading_objects_from_file.c ../geom_lib/src/algos.c \
		-lm
	./tmp_coverage.$(EXE_EXT) $(TEST_CASES_FILES)
	lcov -t "$(COVERAGE_INFO_FILE_NAME)" -o  $(TEST_OUT_DIR)/$(COVERAGE_INFO_FILE_NAME).info -c -d .
	genhtml -o $(TEST_OUT_DIR)/coverage_report $(TEST_OUT_DIR)/$(COVERAGE_INFO_FILE_NAME).info
	rm -f tmp_coverage*
	@echo "\nCoverage report has been successfully created!"
	@echo "See results in $(TEST_OUT_DIR)/coverage_report/index.html\n"

# Очистка файлов и директорий отчёта покрытия кода тестами
clean_coverage:
	rm -r -f $(TEST_OUT_DIR)/coverage_report
	rm -f $(TEST_OUT_DIR)/$(COVERAGE_INFO_FILE_NAME).info
