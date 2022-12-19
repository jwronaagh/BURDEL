#include <SFML/Graphics.hpp> // biblioteka graficzna
#include <time.h> 

/* Co dodać?
* naprawić by działało
* licznik czasu
* przycisk resetu
* fajny komunikat o przegranej
* menu?
*
*/


using namespace std;
using namespace sf; // przedrostek na początky by nie dodawać go do każdej funkjci SFML


void czypusteobok() {

}

int main()
{

    srand(time(0)); // kij wie co, jakiś czas potrzebny do losowania

    ///tworzenie planszy///

    RenderWindow window(VideoMode(400, 400), "Saper"); // tworzymy okno gry

    int w = 32; // pole do popisu
    int grid[12][12]; // 12 wierszy 12 kolumn siatka odkryta
    int sgrid[12][12]; // 12 wierszy 12 kolumn siatka zakryta

    ///import tekstur///

    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);


    ///losowanie miejsc bomb////

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            sgrid[i][j] = 10;
            if (rand() % 7 == 0) grid[i][j] = 9; // tu można wybrać jak dużo chce się mieć bomb
            else grid[i][j] = 0;
        }
    }


    ///numeracja kafelków zależnych od położenia bomb///

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {

            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;

        }
    }

    ///otwieranie okna gry///

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        int x = pos.x / w;
        int y = pos.y / w;
        bool mbleft = false;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    sgrid[x][y] = grid[x][y];
                    mbleft = true;

                }
                else if (e.key.code == Mouse::Right) sgrid[x][y] = 11;

        }

        window.clear(Color::White);
        for (int i = 1; i <= 10; i++) {

            for (int j = 1; j <= 10; j++) {

                if (mbleft && sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];

                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                window.draw(s);
            }

        }
        window.display();
    }
    return 0;
}
