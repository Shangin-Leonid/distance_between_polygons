
include common.mk

#--------------------------------------------------------------------------------------------------
# Names of directories, files and etc..
#--------------------------------------------------------------------------------------------------

MAIN := polygon_distance
R_and_C := reading_and_calculating

INPUT_FILE ?= ./input.txt
OUTPUT_FILE ?= ./output.txt

AUTO_TESTS_DIR = ./AUTO_TESTS

SLIB_DIR := ./geom_lib

#$(D_DIR)/%.d : FILES_ADDED_TO_D_FILE += $(OBJ_DIR)/$*.o
#FILES_ADDED_TO_D_FILE := $(OBJ_DIR)/$*.o ./$@
$(D_DIR)/%.d : CFLAGS = -MM -MT '$(OBJ_DIR)/$*.o ./$@'


#--------------------------------------------------------------------------------------------------
# Flags for C compilation
#--------------------------------------------------------------------------------------------------

$(MAIN).$(EXE_EXT) : CFLAGS = -g -Wall -Wextra -pedantic -o $@

INCLUDING_LIBS_FLAG := -L$(SLIB_DIR)/ -l$(SLIB_NAME) -lm



#--------------------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------------------

.PHONY: build static_lib run


# =================================   Сборка   ========================================

# Собрать весь проект, вплоть до главного исполняемого файла (кроме тестов)
build: static_lib $(MAIN).$(EXE_EXT)
	@echo "\nMain part of the project has been successfully builded!\n"

# Создание и обновление главного исполняемого файла
$(MAIN).$(EXE_EXT): $(SLIB_FILE) $(OBJ_FILES)
	$(COMPILER) $(CFLAGS) $(OBJ_FILES) \
	$(INCLUDING_LIBS_FLAG)


# =================================   Вспомагательные файлы   ========================================

# Обращение к другому makefile для создания статической библиотеки
static_lib:
	$(MAKE) -C $(SLIB_DIR) $@


# =================================   Запуск   ========================================

# Запуск программы (исполняемого файла)
run:
	@echo "\nRun the programm with '$(INPUT_FILE)' as input. See output in '$(OUTPUT_FILE)'.\n"
	./$(MAIN).$(EXE_EXT) $(INPUT_FILE) $(OUTPUT_FILE)
	@echo "\n"


# =================================   Clean   ==============================================

.PHONY : clean_output_file clean_exe clean_dirs clean \
clean_all clean_slib clean_all_slib total_clean

# Удалить все вспомагательные директории с их содержимым
clean_dirs:
	rm -r -f $(OBJ_DIR) $(D_DIR)

# Удалить файл-вывод программы
clean_output_file:
	rm -f -v $(OUTPUT_FILE)

# Удалить исполняемый файл
clean_exe:
	rm -f -v ./$(MAIN).$(EXE_EXT)

# Очистить основную (текущую) директорию
clean: clean_dirs clean_output_file

# Полностью очистить основную (текущую) директорию
clean_all: clean clean_exe

# Очистить директорию со статической библиотекой
clean_slib:
	$(MAKE) -C $(SLIB_DIR) clean

# Полностью очистить директорию со статической библиотекой
clean_all_slib:
	$(MAKE) -C $(SLIB_DIR) clean_all

# Полностью очистить и основную (текущую) и библиотечную директории
total_clean: clean_all_slib clean_all



#--------------------------------------------------------------------------------------------------
# INCLUDE
#--------------------------------------------------------------------------------------------------

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring dir,$(MAKECMDGOALS)))
ifeq (,$(findstring help,$(MAKECMDGOALS)))
# Подключаем файлы зависимостей ('.d') соответствующих данному '.c' файлу.
$(D_FILES):
include $(wildcard $(D_FILES))
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
