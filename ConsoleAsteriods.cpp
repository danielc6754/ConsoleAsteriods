using namespace std;

#include "ConsoleEngine.h"
#include <string>

class Asteriods : public ConsoleTemplateEngine {
public:
	Asteriods() {
		m_sAppName = L"Asteriods";
	}

private:

protected:
	virtual bool OnUserCreate() {
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		return true;
	}
};

int main() {
	Asteriods game;
	game.ConstructConsole(180, 100, 8, 8);
	game.Start();

	return 0;
}