//
// Created by dtic on 4/30/24.
//

#ifndef JUCECMAKEREPO_BACKGROUND_THREAD_H
#define JUCECMAKEREPO_BACKGROUND_THREAD_H

#include <shared_processing_code/shared_processing_code.h>
#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include "LockFreeQueue.h"

using namespace std;

class BackgroundTask : public juce::Thread
{
private:
    juce::StreamingSocket sourceSocket{}, destinationSocket{};

public:
    StaticLockFreeQueue<array<double, 9>, 128>* Bg2GuiQueuePtr{};

    int incomingPort{8000};
    int outgoingPortDecoded {8050};
    int timerDelayMS{1};
    char buffer[4 * 1024] = {0}; // Initialize buffer to zero
    chrono::time_point<chrono::system_clock> prevMsg = std::chrono::system_clock::now();



    BackgroundTask() : juce::Thread("Background Task Thread") {
        setPriority(juce::Thread::Priority::highest);
    }

    ~BackgroundTask()
    {
        // Ensure the thread is stopped properly
        stopThread(4000); // Wait up to 4 seconds for the thread to finish

        sourceSocket.close();
        destinationSocket.close();
    }

    bool startListening(int port) {

        if (sourceSocket.isConnected())
        {
            sourceSocket.close();
        }

        auto is_connected = sourceSocket.connect("localhost", port);

        if (is_connected) {
            sourceSocket.waitUntilReady(false, -1);

            std::string formattedVals = "HELLO\n";

            auto msg = formattedVals.c_str();
            int bytesWritten = sourceSocket.write(msg,  (strlen(msg)));

            if (bytesWritten < 0)
            {
//                cout << "Error writing to source socket." << endl;
            } else {
//                cout << "Sent " << bytesWritten << " bytes to source: " << msg << endl;
            }
        }

        return is_connected;
    }

    bool connectToHost(int port) {
        if (destinationSocket.isConnected())
        {
            destinationSocket.close();
        }

        return destinationSocket.connect("localhost", port);
    }

    bool connectButtonClicked(array<int, 3> values) {
        incomingPort = values[0];
        outgoingPortDecoded = values[1];
        timerDelayMS = values[2];

        return startServer();
    }

