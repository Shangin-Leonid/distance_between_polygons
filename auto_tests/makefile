
include ../common.mk

#--------------------------------------------------------------------------------------------------
# Names of directories, files and etc..
#--------------------------------------------------------------------------------------------------

SLIB_DIR := ../geom_lib

SAMPLE_DIR := ./correct_output
TEST_OUT_DIR := ./test_report
TEST_CASES_DIR := ./test_cases

FILL_IN_NAME := fill_in_testing_output
CHECK_NAME := check_output_files

TEST_OUT_FILE ?= $(TEST_OUT_DIR)/test_output.txt
RESULT_FILE ?= $(TEST_OUT_DIR)/test_results.txt

SAMPLE_FILES := $(wildcard $(SAMPLE_DIR)/case_*.txt)
TEST_CASES_FILES := $(wildcard $(TEST_CASES_DIR)/case_*.txt)



#--------------------------------------------------------------------------------------------------
# Flags for C compilation
#--------------------------------------------------------------------------------------------------

%.$(EXE_EXT) : CFLAGS = -g -Wall -Wextra -pedantic -o $@

$(D_DIR)/$(CHECK_NAME).d : CFLAGS = -MM -MT './$(CHECK_NAME).$(EXE_EXT) ./$@'
$(D_DIR)/$(FILL_IN_NAME).d : CFLAGS = -MM -MT '$(OBJ_DIR)/$(FILL_IN_NAME).o ./$@'



#--------------------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------------------

.PHONY: build fill_in static_lib do_test


# =================================   Сборка   ========================================

# Главная цель: создание исполняемых файлов, удаление файла с результатами тестов
build: fill_in $(CHECK_NAME).$(EXE_EXT)
	@echo "\nAuto tests have been SUCCESSFULY created!\n"

fill_in: static_lib $(FILL_IN_NAME).$(EXE_EXT)

# Сборка executable (исполняемого) файла для FILL_IN
$(FILL_IN_NAME).$(EXE_EXT): $(SLIB_FILE) $(OBJ_DIR)/$(FILL_IN_NAME).o ../$(OBJ_DIR)/reading_and_calculating.o
	$(COMPILER) $(CFLAGS) $(OBJ_DIR)/$(FILL_IN_NAME).o ../$(OBJ_DIR)/reading_and_calculating.o \
	$(INCLUDING_LIBS_FLAG)

# Сборка executable (исполняемого) файла для CHECK
$(CHECK_NAME).$(EXE_EXT): $(D_DIR)/$(CHECK_NAME).d
	$(COMPILER) $(CFLAGS) $(C_DIR)/$(CHECK_NAME).c


# =================================   Вспомагательные файлы   ========================================

# Обращение к другому makefile для создания статической библиотеки
static_lib:
	$(MAKE) -C $(SLIB_DIR) $@

#.PHONY: ../$(OBJ_DIR)/Reading_and_calculating.o
../$(OBJ_DIR)/reading_and_calculating.o:
	$(MAKE) -C ../ $(OBJ_DIR)/$(notdir $@)


# =================================   Директории   ========================================

# Создание директории для выходных файлов
$(TEST_OUT_DIR):
	mkdir -p $@


# =================================   Запуск   ========================================

# Протестировать программу (требуется предварительная сборка)
do_test: build | $(TEST_OUT_DIR)
	./$(FILL_IN_NAME).$(EXE_EXT) $(TEST_CASES_FILES)
	./$(CHECK_NAME).$(EXE_EXT) $(SAMPLE_FILES)
	cat $(RESULT_FILE)


# =================================   Clean   ========================================

.PHONY: clean_all clean_dirs clean_test_output clean \
clean_exe clean_result_file clean_slib clean_all_slib \
clean_all_main \
total_clean

# Полностью очистить текущую тестовую директорию
clean_all: clean_dirs clean_exe

# Удалить все вспомагательные директории с их содержимым
clean_dirs:
	rm -r -f $(OBJ_DIR) $(D_DIR)

# Удалить директорию с результатами тестов
clean_test_report:
	rm -r -f $(TEST_OUT_DIR)

# Удалить вспомагательные директории с их содержимым и отчётный файл (с информацией о тесте)
clean: clean_dirs

# Очистить от исполняемых файлов
clean_exe:
	rm -f -v $(FILL_IN_NAME).$(EXE_EXT) $(CHECK_NAME).$(EXE_EXT)

# Удалить отчётный файл (с информацией о тесте)
clean_result_file:
	rm -f -v $(RESULT_FILE)

# Очистить директорию со статической библиотекой
clean_slib:
	(cd $(SLIB_DIR); make clean)

# Полностью очистить директорию со статической библиотекой
clean_all_slib:
	(cd $(SLIB_DIR); make clean_all)

# полностью очистить основную директорию
clean_all_main:
	(cd ..; make clean_all)

# Тотально очистить все директориии
total_clean: clean_all_slib clean_all_main clean_all



#--------------------------------------------------------------------------------------------------
# INCLUDE
#--------------------------------------------------------------------------------------------------

# Подключаем файл с правилами статического и runtime анализа
include analysis.mk
# Подключаем файл с правилами создания отчёта покрытия кода тестами
include coverage.mk

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring dir,$(MAKECMDGOALS)))
ifeq (,$(findstring help,$(MAKECMDGOALS)))
ifeq (,$(findstring _a,$(MAKECMDGOALS)))
# Подключаем файлы зависимостей ('.d') соответствующих данному '.c' файлу.
$(D_FILES):
include $(wildcard $(D_FILES))
endif
endif
endif
endif



#--------------------------------------------------------------------------------------------------
# COMMENTS
#--------------------------------------------------------------------------------------------------
#
# Some variables you can find in common.mk
#
# All makefiles have been written for Linux. Sorry, if you have another OS :-(
# I wish you could understand my make-code and then rewrite some parts.
#
