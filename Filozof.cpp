#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

class EatingPhilosophers {
private:
    int num_philosophers;
    vector<mutex> forks; // Mutexes representing forks (prevent conflicts between threads)
    vector<mutex> deadlock; // Additional mutexes to prevent deadlock
    mutex cout_mutex; // Mutex for synchronizing console output

public:
    EatingPhilosophers(int n) : num_philosophers(n), forks(n), deadlock(n) {}

    void philosopher(int id) {
        while (true) {
            think(id);  // Philosopher is thinking
            eat(id);    // Philosopher attempts to eat
        }
    }

    void think(int id) {
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id + 1 << " is thinking..." << endl;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    void eat(int id) {
        int left_fork = id;
        int right_fork = (id + 1) % num_philosophers;

        // DEADLOCK PREVENTION
        lock(deadlock[left_fork], deadlock[right_fork]);

        lock_guard<mutex> left_lock(deadlock[left_fork], adopt_lock);
        lock_guard<mutex> right_lock(deadlock[right_fork], adopt_lock);

        lock_guard<mutex> left_fork_lock(forks[left_fork]);
        lock_guard<mutex> right_fork_lock(forks[right_fork]);

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id + 1 << " is eating." << endl;
        }

        this_thread::sleep_for(chrono::seconds(2));

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id + 1 << " has finished eating." << endl;
        }
    }
};

int main(int argc, char* argv[]) {

    int num_philosophers = stoi(argv[1]); // Get the number of philosophers from command-line arguments
    EatingPhilosophers dp(num_philosophers);
    vector<thread> philosophers;

    // Creating a thread for each philosopher
    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.push_back(thread(&EatingPhilosophers::philosopher, &dp, i));
    }

    // Waiting for threads to complete (though they run in an infinite loop)
    for (auto& p : philosophers) {
        p.join();
    }

    return 0;
}
