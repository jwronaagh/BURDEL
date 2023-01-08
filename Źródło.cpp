#include <SFML/Graphics.hpp> // biblioteka graficzna
#include <SFML/Audio.hpp>
#include <time.h> 
#include <string.h>
#include <iostream>

/* Co dodaæ?
* menu te¿ mnie pokona³o same problemy z tym :/
*
*/



using namespace std;
using namespace sf; // przedrostek na pocz¹tky by nie dodawaæ go do ka¿dej funkjci SFML

int grid[12][12]; // 12 wierszy 12 kolumn siatka odkryta // da³em poza maina ¿eby dzia³a³o w funkcjach
int sgrid[12][12]; // 12 wierszy 12 kolumn siatka zakryta

bool czyWygrana()
{
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			// Jeœli pole jest zakryte i nie jest bomb¹, to znaczy, ¿e gracz jeszcze nie wygra³
			if (sgrid[i][j] == 10 && grid[i][j] != 9) return false;
		}
	}
	return true;
}

void odkryjPuste(int x, int y) // NIE mam pojecia czemu nie dzia³a

{
	// Sprawdzam, czy pole jest ju¿ odkryte lub czy jest bomb¹
	if (sgrid[x][y] == 9 || sgrid[x][y] == 0) return;

	// Odkrywamy pole
	sgrid[x][y] = grid[x][y];

	// Jeœli pole jest puste, to odkrywam równie¿ s¹siednie pola
	if (grid[x][y] == 0)
	{
		if (x > 1) odkryjPuste(x - 1, y); // pole po lewej
		if (x < 12) odkryjPuste(x + 1, y); // pole po prawej
		if (y > 1) odkryjPuste(x, y - 1); // pole u góry
		if (y < 12) odkryjPuste(x, y + 1); // pole na dole
	}
}


