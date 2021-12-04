cmake_minimum_required(VERSION 3.15)
project(physx_sandbox)

set( CMAKE_VERBOSE_MAKEFILE ON )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

string(TOUPPER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_ID_UPPER)
if(${CXX_COMPILER_ID_UPPER} STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")

	if(MSVC)
	add_compile_options(
		$<$<CONFIG:>:/MT> #---------|
		$<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
		$<$<CONFIG:Release>:/MT> #--|
	)
	endif()
endif()

IF(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DNOMINMAX)
	add_definitions(-D_USE_MATH_DEFINES)

	file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_LIST_DIR}/libs/freeglut-3.2.1.tar.gz DESTINATION ${CMAKE_CURRENT_LIST_DIR}/libs)
	set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build FreeGLUT shared library.")
	set(FREEGLUT_BUILD_DEMOS OFF CACHE INTERNAL "Build FreeGLUT demos.")
	add_subdirectory(libs/freeglut-3.2.1 ${CMAKE_CURRENT_BINARY_DIR}/libs/freeglut-3.2.1 EXCLUDE_FROM_ALL)
	set_target_properties(freeglut_static PROPERTIES FOLDER "External Dependencies")
ENDIF(WIN32)

####################################

include( ./CMakeUtils-PhysX.cmake )

SET(SRC_APP_PHYSX_SNIPPETCOMMON
	./snippet_common/ClassicMain.cpp
	./snippet_common/SnippetPrint.h
	./snippet_common/SnippetPVD.h
)

SET(SRC_APP_PHYSX_SNIPPETRENDER
	./snippet_render/SnippetCamera.h
	./snippet_render/SnippetCamera.cpp
	./snippet_render/SnippetRender.h
	./snippet_render/SnippetRender.cpp
)

SET(SRC_APP_PHYSX_SNIPPETUTILS
	./snippet_utils/SnippetUtils.h
	./snippet_utils/SnippetUtils.cpp
)

####################################

SET(SRC_APP_PHYSX_SNIPPET_HELLOWORLD
	./snippet_helloworld/SnippetHelloWorld.cpp
	./snippet_helloworld/SnippetHelloWorldRender.cpp
)

add_executable( physx_snippet_helloworld ${SRC_APP_PHYSX_SNIPPET_HELLOWORLD} ${SRC_APP_PHYSX_SNIPPETCOMMON} ${SRC_APP_PHYSX_SNIPPETRENDER} ${SRC_APP_PHYSX_SNIPPETUTILS} )
target_link_libraries( physx_snippet_helloworld freeglut_static )
target_compile_definitions( physx_snippet_helloworld PUBLIC FREEGLUT_STATIC RENDER_SNIPPET)
target_include_directories( physx_snippet_helloworld PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/freeglut-3.2.1/include )

setup_physx_41(physx_snippet_helloworld "${SRC_APP_PHYSX_SNIPPET_HELLOWORLD}" "${SRC_APP_PHYSX_SNIPPETCOMMON}" "${SRC_APP_PHYSX_SNIPPETRENDER}" "${SRC_APP_PHYSX_SNIPPETUTILS}")

####################################

SET(SRC_APP_PHYSX_SNIPPET_ARTICULATION_41
	./snippet_articulation_41/SnippetArticulation.cpp
	./snippet_articulation_41/SnippetArticulationRender.cpp
)

add_executable( physx_snippet_articulation_41 ${SRC_APP_PHYSX_SNIPPET_ARTICULATION_41} ${SRC_APP_PHYSX_SNIPPETCOMMON} ${SRC_APP_PHYSX_SNIPPETRENDER} ${SRC_APP_PHYSX_SNIPPETUTILS} )
target_link_libraries( physx_snippet_articulation_41 freeglut_static )
target_compile_definitions( physx_snippet_articulation_41 PUBLIC FREEGLUT_STATIC RENDER_SNIPPET)
target_include_directories( physx_snippet_articulation_41 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/freeglut-3.2.1/include )

setup_physx_41(physx_snippet_articulation_41 "${SRC_APP_PHYSX_SNIPPET_ARTICULATION_41}" "${SRC_APP_PHYSX_SNIPPETCOMMON}" "${SRC_APP_PHYSX_SNIPPETRENDER}" "${SRC_APP_PHYSX_SNIPPETUTILS}")

####################################

SET(SRC_APP_PHYSX_SNIPPET_ARTICULATION_34
	./snippet_articulation_34/SnippetArticulation.cpp
	./snippet_articulation_34/SnippetArticulationRender.cpp
)

add_executable( physx_snippet_articulation_34 ${SRC_APP_PHYSX_SNIPPET_ARTICULATION_34} ${SRC_APP_PHYSX_SNIPPETCOMMON} ${SRC_APP_PHYSX_SNIPPETRENDER} ${SRC_APP_PHYSX_SNIPPETUTILS} )
target_link_libraries( physx_snippet_articulation_34 freeglut_static )
target_compile_definitions( physx_snippet_articulation_34 PUBLIC FREEGLUT_STATIC RENDER_SNIPPET)
target_include_directories( physx_snippet_articulation_34 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/freeglut-3.2.1/include )

setup_physx_41(physx_snippet_articulation_34 "${SRC_APP_PHYSX_SNIPPET_ARTICULATION_34}" "${SRC_APP_PHYSX_SNIPPETCOMMON}" "${SRC_APP_PHYSX_SNIPPETRENDER}" "${SRC_APP_PHYSX_SNIPPETUTILS}")

####################################
