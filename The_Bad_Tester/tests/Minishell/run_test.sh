DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

make --no-print-directory -C ${DIR}/.. TEST_FOLDER=${DIR} PRELOADS=malloc EXEC_NAME=run_tests &&
#valgrind --trace-children=no --child-silent-after-fork=yes ${DIR}/run_tests ${DIR}/Tests ~/Desktop/minishell/minishell
${DIR}/run_tests ${DIR}/Tests ${DIR}/../../../minishell