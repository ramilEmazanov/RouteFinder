function(check expected_code expected_text)
    execute_process(COMMAND "${PROGRAM}" ${ARGN}
        RESULT_VARIABLE code OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${code}" STREQUAL "${expected_code}")
        message(FATAL_ERROR "${ARGN}: expected exit ${expected_code}, got ${code}: ${output}${error}")
    endif()
    if(NOT "${output}${error}" MATCHES "${expected_text}")
        message(FATAL_ERROR "${ARGN}: missing '${expected_text}' in '${output}${error}'")
    endif()
endfunction()

foreach(date IN ITEMS 2026-11-23 2024-02-29 2000-02-29 0001-01-01 9999-12-31)
    check(0 "Дата корректна: ${date}" --validate-date "${date}")
endforeach()

foreach(date IN ITEMS 2026-02-29 1900-02-29 2026-04-31 2026-00-10
        2026-13-01 2026-01-00 2026-01-32 0000-01-01 2026-1-01
        2026/01/01 abcd-ef-gh 2026-01-01extra "")
    # Quoting preserves an empty date as an argument to this function.
    check(1 "YYYY-MM-DD" --validate-date "${date}")
endforeach()

check(0 "Использование" --help)
check(0 "Использование" -h)
check(1 "Использование")
check(1 "Использование" --unknown)
check(1 "Использование" --validate-date)
check(1 "Использование" --validate-date 2026-11-23 extra)
