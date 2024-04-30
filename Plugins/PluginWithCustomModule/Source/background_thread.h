//
// Created by dtic on 4/30/24.
//

#ifndef JUCECMAKEREPO_BACKGROUND_THREAD_H
#define JUCECMAKEREPO_BACKGROUND_THREAD_H

#include <shared_processing_code/shared_processing_code.h>
#include <shared_plugin_helpers/shared_plugin_helpers.h>

class BackgroundTask : public juce::Thread
{
private:
    juce::StreamingSocket sourceSocket{}, destinationSocket{};
    juce::StreamingSocket* listenerSocket{nullptr};

public:
    int incomingPort{8000};
    int outgoingPort{8050};
    int timerDelayMS{100};



    BackgroundTask() : juce::Thread("Background Task Thread") {
        startServer();
    }

    ~BackgroundTask()
    {
        // Ensure the thread is stopped properly
        stopThread(4000); // Wait up to 4 seconds for the thread to finish

        sourceSocket.close();
        destinationSocket.close();
    }

    bool startListening(int port) {
        if (listenerSocket!=nullptr)
        {
            if (listenerSocket->isConnected())
            {
                listenerSocket->close();
            }
        }

        if (sourceSocket.isConnected())
        {
            sourceSocket.close();
        }

        if (sourceSocket.createListener(port))
        {
            listenerSocket = sourceSocket.waitForNextConnection();
            return listenerSocket->isConnected();
        }

        return false;
    }

    bool connectToHost(int port) {
        if (destinationSocket.isConnected())
        {
            destinationSocket.close();
        }

        return destinationSocket.connect("localhost", port);
    }

    bool startServer()
    {
        if (!startListening(incomingPort))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Error",
                                                   "Failed to listen on incoming port " + juce::String(incomingPort),
                                                   "OK");
            return false;
        }

        if (!connectToHost(outgoingPort))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Error",
                                                   "Failed to connect to outgoing port " + juce::String(outgoingPort),
                                                   "OK");
            return false;
        }

        return true;
    }

    void run() override
    {
        using namespace std;

        while (! threadShouldExit())
        {
            // Your thread code here. This block is executed repeatedly until the thread is told to exit.
            cout << "Thread running..." << endl;

            // Example of a task: simulate some work
            performTask();

            // Sleep to yield some time to other processes
            wait(100); // Wait for 100 milliseconds
        }
    }

    void performTask()
    {
        using namespace std;

        // Example task implementation
        static const juce::Colour green = juce::Colours::green;
        static const juce::Colour red = juce::Colours::red;

        if (sourceSocket.isConnected())
        {
            char buffer[1024] = {0}; // Initialize buffer to zero
            const int bytesRead = listenerSocket->read(buffer, sizeof(buffer), false);

            if (bytesRead > 0)
            {
                cout << "Received " << bytesRead << " bytes from source." << endl;
                int bytesWritten = destinationSocket.write(buffer, bytesRead);
                if (bytesWritten < 0)
                {
                    cout << "Error writing to destination socket." << endl;
                }
            }

        }
        else
        {
            cout << "Source socket not connected." << endl;
        }
    }

    void startTask()
    {
        if (! isThreadRunning())
        {
            startThread();
        }
        else
        {
            DBG("Thread already running!");
        }
    }

    void stopTask()
    {
        if (isThreadRunning())
        {
            signalThreadShouldExit(); // Tell the thread to stop
            waitForThreadToExit(2000); // Wait up to 2 seconds for it to stop
            if (isThreadRunning())
            {
                DBG("Thread failed to stop gracefully, forcing stop...");
                stopThread(0); // Force thread to stop immediately
            }
        }
    }
};



#endif //JUCECMAKEREPO_BACKGROUND_THREAD_H
