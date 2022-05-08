project(tcp_server_docs)
cmake_minimum_required(VERSION 3.9)

set(SPHINX_EXECUTABLE sphinx-build)
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/../docs)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

file(GLOB DOCS_FILES
    "${SPHINX_SOURCE}/*.rst"
    "${SPHINX_SOURCE}/*.md"
    "${SPHINX_SOURCE}/contributing/*.md")

file(COPY ${SPHINX_SOURCE}/.nojekyll DESTINATION ${SPHINX_BUILD})

add_custom_target(docs ALL DEPENDS ${SPHINX_INDEX_FILE})
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
    COMMAND
        ${SPHINX_EXECUTABLE} -E -b html ${SPHINX_SOURCE} ${SPHINX_BUILD} ${DOCS_FILES}
    MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
    COMMENT "Building DOCS for TCP Server")