    bool startServer()
    {
        if (!connectToHost(outgoingPortDecoded))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Error",
                                                   "Failed to connect to outgoing port " + juce::String(outgoingPortDecoded),
                                                   "OK");
            return false;
        }

        if (!startListening(incomingPort))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Error",
                                                   "Failed to listen on incoming port " + juce::String(incomingPort),
                                                   "OK");
            return false;
        }

        return true;
    }

    void run() override
    {

        while (! threadShouldExit())
        {

            // Record start time


            performTask();



        }
    }

    void performTask()
    {

        // Example task implementation
        static const juce::Colour green = juce::Colours::green;
        static const juce::Colour red = juce::Colours::red;

        if (sourceSocket.isConnected())
        {
            buffer[0] = '\0'; // Clear buffer

            const int bytesRead = sourceSocket.read(buffer, sizeof(buffer), false);

            if (bytesRead > 0)
            {
                // Record end time
                auto endTime = std::chrono::system_clock::now();

                // Calculate time difference
                auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - prevMsg).count();

                // Log the time difference
                std::cout << "Time difference between iterations: " << timeDifference << " ms" << std::endl;
//                cout << "___ Received " << bytesRead << " bytes from source: " << buffer << endl;
                decodeAndSendBuffer(buffer);

                prevMsg = std::chrono::system_clock::now();

            } else if (bytesRead == 0) {
//                cout << "___ 0 Bytes Read " << buffer <<endl;
            } else {
//                cout << "___ Error reading from source socket." << endl;
            }

        }
        else
        {
        }
    }

    std::array<double, 9> decodeAndSendBuffer(const std::string& input)  {
        std::array<double, 9> reqVals{};

        if (input.find("DEBUG") == 0) {
            // cout << input << endl;
            return reqVals; // Ignore debug messages
        }

        if (input[0] == 'D') {
            // cout << "Received data: " << input << endl;

            auto parts = split(input.substr(4), '-'); // Skip 'D01.' and split the rest
            // cout << "Parts size: " << parts.size() << endl;

            if (!parts.empty()) { // Check if the number of data segments is correct
                reqVals = {
                    scaleEEG(hex2dec(parts[1]) * 256 + hex2dec(parts[2])),  // EEG right
                    scaleEEG(hex2dec(parts[3]) * 256 + hex2dec(parts[4])),  // EEG left
                    scaleAccel(hex2dec(parts[5]) * 256 + hex2dec(parts[6])),  // Accelerometer dx
                    scaleAccel(hex2dec(parts[7]) * 256 + hex2dec(parts[8])),  // Accelerometer dy
                    scaleAccel(hex2dec(parts[9]) * 256 + hex2dec(parts[10])), // Accelerometer dz
                    bodyTemp(hex2dec(parts[25]) * 256 + hex2dec(parts[26])),  // Body temperature
                    batteryVoltage(hex2dec(parts[21]) * 256 + hex2dec(parts[22])),  // Battery voltage
                    static_cast<double>(hex2dec(parts[19])),  // Noise (assuming raw value is needed)
                    static_cast<double>(hex2dec(parts[20]))   // Light (assuming raw value is needed)
                };

                // parse the decoded values and send them to the destination socket Format: "eegLeft, eegRight, accX, accY, accZ, bodyTemp, battVolt, noise, light"
                std::string formattedVals = std::to_string(reqVals[0]) + " " +             // EEG left
                                            std::to_string(reqVals[1]) + " " +                   // EEG right
                                            std::to_string(reqVals[2]) + " " +                  // Accelerometer dx
                                            std::to_string(reqVals[3]) + " " +                 // Accelerometer dy
                                            std::to_string(reqVals[4]) + " " +               // Accelerometer dz
                                            std::to_string(reqVals[5]) + " " +             // Body temperature
                                            std::to_string(reqVals[6]) + " " +           // Battery voltage
                                            std::to_string(reqVals[7]) + " " +         // Noise
                                            std::to_string(reqVals[8]) +";";                // Light

                auto msg = formattedVals.c_str();
                int bytesWritten = destinationSocket.write(msg,  (strlen(msg)));
                if (bytesWritten < 0)
                {
//                    cout << "Error writing to destination socket." << endl;
                } else {
                    // cout << "Sent " << bytesWritten << " bytes to destination: " << msg << endl;
                }


                if (Bg2GuiQueuePtr != nullptr) {
                    Bg2GuiQueuePtr->push(reqVals);
                }

            }
        }

        // convert to a string formatted as "eegl

        return reqVals;
    }

    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
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
        if (sourceSocket.isConnected())
        {
            sourceSocket.close();
        }

        if (destinationSocket.isConnected())
        {
            destinationSocket.close();
        }

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

    static int getByteAt(const std::string& buf, int idx) {
        std::string byteStr = buf.substr(idx * 3, 2);
        return std::stoi(byteStr, nullptr, 16);
    }

    static int getWordAt(const std::string& buf, int idx) {
        int high = getByteAt(buf, idx);
        int low = getByteAt(buf, idx + 1);
        return high * 256 + low;
    }

    static double scaleEEG(int value) {
        double uvRange = 3952;
        double d = value - 32768;
        return d * uvRange / 65536;
    }

    static double scaleAccel(int value) {
        return value * 4.0 / 4096.0 - 2.0;
    }

    static double batteryVoltage(int value) {
        return value / 1024.0 * 6.60;
    }

    static double bodyTemp(int value) {
        double v = value / 1024.0 * 3.3;
        return 15.0 + ((v - 1.0446) / 0.0565537333333333);
    }

    int hex2dec(const std::string& hexStr) {
        return std::stoi(hexStr, nullptr, 16);
    }
};



#endif //JUCECMAKEREPO_BACKGROUND_THREAD_H
