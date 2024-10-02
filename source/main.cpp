#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include "headers/drawer.h"
#include "headers/quadtree.h"

using namespace std;

const float FRAME_INTERVAL = 1.0f / 60.0f;
const int FPS_WINDOW_SIZE = 1000;
const int WIDTH = 640;
const int HEIGHT = 640;
const int NUM_BALLS = 50;

void saveScreenshot(const sf::Image& screenshot, unsigned long long frameCounter) {
    screenshot.saveToFile("screenshots/frame_" + to_string(frameCounter) + ".png");
}

int main(int argc, char* argv[]) {
    vector<sf::Color> palette = {
        sf::Color(11, 106, 136),
        sf::Color(45, 197, 244),
        sf::Color(112, 50, 126),
        sf::Color(146, 83, 161),
        sf::Color(164, 41, 99),
        sf::Color(236, 1, 90),
        sf::Color(240, 99, 164),
        sf::Color(241, 97, 100),
        sf::Color(248, 158, 79),
        sf::Color(252, 238, 33)
    };


    random_device rd;
    unsigned int seed = rd();
    cout << "Seed: " << seed << endl;

    mt19937 gen(seed);
    uniform_int_distribution<> distribW(0, WIDTH);
    uniform_int_distribution<> distribH(0, HEIGHT);
    uniform_int_distribution<> distribMass(4, 300);
    uniform_int_distribution<> distribColor(0, palette.size() - 1);

    Drawer drawer(WIDTH, HEIGHT, "The Tu-Balls");

    vector<Particle> particles;
    for (int i = 0; i < NUM_BALLS; ++i) {
        int x = distribW(gen);
        int y = distribH(gen);
        int mass = distribMass(gen);
        sf::Color color = palette[distribColor(gen)];
        particles.push_back(Particle({x, y}, mass, i, color, seed));
    }

    unsigned long long frameCounter = 0;
    auto oldTime = chrono::high_resolution_clock::now();
    auto lastCaptureTime = std::chrono::high_resolution_clock::now();
    float totalFrameTime = 0.0f;
    queue<float> frameTimes;

    vector<thread> savingThreads;

    /*  GAME LOOP */
    while (drawer.window->isOpen())
    {
        drawer.eventHandler();

        sf::Vector2f center{WIDTH/2, HEIGHT/2};

        // FPS calculation
        auto nowTime = chrono::high_resolution_clock::now();
        float duration = chrono::duration_cast<chrono::milliseconds>(nowTime - oldTime).count() / 1000.f;
        float elapsedTime = chrono::duration_cast<chrono::milliseconds>(nowTime - lastCaptureTime).count() / 1000.f;
        oldTime = nowTime;
        int fps;
        
        if (duration > 0.0f) {
            frameTimes.push(duration);
            totalFrameTime += duration;

            if (frameTimes.size() > FPS_WINDOW_SIZE) {
                totalFrameTime -= frameTimes.front();
                frameTimes.pop();
            }

            fps = frameTimes.size() / totalFrameTime;
        }

        // Create & populate quadtree
        Rectangle boundary(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT);
        QuadTree qtree(boundary, 4);
        vector<string> checkedPairs;

        for (Particle& particle : particles)
            qtree.insert(new Point(particle.position.x, particle.position.y, &particle));

        for (Particle& particleA : particles) {
            Circle range(particleA.position.x, particleA.position.y, particleA.r * 2);

            vector<Point*> points;
            qtree.query(range, points);
            for (Point*& point : points) {
                int idA = particleA.id;
                int idB = point->data->id;

                if (idA == idB)
                    continue;
                
                string pair = idA < idB ? (to_string(idA) + "," + to_string(idB)) : (to_string(idB) + "," + to_string(idA));
                if (find(checkedPairs.begin(), checkedPairs.end(), pair) != checkedPairs.end())
                    continue;

                particleA.collide(point->data);
                checkedPairs.push_back(pair);
            }
        }

        // Draw & Display
        drawer.clearScreen(sf::Color::Black);
        for (Particle& particle : particles) {
            particle.update();
            particle.edges(WIDTH, HEIGHT);
            drawer.drawCircle(particle.position, particle.r, particle.color);
        }

        // Generate frame image
        if (argc > 1 && elapsedTime >= FRAME_INTERVAL) {
            sf::Texture texture;
            texture.create(WIDTH, HEIGHT);
            texture.update(*drawer.window);

            sf::Image screenshot = texture.copyToImage();
            savingThreads.push_back(thread(saveScreenshot, screenshot, frameCounter));

            ++frameCounter;
            lastCaptureTime = nowTime;
        }

        drawer.drawText(to_string(fps), 10, {WIDTH - 12, 4}, sf::Color::Green);
        drawer.window->display();
    }

    if (argc > 1) {
        cout << "Threads used: " << savingThreads.size() << endl;
        for (thread& th : savingThreads)
            if (th.joinable())
                th.join();
    }

    return 0;
}
