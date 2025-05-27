# SO2-Projekt 1
# Problem jedzących filozofów

## Opis problemu
Problem jedzących filozofów to klasyczny problem synchronizacji w programowaniu. Kilku filozofów siedzi wokół okrągłego stołu i na zmianę myśli oraz je. Każdy z nich potrzebuje dwóch widelców (lewego i prawego), aby móc jeść. Współdzielenie widelców prowadzi do problemów, takich jak zakleszczenie czy zagłodzenie.

## Struktura projektu
Projekt składa się z jednej głównej klasy:
- **EatingPhilosophers** – zarządza widelcami oraz synchronizacją filozofów, zapewniając, że mogą oni myśleć i jeść bez ryzyka zakleszczenia.

## Wątki i ich reprezentacja
- W projekcie wykorzystano wielowątkowość poprzez bibliotekę `std::thread`.
- Każdy filozof jest osobnym wątkiem wykonującym cykliczne operacje.
- `std::mutex` zapewnia synchronizowaną dostępność do widelców i operacji wyjścia strumienia, aby uniknąć nakładania tekstu i zakleszczenia.
- Główna funkcja `main()` inicjalizuje filozofów i uruchamia wątki.

## Sekcje krytyczne i ich rozwiązanie

### Dostęp do widelców
**Problem:** Filozofowie mogą próbować jednocześnie podnieść te same widelce, co może prowadzić do zakleszczenia.

**Rozwiązanie:**
- Zastosowano `std::lock(deadlock[left_fork], deadlock[right_fork])`, aby uniknąć sytuacji, w której filozof podnosi jeden widelec i czeka na drugi.
- Mechanizm `lock_guard<std::mutex>` zapewnia automatyczne zwalnianie widelców po zakończeniu jedzenia.

### Zagłodzenie
**Problem:** Filozofowie mogą nie mieć możliwości podniesienia dwóch widelców na raz przez dłuższy czas.

**Rozwiązanie:**
- Zasoby są zwalniane sprawiedliwie dzięki `lock_guard`, co zapewnia równy dostęp do widelców.
- Filozofowie nie trzymają widelców dłużej niż to konieczne, co pozwala innym uzyskać do nich dostęp.

### Wyjście do konsoli (`std::cout`)
**Problem:** Kilka wątków może jednocześnie pisać do `std::cout`, co prowadzi do pomieszania tekstu.

**Rozwiązanie:** Dodanie mutexa `cout_mutex`, który zapewnia, że tylko jeden wątek w danym momencie wypisuje na ekran.

## Instrukcja uruchomienia

### Windows
1. Ustaw liczbę filozofów:  
   **Visual Studio 2022**:  
   `Projekt -> (nazwa) Właściwości` lub `(alt+F7) -> Debugowanie -> Argumenty Polecenia -> Wpisz liczbę filozofów -> Zastosuj -> OK -> Zamknij`
2. Uruchom Debugowanie

### Linux
1. Skompiluj program:
   ```sh
   g++ -o main main.cpp -pthread
   ```
2. Uruchom program, podając liczbę filozofów (np. 5):
   ```sh
   ./main 5
   ```
3. Program działa w nieskończonej pętli. Aby go zakończyć, użyj `Ctrl + C`.

## Podsumowanie
Projekt implementuje problem jedzących filozofów w C++ z użyciem wielowątkowości. Problem został rozwiązany bez zagłodzenia filozofów, a klasa `EatingPhilosophers` zarządza synchronizacją widelców i filozofów, zapobiegając zakleszczeniom. Wykorzystano `std::mutex`, `std::lock`, `lock_guard`, a także osobny mutex do synchronizacji wyjścia na konsolę. Program działa na **Windows** i **Linux**, pozwalając na dynamiczne określenie liczby filozofów przy uruchomieniu.

## Autor
**Franciszek Fedorowicz**  
Nr. 272845 | Systemy Operacyjne Projekt (grupa 13)


