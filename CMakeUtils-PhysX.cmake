set(PHSYX41_BASE_ROOT_PATH
	# path for physx github repo
	".../github/PhysX"
)

set(PHSYX41_BASE_LIBRARY_PATH
	# path for physx msvc build folder
	".../github/PhysX/physx/bin/win.x86_64.vc142.mt"
)

set(PHSYX41_LIBS_WIN
	"PhysX_64.lib"
	"PhysXCommon_64.lib"
	"PhysXCooking_64.lib"
	"PhysXFoundation_64.lib"

	"PhysXPvdSDK_static_64.lib"
	"PhysXVehicle_static_64.lib"
	"PhysXExtensions_static_64.lib"
	"PhysXCharacterKinematic_static_64.lib"
)

set(MY_PHSYX41_PATH "PATH=%PATH%;${PHSYX41_BASE_LIBRARY_PATH}/$(Configuration)")

function(setup_physx_41 APP_NAME APP_SRC PHYSX_SRC_SNIPPETCOMMON PHYSX_SRC_SNIPPETRENDER PHYSX_SRC_SNIPPETUTILS)
	set_target_properties(${APP_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PHSYX41_PATH}")

	target_include_directories(${APP_NAME} PUBLIC ${PHSYX41_BASE_ROOT_PATH}/physx/include)
	target_include_directories(${APP_NAME} PUBLIC ${PHSYX41_BASE_ROOT_PATH}/pxshared/include)

	foreach(lib ${PHSYX41_LIBS_WIN})
		target_link_libraries(${APP_NAME} debug ${PHSYX41_BASE_LIBRARY_PATH}/debug/${lib})
		target_link_libraries(${APP_NAME} optimized ${PHSYX41_BASE_LIBRARY_PATH}/release/${lib})
	endforeach()

	source_group("" FILES ${APP_SRC})
	source_group("SnippetCommon" FILES ${PHYSX_SRC_SNIPPETCOMMON})
	source_group("SnippetRender" FILES ${PHYSX_SRC_SNIPPETRENDER})
	source_group("SnippetUtils" FILES ${PHYSX_SRC_SNIPPETUTILS})
endfunction()