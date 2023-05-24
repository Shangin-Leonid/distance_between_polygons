# This file needs to be improved!

#--------------------------------------------------------------------------------------------------
# Names of directories, files and etc..
#--------------------------------------------------------------------------------------------------

REDIRECT ?= NO

ALL_C_FILES := ./$(C_DIR)/*.c \
			../$(C_DIR)/*.c \
			$(SLIB_DIR)/$(C_DIR)/*.c

# Переменные для перенаправления результатов анализа кода в файл
ifeq (YES,$(REDIRECT))
%_a : REDIRECT_FILE = $(TEST_OUT_DIR)/$@_logs.txt
%_a : DO_REDIRECT = >
%_a : REDIRECT_MESSAGE = @echo "\nSee '$@' results in $(REDIRECT_FILE)\n"
valgrind_a : DO_REDIRECT = --log-file=
cpplint_a : EXTRA_DO_REDIRECT = 2>&1
else
endif



#--------------------------------------------------------------------------------------------------
# Flags for analyzators
#--------------------------------------------------------------------------------------------------

# Переменные с параметрами для анализаторов
valgrind_a : ANALYSIS_PARAMS = $(DO_REDIRECT)$(REDIRECT_FILE) \
		 --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose
clang_tidy_a : ANALYSIS_PARAMS = -checks=*
cppcheck_a : ANALYSIS_PARAMS = --enable=all
cpplint_a : ANALYSIS_PARAMS = --linelength=120



#--------------------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------------------

.PHONY: clang_tidy_a cppcheck_a cpplint_a valgrind_a

# Статический анализ с помощью 'cpplint'
cpplint_a: | $(TEST_OUT_DIR)
	$(REDIRECT_MESSAGE)
	cpplint $(ANALYSIS_PARAMS) \
			$(ALL_C_FILES) \
			$(DO_REDIRECT) $(REDIRECT_FILE) $(EXTRA_DO_REDIRECT)

# Статический анализ с помощью 'clang-tidy'
clang_tidy_a: | $(TEST_OUT_DIR)
	clang-tidy $(ANALYSIS_PARAMS) \
			$(ALL_C_FILES) \
		$(DO_REDIRECT) $(REDIRECT_FILE)
	$(REDIRECT_MESSAGE)

# Статический анализ с помощью 'cppcheck'
cppcheck_a: | $(TEST_OUT_DIR)
	cppcheck $(ANALYSIS_PARAMS) \
			$(ALL_C_FILES) \
		$(DO_REDIRECT) $(REDIRECT_FILE)
	$(REDIRECT_MESSAGE)

# Runtime анализ с помощью 'valgrind'
valgrind_a: build | $(TEST_OUT_DIR)
	valgrind $(ANALYSIS_PARAMS) \
			 \
	./$(FILL_IN_NAME).$(EXE_EXT) $(TEST_CASES_FILES)
			 \
	valgrind $(ANALYSIS_PARAMS) \
			 \
	./$(CHECK_NAME).$(EXE_EXT) $(SAMPLE_FILES)
			 \
	cat $(RESULT_FILE)
	$(REDIRECT_MESSAGE)