# SO2-Projekt 2

# Aplikacja czatu klient-serwer (WinSock)

## Opis projektu

Projekt przedstawia prostą aplikację komunikatora tekstowego stworzoną w języku C++ z wykorzystaniem biblioteki WinSock. Umożliwia komunikację wielu klientów w czasie rzeczywistym poprzez centralny serwer TCP, obsługujący przesyłanie wiadomości między użytkownikami.

## Struktura projektu

Projekt składa się z dwóch plików źródłowych:

* **Server.cpp** – implementacja serwera, który nasłuchuje na połączenia i obsługuje klientów.
* **Client.cpp** – implementacja klienta, który łączy się z serwerem, wysyła i odbiera wiadomości.

## Wątki i ich reprezentacja

* W projekcie wykorzystano wielowątkowość z biblioteki `std::thread`.
* Każdy klient jest obsługiwany w osobnym wątku serwera.
* Serwer ma dodatkowy wątek pozwalający mu wysyłać wiadomości z poziomu własnej konsoli.
* Klient również korzysta z osobnego wątku do odbierania wiadomości, aby mógł jednocześnie pisać i odbierać.

## Sekcje krytyczne i ich rozwiązanie

### Współdzielenie klientów

**Problem:** Równoczesny dostęp do listy połączonych klientów może prowadzić do konfliktów wątków.

**Rozwiązanie:**

* Zastosowano `std::mutex` (muteks `clients_mutex`) do synchronizacji dostępu do listy klientów na serwerze.
* Przesyłanie wiadomości (broadcast) odbywa się tylko po uzyskaniu blokady.

### Odbiór wiadomości przez klienta

**Problem:** Równoległy odbiór danych przez klienta może kolidować z innymi operacjami, np. wejściem tekstowym.

**Rozwiązanie:**

* Wydzielono wątek `receive_messages`, który niezależnie nasłuchuje na wiadomości przychodzące.
* Każda odebrana wiadomość jest wypisywana bez kolizji z wejściem użytkownika.

### Wyjście do konsoli (`std::cout`)

**Problem:** Równoczesne wypisywanie komunikatów przez kilka wątków może powodować nakładanie się tekstu.

**Rozwiązanie:**

* Komunikaty są wypisywane z zachowaniem synchronizacji strumienia (np. `std::cout.flush()`), co minimalizuje kolizje.
* Potencjalne rozszerzenie: dodanie dedykowanego mutexa `cout_mutex` (obecnie niewykorzystany).

## Instrukcja uruchomienia

### Windows

#### Kompilacja (w Visual Studio lub CMD/MSVC):

1. Upewnij się, że linkujesz bibliotekę `ws2_32.lib`.
2. Skorzystaj z poniższych komend w terminalu Visual Studio:

```sh
cl Server.cpp /EHsc /FeServer.exe ws2_32.lib
cl Client.cpp /EHsc /FeClient.exe ws2_32.lib
```

#### Uruchomienie:

1. Najpierw uruchom `Server.exe`.
2. Następnie w innym oknie terminala uruchom `Client.exe`.

Domyślnie klient łączy się z `127.0.0.1:8888`.

### Zamykanie:

* Wpisz `/exit` w kliencie, aby rozłączyć się z serwerem.
* Wpisz `/exit` na serwerze, aby zakończyć wysyłanie wiadomości z konsoli (serwer nadal działa).

## Podsumowanie

Projekt implementuje prosty model czatu w architekturze klient-serwer z obsługą wielu klientów. Serwer rozgłasza wiadomości przychodzące od klientów do wszystkich pozostałych. Rozwiązano problemy synchronizacji poprzez zastosowanie `std::thread`, `std::mutex`, a także mechanizmów zarządzania połączeniami i wypisywaniem danych do konsoli.

## Autor

**Franciszek Fedorowicz**
Nr. 272845 | Systemy Operacyjne Projekt (grupa 13)

---



