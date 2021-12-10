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

set(PHSYX34_BASE_ROOT_PATH
	# path for physx github repo
	".../github/PhysX-3.4"
)

set(PHSYX34_BASE_LIBRARY_PATH
	# path for physx msvc build folder
	".../github/PhysX-3.4/PhysX_3.4/Lib/vc15win64"
)

set(PHSYX34_BASE_PXSHARED_LIBRARY_PATH
	# path for physx msvc build folder
	".../github/PhysX-3.4/PxShared/lib/vc15win64"
)

set(PHSYX34_LIBS_WIN_DEBUG
	SimulationControllerDEBUG.lib
	SceneQueryDEBUG.lib
	PhysX3VehicleDEBUG.lib
	PhysX3ExtensionsDEBUG.lib
	PhysX3DEBUG_x64.lib
	PhysX3CookingDEBUG_x64.lib
	PhysX3CommonDEBUG_x64.lib
	PhysX3CharacterKinematicDEBUG_x64.lib
	LowLevelParticlesDEBUG.lib
	LowLevelDynamicsDEBUG.lib
	LowLevelDEBUG.lib
	LowLevelClothDEBUG.lib
	LowLevelAABBDEBUG.lib
)

set(PHSYX34_LIBS_WIN
	SimulationController.lib
	SceneQuery.lib
	PhysX3Vehicle.lib
	PhysX3Extensions.lib
	PhysX3_x64.lib
	PhysX3Cooking_x64.lib
	PhysX3Common_x64.lib
	PhysX3CharacterKinematic_x64.lib
	LowLevelParticles.lib
	LowLevelDynamics.lib
	LowLevel.lib
	LowLevelCloth.lib
	LowLevelAABB.lib
)

set(PHSYX34_PXLIBS_WIN_DEBUG
	PsFastXmlDEBUG_x64.lib
	PxFoundationDEBUG_x64.lib
	PxPvdSDKDEBUG_x64.lib
	PxTaskDEBUG_x64.lib
)

set(PHSYX34_PXLIBS_WIN
	PsFastXml_x64.lib
	PxFoundation_x64.lib
	PxPvdSDK_x64.lib
	PxTask_x64.lib
)

set(MY_PHSYX34_PATH "PATH=%PATH%;${PHSYX34_BASE_ROOT_PATH}/PhysX_3.4/Bin/vc15win64;${PHSYX34_BASE_ROOT_PATH}/PxShared/bin/vc15win64")

function(setup_physx_34 APP_NAME APP_SRC PHYSX_SRC_SNIPPETCOMMON PHYSX_SRC_SNIPPETRENDER PHYSX_SRC_SNIPPETUTILS)
	set_target_properties(${APP_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PHSYX34_PATH}")

	target_include_directories(${APP_NAME} PUBLIC ${PHSYX34_BASE_ROOT_PATH}/PhysX_3.4/Include)
	target_include_directories(${APP_NAME} PUBLIC ${PHSYX34_BASE_ROOT_PATH}/PxShared/include)
	target_include_directories(${APP_NAME} PUBLIC ${PHSYX34_BASE_ROOT_PATH}/PxShared/src/foundation/include)
	
	foreach(lib ${PHSYX34_LIBS_WIN_DEBUG})
		target_link_libraries(${APP_NAME} debug ${PHSYX34_BASE_LIBRARY_PATH}/${lib})
	endforeach()
	
	foreach(lib ${PHSYX34_LIBS_WIN})
		target_link_libraries(${APP_NAME} optimized ${PHSYX34_BASE_LIBRARY_PATH}/${lib})
	endforeach()
	
	foreach(lib ${PHSYX34_PXLIBS_WIN_DEBUG})
		target_link_libraries(${APP_NAME} debug ${PHSYX34_BASE_PXSHARED_LIBRARY_PATH}/${lib})
	endforeach()
	
	foreach(lib ${PHSYX34_PXLIBS_WIN})
		target_link_libraries(${APP_NAME} optimized ${PHSYX34_BASE_PXSHARED_LIBRARY_PATH}/${lib})
	endforeach()

	source_group("" FILES ${APP_SRC})
	source_group("SnippetCommon" FILES ${PHYSX_SRC_SNIPPETCOMMON})
	source_group("SnippetRender" FILES ${PHYSX_SRC_SNIPPETRENDER})
	source_group("SnippetUtils" FILES ${PHYSX_SRC_SNIPPETUTILS})
endfunction()
