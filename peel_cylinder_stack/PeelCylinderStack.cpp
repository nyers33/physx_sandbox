//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

// ****************************************************************************
// This snippet illustrates simple use of physx
//
// It creates a number of box stacks on a plane, and if rendering, allows the
// user to create new stacks and fire a ball from the camera position
// ****************************************************************************

#include <ctype.h>

#include "PxPhysicsAPI.h"

#include "../snippet_common/SnippetPrint.h"
#include "../snippet_common/SnippetPVD.h"
#include "../snippet_utils/SnippetUtils.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation	= NULL;
PxPhysics*				gPhysics	= NULL;
PxCooking*				gCooking	= NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;

PxMaterial*				gDefMaterial	= NULL;
PxMaterial*				gMaterial		= NULL;

PxPvd*                  gPvd		= NULL;

static PxVec3 cylinderConvexVerts[] =
{	PxVec3(1.0f, 1.0f, 0.0f), PxVec3(cosf(0.125f * M_PI), 1.0f, sinf(0.125f * M_PI)), PxVec3(M_SQRT1_2, 1.0f, M_SQRT1_2), PxVec3(sinf(0.125f * M_PI), 1.0f, cosf(0.125f * M_PI)),
	PxVec3(0.0f, 1.0f, 1.0f), PxVec3(-sinf(0.125f * M_PI), 1.0f, cosf(0.125f * M_PI)), PxVec3(-M_SQRT1_2, 1.0f, M_SQRT1_2), PxVec3(-cosf(0.125f * M_PI), 1.0f, sinf(0.125f * M_PI)),
	PxVec3(-1.0f, 1.0f, 0.0f), PxVec3(-cosf(0.125f * M_PI), 1.0f, -sinf(0.125f * M_PI)), PxVec3(-M_SQRT1_2, 1.0f, -M_SQRT1_2), PxVec3(-sinf(0.125f * M_PI), 1.0f, -cosf(0.125f * M_PI)),
	PxVec3(0.0f, 1.0f, -1.0f), PxVec3(sinf(0.125f * M_PI), 1.0f, -cosf(0.125f * M_PI)), PxVec3(M_SQRT1_2, 1.0f, -M_SQRT1_2), PxVec3(cosf(0.125f * M_PI), 1.0f, -sinf(0.125f * M_PI)),
	PxVec3(1.0f, -1.0f, 0.0f), PxVec3(cosf(0.125f * M_PI), -1.0f, sinf(0.125f * M_PI)), PxVec3(M_SQRT1_2, -1.0f, M_SQRT1_2), PxVec3(sinf(0.125f * M_PI), -1.0f, cosf(0.125f * M_PI)),
	PxVec3(0.0f, -1.0f, 1.0f), PxVec3(-sinf(0.125f * M_PI), -1.0f, cosf(0.125f * M_PI)), PxVec3(-M_SQRT1_2, -1.0f, M_SQRT1_2), PxVec3(-cosf(0.125f * M_PI), -1.0f, sinf(0.125f * M_PI)),
	PxVec3(-1.0f, -1.0f, 0.0f), PxVec3(-cosf(0.125f * M_PI), -1.0f, -sinf(0.125f * M_PI)), PxVec3(-M_SQRT1_2, -1.0f, -M_SQRT1_2), PxVec3(-sinf(0.125f * M_PI), -1.0f, -cosf(0.125f * M_PI)),
	PxVec3(0.0f, -1.0f, -1.0f), PxVec3(sinf(0.125f * M_PI), -1.0f, -cosf(0.125f * M_PI)), PxVec3(M_SQRT1_2, -1.0f, -M_SQRT1_2), PxVec3(cosf(0.125f * M_PI), -1.0f, -sinf(0.125f * M_PI))
};

void createStack(const PxTransform& t, PxReal radius, PxReal halfHeight)
{
	for (PxU32 i = 0; i < 32; i++)
	{
		cylinderConvexVerts[i].x *= radius;
		cylinderConvexVerts[i].y *= halfHeight;
		cylinderConvexVerts[i].z *= radius;
	}

	PxCookingParams params = gCooking->getParams();
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;
	params.gaussMapLimit = 512;
	gCooking->setParams(params);

	PxConvexMeshDesc desc;
	desc.points.data = cylinderConvexVerts;
	desc.points.count = 32;
	desc.points.stride = sizeof(PxVec3);
	desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PxU32 meshSize = 0;
	PxConvexMesh* convex = NULL;

	bool directInsertion = false;

	if (directInsertion)
	{
		// directly insert mesh into PhysX
		convex = gCooking->createConvexMesh(desc, gPhysics->getPhysicsInsertionCallback());
		PX_ASSERT(convex);
	}
	else
	{
		// serialize the cooked mesh into a stream
		PxDefaultMemoryOutputStream outStream;
		bool res = gCooking->cookConvexMesh(desc, outStream);
		PX_UNUSED(res);
		PX_ASSERT(res);
		meshSize = outStream.getSize();

		// create the mesh from a stream
		PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
		convex = gPhysics->createConvexMesh(inStream);
		PX_ASSERT(convex);
	}

	PxShape* shape = gPhysics->createShape(PxConvexMeshGeometry(convex), *gMaterial);

	int nStack = 14;

	float altitude = halfHeight;
	float offsetX = 0.0f;
	while (nStack)
	{
		for (PxU32 i = 0; i < nStack; i++)
		{
			PxVec3 pos(offsetX + float(i) * radius * 2.2f, altitude, 0.0f);
			PxTransform localTm(pos);

			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			gScene->addActor(*body);
		}
		nStack--;
		altitude += halfHeight * 2.0f + 0.01f;
		offsetX += radius * 1.1f;
	}

	shape->release();
}

void initPhysics(bool interactive)
{
#if PX_PHYSICS_VERSION_MAJOR == 3
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
#else
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
#endif

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gDefMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gDefMaterial);
	gScene->addActor(*groundPlane);

	createStack(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)), 1.0f, 1.0f);
}

void stepPhysics(bool /*interactive*/)
{
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);
}
	
void cleanupPhysics(bool /*interactive*/)
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	PX_RELEASE(gCooking);
	if(gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
	
	printf("PeelCylinderStack done.\n");
}

void keyPress(unsigned char key, const PxTransform& camera)
{
}

int snippetMain(int, const char*const*)
{
#ifdef RENDER_SNIPPET
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
