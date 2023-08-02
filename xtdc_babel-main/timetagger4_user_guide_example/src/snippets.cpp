#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>

std::mutex mtx;
std::vector<double> dataBuffer1;
std::vector<double> dataBuffer2;
bool activeBufferForProcessing = true;
bool activeBufferForWriting = false;
bool exitFlag = false;

// Function to save data to a binary file
void saveDataToBinaryFile(const std::vector<double>& data, const std::string& filename) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    size_t dataSize = data.size();
    outfile.write(reinterpret_cast<const char*>(&dataSize), sizeof(size_t));
    outfile.write(reinterpret_cast<const char*>(data.data()), dataSize * sizeof(double));

    outfile.close();
}

// Function for data processing (processingThread)
void processData() {
    while (!exitFlag) {
        // Process data and store it in the active buffer
        std::vector<double> active;
        // Process your data here and add it to 'active'
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock the buffer while updating
            // Processing and adding data to 'active'
            // For example, you can push_back elements to 'active' based on your processing
            // and then clear the active buffer
            // active = dataBuffer1; // Copy data to the active buffer
            // dataBuffer1.clear(); // Clear the active buffer
        }
        // After processing, set the active buffer for writing
        activeBufferForWriting = true;
        activeBufferForProcessing = false;
    }
}

// Function for writing data to file (writingThread)
void writeDataToFile() {
    while (!exitFlag) {
        // Wait until the active buffer is ready for writing
        while (!activeBufferForWriting) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Write data from the active buffer to a binary file
        std::vector<double> active;
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock the buffer while writing
            active = dataBuffer2; // Copy data to the active buffer for writing
            dataBuffer2.clear(); // Clear the active buffer after writing
        }

        saveDataToBinaryFile(active, "data.bin");

        // After writing, set the active buffer for processing
        activeBufferForProcessing = true;
        activeBufferForWriting = false;
    }
}

int main() {
    // Start the processingThread and writingThread
    std::thread processingThread(processData);
    std::thread writingThread(writeDataToFile);

    // Simulate data processing and writing for some time
    // For example, you can have your main while loop here, where you process data and switch buffers

    // Wait for threads to finish
    processingThread.join();
    writingThread.join();

    return 0;
}
