#include <CodeAdapter\EasyCA.h>
#include <CodeAdapterSFML\SFMLFactory.h>

#include "MenuScene.h"




int main()
{
	initializeCA<SFMLFactory>();


	auto window = caFactory->createWindow();
	window->create(caDraw::Size(1024, 768), { "Linked Block Life" }, false);


	auto sceneManager = canew<caUtil::SceneManager>();
	sceneManager->replaceScene(*window, canew<MenuScene>());

	window->setSceneManager(sceneManager);


	while (window->isRunning())
	{
		caTouch->update();
		caKeyboard->update(*window);


		if (sceneManager->hasScene() == false)
		{
			window->exit();
		}


		window->update();
		window->draw(caDraw::Color(230, 230, 230));
	}


	return 0;
}

