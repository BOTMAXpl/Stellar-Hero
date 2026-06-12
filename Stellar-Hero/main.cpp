#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "gracz.h"
#include "pocisk.h"
#include "wrog.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "pocisk_wroga.h"
#include "dzialko.h"
#include "laser_wroga.h"
#include "portal.h"
#include "atak_obszarowy.h"
#include "ostrzezenie.h"
using namespace std;

void zaladuj_wrogow(int poziom, vector<obiekt*>& lista) {
    if (poziom == 1) {
        lista.push_back(new wrog(1000, 300));
        lista.push_back(new wrog(1400, 500));
        lista.push_back(new dzialko(200, 200));
        lista.push_back(new dzialko(1600, 800));
    }
    else if (poziom == 2) {
        lista.push_back(new wrog(800, 200));
        lista.push_back(new wrog(1000, 400));
        lista.push_back(new wrog(1200, 600));
        lista.push_back(new wrog(1400, 800));
        lista.push_back(new dzialko(100, 100));
        lista.push_back(new dzialko(1800, 100));
    }
    else if (poziom == 3) {
        lista.push_back(new wrog(700, 300));
        lista.push_back(new wrog(800, 500));
        lista.push_back(new wrog(900, 700));
        lista.push_back(new wrog(1200, 500));
        lista.push_back(new dzialko(100, 100));
        lista.push_back(new dzialko(1800, 100));
        lista.push_back(new dzialko(100, 900));
        lista.push_back(new dzialko(1800, 900));
    }
}

