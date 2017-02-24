#include "HealthComponent.h"
#include "Game.h"
void HealthComponent::Start()
{
	Initialize();
	currentHealth = 100.0;
	standardMat.diffuseColor = glm::vec3(0.0, 1.0, 0.0);
	Finalize();
}

void HealthComponent::Update()
{
	Initialize();
	vector<GameObject*> currBullets = Game::FindGameObjectsWithTag(Tags::TAGS_PROJECTILE);
	for (int i = 0; i < currBullets.size(); i++) {
		if (glm::distance(currBullets[i]->transform.position, transform.position) < 1.75) {
			currentHealth -= 25.0;
		}
	}

	standardMat.diffuseColor = glm::vec3(0.0, currentHealth/100.0, 0.0);
	if (currentHealth < 0.0) {
		transform.position = glm::vec3(0.0, -1005.0, 0.0);
	}
	Finalize();

}

