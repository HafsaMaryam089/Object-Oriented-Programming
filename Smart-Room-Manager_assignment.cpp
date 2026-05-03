#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Device {
private:
    string name;
    bool isOn;
    int brightness;

public:
    // Default constructor
    Device() {
        name = "Unknown";
        isOn = false;
        brightness = 50;
    }

    // Overloaded constructor
    Device(string n, bool status, int b) {
        name = n;
        isOn = status;
        setBrightness(b);
    }

    // Destructor
    ~Device() {
        
    }

  
    void printRemoval() const {   //display removal
        if (name != "Unknown") {
            cout << "Device :" << name << " is being removed.\n";
        }
    }

    // Setter/Getter Functions
    void setName(string n) 
    { name = n; }

    string getName() const
    { return name; }

    void setBrightness(int b) {
        if (b >= 0 && b <= 100) {
            brightness = b;
        }
        else {
            cout << "Invalid brightness \n";
            brightness = 50;
        }
    }
    int getBrightness() const { return brightness; }

    void turnOn() { isOn = true; }      //toggledDevice
    void turnOff() { isOn = false; }
    bool getStatus() const { return isOn; }

    // Display Func
    void showInfo() const {
        cout << "Name: " << name
            << ", Status: " << (isOn ? "ON" : "OFF")
            << ", Brightness: " << brightness << endl;
    }
};

class Room {
private:
    string roomName;
    Device devices[10]; 
    int count;

public:
    // Constructor
    Room(string name) {
        roomName = name;
        count = 0;
        loadFromFile(); // Load devices on creation
    }

    // Destructor
    ~Room() {
        saveToFile(); //Save devices on destruction
        for (int i = 0; i < count; i++) {
            devices[i].printRemoval();
        }
        cout << "Room :" << roomName << " manager closed.\n";
    }

    // Add device to array
    void addDevice(Device d) {
        if (count < 10) {
            devices[count] = d;
            count++;
            cout << "Device added successfully!\n";
            saveToFile(); // Autosave 
        }
        else {
            cout << "Room is full! Cannot add more than 5 devices.\n";
        }
    }

    // Shows all devices 
    void showAllDevices() const {
        if (count == 0) {
            cout << "No devices in the room.\n";
            return;
        }
        cout << "\n--- Devices in " << roomName << endl;
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". ";
            devices[i].showInfo();
        }
    }

    // Returns index of device
    int findDevice(string name) {
        for (int i = 0; i < count; i++) {
            if (devices[i].getName() == name) {
                return i;
            }
        }
        return -1;
    }

    // Turns device ON/OFF
    void toggleDevice(string name) {
        int index = findDevice(name);
        if (index != -1) {
            if (devices[index].getStatus() == true) {
                devices[index].turnOff();
                cout << ">> SUCCESS: " << name << " is now OFF.\n";
            }
            else {
                devices[index].turnOn();
                cout << ">> SUCCESS: " << name << " is now ON.\n";
            }
            saveToFile();
        }
        else {
            cout << "Device not found. Check your spelling!\n";
        }
    }


    void setDeviceBrightness(string name, int brightness) {
        int index = findDevice(name);
        if (index != -1) {
            devices[index].setBrightness(brightness);
            cout << "Brightness of " << name << " updated to " << devices[index].getBrightness() << ".\n";
            saveToFile();
        }
        else {
            cout << "Device not found.\n";
        }
    }

   
    void saveToFile() const {
        ofstream outFile("device.txt");
        if (outFile.is_open()) {
            outFile << count << endl;
            for (int i = 0; i < count; i++) {
                int statusInt = devices[i].getStatus() ? 1 : 0;

                outFile << devices[i].getName() << "\n"
                    << statusInt << "\n"
                    << devices[i].getBrightness() << "\n";
            }
            outFile.close();
        }
    }

    void loadFromFile() {
        ifstream inFile("device.txt");
        if (inFile.is_open()) {
            inFile >> count;
            inFile.ignore(); 
            for (int i = 0; i < count; i++) {
                string n;
                int s_int; 
                int b;

                getline(inFile, n);
                inFile >> s_int >> b;
                inFile.ignore(); 

                devices[i].setName(n);

              
                if (s_int == 1) {
                    devices[i].turnOn();
                }
                else {
                    devices[i].turnOff();
                }

                devices[i].setBrightness(b);
            }
            inFile.close();
        }
    }
};

int main() {
    Room myRoom("Living Room");
    int choice;
    string name;
    int brightness;

    do {
        cout << "\n=== Smart Room Manager ===\n";
        cout << "1. Add a Device"<<endl;
        cout << "2. Show All Devices" << endl;
        cout << "3. Turn Device ON/OFF" << endl;
        cout << "4. Change Brightness" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter device name: ";
            cin >> ws;
            getline(cin, name);

            int b;
            cout << "Enter starting brightness: ";
            cin >> b;

            char Choice;
            cout << "Is the device currently ON? (y/n): ";
            cin >> Choice;

            bool initialStatus = (Choice == 'y' || Choice == 'Y');

            Device newDevice(name, initialStatus, b);
            myRoom.addDevice(newDevice);
            break;
        }
        case 2:
            myRoom.showAllDevices();
            break;
        case 3:
            cout << "Enter device name to toggle: ";
            cin >> ws;
            getline(cin, name);
            myRoom.toggleDevice(name);
            break;
        case 4:
            cout << "Enter device name: ";
            cin >> ws;
            getline(cin, name);
            cout << "Enter new brightness : ";
            cin >> brightness;
            myRoom.setDeviceBrightness(name, brightness);
            break;
        case 5:
            cout << "Exiting system. Saving data \n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
