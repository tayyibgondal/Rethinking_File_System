#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>

#include <vector>
#include <thread>
#include <chrono>

#include <semaphore.h>
#include <deque>

// using namespace std;

class Capitalizer
{
public:
    std::string capitalize(const std::string &input) const
    {
        std::string output = input;

        // #include <algorithm>
        transform(output.begin(), output.end(), output.begin(), ::toupper);
        return output;
    }
};

class MultiPrompt
{
public:
    MultiPrompt(bool const &continueRun, sem_t &isFull)
        : continueRun(continueRun), isFull(isFull)
    {
        sem_init(&isFull, 0, 0);
        sem_init(&jsem, 0, 1);
    }

    void callCapitalize()
    {
        std::cout << "Started Working\n";
        while (true)
        {
            sem_wait(&isFull);
            // Check whether last sem_post() was a termination call;
            if (!continueRun)
                break;
            sem_wait(&jsem);

            std::string input = jobs.front();
            jobs.pop_front();

            // Update output for client
            std::string capitalized = capitalizer.capitalize(input);
            *output_pointers.front() = capitalized;
            output_pointers.pop_front();

            sem_post(semaphores.front());
            semaphores.pop_front();
            sem_post(&jsem);

            // Log to file
            time_t now = time(nullptr);
            char timestamp[30];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
            long int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
            char ms[4];
            sprintf(ms, "%03ld", milliseconds);
            std::string log_entry = std::string(timestamp) + "." + std::string(ms) + " " + input + " " + capitalized + "\n";
            std::ofstream logfile("log.txt", std::ios::app);
            if (logfile.is_open())
            {
                logfile << log_entry;
                logfile.close();
            }
        }
        std::cout << "Stopped Working\n";
    }

    void addJob(const std::string &job, std::string &output, sem_t *sema)
    {
        sem_wait(&jsem);
        jobs.push_back(job);
        semaphores.push_back(sema);
        output_pointers.push_back(&output);
        sem_post(&isFull);
        sem_post(&jsem);
    }

private:
    Capitalizer capitalizer;
    sem_t &isFull;
    sem_t jsem;
    bool const &continueRun;
    std::deque<std::string> jobs;
    std::deque<sem_t *> semaphores;
    std::deque<std::string *> output_pointers;
};

class Prompt
{
public:
    Prompt(MultiPrompt &multiPrompt, const std::string &inputFile) : multiPrompt(multiPrompt), inputFile(inputFile)
    {
        sem_init(&sem, 0, 0);
    }

    void run()
    {
        std::cout << "File Running: " << inputFile << std::endl;
        std::ifstream input(this->inputFile);
        if (input.is_open())
        {
            std::string line;
            std::string output;
            while (std::getline(input, line))
            {
                multiPrompt.addJob(line, output, &sem);
                sem_wait(&sem);
                // Now it knows ans is here
                std::cout << "Returned result: " << output << std::endl;
            }
            input.close();
        }
        else
        {
            std::cout << "Unable to open file: " << this->inputFile << std::endl;
        }
    }

private:
    MultiPrompt &multiPrompt;
    std::string inputFile;
    sem_t sem;
};

int main()
{
    // Files containing input strings
    std::vector<std::string> inputFiles = {"input1.txt", "input2.txt", "input3.txt"};

    // mainRun
    bool continueRun = true;
    sem_t isFull;
    MultiPrompt multiPrompt(continueRun, isFull);

    // Create a vector of Prompt objects
    std::vector<Prompt> prompts;
    for (const auto &inputFile : inputFiles)
    {
        prompts.push_back(Prompt(multiPrompt, inputFile));
    }

    // Create threads to run Prompt objects
    std::vector<std::thread> threads;
    for (auto &prompt : prompts)
    {
        threads.push_back(std::thread(&Prompt::run, &prompt));
    }

    // Create thread to run MultiPrompt
    std::thread multiPromptThread(&MultiPrompt::callCapitalize, &multiPrompt);

    // Join threads
    for (auto &t : threads)
    {
        t.join();
    }

    // All processing done
    continueRun = false;
    sem_post(&isFull);
    // Join MultiPrompt thread
    multiPromptThread.join();

    return 0;
}
