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

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;

PxMaterial*				gMaterial		= NULL;
PxMaterial*				gDefMaterial	= NULL;


PxPvd*                  gPvd        = NULL;

void createBridge(const PxTransform& t)
{
	const PxVec3 halfExtent(1.0f, 0.1f, 2.0f);
	PxShape* shapePlank = gPhysics->createShape(PxBoxGeometry(halfExtent), *gMaterial);

	const float radius = 1.0f;
	PxShape* shapeBall = gPhysics->createShape(PxSphereGeometry(radius), *gMaterial);

	const PxVec3 dir(1.0f, 0.0f, 0.0f);
	const PxVec3 posOffset = dir * halfExtent.x;

	const int nPlank = 20;
	const int nBridge = 10;

	float chainette_x[nPlank + 1];
	float chainette_Dx[nPlank + 1];
	const float coeff = 20.0f;

	for (PxU32 i = 0; i < nPlank + 1; i++)
	{
		const float x = float(i) - 0.5f * float(nPlank);

		chainette_x[i] = coeff * coshf(x / coeff);
		chainette_Dx[i] = sinhf(x / coeff);
	}

	for (PxU32 j = 0; j < nBridge; j++)
	{
		PxRigidActor* actorHandles[nPlank];

		const float rowCoeff = float(j) / float(nBridge - 1);

		PxVec3 pos(0.0f, 40.0f + 3.0f * radius, float(j) * halfExtent.z * 4.0f);
		PxTransform localTm(pos);

		if (1)
		{
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shapeBall);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f * 0.75f / (M_PI * powf(radius, 3.0f)));
			gScene->addActor(*body);
		}

		PxVec3 rightPos(0.0f, 0.0f, 0.0f);
		unsigned int groupBit = 0;
		for (PxU32 i = 0; i < nPlank; i++)
		{
			float mass = 1.0f;
			if (i == 0 || i == nPlank - 1)
				mass = 0.0f;

			PxMat33 rotMat(PxIdentity);
			const float alpha = -atanf(chainette_Dx[i]);
			const float curveCoeff = 0.5f + rowCoeff * 2.5f;

			rotMat[0][0] = rotMat[1][1] = cos(alpha * curveCoeff);
			rotMat[1][0] = -sin(alpha * curveCoeff);
			rotMat[0][1] = sin(alpha * curveCoeff);

			PxQuat quat(alpha * curveCoeff, PxVec3(0.0f, 0.0f, -1.0f));

			// 1st row of matrix
			PxVec3 r(rotMat[0][0], rotMat[1][0], rotMat[2][0]);

			{
				PxTransform localTm(pos, quat);

				const PxVec3 leftPos = pos - r * halfExtent.x;
				if (i)
				{
					pos += rightPos - leftPos;
				}

				if (mass == 0.0f)
				{
					PxRigidStatic* body = gPhysics->createRigidStatic(t.transform(localTm));
					body->attachShape(*shapePlank);
					actorHandles[i] = body;
					gScene->addActor(*body);
				}
				else
				{
					PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
					body->attachShape(*shapePlank);
					PxRigidBodyExt::updateMassAndInertia(*body, mass / (8.0f * halfExtent.x * halfExtent.y * halfExtent.z));
					actorHandles[i] = body;
					gScene->addActor(*body);
				}
			}

			rightPos = pos + r * halfExtent.x;
			pos += r * 2.0f * halfExtent.x;
		}

		for (PxU32 i = 0; i < nPlank - 1; i++)
		{
			PxRevoluteJointCreate(*gPhysics,
				actorHandles[i], PxTransform(posOffset,PxQuat(M_PI_2, PxVec3(0.0f, 1.0f, 0.0f))),
				actorHandles[i+1], PxTransform(-posOffset, PxQuat(M_PI_2, PxVec3(0.0f, 1.0f, 0.0f)))
			);
			
		}
	}

	shapePlank->release();
	shapeBall->release();
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
	gMaterial = gPhysics->createMaterial(0.0f, 0.5f, 0.0f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gDefMaterial);
	gScene->addActor(*groundPlane);

	createBridge(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
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
	if(gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
	
	printf("PeelCatenaryBridge done.\n");
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
