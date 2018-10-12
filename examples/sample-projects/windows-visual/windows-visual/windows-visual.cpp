#include "stdafx.h"

#include "modio.h"








int main(void)
{
	modio::Instance modio_instance(MODIO_ENVIRONMENT_TEST, 7, "e91c01b8882f4affeddd56c96111977b");

	volatile static bool finished = false;

	auto wait = [&]() {
		while (!finished)
		{
			modio_instance.sleep(10);
			modio_instance.process();
			std::cout << modio_instance.getModState(1173) << std::endl;
		}
	};

	auto finish = [&]() {
		finished = true;
	};

	wait();

	std::cout << "Process finished" << std::endl;








	system("pause");
	return 0;
}