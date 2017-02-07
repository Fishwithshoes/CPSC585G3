#include "CommonIncludes.h"
#include <PxPhysicsAPI.h>
<<<<<<< HEAD
#include <PxDefaultErrorCallback.h>
#include <PxDefaultAllocator.h>

//#pragma comment(lib, "PhysX3_x86.lib")
//#pragma comment(lib, "Foundation.lib")
//#pragma comment(lib, "PhysX3Extensions.lib")

=======
#include <PxTransform.h>
>>>>>>> conlan

class Physics
{
public:
<<<<<<< HEAD
	Physics();
	~Physics();
	void initializePhysX();
private:

};

=======
	//Physics();
	//~Physics();
	void initializePhysX();
	void createTestBox(physx::PxTransform& t, physx::PxReal sideLength);
private:

};
>>>>>>> conlan
