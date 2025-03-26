# SO2-Projekt
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