int main() {
    sf::RenderWindow okno(sf::VideoMode(1920, 1080), "Stellar-Hero", sf::Style::Fullscreen);
    okno.setFramerateLimit(60);
    srand(time(NULL));

    sf::Font czcionka;
    if (!czcionka.loadFromFile("MAIAN.ttf")) {
    }

    int stan_gry = 0;
    int wybrana_opcja = 0;

    int wybrany_tryb = 0;
    float czas_gry = 0.0f;
    float czas_do_kolejnego_wroga = 2.0f;

    int aktualny_poziom = 1;
    float czas_napisu = 0;
    bool portal_otwarty = false;

    vector<string> napisy_glowne = {"Start", "Informacje", "Wyjscie"};
    vector<string> napisy_tryb = {"Classic", "Endless"};
    vector<string> napisy_gracze = {"1 Gracz", "2 Graczy"};

    sf::Texture tekstura_mapy;
    tekstura_mapy.loadFromFile("mapa1.png");
    sf::Sprite tlo_poziomu;
    tlo_poziomu.setTexture(tekstura_mapy);

    sf::Texture tex_serce_puste;
    tex_serce_puste.loadFromFile("serce1.png");
    tex_serce_puste.setSmooth(false);

    sf::Texture tex_serce_pelne;
    tex_serce_pelne.loadFromFile("serce2.png");
    tex_serce_pelne.setSmooth(false);

    sf::Sprite sprite_serce;
    sprite_serce.setScale(4.0f, 4.0f);

    vector<obiekt*> lista_obiektow;
    gracz* glowny_gracz = new gracz(1);
    gracz* gracz2 = nullptr;
    lista_obiektow.push_back(glowny_gracz);

    lista_obiektow.push_back(new wrog(1000, 300));
    lista_obiektow.push_back(new wrog(1400, 500));
    lista_obiektow.push_back(new wrog(1100, 800));
    lista_obiektow.push_back(new dzialko(200, 200));
    lista_obiektow.push_back(new dzialko(1600, 800));

    sf::Clock zegar;
    bool wcisnieta_spacja = false;
    bool wcisniety_enter = false;

    while (okno.isOpen()) {
        float dt = zegar.restart().asSeconds();
        sf::Event zdarzenie;

        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed)
                okno.close();

            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) {
                if (stan_gry == 3) stan_gry = 0;
                else okno.close();
            }

            if (stan_gry != 3 && zdarzenie.type == sf::Event::KeyPressed) {

                if (zdarzenie.key.code == sf::Keyboard::Up) {
                    wybrana_opcja--;
                    if (wybrana_opcja < 0) wybrana_opcja = 0;
                }
                if (zdarzenie.key.code == sf::Keyboard::Down) {
                    wybrana_opcja++;
                    if (stan_gry == 0 && wybrana_opcja > 2) wybrana_opcja = 2;
                    if ((stan_gry == 1 || stan_gry == 2) && wybrana_opcja > 1) wybrana_opcja = 1;
                }

                if (zdarzenie.key.code == sf::Keyboard::Enter) {
                    if (stan_gry == 0) {
                        if (wybrana_opcja == 0) stan_gry = 1;
                        else if (wybrana_opcja == 1) stan_gry = 4;
                        else if (wybrana_opcja == 2) okno.close();
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 1) {
                        if (wybrana_opcja == 0) { stan_gry = 2; wybrany_tryb = 0; }
                        else if (wybrana_opcja == 1) { stan_gry = 2; wybrany_tryb = 1; }
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 2) {
                        stan_gry = 3;
                        aktualny_poziom = 1;
                        portal_otwarty = false;

                        czas_gry = 0.0f;
                        czas_do_kolejnego_wroga = 2.0f;

                        for (int j = 0; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                        lista_obiektow.clear();

                        glowny_gracz = new gracz(1);
                        lista_obiektow.push_back(glowny_gracz);

                        if (wybrana_opcja == 1) {
                            gracz2 = new gracz(2);
                            lista_obiektow.push_back(gracz2);
                        } else {
                            gracz2 = nullptr;
                        }

                        if (wybrany_tryb == 0) {
                            czas_napisu = 3.0f;
                            zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        } else {
                            czas_napisu = 0.0f;
                        }

                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 4) {
                        stan_gry = 0;
                    }
                }
            }
        }

        if (stan_gry == 3) {

            if (wybrany_tryb == 1) {
                czas_gry += dt;
                czas_do_kolejnego_wroga -= dt;

                if (czas_do_kolejnego_wroga <= 0) {

                    int ile_wrogow = 1 + (int)(std::pow(1.5, czas_gry / 30.0f) - 1);

                    for (int k = 0; k < ile_wrogow; k++) {
                        float rand_x = 100 + rand() % 1700;
                        float rand_y = 100 + rand() % 800;
                        int losowy_typ = rand() % 2;
                        lista_obiektow.push_back(new ostrzezenie(rand_x + 40, rand_y + 40, losowy_typ));
                    }
                    float nowy_czas = 2.0f - (czas_gry / 100.0f);
                    if (nowy_czas < 0.6f) nowy_czas = 0.6f;
                    czas_do_kolejnego_wroga = nowy_czas + ((rand() % 10) / 10.0f);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (wcisnieta_spacja == false) {
                    sf::Vector2f start = glowny_gracz->daj_pozycje();
                    sf::Vector2f kier = glowny_gracz->wez_kierunek();
                    start.x += 26;
                    start.y += 20;
                    start.x += kier.x * 70;
                    start.y += kier.y * 70;
                    pocisk* nowy_laser = new pocisk(start, kier);
                    lista_obiektow.push_back(nowy_laser);
                    wcisnieta_spacja = true;
                }
            } else {
                wcisnieta_spacja = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                if (glowny_gracz != nullptr && glowny_gracz->gotowy_obszarowy()) {
                    sf::Vector2f srodek = glowny_gracz->daj_pozycje();
                    srodek.x += 26;
                    srodek.y += 20;
                    lista_obiektow.insert(lista_obiektow.begin(), new atak_obszarowy(srodek));
                    glowny_gracz->uzyj_obszarowego();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (wcisniety_enter == false && gracz2 != nullptr) {
                    sf::Vector2f start = gracz2->daj_pozycje();
                    sf::Vector2f kier = gracz2->wez_kierunek();
                    start.x += 26;
                    start.y += 20;
                    start.x += kier.x * 70;
                    start.y += kier.y * 70;
                    pocisk* nowy_laser = new pocisk(start, kier);
                    lista_obiektow.push_back(nowy_laser);
                    wcisniety_enter = true;
                }
            } else {
                wcisniety_enter = false;
            }

            int zywych_wrogow = 0;

            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->licz(dt);

                sf::Vector2f start_wroga;
                bool czy_strzela = false;

                wrog* w = dynamic_cast<wrog*>(lista_obiektow[i]);
                if (w != nullptr) {
                    zywych_wrogow++;
                    if(w->gotowy_do_strzalu()) {
                        start_wroga = w->daj_srodek();
                        czy_strzela = true;
                        w->zresetuj_strzal();
                    }
                }

                dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[i]);
                if (dz != nullptr) {
                    zywych_wrogow++;
                    if (dz->gotowy_do_strzalu()) {
                        start_wroga = dz->daj_srodek();
                        czy_strzela = true;
                        dz->zresetuj_strzal();
                    }
                }

                if (czy_strzela) {
                    sf::Vector2f cel = glowny_gracz->daj_pozycje();

                    if (gracz2 != nullptr) {
                        sf::Vector2f pos2 = gracz2->daj_pozycje();
                        float dystans1 = (cel.x - start_wroga.x) * (cel.x - start_wroga.x) + (cel.y - start_wroga.y) * (cel.y - start_wroga.y);
                        float dystans2 = (pos2.x - start_wroga.x) * (pos2.x - start_wroga.x) + (pos2.y - start_wroga.y) * (pos2.y - start_wroga.y);
                        if (dystans2 < dystans1) cel = pos2;
                    }

                    cel.x += 20;
                    cel.y += 20;

                    sf::Vector2f kierunek_strzalu = cel - start_wroga;
                    float dlugosc = std::sqrt(kierunek_strzalu.x * kierunek_strzalu.x + kierunek_strzalu.y * kierunek_strzalu.y);

                    if (dlugosc != 0) {
                        kierunek_strzalu.x /= dlugosc;
                        kierunek_strzalu.y /= dlugosc;

                        if (w != nullptr) lista_obiektow.push_back(new pocisk_wroga(start_wroga, kierunek_strzalu));
                        if (dz != nullptr) lista_obiektow.push_back(new laser_wroga(start_wroga, kierunek_strzalu));
                    }
                }
            }

            if(wybrany_tryb == 0 && zywych_wrogow == 0 && portal_otwarty == false){
                lista_obiektow.push_back(new portal(960, 540));
                portal_otwarty = true;
            }

            for (int i = 0; i < lista_obiektow.size(); i++) {
                if (lista_obiektow[i]->czy_usunac() == true) {

                    ostrzezenie* ost = dynamic_cast<ostrzezenie*>(lista_obiektow[i]);
                    if (ost != nullptr) {
                        if (ost->daj_typ() == 0) {
                            lista_obiektow.push_back(new wrog(ost->daj_pozycje().x - 40, ost->daj_pozycje().y - 40));
                        } else {
                            lista_obiektow.push_back(new dzialko(ost->daj_pozycje().x - 40, ost->daj_pozycje().y - 40));
                        }
                    }

                    delete lista_obiektow[i];
                    lista_obiektow.erase(lista_obiektow.begin() + i);
                    i--;
                }
            }

            for (int i = 0; i < lista_obiektow.size(); i++) {
                portal* prt = dynamic_cast<portal*>(lista_obiektow[i]);
                if (prt != nullptr) {
                    bool prt_hit = false;
                    if (glowny_gracz != nullptr && prt->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) prt_hit = true;
                    if (gracz2 != nullptr && prt->daj_kolizje().intersects(gracz2->daj_kolizje())) prt_hit = true;

                    if (prt_hit) {
                        aktualny_poziom++;
                        if (aktualny_poziom > 3) aktualny_poziom = 1;

                        for (int j = 0; j < lista_obiektow.size(); j++) {
                            if (dynamic_cast<gracz*>(lista_obiektow[j]) == nullptr) {
                                delete lista_obiektow[j];
                                lista_obiektow.erase(lista_obiektow.begin() + j);
                                j--;
                            }
                        }

                        portal_otwarty = false;
                        czas_napisu = 3.0f;
                        if (glowny_gracz != nullptr) glowny_gracz->zresetuj();
                        if (gracz2 != nullptr) gracz2->zresetuj();
                        zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        break;
                    }
                }

                pocisk* p = dynamic_cast<pocisk*>(lista_obiektow[i]);
                if (p != nullptr) {
                    for (int j = 0; j < lista_obiektow.size(); j++) {
                        wrog* w = dynamic_cast<wrog*>(lista_obiektow[j]);
                        if (w != nullptr && p->czy_trafia(w->daj_srodek())) {
                            w->zabij();
                        }
                        dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[j]);
                        if (dz != nullptr && p->czy_trafia(dz->daj_srodek())) {
                            dz->zabij();
                            for (int k = 0; k < lista_obiektow.size(); k++) {
                                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[k]);
                                if (lw != nullptr && lw->wez_pozycje_startowa() == dz->daj_srodek()) {
                                    lw->zniszcz();
                                }
                            }
                        }
                    }
                }

                atak_obszarowy* aoe = dynamic_cast<atak_obszarowy*>(lista_obiektow[i]);
                if (aoe != nullptr) {
                    for (int j = 0; j < lista_obiektow.size(); j++) {
                        wrog* w = dynamic_cast<wrog*>(lista_obiektow[j]);
                        if (w != nullptr && aoe->daj_kolizje().contains(w->daj_srodek())) {
                            w->zabij();
                        }
                        dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[j]);
                        if (dz != nullptr && aoe->daj_kolizje().contains(dz->daj_srodek())) {
                            dz->zabij();
                            for (int k = 0; k < lista_obiektow.size(); k++) {
                                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[k]);
                                if (lw != nullptr && lw->wez_pozycje_startowa() == dz->daj_srodek()) {
                                    lw->zniszcz();
                                }
                            }
                        }
                    }
                }

                pocisk_wroga* pw = dynamic_cast<pocisk_wroga*>(lista_obiektow[i]);
                if (pw != nullptr) {
                    bool dostal = false;
                    if (glowny_gracz != nullptr && pw->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) { glowny_gracz->dostan_obrazenia(); dostal = true; }
                    if (gracz2 != nullptr && pw->daj_kolizje().intersects(gracz2->daj_kolizje())) { gracz2->dostan_obrazenia(); dostal = true; }
                    if (dostal) pw->zniszcz();
                }

                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[i]);
                if (lw != nullptr) {
                    if (glowny_gracz != nullptr) {
                        sf::Vector2f srodek = glowny_gracz->daj_pozycje();
                        srodek.x += 20; srodek.y += 20;
                        if (lw->czy_trafia(srodek)) glowny_gracz->dostan_obrazenia();
                    }
                    if (gracz2 != nullptr) {
                        sf::Vector2f srodek = gracz2->daj_pozycje();
                        srodek.x += 20; srodek.y += 20;
                        if (lw->czy_trafia(srodek)) gracz2->dostan_obrazenia();
                    }
                }
            }
            bool martwy_1 = (glowny_gracz != nullptr && glowny_gracz->ile_zyc() <= 0);
            bool martwy_2 = (gracz2 != nullptr && gracz2->ile_zyc() <= 0);

            if (martwy_1 || martwy_2) {
                stan_gry = 0;

                for (int j = 0; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                lista_obiektow.clear();

                glowny_gracz = new gracz(1);
                gracz2 = nullptr;
                lista_obiektow.push_back(glowny_gracz);

                lista_obiektow.push_back(new wrog(1000, 300));
                lista_obiektow.push_back(new wrog(1400, 500));
                lista_obiektow.push_back(new wrog(1100, 800));
                lista_obiektow.push_back(new dzialko(200, 200));
                lista_obiektow.push_back(new dzialko(1600, 800));
            }
        }

        okno.clear(sf::Color(30, 30, 30));

        if (stan_gry == 3) {
            okno.draw(tlo_poziomu);
            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->rysuj(okno);
            }

            if (glowny_gracz != nullptr) {
                for (int i = 0; i < 3; i++) {
                    if (i < glowny_gracz->ile_zyc()) sprite_serce.setTexture(tex_serce_pelne, true);
                    else sprite_serce.setTexture(tex_serce_puste, true);
                    sprite_serce.setPosition(50 + i * 80, 50);
                    okno.draw(sprite_serce);
                }
            }

            if (gracz2 != nullptr) {
                for (int i = 0; i < 3; i++) {
                    if (i < gracz2->ile_zyc()) sprite_serce.setTexture(tex_serce_pelne, true);
                    else sprite_serce.setTexture(tex_serce_puste, true);
                    sprite_serce.setPosition(1870 - 80 * 3 + i * 80, 50);
                    okno.draw(sprite_serce);
                }
            }

            if (wybrany_tryb == 1) {
                int minuty = (int)czas_gry / 60;
                int sekundy = (int)czas_gry % 60;
                string str_sekundy = (sekundy < 10) ? "0" + to_string(sekundy) : to_string(sekundy);
                sf::Text tekst_czasu(to_string(minuty) + ":" + str_sekundy, czcionka, 80);

                sf::FloatRect granice = tekst_czasu.getLocalBounds();
                tekst_czasu.setPosition(960 - granice.width / 2.0f, 30);
                tekst_czasu.setFillColor(sf::Color::White);

                okno.draw(tekst_czasu);
            }
        }

        if (czas_napisu > 0) {
            czas_napisu -= dt;
            string t = "POZIOM " + to_string(aktualny_poziom);
            sf::Text napis_poziomu(t, czcionka, 150);
            napis_poziomu.setPosition(700, 400);
            napis_poziomu.setFillColor(sf::Color(255, 255, 255, 200));
            okno.draw(napis_poziomu);
        }

        else {
            vector<string> do_narysowania;
            if (stan_gry == 0) do_narysowania = napisy_glowne;
            else if (stan_gry == 1) do_narysowania = napisy_tryb;
            else if (stan_gry == 2) do_narysowania = napisy_gracze;

            if (stan_gry == 4) {
                sf::Text info("To jest projekt z cpp.\nWcisnij Enter zeby wrocic.", czcionka, 50);
                info.setPosition(400, 400);
                okno.draw(info);
            }
            else {
                for (int i = 0; i < do_narysowania.size(); i++) {
                    sf::Text tekst(do_narysowania[i], czcionka, 80);
                    tekst.setPosition(800, 300 + (i * 120));
                    if (i == wybrana_opcja) {
                        tekst.setFillColor(sf::Color::Yellow);
                    } else {
                        tekst.setFillColor(sf::Color::White);
                    }
                    okno.draw(tekst);
                }
            }
        }
        okno.display();
    }

    for (int i = 0; i < lista_obiektow.size(); i++) {
        delete lista_obiektow[i];
    }

    return 0;
}
