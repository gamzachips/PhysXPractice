#pragma once
class PhysicsEvent : public PxSimulationEventCallback
{
	//This is called when a breakable constraint breaks.
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
	{
	}
	//This is called during PxScene::fetchResults with the actors which have just been woken up.						
	void onWake(PxActor** actors, PxU32 count)
	{
	}

	////This is called during PxScene::fetchResults with the actors which have just been put to sleep.						
	void onSleep(PxActor** actors, PxU32 count)
	{
	}

	//This is called during PxScene::fetchResults with the current trigger pair events.
	void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
	{
		//loop through all trigger-pairs of PhysX simulation
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			//get current trigger actor & other actor info 
			//from current trigger-pair 
			const PxTriggerPair& curTriggerPair = pairs[i];
		}
	}

	//The method will be called for a pair of actors if one of the colliding shape pairs requested contact notification.
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		const PxU32 buff = 64; //buffer size
		PxContactPairPoint contacts[buff];

		//loop through all contact pairs of PhysX simulation
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			//extract contant info from current contact-pair 
			const PxContactPair& curContactPair = pairs[i];
			PxU32 nbContacts = curContactPair.extractContacts(contacts, buff);

			for (PxU32 j = 0; j < nbContacts; j++)
			{
				//print all positions of contact.   
				PxVec3 point = contacts[j].position;
				//cout<<"Contact point ("<<point.x <<" "<< point.y<<" "<<point.x<<")\n";
			}

			PxActor* triggerActor = pairHeader.actors[0];
			PxActor* otherActor = pairHeader.actors[1];

			Object* triggerObject = static_cast<Object*>(triggerActor->userData);
			Object* otherObject = static_cast<Object*>(otherActor->userData);
			triggerObject->OnTriggerEnter(otherObject);
			otherObject->OnTriggerEnter(triggerObject);
		}
	}

	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
	{

	}
};