int main()
{

	srand(time(0)); // kij wie co, jakiœ czas potrzebny do losowania

	///wymiary planszy///

	RenderWindow window(VideoMode(400, 500), "Saper"); // tworzymy okno gry

	int w = 32; // pole do popisu
	int iloscmin = 0;

	///import tekstur///

	Texture t;
	t.loadFromFile("images/tiles.jpg");
	Sprite s(t);


	///import czcionki////
	Font open_sans;
	open_sans.loadFromFile("czcionki/opensus.ttf");


	//Dzika muzyka

	Music muzyczka;
	muzyczka.openFromFile("dzwieki/tlomuzyka.ogg");
	muzyczka.setVolume(2.f);
	muzyczka.setLoop(true); // zapêtlanie utworu
	muzyczka.play();

	//Dzikie dŸwiêki
	SoundBuffer bufferklik;
	bufferklik.loadFromFile("dzwieki/klik.wav");

	Sound klik;
	klik.setBuffer(bufferklik);

	SoundBuffer bufferwybuch;
	bufferwybuch.loadFromFile("dzwieki/wybuch.wav");

	Sound wybuch;
	wybuch.setBuffer(bufferwybuch);

	//Przycisk
	RectangleShape resetButton(Vector2f(100, 50));
	resetButton.setFillColor(Color::Red);
	resetButton.setOutlineThickness(5);
	resetButton.setOutlineColor(Color(163, 26, 16));
	resetButton.setPosition(150, 400); // Ustawiam pozycjê przycisku na ekranie
	//Napis Resetu
	Text reset;
	reset.setFont(open_sans);
	reset.setString("Reset!");
	reset.setFillColor(Color(158, 8, 33));
	reset.setStyle(sf::Text::Bold);
	reset.setPosition(160, 405);
	//Napis przegranej
	Text przegrana;
	przegrana.setFont(open_sans);
	przegrana.setString("Przegrana!");
	przegrana.setCharacterSize(40);
	przegrana.setFillColor(Color::Red);
	przegrana.setPosition(110, 400);
	//Napis wygranej, nie wiem dalczego nie dziala warunek jest w 304 linijce
	int odkryte_pola = 0;

	Text wygrana;
	wygrana.setFont(open_sans);
	wygrana.setString("Wygrana!");
	wygrana.setCharacterSize(40);
	wygrana.setFillColor(Color::Green);
	wygrana.setPosition(110, 400);
	//Napis Ilosc bomb
	Text bombText;
	bombText.setFont(open_sans);
	bombText.setCharacterSize(14);
	bombText.setFillColor(Color::Black);
	bombText.setString("Ilosc bomb: " + to_string(iloscmin));
	bombText.setPosition(10, 10);
	//Bomby oznaczone
	int bombsMarked = 0; // iloœæ oznaczonych bomb
	Text bombyOznaczone;
	bombyOznaczone.setFont(open_sans);
	bombyOznaczone.setCharacterSize(14);
	bombyOznaczone.setFillColor(Color::Black);
	bombyOznaczone.setPosition(275, 10);

	Clock clock;
	Time elapsedTime;
	Text timerText;
	Font font;
	timerText.setFont(open_sans);
	timerText.setCharacterSize(14);
	timerText.setFillColor(Color::Black);
	timerText.setPosition(150, 10);




	///losowanie miejsc bomb////

	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++) {
			sgrid[i][j] = 10;
			if (rand() % 7 == 0) { // tu mo¿na wybraæ jak du¿o chce siê mieæ bomb
				grid[i][j] = 9;
				iloscmin++;
			}
			else grid[i][j] = 0;
		}
	}

	///numeracja kafelków zale¿nych od po³o¿enia bomb///

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

	

	while (window.isOpen())
	{
		Event e;

		elapsedTime = clock.getElapsedTime();
		timerText.setString("Czas: " + to_string(elapsedTime.asSeconds()));

		Vector2i pos = Mouse::getPosition(window);
		int x = pos.x / w;
		int y = pos.y / w;
		bool mbleft = false;

		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			// odkrywanie pól


			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				{
					sgrid[x][y] = grid[x][y];
					mbleft = true;
					klik.play();

				}

			// zaznaczanie nieodkrytych pól flagami
				else if (e.key.code == Mouse::Right) {

					if (sgrid[x][y] == 10) {

						sgrid[x][y] = 11;
						bombsMarked++;
					}

					else if (sgrid[x][y] == 11) {
						sgrid[x][y] = 10;
						bombsMarked--;

					}
					
					bombyOznaczone.setString("Oznaczone pola: " + to_string(bombsMarked));
				}
		}

		// sprawdzamy czy wygrana

		int miny = 0;

		for (int i = 1; i <= 10; i++) {


			for (int j = 1; j <= 10; j++) {

				if (sgrid[i][j] != 0) miny++;


			}


		}



		window.clear(Color::White);  // ustawia nowe okno o kolorze bia³ym
		for (int i = 1; i <= 10; i++) {

			for (int j = 1; j <= 10; j++) {

				if (mbleft && sgrid[x][y] == 9) { // trafienie na bombê
					sgrid[i][j] = grid[i][j];
					muzyczka.stop();
					wybuch.play();
				}

				s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
				s.setPosition(i * w, j * w);
				window.draw(s);

			}

		}
		if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
		{
			// Pobieram pozycjê kursora myszy na ekranie
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			// Sprawdzam, czy kursor myszy znajduje siê nad obszarem przycisku
			if (resetButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				iloscmin = 0;
				// Jeœli tak, to resetuje grê
				for (int i = 1; i <= 10; i++) {
					for (int j = 1; j <= 10; j++) {
						sgrid[i][j] = 10;
					}
				}

				// Losuje nowe po³o¿enie bomb
				for (int i = 1; i <= 10; i++) {
					for (int j = 1; j <= 10; j++) {
						if (rand() % 50 == 0) {
							grid[i][j] = 9; // tu mo¿na wybraæ jak du¿o chce siê mieæ bomb
							iloscmin++;
						}
						else grid[i][j] = 0;
					}
				}

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
				// Aktualizuje iloœæ bomb
				iloscmin = 0;
				for (int i = 1; i <= 10; i++)
				{
					for (int j = 1; j <= 10; j++)
					{
						if (grid[i][j] == 9) iloscmin++;

					}
				}

				// Aktualizuje tekst z iloœci¹ bomb
				bombText.setString("Ilosc bomb: " + to_string(iloscmin));
				bombsMarked = 0;
				bombyOznaczone.setString("Oznaczone miny: " + to_string(bombsMarked));

				clock.restart();

				mbleft = false;
				muzyczka.play();

			}
		}

		if (mbleft && sgrid[x][y] == 9)
		{
			window.draw(przegrana);
			window.display();
			sleep(sf::seconds(3));
		}
		if (czyWygrana()) {

			window.draw(wygrana);
			window.display();
			sleep(sf::seconds(3));

		}


		window.draw(timerText);
		window.draw(bombyOznaczone);
		window.draw(bombText);
		window.draw(resetButton);
		window.draw(reset);
		window.display();


	}

	return 0;
}