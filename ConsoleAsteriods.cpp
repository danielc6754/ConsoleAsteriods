using namespace std;

#include "ConsoleEngine.h"
#include <string>

class Asteriods : public ConsoleTemplateEngine {
public:
	Asteriods() {
		m_sAppName = L"Asteriods";
	}

private:
	struct sSpaceObject {
		float x = 0;
		float y = 0;
		float dx = 0;
		float dy = 0;
		int nSize = 0;
		float angle;
	};

	vector<sSpaceObject> vecAsteroids;
	sSpaceObject player;

	vector<pair<float, float>> vecModelShip;
	vector<pair<float, float>> vecModelAsteroid;
protected:
	virtual bool OnUserCreate() {
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });
		
		// Initialize Player Position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		// Simple Isoceles Triangle
		vecModelShip = { { 0.0f, -5.0f }, { -2.5f, +2.5f }, { +2.5f, +2.5f } };
		
		int verts = 20;
		for (int i = 0; i < verts; i++) {
			float radius = 1.0f;
			float a = ((float)i / (float)verts) * 6.28318f;
			vecModelAsteroid.push_back(make_pair(radius * sinf(a), radius * cosf(a)));
		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		// User Inputs
		// Steer
		if (m_keys[VK_LEFT].bHeld)
			player.angle -= 5.0f * fElapsedTime;
		if (m_keys[VK_RIGHT].bHeld)
			player.angle += 5.0f * fElapsedTime;
		// Thrust
		if (m_keys[VK_UP].bHeld) {
			// ACCEL changes VELOCITY with respect to TIME
			player.dx += sin(player.angle) * 20.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
		}

		// VELOCITY changes POSITION with respect to TIME
		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;

		// Keep ship in gamespace
		WrapCoordinates(player.x, player.y, player.x, player.y);

		// Update and draw asteroids
		for (auto& a : vecAsteroids) {
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			// Drawing asteroid 
			DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, a.nSize);
		}

		// Draw Ship
		DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

		return true;
	}

	void DrawWireFrameModel(const vector<pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE) {
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pair
		vector<pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++) {
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}
		// Draw Closed Polygon
		for (int i = 0; i < (verts + 1); i++) {
			int j = (i + 1);
			DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
		}
	}

	void WrapCoordinates(float ix, float iy, float& ox, float& oy) {
		ox = ix;
		oy = iy;

		if (ix < 0.0f)
			ox = ix + (float)ScreenWidth();
		if (ix >= (float)ScreenWidth())
			ox = ix - (float)ScreenWidth();
		if (iy < 0.0f)
			oy = iy + (float)ScreenHeight();
		if (iy >= (float)ScreenHeight())
			oy = iy - (float)ScreenHeight();
	}

	virtual void Draw(int x, int y, wchar_t c = 0x2588, short col = 0x000F) {
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);
		ConsoleTemplateEngine::Draw(fx, fy, c, col);
	}
};

int main() {
	Asteriods game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}